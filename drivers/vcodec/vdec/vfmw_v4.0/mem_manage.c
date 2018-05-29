/*
 *      Video Decode driver for hikey960   memory
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
 
#include "mem_manage.h"
#include "sysconfig.h"
#include "sysconfig.h"

#define    MAX_MEM_MAN_RECORD_NUM    (32*32)
static MEM_RECORD_S  s_MemRecord[MAX_MEM_MAN_RECORD_NUM];
static OSAL_IRQ_SPIN_LOCK g_SpinLock_Record;

void MEM_InitLock(void)
{
    spin_lock_init(&g_SpinLock_Record.irq_lock);
	g_SpinLock_Record.isInit = 1;
}

void MEM_InitMemManager(void)
{
	spin_lock_irqsave(&g_SpinLock_Record.irq_lock, g_SpinLock_Record.irq_lockflags);
	memset(&s_MemRecord, 0, sizeof(s_MemRecord));
	spin_unlock_irqrestore(&g_SpinLock_Record.irq_lock, g_SpinLock_Record.irq_lockflags);
}

 int MEM_AddMemRecord(unsigned int PhyAddr, void *VirAddr, unsigned int Length)
{
	 int i;
	char IsErrorFlag = 0;
	 int TargetPos  = -1;
	 int ret        = -1;

	spin_lock_irqsave(&g_SpinLock_Record.irq_lock, g_SpinLock_Record.irq_lockflags);
	for (i = 0; i < MAX_MEM_MAN_RECORD_NUM; i++) {
		if ((s_MemRecord[i].PhyAddr <= PhyAddr) && (PhyAddr < s_MemRecord[i].PhyAddr + s_MemRecord[i].Length)) {
			IsErrorFlag = 1;
			break;
		}

		if (s_MemRecord[i].Length == 0 && TargetPos == -1)
			TargetPos = i;
	}

	if (IsErrorFlag == 1) {
		dprint(KERN_CRIT, "%s conflict occured\n ", __func__);
		ret = -1;
	} else if (TargetPos == -1) {
		dprint(KERN_CRIT, "%s no free record slot\n ", __func__);
		ret = -1;
	} else {
		s_MemRecord[TargetPos].PhyAddr = PhyAddr;
		s_MemRecord[TargetPos].VirAddr = VirAddr;
		s_MemRecord[TargetPos].Length  = Length;
		ret = 0;
	}

	spin_unlock_irqrestore(&g_SpinLock_Record.irq_lock, g_SpinLock_Record.irq_lockflags);
	return ret;
}

 int MEM_DelMemRecord(unsigned int PhyAddr, void *VirAddr, unsigned int Length)
{
	 int i;
	spin_lock_irqsave(&g_SpinLock_Record.irq_lock, g_SpinLock_Record.irq_lockflags);
	for (i = 0; i < MAX_MEM_MAN_RECORD_NUM; i++) {
		if (s_MemRecord[i].Length == 0)
			continue;

		if (PhyAddr == s_MemRecord[i].PhyAddr && VirAddr == s_MemRecord[i].VirAddr &&
		    Length == s_MemRecord[i].Length) {
			s_MemRecord[i].Length  = 0;
			s_MemRecord[i].PhyAddr = 0;
			s_MemRecord[i].VirAddr = 0;
            spin_unlock_irqrestore(&g_SpinLock_Record.irq_lock, g_SpinLock_Record.irq_lockflags);
			return 0;
		}
	}

	spin_unlock_irqrestore(&g_SpinLock_Record.irq_lock, g_SpinLock_Record.irq_lockflags);
	return -1;
}

void *MEM_Phy2Vir(unsigned int PhyAddr)
{
	unsigned int i;
	unsigned char *VirAddr = NULL;
	for (i = 0; i < MAX_MEM_MAN_RECORD_NUM; i++) {
		if (s_MemRecord[i].Length == 0)
			continue;

		if ((PhyAddr >= s_MemRecord[i].PhyAddr) && (PhyAddr < s_MemRecord[i].PhyAddr + s_MemRecord[i].Length)) {
			VirAddr = s_MemRecord[i].VirAddr + (PhyAddr - s_MemRecord[i].PhyAddr);
			break;
		}
	}

	return (void *) VirAddr;
}

unsigned int MEM_Vir2Phy(unsigned char *VirAddr)
{
	unsigned int i;
	unsigned int PhyAddr = 0;
	for (i = 0; i < MAX_MEM_MAN_RECORD_NUM; i++) {
		if (s_MemRecord[i].Length == 0)
			continue;

		if ((VirAddr >= s_MemRecord[i].VirAddr) && (VirAddr < s_MemRecord[i].VirAddr + s_MemRecord[i].Length)) {
			PhyAddr = s_MemRecord[i].PhyAddr + (VirAddr - s_MemRecord[i].VirAddr);
			break;
		}
	}

	return PhyAddr;
}

void MEM_WritePhyWord(unsigned int PhyAddr, unsigned int Data32)
{
	unsigned int *pDst = (unsigned int *) MEM_Phy2Vir(PhyAddr);
	if (pDst != NULL)
		*pDst = Data32;
}

unsigned int MEM_ReadPhyWord(unsigned int PhyAddr)
{
	unsigned int Data32 = 0;
	unsigned int *pDst = (unsigned int *) MEM_Phy2Vir(PhyAddr);
	if (pDst != NULL)
		Data32 = *(volatile unsigned int *)pDst;

	return Data32;
}

 int  MEM_MapRegisterAddr(unsigned int RegStartPhyAddr, unsigned int RegByteLen, MEM_RECORD_S *pMemRecord)
{
	unsigned char *ptr;
	if (pMemRecord == NULL || RegStartPhyAddr == 0 || RegByteLen == 0)
		return -1;

	memset(pMemRecord, 0, sizeof(*pMemRecord));
	ptr = ioremap_nocache(RegStartPhyAddr, RegByteLen);
	if (ptr != NULL) {
		pMemRecord->PhyAddr = RegStartPhyAddr;
		pMemRecord->VirAddr = ptr;
		pMemRecord->Length = RegByteLen;
		return 0;
	}

	return -1;
}

void MEM_UnmapRegisterAddr(unsigned int PhyAddr, unsigned char *VirAddr, unsigned int Size)
{
	if (PhyAddr == 0 || VirAddr == 0)
		return;

	iounmap(VirAddr);
}
