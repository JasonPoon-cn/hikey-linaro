/*
 *      Video Decode driver for hikey960   vdm hal .h file 
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

#include "sysconfig.h"
#include "vfmw.h"
#include "mem_manage.h"
#include "scd_drv.h"
#include "vdm_hal_api.h"
#include "vdm_hal_local.h"
#ifdef VFMW_MPEG2_SUPPORT
 int MP2HAL_StartDec(OMXVDH_REG_CFG_S *pVdhRegCfg);
#endif
#ifdef VFMW_H264_SUPPORT
 int H264HAL_StartDec(OMXVDH_REG_CFG_S *pVdhRegCfg);
#endif
#ifdef VFMW_HEVC_SUPPORT
 int HEVCHAL_StartDec(OMXVDH_REG_CFG_S *pVdhRegCfg);
#endif
#ifdef VFMW_MPEG4_SUPPORT
 int MP4HAL_StartDec(OMXVDH_REG_CFG_S *pVdhRegCfg);
#endif
#ifdef VFMW_REAL8_SUPPORT
 int RV8HAL_StartDec(OMXVDH_REG_CFG_S *pVdhRegCfg);
#endif
#ifdef VFMW_REAL9_SUPPORT
 int RV9HAL_StartDec(OMXVDH_REG_CFG_S *pVdhRegCfg);
#endif
#ifdef VFMW_VC1_SUPPORT
 int VC1HAL_StartDec(OMXVDH_REG_CFG_S *pVdhRegCfg);
#endif
#ifdef VFMW_DIVX3_SUPPORT
 int DIVX3HAL_StartDec(OMXVDH_REG_CFG_S *pVdhRegCfg);
#endif
#ifdef VFMW_VP8_SUPPORT
 int VP8HAL_StartDec(OMXVDH_REG_CFG_S *pVdhRegCfg);
#endif
#include "vfmw_intf.h"
#ifdef HIVDEC_SMMU_SUPPORT
#include "smmu.h"
#endif

#define    ALL_RESET_OK_BIT                    (0)
#define    MFDE_RESET_OK_BIT                   (1)
#define    ALL_RESET_CTRL_BIT                  (0)
#define    MFDE_RESET_CTRL_BIT                 (1)

#define    VREG_VDH_START                      (0x000)
#define    REG_VDH_SELRST                      (0x0008)
#define    VREG_VDH_STATE                      (0x01c)
#define    VREG_VCTRL_STATE                    (0x028)
//performance count registers
#define    VREG_DEC_CYCLEPERPIC                (0x0B0)
#define    VREG_RD_BDWIDTH_PERPIC              (0x0B4)
#define    VREG_WR_BDWIDTH_PERPIC              (0x0B8)
#define    VREG_RD_REQ_PERPIC                  (0x0BC)
#define    VREG_WR_REQ_PERPIC                  (0x0C0)
#define    VREG_MB0_QP_IN_CURR_PIC             (0x0D0)
#define    VREG_SWITCH_ROUNDING                (0x0D4)
//sed registers
#define    VREG_SED_STA                        (0x1000)
#define    VREG_SED_END0                       (0x1014)
#define    VREG_LUMA_HISTORGRAM                (0x8100)
#define    VREG_LUMA_SUM_LOW                   (0x8180)
#define    VREG_LUMA_SUM_HIGH                  (0x8184)
//clock div offset
#define    PERI_CRG_CORE_DIV                   (0xCC)
#define    PERI_CRG_AXI_DIV                    (0xD0)
//mask & clear
#define    VREG_INT_STATE                      (0x020)
#define    VREG_INT_MASK                       (0x024)

typedef struct {	// vdm register base vir addr
     union {
         struct {
         int *paddr[3] ;
         } ;
         struct {
         int *pVdmRegVirAddr;
         int *pPERICRGVirAddr;
         int *pBpdRegVirAddr;
         };
     } ;
} VDMHAL_HWMEM_S;

static VDMHAL_HWMEM_S  g_HwMem;
static VDMHAL_BACKUP_S g_VdmRegState;
static VDMDRV_SLEEP_STAGE_E s_eVdmDrvSleepState = VDMDRV_SLEEP_STAGE_NONE;
static VDMDRV_STATEMACHINE_E s_VdmState = VDM_IDLE_STATE;
static OSAL_EVENT g_VdmHwDoneEvent;
static struct semaphore g_VDHSem;

void VDMHAL_IMP_GlbReset(unsigned int VdhRegBaseAddr);
void VDMHAL_IMP_ClearIntState(unsigned int VdhRegBaseAddr);
int VDMHAL_IMP_CheckReg(REG_ID_E reg_id , unsigned int VdhRegBaseAddr);
void VDMHAL_IMP_StartHwRepair(unsigned int VdhRegBaseAddr);
void VDMHAL_IMP_StartHwDecode(unsigned int VdhRegBaseAddr);
int VDMHAL_IMP_PrepareDec(OMXVDH_REG_CFG_S *pVdhRegCfg , unsigned int VdhRegBaseAddr);
int VDMHAL_IMP_PrepareRepair(OMXVDH_REG_CFG_S *pVdhRegCfg , unsigned int VdhRegBaseAddr);
int VDMHAL_IMP_BackupInfo(unsigned int VdhRegBaseAddr);

inline unsigned int VDMHAL_GetIntMaskCfg( int ModuleLowlyEnable)
{
	unsigned int D32 = 0;
#ifdef VDM_BUSY_WAITTING
	D32 = 0xFFFFFFFF;
#else
	if (ModuleLowlyEnable == 1)
		D32 = 0xFFFFFFFA;
	else
		D32 = 0xFFFFFFFE;
#endif
	return D32;
}

void VDMHAL_EnableInt(unsigned int VdhRegBaseAddr)
{
	unsigned int D32  = 0;
	int *p32 = NULL;
	int ModuleLowlyEnable = 0;
	ModuleLowlyEnable        = 0;
    
	if (g_HwMem.pVdmRegVirAddr == NULL) {
		if ((p32 = ( int *) MEM_Phy2Vir(VdhRegBaseAddr)) != NULL) {
			g_HwMem.pVdmRegVirAddr = p32;
		} else {
			dprint(KERN_CRIT, "vdm register virtual address not mapped, reset failed\n");
			return;
		}
	}

	D32 = VDMHAL_GetIntMaskCfg(ModuleLowlyEnable);
	write_register_value(0 , VREG_INT_MASK, D32);
}

unsigned int VDMHAL_ReduceCoreFrequency(void)
{
	unsigned int div_value = 0;
	unsigned int sc_div_vdec = 0;
	read_register_value(1 , PERI_CRG_CORE_DIV, &sc_div_vdec);   
	div_value = sc_div_vdec & 0x1F;
	sc_div_vdec = ((sc_div_vdec & 0x1F) << 1) + 1;   
	if (sc_div_vdec > 0x1F)
		sc_div_vdec = 0x1F;

	write_register_value(1 , PERI_CRG_CORE_DIV, 0x1F0000 | sc_div_vdec);
	udelay(1);
	return div_value;
}

void VDMHAL_RestoreCoreFrequency(unsigned int DivValue)
{
	write_register_value(1  , PERI_CRG_CORE_DIV, 0x1F0000 | DivValue);
	udelay(1);
}

unsigned int VDMHAL_ReduceAXIFrequency(void)
{
	unsigned int div_value = 0;
	unsigned int sc_div_vcodecbus = 0;
	read_register_value(1 , PERI_CRG_AXI_DIV, &sc_div_vcodecbus);
	div_value = sc_div_vcodecbus & 0x1F;
	sc_div_vcodecbus = ((sc_div_vcodecbus & 0x1F) << 1) + 1;
	if (sc_div_vcodecbus > 0x1F)
		sc_div_vcodecbus = 0x1F;
	write_register_value(1 , PERI_CRG_AXI_DIV, 0x1F0000 | sc_div_vcodecbus);
	udelay(1);
	return div_value;
}

void VDMHAL_RestoreAXIFrequency(unsigned int DivValue)
{
	write_register_value(1 , PERI_CRG_AXI_DIV, 0x1F0000 | DivValue);
	udelay(1);
}

 int VDMHAL_CfgRpReg(OMXVDH_REG_CFG_S *pVdhRegCfg)
{
	 int D32 = 0;
	write_register_value(0 , VREG_AVM_ADDR, pVdhRegCfg->VdhAvmAddr);
	D32 = 0x2000C203;
	write_register_value(0  , VREG_BASIC_CFG1, D32);
	D32 = 0x00300C03;
	write_register_value(0  , VREG_SED_TO, D32);
	write_register_value(0  , VREG_ITRANS_TO, D32);
	write_register_value(0  , VREG_PMV_TO, D32);
	write_register_value(0  , VREG_PRC_TO, D32);
	write_register_value(0  , VREG_RCN_TO, D32);
	write_register_value(0  , VREG_DBLK_TO, D32);
	write_register_value(0  , VREG_PPFD_TO, D32);
	return 0;
}

void VDMHAL_IMP_Init(unsigned int VdhRegBaseAddr , unsigned int PERICRG_RegBaseAddr)
{ 
   unsigned int BpdRegBaseAddr = VdhRegBaseAddr + BPD_REG_OFFSET;
	memset(&g_HwMem, 0, sizeof(g_HwMem));
	memset(&g_VdmRegState, 0, sizeof(g_VdmRegState));
	g_HwMem.pVdmRegVirAddr  = ( int *) MEM_Phy2Vir(VdhRegBaseAddr);
	g_HwMem.pPERICRGVirAddr = ( int *) MEM_Phy2Vir(PERICRG_RegBaseAddr);
	g_HwMem.pBpdRegVirAddr  = ( int *) MEM_Phy2Vir(BpdRegBaseAddr);
	VDMHAL_IMP_GlbReset(VdhRegBaseAddr);
	VDMHAL_IMP_WriteScdEMARID();
	s_eVdmDrvSleepState = VDMDRV_SLEEP_STAGE_NONE;
	s_VdmState = VDM_IDLE_STATE; 
    g_VdmHwDoneEvent.flag = 0;
	init_waitqueue_head(&(g_VdmHwDoneEvent.queue_head));   
    sema_init(&g_VDHSem , 1) ;
}

void VDMHAL_IMP_DeInit(void)
{
	s_eVdmDrvSleepState = VDMDRV_SLEEP_STAGE_NONE;
	s_VdmState = VDM_IDLE_STATE;
}

void VDMHAL_IMP_ResetVdm(unsigned int VdhRegBaseAddr)
{
	 int i;
	 int tmp = 0;
	unsigned int reg;
	unsigned int reg_rst_ok;
	unsigned int div_value;
	unsigned int *pVdmResetVirAddr;
	unsigned int *pVdmResetOkVirAddr;
	unsigned int SOFTRST_REQ_Addr    = VdhRegBaseAddr + SOFTRST_REQ_OFFSET;
	unsigned int SOFTRST_OK_ADDR     = VdhRegBaseAddr + SOFTRST_OK_OFFSET;   
	pVdmResetVirAddr   = ( int *) MEM_Phy2Vir(SOFTRST_REQ_Addr);
	pVdmResetOkVirAddr = ( int *) MEM_Phy2Vir(SOFTRST_OK_ADDR);
	if (pVdmResetVirAddr == NULL || pVdmResetOkVirAddr == NULL) {
		dprint(KERN_CRIT, "VDMHAL_ResetVdm: map vdm register fail\n");
		return;
	}

	read_register_value(0 ,VREG_INT_MASK, &tmp);
	div_value = VDMHAL_EXT_ReduceFrequency(MFDE_RESET_REQUIRE);

	reg = *(volatile unsigned int *)pVdmResetVirAddr;
	*(volatile unsigned int *)pVdmResetVirAddr = reg | (unsigned int) (1 << MFDE_RESET_CTRL_BIT);

	for (i = 0; i < 100; i++) {
		reg_rst_ok = *(volatile unsigned int *)pVdmResetOkVirAddr;
		if (reg_rst_ok & (1 << MFDE_RESET_OK_BIT))
			break;
		udelay(10);
	}

	if (i >= 100)
		dprint(KERN_CRIT, "%s reset failed\n", __func__);

	*(volatile unsigned int *)pVdmResetVirAddr = reg & (unsigned int) (~(1 << MFDE_RESET_CTRL_BIT));
	VDMHAL_EXT_RestoreFrequency(MFDE_RESET_REQUIRE, div_value);
	write_register_value(0  , VREG_INT_MASK, tmp);
	s_VdmState = VDM_IDLE_STATE;
}

void VDMHAL_IMP_GlbReset(unsigned int VdhRegBaseAddr)
{
	 int i;
	unsigned int reg, reg_rst_ok;
	unsigned int div_value;
	unsigned int *pResetVirAddr   = NULL;
	unsigned int *pResetOKVirAddr = NULL;
 	unsigned int SOFTRST_REQ_Addr    = VdhRegBaseAddr + SOFTRST_REQ_OFFSET;
	unsigned int SOFTRST_OK_ADDR     = VdhRegBaseAddr + SOFTRST_OK_OFFSET;   
	pResetVirAddr   = ( int *) MEM_Phy2Vir(SOFTRST_REQ_Addr);
	pResetOKVirAddr = ( int *) MEM_Phy2Vir(SOFTRST_OK_ADDR);
	if (pResetVirAddr == NULL || pResetOKVirAddr == NULL) {
		dprint(KERN_CRIT, "VDMHAL_GlbReset: map vdm register fail\n");
		return;
	}

	div_value = VDMHAL_EXT_ReduceFrequency(GLB_RESET_REQUIRE);
	reg = *(volatile unsigned int *)pResetVirAddr;
	*(volatile unsigned int *)pResetVirAddr = reg | (unsigned int) (1 << ALL_RESET_CTRL_BIT);
	for (i = 0; i < 100; i++) {
		reg_rst_ok = *(volatile unsigned int *)pResetOKVirAddr;
		if (reg_rst_ok & (1 << ALL_RESET_OK_BIT))
			break;
		udelay(10);
	}

	if (i >= 100)
		dprint(KERN_CRIT, "Glb Reset Failed\n");

	*(volatile unsigned int *)pResetVirAddr = reg & (unsigned int) (~(1 << ALL_RESET_CTRL_BIT));
	VDMHAL_EXT_RestoreFrequency(GLB_RESET_REQUIRE, div_value);
}

void VDMHAL_IMP_ClearIntState(unsigned int VdhRegBaseAddr)
{
	int *p32;
	int D32 = 0xFFFFFFFF;
	if (g_HwMem.pVdmRegVirAddr == NULL) {
		if ((p32 = ( int *) MEM_Phy2Vir(VdhRegBaseAddr)) != NULL) {
			g_HwMem.pVdmRegVirAddr = p32;
		} else {
			dprint(KERN_CRIT, " %s %d vdm register virtual address not mapped, reset failed\n", __func__, __LINE__);
			return;
		}
	}

	write_register_value(0  , VREG_INT_STATE, D32);
}

 int VDMHAL_IMP_CheckReg(REG_ID_E reg_id , unsigned int VdhRegBaseAddr)
{
	 int *p32;
	 int dat = 0;
	unsigned int reg_type;


	if (g_HwMem.pVdmRegVirAddr == NULL) {
		if ((p32 = ( int *) MEM_Phy2Vir(VdhRegBaseAddr)) != NULL) {
			g_HwMem.pVdmRegVirAddr = p32;
		} else {
			dprint(KERN_CRIT, " %s %d vdm register virtual address not mapped, reset failed\n", __func__, __LINE__);
			return 0;
		}
	}

	switch (reg_id) {
	case VDH_STATE_REG:
		reg_type = VREG_VDH_STATE;
		break;

	case INT_STATE_REG:
		reg_type = VREG_INT_STATE;
		break;

	case INT_MASK_REG:
		reg_type = VREG_INT_MASK;
		break;

	case VCTRL_STATE_REG:
		reg_type = VREG_VCTRL_STATE;
		break;

	default:
		dprint(KERN_CRIT, "%s: unkown reg_id is %d\n", __func__, reg_id);
		return 0;
	}

	read_register_value(0 , reg_type , &dat);
	return dat;
}

 int VDMHAL_IMP_PrepareDec(OMXVDH_REG_CFG_S *pVdhRegCfg , unsigned int VdhRegBaseAddr)
{
	VDMHAL_HWMEM_S *pHwMem = &(g_HwMem);
	 int *p32;

	if (NULL == pVdhRegCfg)	{
		dprint(KERN_CRIT, "%s: parameter is NULL\n", __func__);
		return -1;
	}

	if (NULL == pHwMem->pVdmRegVirAddr)	{
		if (NULL != (p32 = ( int *)MEM_Phy2Vir(VdhRegBaseAddr))) {
			pHwMem->pVdmRegVirAddr = p32;
		} else {
			dprint(KERN_CRIT, "vdm register virtual address not mapped, VDMHAL_PrepareDecfailed\n");
			return -1;
		}
	}

	if (VFMW_AVS == pVdhRegCfg->VidStd)
		WR_SCDREG(REG_AVS_FLAG, 0x00000001);
	else
		WR_SCDREG(REG_AVS_FLAG, 0x00000000);

	WR_SCDREG(REG_VDH_SELRST, 0x00000001);

	switch (pVdhRegCfg->VidStd) {
#ifdef VFMW_H264_SUPPORT
	case VFMW_H264:
		return H264HAL_StartDec(pVdhRegCfg);
		break;
#endif
#ifdef VFMW_HEVC_SUPPORT
	case VFMW_HEVC:
		return HEVCHAL_StartDec(pVdhRegCfg);
		break;
#endif
#ifdef VFMW_MPEG2_SUPPORT
	case VFMW_MPEG2:
		return MP2HAL_StartDec(pVdhRegCfg);
		break;
#endif
#ifdef VFMW_MPEG4_SUPPORT
	case VFMW_MPEG4:
		return MP4HAL_StartDec(pVdhRegCfg);
		break;
#endif
#ifdef VFMW_REAL8_SUPPORT
	case VFMW_REAL8:
		return RV8HAL_StartDec(pVdhRegCfg);
		break;
#endif
#ifdef VFMW_REAL9_SUPPORT
	case VFMW_REAL9:
		return RV9HAL_StartDec(pVdhRegCfg);
		break;
#endif
#ifdef VFMW_DIVX3_SUPPORT
	case VFMW_DIVX3:
		return DIVX3HAL_StartDec(pVdhRegCfg);
		break;
#endif
#ifdef VFMW_VC1_SUPPORT
	case VFMW_VC1:
		return VC1HAL_StartDec(pVdhRegCfg);
		break;
#endif
#ifdef VFMW_VP8_SUPPORT
	case VFMW_VP8:
		return VP8HAL_StartDec(pVdhRegCfg);
		break;
#endif
#ifdef VFMW_MVC_SUPPORT
	case VFMW_MVC:
		return H264HAL_StartDec(pVdhRegCfg);
		break;
#endif
	default:
		break;
	}

	return -1;
}

 int VDMHAL_WaitEvent(unsigned int uiTimes)
{
     int sret = wait_event_interruptible_timeout((g_VdmHwDoneEvent.queue_head), (g_VdmHwDoneEvent.flag != 0), (msecs_to_jiffies(uiTimes)));
	g_VdmHwDoneEvent.flag = 0;
	return (sret != 0) ? 0 : -1;
}

 int VDMHAL_IsVdmRun()
{
	int Data32 = 0;   
	if (g_HwMem.pVdmRegVirAddr == NULL) {
		dprint(KERN_CRIT, "VDM register not mapped yet\n");
		return 0;
	}

	read_register_value(0 ,VREG_VCTRL_STATE, &Data32);
	if (Data32 == 1)
		return 0;
    return 1;    //work
}

 int VDMHAL_IMP_BackupInfo(unsigned int VdhRegBaseAddr)
{
	 int i = 0;
	g_VdmRegState.Int_State_Reg = VDMHAL_IMP_CheckReg(INT_STATE_REG , VdhRegBaseAddr);
	read_register_value(0 ,VREG_BASIC_CFG1, &g_VdmRegState.BasicCfg1);
	read_register_value(0 ,VREG_VDH_STATE, &g_VdmRegState.VdmState);
	read_register_value(0 ,VREG_MB0_QP_IN_CURR_PIC, &g_VdmRegState.Mb0QpInCurrPic);
	read_register_value(0 ,VREG_SWITCH_ROUNDING, &g_VdmRegState.SwitchRounding);
	{
		read_register_value(0 ,VREG_SED_STA, &g_VdmRegState.SedSta);
		read_register_value(0 ,VREG_SED_END0, &g_VdmRegState.SedEnd0);
		read_register_value(0 ,VREG_DEC_CYCLEPERPIC, &g_VdmRegState.DecCyclePerPic);
		read_register_value(0 ,VREG_RD_BDWIDTH_PERPIC, &g_VdmRegState.RdBdwidthPerPic);
		read_register_value(0 ,VREG_WR_BDWIDTH_PERPIC, &g_VdmRegState.WrBdWidthPerPic);
		read_register_value(0 ,VREG_RD_REQ_PERPIC, &g_VdmRegState.RdReqPerPic);
		read_register_value(0 ,VREG_WR_REQ_PERPIC, &g_VdmRegState.WrReqPerPic);
		read_register_value(0 ,VREG_LUMA_SUM_LOW, &g_VdmRegState.LumaSumLow);
		read_register_value(0 ,VREG_LUMA_SUM_HIGH, &g_VdmRegState.LumaSumHigh);
	}

	for (i = 0; i < 32; i++) {
		read_register_value(0 ,VREG_LUMA_HISTORGRAM + i * 4, &g_VdmRegState.LumaHistorgam[i]);
	}

	return 0;
}

void VDMHAL_GetRegState(VDMHAL_BACKUP_S *pVdmRegState)
{
	memcpy(pVdmRegState, &g_VdmRegState, sizeof(g_VdmRegState));
	s_VdmState = VDM_IDLE_STATE;
}

 int VDMHAL_IMP_PrepareRepair(OMXVDH_REG_CFG_S *pVdhRegCfg , unsigned int VdhRegBaseAddr)
{
	VDMHAL_HWMEM_S *pHwMem = &(g_HwMem);
	 int *p32;
	if (NULL == pVdhRegCfg)	{
		dprint(KERN_CRIT, "%s: parameter is NULL\n", __func__);
		return -1;
	}

	if ( NULL == pHwMem->pVdmRegVirAddr) {
		if ( NULL != (p32 = ( int *)MEM_Phy2Vir(VdhRegBaseAddr))) {
			pHwMem->pVdmRegVirAddr = p32;
		} else {
			dprint(KERN_CRIT, "vdm register virtual address not mapped, VDMHAL_PrepareRepair failed\n");
			return -1;
		}
	}

	if (pVdhRegCfg->RepairTime == FIRST_REPAIR) {
		if (pVdhRegCfg->ValidGroupNum0 > 0)
			VDMHAL_CfgRpReg(pVdhRegCfg);
		else
			return -1;
	} else if (pVdhRegCfg->RepairTime == SECOND_REPAIR) {
		dprint(KERN_CRIT, "SECOND_REPAIR Parameter Error\n");
		return -1;
	}

	return 0;
}

void VDMHAL_IMP_StartHwRepair(unsigned int VdhRegBaseAddr)
{
	int D32 = 0;
   
	read_register_value(0 ,VREG_BASIC_CFG0, &D32);
	D32 = 0x4000000;
	write_register_value(0  , VREG_BASIC_CFG0, D32);
#ifdef HIVDEC_SMMU_SUPPORT
	SMMU_SetMasterReg(MFDE, SECURE_OFF, SMMU_ON);
#endif
	VDMHAL_IMP_ClearIntState(VdhRegBaseAddr);
	VDMHAL_EnableInt(VdhRegBaseAddr);
	mb();
	write_register_value(0  , VREG_VDH_START, 0);
	write_register_value(0  , VREG_VDH_START, 1);
	write_register_value(0  , VREG_VDH_START, 0);
}

void VDMHAL_IMP_StartHwDecode(unsigned int VdhRegBaseAddr)
{
#ifdef HIVDEC_SMMU_SUPPORT
	SMMU_SetMasterReg(MFDE, SECURE_OFF, SMMU_ON);
#endif
	VDMHAL_IMP_ClearIntState(VdhRegBaseAddr);
	VDMHAL_EnableInt(VdhRegBaseAddr);
	mb();
	write_register_value(0  , VREG_VDH_START, 0);
	write_register_value(0  , VREG_VDH_START, 1);
	write_register_value(0  , VREG_VDH_START, 0);
}

void VDMHAL_IMP_WriteScdEMARID(void)
{
    #define   DEFAULT_EMAR_ID_VALUE        (0x161f7)
	WR_SCDREG(REG_EMAR_ID, DEFAULT_EMAR_ID_VALUE);
}

unsigned int VDMHAL_EXT_ReduceFrequency(RESET_REQUIRE_TYPE_E eResetRequire)
{
	unsigned int div_value;
	if (eResetRequire == SCD_RESET_REQUIRE || eResetRequire == MFDE_RESET_REQUIRE)
		div_value = VDMHAL_ReduceCoreFrequency();
	else
		div_value = VDMHAL_ReduceAXIFrequency();

	return div_value;
}

void VDMHAL_EXT_RestoreFrequency(RESET_REQUIRE_TYPE_E eResetRequire, unsigned int DivValue)
{
	if (eResetRequire == SCD_RESET_REQUIRE || eResetRequire == MFDE_RESET_REQUIRE)
		VDMHAL_RestoreCoreFrequency(DivValue);
	else
		VDMHAL_RestoreAXIFrequency(DivValue);
}

void  VDMHAL_ISR(unsigned int VdhRegBaseAddr)
{
	VDMHAL_IMP_BackupInfo(VdhRegBaseAddr);
	VDMHAL_IMP_ClearIntState(VdhRegBaseAddr);
    g_VdmHwDoneEvent.flag = 1;
	wake_up_interruptible(&(g_VdmHwDoneEvent.queue_head));    
}

void VDMHAL_AfterDec(OMXVDH_REG_CFG_S *pVdhRegCfg , unsigned int VdhRegBaseAddr)
{
	s_VdmState = VDM_DECODE_STATE;
	if (pVdhRegCfg->VdmStateMachine == VDM_DECODE_STATE) {
		if (pVdhRegCfg->ErrRationAndRpStratageFlag)
			VDMHAL_ActivateVDH(pVdhRegCfg , VdhRegBaseAddr);
		else if (VDMHAL_IMP_PrepareRepair(pVdhRegCfg , VdhRegBaseAddr) == 0)
			VDMHAL_IMP_StartHwRepair(VdhRegBaseAddr);
	} else if (pVdhRegCfg->VdmStateMachine == VDM_REPAIR_STATE_0) {
		if (pVdhRegCfg->AvsSecondFld == 1)
			;
		else if (pVdhRegCfg->IsMpeg4Nvop == 1)
#ifdef VFMW_MPEG4_SUPPORT
			VDMHAL_ActivateVDH(pVdhRegCfg , VdhRegBaseAddr);
#endif
		else if (pVdhRegCfg->IsVc1Skpic == 1)    //vc1 copy
#ifdef VFMW_VC1_SUPPORT
			VDMHAL_ActivateVDH(pVdhRegCfg , VdhRegBaseAddr);
#endif
		else
			VDMHAL_ActivateVDH(pVdhRegCfg , VdhRegBaseAddr);
	} else if (pVdhRegCfg->VdmStateMachine == VDM_REPAIR_STATE_1) {

		VDMHAL_ActivateVDH(pVdhRegCfg , VdhRegBaseAddr);
	}
}

void VDMHAL_ActivateVDH(OMXVDH_REG_CFG_S *pVdhRegCfg , unsigned int VdhRegBaseAddr)
{
	s_VdmState = VDM_DECODE_STATE;
#ifdef VFMW_MPEG4_SUPPORT
	if (pVdhRegCfg->IsMpeg4Nvop == 1) {
		VDMHAL_IMP_PrepareRepair(pVdhRegCfg , VdhRegBaseAddr);
		VDMHAL_IMP_StartHwRepair(VdhRegBaseAddr);
	}
#endif
#ifdef VFMW_VC1_SUPPORT
	else if (pVdhRegCfg->IsVc1Skpic == 1) {
		VDMHAL_IMP_PrepareRepair(pVdhRegCfg , VdhRegBaseAddr);
		VDMHAL_IMP_StartHwRepair(VdhRegBaseAddr);
	}
#endif
	else {
		VDMHAL_IMP_PrepareDec(pVdhRegCfg , VdhRegBaseAddr);
		VDMHAL_IMP_StartHwDecode(VdhRegBaseAddr);
	}
}

 int VDMHAL_PrepareSleep(void)
{
	 int ret = 0;
	if (down_interruptible(&g_VDHSem))
        return -1;
	if (s_eVdmDrvSleepState == VDMDRV_SLEEP_STAGE_NONE) {
		if (VDM_IDLE_STATE == s_VdmState) {
			dprint(KERN_ALERT, "%s, idle state \n", __func__);
			s_eVdmDrvSleepState = VDMDRV_SLEEP_STAGE_SLEEP;
		} else {
			dprint(KERN_ALERT, "%s, work state \n", __func__);
			s_eVdmDrvSleepState = VDMDRV_SLEEP_STAGE_PREPARE;
		}

		ret = 0;
	} else {
		ret = -1;
	}

	up(&g_VDHSem) ;
	return ret;
}

void VDMHAL_ForceSleep(unsigned int VdhRegBaseAddr)
{
	dprint(KERN_ALERT, "%s, force state \n", __func__);
	if (down_interruptible(&g_VDHSem))
        return ;
	if (s_eVdmDrvSleepState != VDMDRV_SLEEP_STAGE_SLEEP) {
		VDMHAL_IMP_ResetVdm(VdhRegBaseAddr);
		s_eVdmDrvSleepState = VDMDRV_SLEEP_STAGE_SLEEP;
	}

	up(&g_VDHSem) ;
	dprint(KERN_CRIT, "====== forece VDMHAL sleep ======\n");
}

void VDMHAL_ExitSleep(void)
{
	if (down_interruptible(&g_VDHSem))
        return ;
	s_eVdmDrvSleepState = VDMDRV_SLEEP_STAGE_NONE;
	up(&g_VDHSem) ;
}

VDMDRV_SLEEP_STAGE_E VDMHAL_GetSleepStage(void)
{
	return s_eVdmDrvSleepState;
}

void VDMHAL_SetSleepStage(VDMDRV_SLEEP_STAGE_E sleepState)
{
	if (down_interruptible(&g_VDHSem))
        return ;
	s_eVdmDrvSleepState = sleepState;
	up(&g_VDHSem) ;
}

void  read_register_value(unsigned int index , unsigned int reg , int *val)
{
    *val = *((volatile  int*)((char*)g_HwMem.paddr[index] + reg));
}

void  write_register_value(unsigned int index , unsigned int reg , int val)
{
     *((volatile  int*)((char*)g_HwMem.paddr[index] + reg)) = val;
}
