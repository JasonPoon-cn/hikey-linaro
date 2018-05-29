/*
 *      Video Decode driver for hikey960   SCD .h file
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
 
#ifndef __SCD_DRV_H__
#define __SCD_DRV_H__

#include "sysconfig.h"
#include "mem_manage.h"
#include "vfmw.h"


typedef enum {
	SCDDRV_SLEEP_STAGE_NONE = 0,
	SCDDRV_SLEEP_STAGE_PREPARE,
	SCDDRV_SLEEP_STAGE_SLEEP
} SCDDRV_SLEEP_STAGE_E;

typedef struct {
	char  ScdIntMask;
	char  SliceCheckFlag;
	char  ScdStart;
	unsigned int  DownMsgPhyAddr;
	unsigned int  UpMsgPhyAddr;
	 int UpLen;
	unsigned int  BufferFirst;
	unsigned int  BufferLast;
	unsigned int  BufferIni;
	 int ScdProtocol;
	 int ScdIniClr;
	unsigned int  DspSpsMsgMemAddr;
	 int DspSpsMsgMemSize;
	unsigned int  DspPpsMsgMemAddr;
	 int DspPpsMsgMemSize;
	unsigned int  DvmMemAddr;
	 int DvmMemSize;
	unsigned int  DspSedTopMemAddr;
	 int DspSedTopMemSize;
	unsigned int  DspCaMnMemAddr;
	 int DspCaMnMemSize;
	 int ScdLowdlyEnable;
	 int reg_avs_flag;

	 int *pDownMsgVirAddr;
	 int *pUpMsgVirAddr;
	 int *pDspSpsMsgMemVirAddr;
	 int *pDspPpsMsgMemVirAddr;
	 int *pDvmMemVirAddr;
	 int *pDspSedTopMemVirAddr;
	 int *pDspCaMnMemVirAddr;
} SM_CTRLREG_S;

typedef struct {
	char  ScdIntMask;
	char  SliceCheckFlag;
	char  ScdStart;
	unsigned int  DownMsgPhyAddr;
	unsigned int  UpMsgPhyAddr;
	 int UpLen;
	unsigned int  BufferFirst;
	unsigned int  BufferLast;
	unsigned int  BufferIni;
	 int ScdProtocol;
	 int ScdIniClr;
	unsigned int  DspSpsMsgMemAddr;
	 int DspSpsMsgMemSize;
	unsigned int  DspPpsMsgMemAddr;
	 int DspPpsMsgMemSize;
	unsigned int  DvmMemAddr;
	 int DvmMemSize;
	unsigned int  DspSedTopMemAddr;
	 int DspSedTopMemSize;
	unsigned int  DspCaMnMemAddr;
	 int DspCaMnMemSize;
	 int ScdLowdlyEnable;
	 int reg_avs_flag;

	compat_ulong_t pDownMsgVirAddr;
	compat_ulong_t pUpMsgVirAddr;
	compat_ulong_t pDspSpsMsgMemVirAddr;
	compat_ulong_t pDspPpsMsgMemVirAddr;
	compat_ulong_t pDvmMemVirAddr;
	compat_ulong_t pDspSedTopMemVirAddr;
	compat_ulong_t pDspCaMnMemVirAddr;
} COMPAT_SM_CTRLREG_S;

typedef struct {
	int ScdProtocol;
	int Scdover;
	int ScdInt;
	int ScdNum;
	unsigned int ScdRollAddr;
	int SrcEaten;
	int UpLen;
} SCD_STATE_REG_S;

typedef enum {
	CONFIG_SCD_REG_CMD = 100,
} CONFIG_SCD_CMD;

typedef struct {
	CONFIG_SCD_CMD cmd;
	 int         eVidStd;
	unsigned int         SResetFlag;
	unsigned int         GlbResetFlag;
	SM_CTRLREG_S   SmCtrlReg;
} OMXSCD_REG_CFG_S;

typedef struct {
	CONFIG_SCD_CMD cmd;
	 int         eVidStd;
	COMPAT_SM_CTRLREG_S IoctlSmCtrlReg;
} COMPAT_OMXSCD_REG_CFG_S;

 int SCDDRV_PrepareSleep(void);

SCDDRV_SLEEP_STAGE_E SCDDRV_GetSleepStage(void);

void SCDDRV_SetSleepStage(SCDDRV_SLEEP_STAGE_E sleepState);

void SCDDRV_ForceSleep(unsigned int VdhRegBaseAddr);

void SCDDRV_ExitSleep(void);

 int SCDDRV_ResetSCD(unsigned int VdhRegBaseAddr);

 int SCDDRV_WaitEvent(unsigned int uiTimes);

 int SCDDRV_WriteReg(SM_CTRLREG_S *pSmCtrlReg);

void SCDDRV_GetRegState(SCD_STATE_REG_S *pScdStateReg);

void SCDDRV_ISR(void);

void SCDDRV_init(void);

void SCDDRV_DeInit(void);

#ifdef ENV_ARMLINUX_KERNEL
 int SCDDRV_IsScdIdle(void);
#endif

#endif
