/*
 *      Video Decode driver for hikey960  memory .h file
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

#ifndef _VFMW_MEM_MANAGE_HEAD_
#define _VFMW_MEM_MANAGE_HEAD_

#include "vfmw.h"

typedef struct {
	unsigned int PhyAddr;
	unsigned int Length;
	int IsSecMem;
	unsigned char *VirAddr;
} MEM_RECORD_S;

void MEM_InitLock(void) ;

void MEM_InitMemManager(void);

 int MEM_AddMemRecord(unsigned int PhyAddr, void *VirAddr, unsigned int Length);

void MEM_DumpRecord(void);

 int MEM_DelMemRecord(unsigned int PhyAddr, void *VirAddr, unsigned int Length);

void *MEM_Phy2Vir(unsigned int PhyAddr);

unsigned int MEM_Vir2Phy(unsigned char *VirAddr);

void MEM_WritePhyWord(unsigned int PhyAddr, unsigned int Data32);

unsigned int MEM_ReadPhyWord(unsigned int PhyAddr);

 int MEM_MapRegisterAddr(unsigned int RegStartPhyAddr, unsigned int RegByteLen, MEM_RECORD_S *pMemRecord);

void MEM_UnmapRegisterAddr(unsigned int PhyAddr, unsigned char *VirAddr, unsigned int Size);

#endif
