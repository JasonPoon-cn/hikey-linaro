/*
 *      Video Decode driver for hikey960   smmu .h file 
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

#ifndef __HIVDEC_SMMU_H__
#define __HIVDEC_SMMU_H__

#include "sysconfig.h"    //for VDM_REG_PHY_ADDR, SCD_REG_PHY_ADDR, BPD_REG_PHY_ADDR
#include "vfmw.h"

#define SMMU_OK     0
#define SMMU_ERR   -1

#define SECURE_ON   1
#define SECURE_OFF  0
#define SMMU_ON     1
#define SMMU_OFF    0

typedef enum {
	MFDE = 0,
	BPD,
	SCD,
} SMMU_MASTER_TYPE;

 int SMMU_Init(unsigned int VdhRegBaseAddr);

void SMMU_DeInit(void);

void SMMU_SetMasterReg(SMMU_MASTER_TYPE master_type, unsigned char secure_en, unsigned char mmu_en);

void SMMU_InitGlobalReg(unsigned long long smmuPageBase);

void SMMU_IntServProc(void);

#endif
