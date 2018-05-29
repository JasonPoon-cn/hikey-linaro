/*
 *      Video Decode driver for hikey960   vdm hal api .h file
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
 
#ifndef _VDM_HAL_API_HEADER_
#define _VDM_HAL_API_HEADER_

#include "sysconfig.h"
#include "mem_manage.h"
#include "vfmw.h"
#include "vdm_hal_local.h"
#include "vfmw_intf.h"

typedef enum {
	GLB_RESET_REQUIRE = 0,
	BPD_RESET_REQUIRE,
	SCD_RESET_REQUIRE,
	MFDE_RESET_REQUIRE,
	BUTT_RESET_REQUIRE,    
} RESET_REQUIRE_TYPE_E;

void VDMHAL_IMP_ResetVdm(unsigned int VdhRegBaseAddr);

void VDMHAL_IMP_WriteScdEMARID(void);

void VDMHAL_IMP_Init(unsigned int VdhRegBaseAddr , unsigned int PERICRG_RegBaseAddr);

void VDMHAL_IMP_DeInit(void);

void VDMHAL_ISR(unsigned int VdhRegBaseAddr);

void VDMHAL_AfterDec(OMXVDH_REG_CFG_S *pVdhRegCfg , unsigned int VdhRegBaseAddr);

void VDMHAL_ActivateVDH(OMXVDH_REG_CFG_S *pVdhRegCfg , unsigned int VdhRegBaseAddr);

void VDMHAL_GetRegState(VDMHAL_BACKUP_S *pVdmRegState);

 int VDMHAL_IsVdmRun(void);

 int VDMHAL_WaitEvent(unsigned int uiTimes);

 int VDMHAL_PrepareSleep(void);

void VDMHAL_ForceSleep(unsigned int VdhRegBaseAddr);

void VDMHAL_ExitSleep(void);

VDMDRV_SLEEP_STAGE_E VDMHAL_GetSleepStage(void);

void VDMHAL_SetSleepStage(VDMDRV_SLEEP_STAGE_E sleepState);

unsigned int VDMHAL_EXT_ReduceFrequency(RESET_REQUIRE_TYPE_E eResetRequire);

void VDMHAL_EXT_RestoreFrequency(RESET_REQUIRE_TYPE_E eResetRequire, unsigned int DivValue);

#endif
