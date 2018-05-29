/*
 *      Video Decode driver for hikey960   SCD master 
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
 
#include "scd_drv.h"
#include "vfmw_intf.h"
#include "vdm_hal_api.h"
#ifdef HIVDEC_SMMU_SUPPORT
#include "smmu.h"
#endif

#define SCD_RESET_CTRL_BIT             (2)
#define SCD_RESET_OK_BIT               (2)
#define SCD_TIME_OUT_COUNT             (200)
#define REG_SCD_START                  (0x800)
#define REG_LIST_ADDRESS               (0x804)
#define REG_UP_ADDRESS                 (0x808)
#define REG_UP_LEN                     (0x80c)
#define REG_BUFFER_FIRST               (0x810)
#define REG_BUFFER_LAST                (0x814)
#define REG_BUFFER_INI                 (0x818)
#define REG_SCD_PROTOCOL               (0x820)
#define REG_DSP_SPS_MSG_ADDRESS        (0x828)
#define REG_DSP_PPS_MSG_ADDRESS        (0x82c)
#define REG_DVM_MSG_ADDRESS            (0x830)
#define REG_SED_TOP_ADDRESS            (0x834)
#define REG_CA_MN_ADDRESS              (0x838)
#define REG_SCD_OVER                   (0x840)
#define REG_SCD_INT                    (0x844)
#define REG_SCD_NUM                    (0x84c)
#define REG_ROLL_ADDR                  (0x850)
#define REG_SRC_EATEN                  (0x854)

#define REG_SCD_INT_MASK               (0x81c)
#define REG_SCD_INI_CLR                (0x824)

typedef enum {
	SCD_IDLE = 0,
	SCD_WORKING,
} SCD_STATE_E;

static SCDDRV_SLEEP_STAGE_E  s_eScdDrvSleepStage = SCDDRV_SLEEP_STAGE_NONE;
static SCD_STATE_REG_S gScdStateReg;
static SCD_STATE_E s_SCDState = SCD_IDLE;
static OSAL_EVENT g_ScdHwDoneEvent;
static struct semaphore g_SCDSem;

 int SCDDRV_ResetSCD(unsigned int VdhRegBaseAddr)
{
	unsigned int tmp;
	unsigned int i;
	unsigned int reg_rst_ok;
	unsigned int reg;
	unsigned int div_value;
	unsigned int *pScdResetReg   = NULL;
	unsigned int *pScdResetOkReg = NULL;
	unsigned int SOFTRST_REQ_Addr    = VdhRegBaseAddr + SOFTRST_REQ_OFFSET;
	unsigned int SOFTRST_OK_ADDR     = VdhRegBaseAddr + SOFTRST_OK_OFFSET;   
	pScdResetReg   = (unsigned int *) MEM_Phy2Vir(SOFTRST_REQ_Addr);
	pScdResetOkReg = (unsigned int *) MEM_Phy2Vir(SOFTRST_OK_ADDR);

	if (pScdResetReg == NULL || pScdResetOkReg == NULL) {
		dprint(KERN_CRIT, "scd reset register map fail\n");
		return -1;
	}

	tmp = RD_SCDREG(REG_SCD_INT_MASK);

	div_value = VDMHAL_EXT_ReduceFrequency(SCD_RESET_REQUIRE);

	reg = *(volatile unsigned int *)pScdResetReg;
	*(volatile unsigned int *)pScdResetReg = reg | (unsigned int) (1 << SCD_RESET_CTRL_BIT);

	for (i = 0; i < 100; i++) {
		reg_rst_ok = *(volatile unsigned int *)pScdResetOkReg;
		if (reg_rst_ok & (1 << SCD_RESET_OK_BIT))
			break;
		udelay(10);
	}

	if (i >= 100)
		dprint(KERN_CRIT, "%s reset failed\n", __func__);
	else
		dprint(KERN_ALERT, "%s reset success\n", __func__);

	*(volatile unsigned int *)pScdResetReg = reg & (unsigned int) (~(1 << SCD_RESET_CTRL_BIT));

	VDMHAL_EXT_RestoreFrequency(SCD_RESET_REQUIRE, div_value);

	WR_SCDREG(REG_SCD_INT_MASK, tmp);

	s_SCDState = SCD_IDLE;
	return 0;
}

 int SCDDRV_WaitEvent(unsigned int uiTimes)
{
     int sret = wait_event_interruptible_timeout((g_ScdHwDoneEvent.queue_head), (g_ScdHwDoneEvent.flag != 0), (msecs_to_jiffies(uiTimes)));
	g_ScdHwDoneEvent.flag = 0;
	return (sret != 0) ? 0 : -1;
}

 int SCDDRV_PrepareSleep(void)
{
	 int ret = 0;
	if (down_interruptible(&g_SCDSem))
        return -1 ;
	if (s_eScdDrvSleepStage == SCDDRV_SLEEP_STAGE_NONE) {
		if (SCD_IDLE == s_SCDState) {
			dprint(KERN_ALERT, "%s, idle state \n", __func__);
			s_eScdDrvSleepStage = SCDDRV_SLEEP_STAGE_SLEEP;
		} else {
			dprint(KERN_ALERT, "%s, decoded state \n", __func__);
			s_eScdDrvSleepStage = SCDDRV_SLEEP_STAGE_PREPARE;
		}

		ret = 0;
	} else {
		ret = -1;
	}

	up(&g_SCDSem);
	return ret;
}

SCDDRV_SLEEP_STAGE_E SCDDRV_GetSleepStage(void)
{
	return s_eScdDrvSleepStage;
}

void SCDDRV_SetSleepStage(SCDDRV_SLEEP_STAGE_E sleepState)
{
	if (down_interruptible(&g_SCDSem))
        return ;
	s_eScdDrvSleepStage = sleepState;
	up(&g_SCDSem);
}

void SCDDRV_ForceSleep(unsigned int VdhRegBaseAddr)
{
	dprint(KERN_ALERT, "%s, force state \n", __func__);
	if (down_interruptible(&g_SCDSem))
        return ;
	if (s_eScdDrvSleepStage != SCDDRV_SLEEP_STAGE_SLEEP) {
		SCDDRV_ResetSCD(VdhRegBaseAddr);
		s_eScdDrvSleepStage = SCDDRV_SLEEP_STAGE_SLEEP;
	}
	up(&g_SCDSem);
}

void SCDDRV_ExitSleep(void)
{
	if (down_interruptible(&g_SCDSem))
        return ;
	s_eScdDrvSleepStage = SCDDRV_SLEEP_STAGE_NONE;
	up(&g_SCDSem);
}

 int SCDDRV_WriteReg(SM_CTRLREG_S *pSmCtrlReg)
{
	if (s_SCDState != SCD_IDLE)
		return -1;

	s_SCDState = SCD_WORKING;
	WR_SCDREG(REG_SCD_INI_CLR, 1);

	// LIST_ADDRESS
	WR_SCDREG(REG_LIST_ADDRESS, (unsigned int)pSmCtrlReg->DownMsgPhyAddr);

	// UP_ADDRESS
	WR_SCDREG(REG_UP_ADDRESS, (unsigned int) pSmCtrlReg->UpMsgPhyAddr);

	// UP_LEN
	WR_SCDREG(REG_UP_LEN, (unsigned int) pSmCtrlReg->UpLen);

	// BUFFER_FIRST
	WR_SCDREG(REG_BUFFER_FIRST, (unsigned int) pSmCtrlReg->BufferFirst);

	// BUFFER_LAST
	WR_SCDREG(REG_BUFFER_LAST, (unsigned int) pSmCtrlReg->BufferLast);

	// BUFFER_INI
	WR_SCDREG(REG_BUFFER_INI, (unsigned int) pSmCtrlReg->BufferIni);

	// AVS_FLAG
	WR_SCDREG(REG_AVS_FLAG, (unsigned int) pSmCtrlReg->reg_avs_flag);

#ifdef VFMW_SCD_LOWDLY_SUPPORT
	// SCD LOW DELAY
	WR_SCDREG(REG_DSP_SPS_MSG_ADDRESS, (unsigned int) pSmCtrlReg->DspSpsMsgMemAddr);
	WR_SCDREG(REG_DSP_PPS_MSG_ADDRESS, (unsigned int) pSmCtrlReg->DspPpsMsgMemAddr);
	WR_SCDREG(REG_DVM_MSG_ADDRESS, (unsigned int) pSmCtrlReg->DvmMemAddr);
	WR_SCDREG(REG_SED_TOP_ADDRESS, (unsigned int) pSmCtrlReg->DspSedTopMemAddr);
	WR_SCDREG(REG_CA_MN_ADDRESS, (unsigned int) pSmCtrlReg->DspCaMnMemAddr);
#endif

	// SCD_PROTOCOL
	WR_SCDREG(REG_SCD_PROTOCOL, (unsigned int) ((pSmCtrlReg->ScdLowdlyEnable << 8)
		| ((pSmCtrlReg->SliceCheckFlag << 4) & 0x10)
		| (pSmCtrlReg->ScdProtocol & 0x0f)));
#ifdef HIVDEC_SMMU_SUPPORT
	SMMU_SetMasterReg(SCD, SECURE_OFF, SMMU_ON);
#endif


#ifndef SCD_BUSY_WAITTING
	WR_SCDREG(REG_SCD_INT_MASK, 0);
#endif
	// SCD_START
	WR_SCDREG(REG_SCD_START, 0);
	WR_SCDREG(REG_SCD_START, (unsigned int) (pSmCtrlReg->ScdStart & 0x01));

	return 0;
}

void SCDDRV_SaveStateReg(void)
{
	gScdStateReg.ScdProtocol = RD_SCDREG(REG_SCD_PROTOCOL);
	gScdStateReg.Scdover     = RD_SCDREG(REG_SCD_OVER);
	gScdStateReg.ScdInt      = RD_SCDREG(REG_SCD_INT);
	gScdStateReg.ScdNum      = RD_SCDREG(REG_SCD_NUM);
	gScdStateReg.ScdRollAddr = RD_SCDREG(REG_ROLL_ADDR);
	gScdStateReg.SrcEaten    = RD_SCDREG(REG_SRC_EATEN);
	gScdStateReg.UpLen       = RD_SCDREG(REG_UP_LEN);
}

void SCDDRV_init(void)
{
	memset(&gScdStateReg, 0, sizeof(gScdStateReg));
	s_eScdDrvSleepStage = SCDDRV_SLEEP_STAGE_NONE;
	s_SCDState = SCD_IDLE;    
    g_ScdHwDoneEvent.flag = 0;
	init_waitqueue_head(&(g_ScdHwDoneEvent.queue_head));
    sema_init(&g_SCDSem , 1) ;
}

void SCDDRV_DeInit(void)
{
	s_eScdDrvSleepStage = SCDDRV_SLEEP_STAGE_NONE;
	s_SCDState = SCD_IDLE;
}

void SCDDRV_ISR(void)
{
	 int dat = 0;
	dat = RD_SCDREG(REG_SCD_OVER) & 0x01;
	if ((dat & 1) == 0) {
		printk(KERN_ERR "End0: SM_SCDIntServeProc()\n");
	}

	WR_SCDREG(REG_SCD_INI_CLR, 1);
	SCDDRV_SaveStateReg();	
    g_ScdHwDoneEvent.flag = 1;
	wake_up_interruptible(&(g_ScdHwDoneEvent.queue_head));
}

void SCDDRV_GetRegState(SCD_STATE_REG_S *pScdStateReg)
{
	memcpy(pScdStateReg, &gScdStateReg, sizeof(gScdStateReg));
	s_SCDState = SCD_IDLE;
}

 int WaitSCDFinish(void)
{
	 int i;
	if (SCD_WORKING == s_SCDState) {
		for (i = 0; i < SCD_TIME_OUT_COUNT; i++) {
			if ((RD_SCDREG(REG_SCD_OVER) & 1))
				return 0;
			else
				;    
		}

		return -1;
	} else {
		return 0;
	}
}

#ifdef ENV_ARMLINUX_KERNEL
 int SCDDRV_IsScdIdle(void)
{
	 int ret = 0;
	if (SCD_IDLE == s_SCDState) {
		ret = 0;
	} else if (SCD_WORKING == s_SCDState) {
		ret = -1;
	} else {
		ret = -1;
		dprint(KERN_ERR, "%s : s_SCDState : %d is wrong\n", __func__, s_SCDState);
	}
	return ret;
}

#endif
