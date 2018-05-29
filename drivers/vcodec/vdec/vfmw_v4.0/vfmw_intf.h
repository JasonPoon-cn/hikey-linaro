/*
 *      Video Decode driver for hikey960   vfmw interface .h file 
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
 
#ifndef __VFMW_INTF_H__
#define __VFMW_INTF_H__

#include "vfmw.h"
#include "sysconfig.h"
#include "mem_manage.h"
#include "vdm_drv.h"
#include "scd_drv.h"
#include "bitplane.h"

 VFMW_DTS_CONFIG_S*  get_dts_config(void) ;

 int VCTRL_OpenDrivers(void);

 int VCTRL_OpenVfmw(void);

 int VCTRL_CloseVfmw(void);

 int VCTRL_VDMHal_Process(OMXVDH_REG_CFG_S *pVdmRegCfg, VDMHAL_BACKUP_S *pVdmRegState);

 int VCTRL_SCDHal_Process(OMXSCD_REG_CFG_S *pScdRegCfg, SCD_STATE_REG_S *pScdStateReg);

 int VCTRL_BPDHal_Process(OMXBPD_REG_S *pBpdReg);

 int VCTRL_VDMHAL_IsRun(void);

void VCTRL_Suspend(void);

void VCTRL_Resume(void);

int VCTRL_Scen_Ident(void);

#endif
