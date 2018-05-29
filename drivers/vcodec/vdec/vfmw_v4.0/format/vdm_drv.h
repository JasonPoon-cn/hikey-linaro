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
 
#ifndef __VDM_DRV_HEADER__
#define __VDM_DRV_HEADER__
#include "vfmw.h"
#include "sysconfig.h"

typedef enum {
	VDH_STATE_REG   = 1,
	INT_STATE_REG   = 2,
	INT_MASK_REG    = 3,
	VCTRL_STATE_REG = 4,
} REG_ID_E;

typedef enum {
	VDM_IDLE_STATE     = 0,
	VDM_DECODE_STATE   = 1,
	VDM_REPAIR_STATE_0 = 2,
	VDM_REPAIR_STATE_1 = 3
} VDMDRV_STATEMACHINE_E;

typedef enum {
	VDMDRV_SLEEP_STAGE_NONE = 0,
	VDMDRV_SLEEP_STAGE_PREPARE,
	VDMDRV_SLEEP_STAGE_SLEEP
} VDMDRV_SLEEP_STAGE_E;

typedef enum {
	FIRST_REPAIR = 0,
	SECOND_REPAIR
} REPAIRTIME_S;

typedef enum hi_CONFIG_VDH_CMD {
	CONFIG_VDH_AfterDec_CMD = 200,
	CONFIG_VDH_ACTIVATEDEC_CMD
} CONFIG_VDH_CMD;

typedef struct {
	CONFIG_VDH_CMD vdh_cmd;
	unsigned int vdh_reset_flag;
	unsigned int GlbResetFlag;
	 int VdhStartRepairFlag;
	 int VdhStartHwDecFlag;
	 int VdhAvsFlag;
	 int VdhSelRst;
	 int VdhBasicCfg0;
	 int VdhBasicCfg1;
	 int VdhAvmAddr;
	 int VdhVamAddr;
	 int VdhStreamBaseAddr;
	 int VdhEmarId;
	 int VdhSedTo;
	 int VdhItransTo;
	 int VdhPmvTo;
	 int VdhPrcTo;
	 int VdhRcnTo;
	 int VdhDblkTo;
	 int VdhPpfdTo;
	 int VdhPartLevel;
	 int VdhYstAddr;
	 int VdhYstride;
	 int VdhUvoffset;
	 int VdhHeadInfOffset;
	 int VdhLineNumAddr;
	 int VdhPpfdBufAddr;
	 int VdhPpfdBufLen;
	 int VdhRefPicType;
	 int VdhFfAptEn;
	 int VdhIntState;
	 int VdhIntMask;
	 int LowlyEnable;
	VDMDRV_STATEMACHINE_E VdmStateMachine;
	REPAIRTIME_S RepairTime;
	VID_STD_E VidStd;
	 int PicStruct;
	 int ValidGroupNum0;
	 int ValidGroupNum1;
	 int ErrRationAndRpStratageFlag;
	 int IsMpeg4Nvop;
	 int IsVc1Skpic;
	 int IsVp6Nvop;
	 int AvsFirstFld;
	 int AvsSecondFld;
} OMXVDH_REG_CFG_S;

typedef struct {
	unsigned int Int_State_Reg;

	unsigned int BasicCfg1;
	unsigned int VdmState;
	unsigned int Mb0QpInCurrPic;
	unsigned int SwitchRounding;
	unsigned int SedSta;
	unsigned int SedEnd0;

	unsigned int DecCyclePerPic;
	unsigned int RdBdwidthPerPic;
	unsigned int WrBdWidthPerPic;
	unsigned int RdReqPerPic;
	unsigned int WrReqPerPic;
	unsigned int LumaSumHigh;
	unsigned int LumaSumLow;
	unsigned int LumaHistorgam[32];
} VDMHAL_BACKUP_S;

#endif
