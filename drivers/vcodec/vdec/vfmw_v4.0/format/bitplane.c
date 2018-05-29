/*
 *      Video Decode driver for hikey960   BPD master
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
 
#include "vfmw_intf.h"
#include "vdm_hal_api.h"
#include "bitplane.h"
#ifdef HIVDEC_SMMU_SUPPORT
#include "smmu.h"
#endif

#define    BPD_RESET_CTRL_BIT                 (3)
#define    BPD_RESET_OK_BIT                   (3)
#define    BPD_CLK_CTRL_OFF                   (0xaeb)
//clock switch registers
#define    VREG_CRG_CLK_EN                    (0xf804)
#define    WAIT_NO_ISR_MAX                    (400)
//control registers
#define    REG_BPD_START                      (0x000)
#define    REG_BPD_CFG0                       (0x004)
#define    REG_BPD_CFG1                       (0x008)
#define    REG_BPD_CFG2                       (0x00c)
#define    REG_BPD_CFG3                       (0x010)
#define    REG_BPD_CFG4                       (0x014)
#define    REG_BPD_CFG5                       (0x018)
#define    REG_BPD_CFG6                       (0x01c)
#define    REG_BPD_CFG7                       (0x020)
#define    REG_BPD_CFG8                       (0x024)
#define    REG_BPD_CFG9                       (0x028)
#define    REG_BPD_CFG10                      (0x02c)
#define    REG_BPD_CFG11                      (0x030)
//state registers
#define    REG_BPD_STATE                      (0x040)
//output registers
#define    REG_BPD_OUT0                       (0x050)
#define    REG_BPD_OUT1                       (0x054)
//safe register
#define    REG_BPD_INT_MASK                   (0x034)
#define    REG_BPD_INT_STATE                  (0x044)

static BPDDRV_SLEEP_STAGE_E s_eBpdDrvSleepStage = BPDDRV_SLEEP_STAGE_NONE;
static BPD_STATE_E s_BPDState = BPD_IDLE;
static struct semaphore g_BPDSem;

void BPD_Start(void);
 int BPDDRV_WaitBpdReadyIfNoIsr(unsigned int VdhRegBaseAddr);

void BPD_ClkControl_Off(void)
{
	unsigned int Data;
	read_register_value(0 ,VREG_CRG_CLK_EN, &Data);

	if (Data != BPD_CLK_CTRL_OFF)
		write_register_value(0 , VREG_CRG_CLK_EN, BPD_CLK_CTRL_OFF);
}

static  int BPD_CfgReg(OMXBPD_REG_CFG_S *pBpdRegCfg)
{
	 int D32 = 0;

	// clear int
	write_register_value(2 , REG_BPD_INT_STATE, 0xFFFFFFFF);
	((BPD_CFG0 *)(&D32))->mvmode_en        = ((BPD_CFG0 *)(&pBpdRegCfg->BpdCfg0))->mvmode_en;
	((BPD_CFG0 *)(&D32))->overflag_en      = ((BPD_CFG0 *)(&pBpdRegCfg->BpdCfg0))->overflag_en;
	((BPD_CFG0 *)(&D32))->pic_coding_type  = ((BPD_CFG0 *)(&pBpdRegCfg->BpdCfg0))->pic_coding_type;
	((BPD_CFG0 *)(&D32))->pic_structure    = ((BPD_CFG0 *)(&pBpdRegCfg->BpdCfg0))->pic_structure;
	((BPD_CFG0 *)(&D32))->profile          = ((BPD_CFG0 *)(&pBpdRegCfg->BpdCfg0))->profile;
	((BPD_CFG0 *)(&D32))->pic_mbheigt_mod3 = ((BPD_CFG0 *)(&pBpdRegCfg->BpdCfg0))->pic_mbheigt_mod3;
	((BPD_CFG0 *)(&D32))->pic_mbwidth_mod3 = ((BPD_CFG0 *)(&pBpdRegCfg->BpdCfg0))->pic_mbwidth_mod3;
	((BPD_CFG0 *)(&D32))->bit_offset       = ((BPD_CFG0 *)(&pBpdRegCfg->BpdCfg0))->bit_offset;
	((BPD_CFG0 *)(&D32))->safe_flag = 0;

	write_register_value(2 , REG_BPD_CFG0, D32);

	write_register_value(2 , REG_BPD_CFG1, pBpdRegCfg->BpdCfg1 );

	D32 = 0;
	((BPD_CFG2 *)(&D32))->pic_mbheight_m1 = ((BPD_CFG2 *)(&pBpdRegCfg->BpdCfg2))->pic_mbheight_m1;
	((BPD_CFG2 *)(&D32))->pic_mbwidth_m1  = ((BPD_CFG2 *)(&pBpdRegCfg->BpdCfg2))->pic_mbwidth_m1;
	write_register_value(2 , REG_BPD_CFG2, D32);

	write_register_value(2 , REG_BPD_CFG3, pBpdRegCfg->BpdCfg3 );

	write_register_value(2 , REG_BPD_CFG4, pBpdRegCfg->BpdCfg4);

	write_register_value(2 , REG_BPD_CFG5, pBpdRegCfg->BpdCfg5);

	write_register_value(2 , REG_BPD_CFG6, pBpdRegCfg->BpdCfg6);

	write_register_value(2 , REG_BPD_CFG7, pBpdRegCfg->BpdCfg7);

	write_register_value(2 , REG_BPD_CFG8, pBpdRegCfg->BpdCfg8);

	write_register_value(2 , REG_BPD_CFG9, pBpdRegCfg->BpdCfg9);

	write_register_value(2 , REG_BPD_CFG10, pBpdRegCfg->BpdCfg10);

	D32 = 0;
	((BPD_CFG11 *)(&D32))->axi_id             = 0;
	((BPD_CFG11 *)(&D32))->axi_rd_outstanding = 3;
	((BPD_CFG11 *)(&D32))->axi_wr_outstanding = 3;
	((BPD_CFG11 *)(&D32))->bpd_axi_sep        = 2;
	write_register_value(2 , REG_BPD_CFG11, D32 );

	return 0;
}

 int BPD_GetParam(OMXBPD_REG_STATE_S *pBpdState)
{
	unsigned int data0;
	unsigned int data1;
	BPD_OUT0 *pbpd_out0;
	BPD_OUT1 *pbpd_out1;

	read_register_value(2 , REG_BPD_OUT0, &data0);
	pbpd_out0 = (BPD_OUT0 *) (&data0);

	read_register_value(2 , REG_BPD_OUT1, &data1);
	pbpd_out1 = (BPD_OUT1 *) (&data1);

	pBpdState->BpdOut0EatenLenth = pbpd_out0->eaten_lenth;

	pBpdState->BpdOut1MvtypembMode  = pbpd_out1->MVTYPEMBMode;
	pBpdState->BpdOut1AcpredMode    = pbpd_out1->ACPREDMode;
	pBpdState->BpdOut1OverflagsMode = pbpd_out1->OVERFLAGSMode;
	pBpdState->BpdOut1FieldtxMode   = pbpd_out1->FIELDTXMode;
	pBpdState->BpdOut1DirectmbMode  = pbpd_out1->DIRECTMBMode;
	pBpdState->BpdOut1ForwardmbMode = pbpd_out1->FORWARDMBMode;
	pBpdState->BpdOut1SkipmbMode    = pbpd_out1->SKIPMBMode;
	pBpdState->BpdOut1Condover      = pbpd_out1->CONDOVER;

	return 0;
}

 int BPD_ConfigReg(OMXBPD_REG_S *pBpdReg , unsigned int VdhRegBaseAddr)
{
	 int ret;
	if (s_BPDState != BPD_IDLE)
		return -1;

	s_BPDState = BPD_WORKING;

	BPD_ClkControl_Off();
	BPD_CfgReg(&(pBpdReg->BpsRegCfg));

	BPD_Start();

	ret = BPDDRV_WaitBpdReadyIfNoIsr(VdhRegBaseAddr);
	if (ret == 0) {
		BPD_GetParam(&(pBpdReg->BpsRegState));
		s_BPDState = BPD_IDLE;
		return 0;
	}

	s_BPDState = BPD_IDLE;
	return -1;
}

void BPD_Reset(unsigned int VdhRegBaseAddr)
{
	unsigned int tmp;
	unsigned int i;
	unsigned int reg_rst_ok;
	unsigned int reg;
	unsigned int div_value;
	unsigned int *pBpdResetReg   = NULL;
	unsigned int *pBpdResetOkReg = NULL;
	unsigned int SOFTRST_REQ_Addr    = VdhRegBaseAddr + SOFTRST_REQ_OFFSET;
	unsigned int SOFTRST_OK_ADDR     = VdhRegBaseAddr + SOFTRST_OK_OFFSET;
	pBpdResetReg   = (unsigned int *) MEM_Phy2Vir(SOFTRST_REQ_Addr);
	pBpdResetOkReg = (unsigned int *) MEM_Phy2Vir(SOFTRST_OK_ADDR);

	if (pBpdResetReg == NULL || pBpdResetOkReg == NULL) {
		dprint(KERN_CRIT, "BPD_Reset: map register fail\n");
		return;
	}

	read_register_value(2 , REG_BPD_INT_MASK, &tmp);

	div_value = VDMHAL_EXT_ReduceFrequency(BPD_RESET_REQUIRE);

	/* require bpd reset */
	reg = *(volatile unsigned int *)pBpdResetReg;
	*(volatile unsigned int *)pBpdResetReg = reg | (unsigned int) (1 << BPD_RESET_CTRL_BIT);

	/*wait for rest ok */
	for (i = 0; i < 100; i++) {
		reg_rst_ok = *(volatile unsigned int *)pBpdResetOkReg;
		if (reg_rst_ok & (1 << BPD_RESET_OK_BIT))
			break;
		udelay(10);
	}

	if (i >= 100)
		dprint(KERN_CRIT, "%s reset failed\n", __func__);

	/* clear reset require */
	*(volatile unsigned int *)pBpdResetReg = reg & (unsigned int) (~(1 << BPD_RESET_CTRL_BIT));

	VDMHAL_EXT_RestoreFrequency(BPD_RESET_REQUIRE, div_value);

	write_register_value(2 , REG_BPD_INT_MASK, tmp);

	s_BPDState = BPD_IDLE;
}

void BPD_Start(void)
{
#ifdef HIVDEC_SMMU_SUPPORT
	SMMU_SetMasterReg(BPD, SECURE_OFF, SMMU_ON);
#endif

	// Start BPD
	write_register_value(2 , REG_BPD_START, 0);
	write_register_value(2 , REG_BPD_START, 1);
	write_register_value(2 , REG_BPD_START, 0);
}

 int IsBpd_Ready(void)
{
	int Data32 = 0;
	read_register_value(2 , REG_BPD_STATE, &Data32);
	Data32 = Data32 & 1;
	Data32 = (Data32 == 0) ? 0 : 1;
	return Data32;
}

 int BPDDRV_WaitBpdReadyIfNoIsr(unsigned int VdhRegBaseAddr)
{
	unsigned int cnt_wait = 0;
	unsigned int max_wait = 10 * WAIT_NO_ISR_MAX;
	unsigned int StartTimeInMs = 0;
	unsigned int CurTimeInMs = 0;

	StartTimeInMs = OSAL_GetTimeInMs();

	for (cnt_wait = 0; cnt_wait < max_wait;) {
		if (IsBpd_Ready()) {
			break;
		} else {
			CurTimeInMs = OSAL_GetTimeInMs();

			if (CurTimeInMs < StartTimeInMs)
				StartTimeInMs = 0;

			cnt_wait = CurTimeInMs - StartTimeInMs;
		}
	}

	if (cnt_wait < max_wait) {
		return 0;
	} else {
		dprint(KERN_CRIT, "BPD TimeOut\n");
		BPD_Reset(VdhRegBaseAddr);
		return -1;
	}
}

 int BPDDRV_PrepareSleep(void)
{
	 int ret = 0;
	if (down_interruptible(&g_BPDSem))
        return -1 ;
	if (s_eBpdDrvSleepStage == BPDDRV_SLEEP_STAGE_NONE) {
		if (s_BPDState == BPD_IDLE) {
			dprint(KERN_ALERT, "%s, idle state \n", __func__);
			s_eBpdDrvSleepStage = BPDDRV_SLEEP_STAGE_SLEEP;
		} else {
			dprint(KERN_ALERT, "%s, work state \n", __func__);
			s_eBpdDrvSleepStage = BPDDRV_SLEEP_STAGE_PREPARE;
		}

		ret = 0;
	} else {
		ret = -1;
	}

	up(&g_BPDSem) ;
	return ret;
}

BPDDRV_SLEEP_STAGE_E BPDDRV_GetSleepStage(void)
{
	return s_eBpdDrvSleepStage;
}

void BPDDRV_SetSleepStage(BPDDRV_SLEEP_STAGE_E sleepState)
{
	if (down_interruptible(&g_BPDSem))
        return ;
	s_eBpdDrvSleepStage = sleepState;
	up(&g_BPDSem) ;
}

void BPDDRV_ForceSleep(unsigned int VdhRegBaseAddr)
{
	dprint(KERN_ALERT, "%s, force state \n", __func__);
	if (down_interruptible(&g_BPDSem))
        return ;
	if (s_eBpdDrvSleepStage != BPDDRV_SLEEP_STAGE_SLEEP) {
		BPD_Reset(VdhRegBaseAddr);
		s_eBpdDrvSleepStage = BPDDRV_SLEEP_STAGE_SLEEP;
	}
	up(&g_BPDSem) ;
}

void BPDDRV_ExitSleep(void)
{
	if (down_interruptible(&g_BPDSem))
        return ;
	s_eBpdDrvSleepStage = BPDDRV_SLEEP_STAGE_NONE;
	up(&g_BPDSem) ;
}

 int BPD_Init(void)
{
    sema_init(&g_BPDSem, 1);
    return 0 ;
}
