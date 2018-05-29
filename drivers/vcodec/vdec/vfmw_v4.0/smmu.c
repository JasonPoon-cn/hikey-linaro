/*
 *      Video Decode driver for hikey960   smmu master
 *      hikey960 :  An Hisilicon multimedia SOC Device for Hisilicon Kirin960 :
 *				- hikey960
 *
 *      Copyright (C) 2001-2017, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of version 2 of the GNU General
 *      Public License as published by the Free Software Foundation.
 *
 *      This program is distributed in the hope that it will be
 *      useful, but WITHOUT ANY WARRANTY; without even the implied
 *      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *      PURPOSE.  See the GNU General Public License for more details.
 *      You should have received a copy of the GNU General Public
 *      License along with this program; if not, write to the Free
 *      Software Foundation, Inc., 59 Temple Place - Suite 330,
 *      Boston, MA  02111-1307, USA.
 *      The full GNU General Public License is included in this
 *      distribution in the file called COPYING.
 *
 */

#ifdef ENV_ARMLINUX_KERNEL
#include <asm/memory.h>
#include <linux/types.h>
#include <linux/gfp.h>
#endif

#include "smmu.h"
#include "sysconfig.h"
#include "mem_manage.h"

#define SMMU_SCR        (0x0000)
#define SMMU_INTSTAT_NS (0x0018)
#define SMMU_SMRx_NS    (0x0020)    //(0x0020+n*0x4) SMMU control register per stream for non-secure
#define SMMU_RLD_EN0_NS (0x01F0)    //SMR reload enable register
#define SMMU_RLD_EN1_NS (0x01F4)
#define SMMU_CB_TTBR0   (0x0204)    //SMMU translation table base register for non-secure context bank0
#define SMMU_CB_TTBCR   (0x020C)    //SMMU Translation Table Base Control Register for Non-Secure Context Bank
#define SMMU_ERR_RDADDR (0x0304)    //SMMU Error Address of TLB miss for Read transaction
#define SMMU_ERR_WRADDR (0x0308)    //SMMU Error Address of TLB miss for Write transaction

#define REG_MFDE_MMU_CFG_SECURE (0x0008)
#define REG_MFDE_MMU_CFG_EN     (0x000c)
#define REG_SCD_MMU_CFG         (0x0820)
#define REG_BPD_MMU_CFG         (0x0004)
#define SMMU_MSTR_GLB_BYPASS    (0x0000)
#define SMMU_MSTR_DBG_0         (0x0010)
#define SMMU_MSTR_DBG_1         (0x0014)
#define SMMU_MSTR_DBG_2         (0x0018)
#define SMMU_MSTR_DBG_3         (0x001c)
#define SMMU_MSTR_DBG_4         (0x0020)
#define SMMU_MSTR_DBG_5         (0x0024)
#define SMMU_MSTR_DBG_6         (0x0028)
#define SMRx_ID_SIZE            (32)
#define SMMU_RWERRADDR_SIZE     (128)

//SMMU common and Master(MFDE/SCD/BPD) virtual base address
typedef struct {
	 int *pSMMUCommonBaseVirAddr;
	 int *pSMMUMasterBaseVirAddr;
	 int *pSMMUMFDERegVirAddr;
	 int *pSMMUBPDRegVirAddr;
	 int *pSMMUSCDRegVirAddr;
} SMMU_REG_VIR_S;

typedef enum {
	MEM_ION = 0,    // ion default
	MEM_ION_CTG,    // ion contigeous
	MEM_CMA,        // kmalloc
	MEM_CMA_ZERO,    // kzalloc
} MEM_TYPE_E;

typedef struct {
	unsigned char IsSecure;
	MEM_TYPE_E MemType;
	unsigned int PhyAddr;
	unsigned int Length;
	unsigned long long VirAddr;
} MEM_DESC_S;

static SMMU_REG_VIR_S gSmmuRegVir;
static MEM_DESC_S     gAllocMem_RD;
static MEM_DESC_S     gAllocMem_WR;

static  int gSmmuInitFlag   = 0;

#define RD_SMMU_VREG(SMMU_BASE_VADDR , SMMU_BASE_REG , SMMU_BASE_VALUE)               \
do {                    \
    (SMMU_BASE_VALUE) = *((volatile  int*)((char*)(SMMU_BASE_VADDR) + (SMMU_BASE_REG))); \
} while(0)

#define WR_SMMU_VREG(SMMU_BASE_VADDR , SMMU_BASE_REG , SMMU_BASE_VALUE)               \
do {                    \
    *((volatile  int*)((char*)(SMMU_BASE_VADDR) + (SMMU_BASE_REG))) = (SMMU_BASE_VALUE); \
} while(0)

/**
 *function: set SMMU common register
 *addr: register's vir addr
 *val: value to be set
 *bw: bit width
 *bs: bit start
 */
static void set_common_reg(unsigned int addr,  int val,  int bw,  int bs)
{
	 int mask = (1UL << bw) - 1UL;
	 int tmp = 0;

    RD_SMMU_VREG(gSmmuRegVir.pSMMUCommonBaseVirAddr , addr , tmp);
	tmp &= ~(mask << bs);
	WR_SMMU_VREG(gSmmuRegVir.pSMMUCommonBaseVirAddr , addr, tmp | ((val & mask) << bs));
}

/**
 *function: set SMMU master register
 *addr: register's vir addr
 *val: value to be set
 *bw: bit width
 *bs: bit start
 */
static void set_master_reg(unsigned int addr,  int val,  int bw,  int bs)
{
	 int mask = (1UL << bw) - 1UL;
	 int tmp = 0;

	RD_SMMU_VREG(gSmmuRegVir.pSMMUMasterBaseVirAddr , addr , tmp);
	tmp &= ~(mask << bs);
	WR_SMMU_VREG(gSmmuRegVir.pSMMUMasterBaseVirAddr , addr, tmp | ((val & mask) << bs));

}

/**
 *function: set mfde/scd/bpd register
 *master_type: MFDE/SCD/BPD
 *addr: register's vir addr
 *val: value to be set
 *bw: bit width
 *bs: bit start
 */
static void set_vdh_master_reg(SMMU_MASTER_TYPE master_type, unsigned int addr,  int val,  int bw,  int bs)
{
	 int mask = (1UL << bw) - 1UL;
	 int tmp = 0;

	switch (master_type) {
	case MFDE:
		RD_SMMU_VREG(gSmmuRegVir.pSMMUMFDERegVirAddr , addr, tmp);
		tmp &= ~(mask << bs);
		WR_SMMU_VREG(gSmmuRegVir.pSMMUMFDERegVirAddr , addr, tmp | ((val & mask) << bs));
		break;

	case BPD:
        RD_SMMU_VREG(gSmmuRegVir.pSMMUBPDRegVirAddr , addr, tmp);
		tmp &= ~(mask << bs);
		WR_SMMU_VREG(gSmmuRegVir.pSMMUBPDRegVirAddr , addr, tmp | ((val & mask) << bs));
		break;

	case SCD:
		RD_SMMU_VREG(gSmmuRegVir.pSMMUSCDRegVirAddr , addr, tmp);
		tmp &= ~(mask << bs);
		WR_SMMU_VREG(gSmmuRegVir.pSMMUSCDRegVirAddr , addr, tmp | ((val & mask) << bs));
		break;

	default:
		break;
	}
}

/**
 *function: set mfde mmu cfg register
 */
static void set_mmu_cfg_reg_mfde(SMMU_MASTER_TYPE master_type, unsigned int secure_en, unsigned int mmu_en)
{
	if (mmu_en) {    //MMU enable
		set_vdh_master_reg(master_type, REG_MFDE_MMU_CFG_EN, 0x1, 1, 12);    //[12]mmu_en=1
		if (secure_en) {  //secure
			dprint(KERN_ALERT, "IN %s not support this mode: mmu_en:secure, secure_en:%d, mmu_en:%d\n", __func__, secure_en, mmu_en);
			set_vdh_master_reg(master_type, REG_MFDE_MMU_CFG_SECURE, 0x1, 1, 31);    //[31]secure_en=1
		} else { //non-secure
			set_vdh_master_reg(master_type, REG_MFDE_MMU_CFG_SECURE, 0x0, 1, 31);    //[31]secure_en=0
		}
	} else {    //MMU disable
		set_vdh_master_reg(master_type, REG_MFDE_MMU_CFG_EN, 0x0, 1, 12);    //[12]mmu_en=0
		if (secure_en) {    //secure
			set_vdh_master_reg(master_type, REG_MFDE_MMU_CFG_SECURE, 0x1, 1, 31);    //[31]secure_en=1
		} else {    //non-secure
			dprint(KERN_ALERT, "IN %s not support this mode: non_mmu:non_secure, secure_en:%d, mmu_en:%d\n", __func__, secure_en, mmu_en);
			set_vdh_master_reg(master_type, REG_MFDE_MMU_CFG_SECURE, 0x0, 1, 31);	//[31]secure_en=0
		}
	}
}

/**
 *function: set bpd mmu cfg register
 */
static void set_mmu_cfg_reg_bpd(SMMU_MASTER_TYPE master_type, unsigned int secure_en, unsigned int mmu_en)
{
	if (mmu_en) {    //MMU enable
		set_vdh_master_reg(master_type, REG_BPD_MMU_CFG, 0x1, 1, 21);    //[21]mmu_en=1
		if (secure_en) {    //secure
			dprint(KERN_ALERT, "IN %s not support this mode: non_mmu:non_secure, secure_en:%d, mmu_en:%d\n", __func__, secure_en, mmu_en);
			set_vdh_master_reg(master_type, REG_BPD_MMU_CFG, 0x1, 1, 20);    //[20]secure_en=1
		} else {    //non-secure
			set_vdh_master_reg(master_type, REG_BPD_MMU_CFG, 0x0, 1, 20);    //[20]secure_en=0
		}
	} else {    //MMU disable
		set_vdh_master_reg(master_type, REG_BPD_MMU_CFG, 0x0, 1, 21);    //[21]mmu_en=0
		if (secure_en) {    //secure
			set_vdh_master_reg(master_type, REG_BPD_MMU_CFG, 0x1, 1, 20);    //[20]secure_en=1
		} else {    //non-secure
			dprint(KERN_ALERT, "IN %s not support this mode: non_mmu:non_secure, secure_en:%d, mmu_en:%d\n", __func__, secure_en, mmu_en);
			set_vdh_master_reg(master_type, REG_BPD_MMU_CFG, 0x0, 1, 20);	//[20]secure_en=0
		}
	}
}

/**
 *function: set scd mmu cfg register
 */
static void set_mmu_cfg_reg_scd(SMMU_MASTER_TYPE master_type, unsigned int secure_en, unsigned int mmu_en)
{
	if (mmu_en) {   //MMU enable
		set_vdh_master_reg(master_type, REG_SCD_MMU_CFG, 0x1, 1, 9);    //[9]mmu_en=1
		if (secure_en) {    //secure
			dprint(KERN_ALERT, "IN %s not support this mode: non_mmu:non_secure, secure_en:%d, mmu_en:%d\n", __func__, secure_en, mmu_en);
			set_vdh_master_reg(master_type, REG_SCD_MMU_CFG, 0x1, 1, 7);    //[7]secure_en=1
		} else {    //non-secure
			set_vdh_master_reg(master_type, REG_SCD_MMU_CFG, 0x0, 1, 7);    //[7]secure_en=0
		}
	} else {    //MMU disable
		set_vdh_master_reg(master_type, REG_SCD_MMU_CFG, 0x0, 1, 9);    //[9]mmu_en=0
		if (secure_en) {    //secure
			set_vdh_master_reg(master_type, REG_SCD_MMU_CFG, 0x1, 1, 7);    //[7]secure_en=1
		} else {    //non-secure
			dprint(KERN_ALERT, "IN %s not support this mode: non_mmu:non_secure, secure_en:%d, mmu_en:%d\n", __func__, secure_en, mmu_en);
			set_vdh_master_reg(master_type, REG_SCD_MMU_CFG, 0x0, 1, 7);	//[7]secure_en=0
		}
	}
}

static  int smmu_mem_alloc(unsigned int size, MEM_DESC_S *pMemDesc)
{
	void *virt_addr = NULL;

	if (pMemDesc == NULL) {
		printk(KERN_ERR "%s: invalid param pMemDesc is NULL\n", __func__);
		return SMMU_ERR;
	}

	if (pMemDesc->VirAddr != 0) {
		printk(KERN_ERR "%s param pMemDesc->VirAddr is not NULL\n", __func__);
		return SMMU_ERR;
	}

	if (MEM_CMA_ZERO == pMemDesc->MemType)
		virt_addr = kzalloc(size, GFP_KERNEL | GFP_DMA);    //restrict [0 ~ 4G]
	else
		virt_addr = kmalloc(size, GFP_KERNEL | GFP_DMA);    //restrict [0 ~ 4G]

	pMemDesc->VirAddr   = (uintptr_t)virt_addr;
	pMemDesc->PhyAddr = __pa(virt_addr);

	return SMMU_OK;
}

static void smmu_mem_dealloc(MEM_DESC_S *pMemDesc)
{
	if (pMemDesc == NULL) {
		printk(KERN_ERR "%s FATAL: Invalid pMemDesc is NULL\n", __func__);
		return;
	}

	if (pMemDesc->VirAddr == 0) {
		printk(KERN_ERR "%s FATAL: Invalid pMemDesc->VirAddr is NULL\n", __func__);
		return;
	}

	kfree((void *)pMemDesc->VirAddr);
	pMemDesc->VirAddr = 0;
	return;
}

/**
 *function: Alloc MEM for TLB miss .
 */
#ifdef ENV_ARMLINUX_KERNEL
static  int alloc_smmu_tlb_miss_addr(void)
{
	 int ret = SMMU_ERR;

	gAllocMem_RD.MemType = MEM_CMA_ZERO;
	ret = smmu_mem_alloc(SMMU_RWERRADDR_SIZE, &gAllocMem_RD);
	if (ret != SMMU_OK) {
		dprint(KERN_CRIT, "%s kzalloc mem for smmu rderr failed\n", __func__);
		return SMMU_ERR;
	}

	gAllocMem_WR.MemType = MEM_CMA_ZERO;
	ret = smmu_mem_alloc(SMMU_RWERRADDR_SIZE, &gAllocMem_WR);
	if (ret != SMMU_OK) {
		dprint(KERN_CRIT, "%s kzalloc mem for smmu wrerr failed\n", __func__);
		smmu_mem_dealloc(&gAllocMem_RD);
		return SMMU_ERR;
	}

	return SMMU_OK;
}
#endif

/**
 *function: init SMMU global registers.
 */
void SMMU_InitGlobalReg(unsigned long long smmuPageBase)
{
	unsigned int i = 0;
	if (gSmmuInitFlag == 1) {
		//0000 0000 0000 1111 0000 0000 0011 1000 --> 0x000f0038
		set_common_reg(SMMU_SCR, 0x0, 1, 0);    //SMMU_SCR[0].glb_bypass
		set_common_reg(SMMU_SCR, 0x7, 3, 3);    //SMMU_SCR[3].int_en, SMMU_SCR[4].cache_l1_en, SMMU_SCR[5].cache_l2_en
		set_common_reg(SMMU_SCR, 0xF, 4, 16);    //SMMU_SCR[19:16].ptw_pf, Maximum read outstanding of PTW request.
		set_common_reg(SMMU_SCR, 0x0, 8, 20);    //SMMU_SCR[27:20].ptw_mid, MID of PTW request.

		//SMRX_S had set default value. Only need to set SMMU_SMRx_NS secure SID  bypass
		//SMMU_SMRx[0]smr_bypass=0(non-bypass); SMMU_SMRx[4]smr_invld_en=0x1; SMMU_SMRx[5:11]smr_ptw_qos=0x3;
		for (i = 0; i < SMRx_ID_SIZE; i += 2) {
			set_common_reg(SMMU_SMRx_NS + i * 0x4, 0x70, 32, 0);    //0x00000003 none secure
		}

		for (i = 1; i < SMRx_ID_SIZE; i += 2) {
			set_common_reg(SMMU_SMRx_NS + i * 0x4, 0x71, 32, 0);    //0x00000002 secure
		}

		set_common_reg(SMMU_RLD_EN0_NS, 0x0, 32, 0);
		set_common_reg(SMMU_RLD_EN1_NS, 0x0, 32, 0);

		set_common_reg(SMMU_CB_TTBR0, smmuPageBase , 32, 0);
		set_common_reg(SMMU_CB_TTBCR, 0x1, 1, 0);

		if (gAllocMem_RD.PhyAddr != 0 && gAllocMem_WR.PhyAddr != 0) {
			set_common_reg(SMMU_ERR_RDADDR, gAllocMem_RD.PhyAddr, 32, 0);
			set_common_reg(SMMU_ERR_WRADDR, gAllocMem_WR.PhyAddr, 32, 0);
		}
		//glb_bypass, 0x0: normal mode, 0x1: bypass mode
		set_master_reg(SMMU_MSTR_GLB_BYPASS, 0x0, 32, 0);    //master mmu enable
	} else {
		return;
	}
}

/**
 *function: set MFDE/SCD/BPD mmu cfg register, MMU or secure.
 */
void SMMU_SetMasterReg(SMMU_MASTER_TYPE master_type, unsigned char secure_en, unsigned char mmu_en)
{
	switch (master_type) {
	case MFDE:
		set_mmu_cfg_reg_mfde(master_type, secure_en, mmu_en);
		break;

	case SCD:
		set_mmu_cfg_reg_scd(master_type, secure_en, mmu_en);
		break;

	case BPD:
		set_mmu_cfg_reg_bpd(master_type, secure_en, mmu_en);
		break;

	default:
		dprint(KERN_CRIT, "%s unkown master type %d\n", __func__, master_type);
		break;
	}
}

void SMMU_IntServProc(void)
{
	 int tmp = -1;
	dprint(KERN_ALERT, "%s enter", __func__);
	RD_SMMU_VREG(gSmmuRegVir.pSMMUCommonBaseVirAddr , SMMU_INTSTAT_NS , tmp); 
	dprint(KERN_ALERT, "SMMU_INTSTAT_NS : 0x%x\n", tmp);
	RD_SMMU_VREG(gSmmuRegVir.pSMMUMasterBaseVirAddr , SMMU_MSTR_DBG_0, tmp);
	dprint(KERN_ALERT, "SMMU_MSTR_DBG_0 : 0x%x\n", tmp);
	RD_SMMU_VREG(gSmmuRegVir.pSMMUMasterBaseVirAddr , SMMU_MSTR_DBG_1, tmp);
	dprint(KERN_ALERT, "SMMU_MSTR_DBG_1 : 0x%x\n", tmp);
	RD_SMMU_VREG(gSmmuRegVir.pSMMUMasterBaseVirAddr , SMMU_MSTR_DBG_2, tmp);
	dprint(KERN_ALERT, "SMMU_MSTR_DBG_2 : 0x%x\n", tmp);
	RD_SMMU_VREG(gSmmuRegVir.pSMMUMasterBaseVirAddr , SMMU_MSTR_DBG_3, tmp);
	dprint(KERN_ALERT, "SMMU_MSTR_DBG_3 : 0x%x\n", tmp);
	RD_SMMU_VREG(gSmmuRegVir.pSMMUMasterBaseVirAddr , SMMU_MSTR_DBG_4, tmp);
	dprint(KERN_ALERT, "SMMU_MSTR_DBG_4 : 0x%x\n", tmp);
	RD_SMMU_VREG(gSmmuRegVir.pSMMUMasterBaseVirAddr , SMMU_MSTR_DBG_5, tmp);
	dprint(KERN_ALERT, "SMMU_MSTR_DBG_5 : 0x%x\n", tmp);
	RD_SMMU_VREG(gSmmuRegVir.pSMMUMasterBaseVirAddr , SMMU_MSTR_DBG_6, tmp);
	dprint(KERN_ALERT, "SMMU_MSTR_DBG_6 : 0x%x\n", tmp);
	dprint(KERN_ALERT, "%s end", __func__);
}

/**
 *function: get registers virtual address, and alloc mem for TLB miss.
 */
 int SMMU_Init(unsigned int VdhRegBaseAddr)
{
	int ret = SMMU_ERR;
    unsigned int ScdRegBaseAddr = VdhRegBaseAddr + SCD_REG_OFFSET ;
    unsigned int BpdRegBaseAddr = VdhRegBaseAddr + BPD_REG_OFFSET;
	memset(&gSmmuRegVir, 0, sizeof(gSmmuRegVir));

	gSmmuRegVir.pSMMUMFDERegVirAddr = ( int *) MEM_Phy2Vir(VdhRegBaseAddr);
	if (gSmmuRegVir.pSMMUMFDERegVirAddr == NULL) {
		dprint(KERN_CRIT, "%s pSMMUMFDERegVirAddr is NULL, SMMU Init failed\n", __func__);
		return SMMU_ERR;
	}

	gSmmuRegVir.pSMMUSCDRegVirAddr = ( int *) MEM_Phy2Vir(ScdRegBaseAddr);
	if (gSmmuRegVir.pSMMUSCDRegVirAddr == NULL) {
		dprint(KERN_CRIT, "%s pSMMUSCDRegVirAddr is NULL, SMMU Init failed\n", __func__);
		return SMMU_ERR;
	}

	gSmmuRegVir.pSMMUBPDRegVirAddr = ( int *) MEM_Phy2Vir(BpdRegBaseAddr);
	if (gSmmuRegVir.pSMMUBPDRegVirAddr == NULL) {
		dprint(KERN_CRIT, "%s pSMMUBPDRegVirAddr is NULL, SMMU Init failed\n", __func__);
		return SMMU_ERR;
	}

	gSmmuRegVir.pSMMUCommonBaseVirAddr = ( int *) MEM_Phy2Vir(VdhRegBaseAddr + 0xE000);
	if (gSmmuRegVir.pSMMUCommonBaseVirAddr == NULL) {
		dprint(KERN_CRIT, "%s pSMMUCommonBaseVirAddr is NULL, SMMU Init failed\n", __func__);
		return SMMU_ERR;
	}

	gSmmuRegVir.pSMMUMasterBaseVirAddr = ( int *) MEM_Phy2Vir(VdhRegBaseAddr + 0xF000);
	if (gSmmuRegVir.pSMMUMasterBaseVirAddr == NULL) {
		dprint(KERN_CRIT, "%s pSMMUMasterBaseVirAddr is NULL, SMMU Init failed\n", __func__);
		return SMMU_ERR;
	}

	memset(&gAllocMem_RD, 0, sizeof(gAllocMem_RD));
	memset(&gAllocMem_WR, 0, sizeof(gAllocMem_WR));

#ifdef ENV_ARMLINUX_KERNEL
	ret = alloc_smmu_tlb_miss_addr();
	if (ret != SMMU_OK) {
		dprint(KERN_CRIT, "%s alloc_smmu_tlb_miss_addr failed\n", __func__);
		return SMMU_ERR;
	}
#endif

	gSmmuInitFlag = 1;
	return SMMU_OK;
}

/**
 *function: free mem of SMMU_ERR_RDADDR and SMMU_ERR_WRADDR.
 */
void SMMU_DeInit(void)
{
	if (gAllocMem_RD.PhyAddr != 0)
		smmu_mem_dealloc(&gAllocMem_RD);

	if (gAllocMem_WR.PhyAddr != 0)
		smmu_mem_dealloc(&gAllocMem_WR);
}
