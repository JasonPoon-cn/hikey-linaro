/*
 *      Video Decode driver for hikey960   vfmw driver
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
 
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kern_levels.h>

#include "sysconfig.h"
#include "vfmw_intf.h"

#ifdef HIVDEC_SMMU_SUPPORT
#include "smmu.h"
#endif
#include "vdm_hal_api.h"

#define TIME_PERIOD(begin, end) ((end >= begin)? (end-begin):(0xffffffff - begin + end))

typedef struct {
	MEM_RECORD_S stVdhReg;
	MEM_RECORD_S stCrgReg;
} DRV_MEM_S;

static DRV_MEM_S             g_RegsBaseAddr;
static VFMW_DTS_CONFIG_S     g_DtsConfig;

VFMW_DTS_CONFIG_S*  get_dts_config(void)
{
    return &g_DtsConfig ;
}

inline unsigned int read_scdreg_value(unsigned int reg)
{
    return MEM_ReadPhyWord(g_DtsConfig.VdhRegBaseAddr + SCD_REG_OFFSET + reg) ;
}

void     write_scdreg_value(unsigned int reg , unsigned int value)
{
    MEM_WritePhyWord(g_DtsConfig.VdhRegBaseAddr + SCD_REG_OFFSET + reg , value) ;
}

unsigned int OSAL_GetTimeInMs(void)
{
	unsigned long long SysTime;
	SysTime = sched_clock();
	do_div(SysTime, 1000000);
	return (unsigned int) SysTime;
}

void VCTRL_Suspend(void)
{
	unsigned char isScdSleep = 0;
	unsigned char isVdmSleep = 0;
	unsigned char isBpdSleep = 0;

	unsigned int SleepCount = 0;
	unsigned int BeginTime, EntrTime, CurTime;

	EntrTime = OSAL_GetTimeInMs();

	SCDDRV_PrepareSleep();

	VDMHAL_PrepareSleep();

	BPDDRV_PrepareSleep();

	BeginTime = OSAL_GetTimeInMs();
	do {
		if (SCDDRV_SLEEP_STAGE_SLEEP == SCDDRV_GetSleepStage())
			isScdSleep = 1;

		if (VDMHAL_GetSleepStage() == VDMDRV_SLEEP_STAGE_SLEEP)
			isVdmSleep = 1;

		if (BPDDRV_GetSleepStage() == BPDDRV_SLEEP_STAGE_SLEEP)
			isBpdSleep = 1;

		if ((isScdSleep == 1) && (isVdmSleep == 1) && (isBpdSleep == 1)) {
			break;
		} else {
			if (SleepCount > 30) {
				if (isScdSleep != 1) {
					dprint(KERN_CRIT, "Force scd sleep\n");
					SCDDRV_ForceSleep(g_DtsConfig.VdhRegBaseAddr);
				}
				if (isVdmSleep != 1) {
					dprint(KERN_CRIT, "Force vdm sleep\n");
					VDMHAL_ForceSleep(g_DtsConfig.VdhRegBaseAddr);
				}
				if (isBpdSleep != 1) {
					dprint(KERN_CRIT, "Force bpd sleep\n");
					BPDDRV_ForceSleep(g_DtsConfig.VdhRegBaseAddr);
				}
				break;
			}

			msleep(10);
			SleepCount++;
		}
	} while ((isScdSleep != 1) || (isVdmSleep != 1) || (isBpdSleep != 1));

	CurTime = OSAL_GetTimeInMs();
	dprint(KERN_CRIT, "Vfmw suspend totally take %d ms\n", TIME_PERIOD(EntrTime, CurTime));

	return;
}

void VCTRL_Resume(void)
{
	unsigned int EntrTime, CurTime;

	EntrTime = OSAL_GetTimeInMs();

	VDMHAL_IMP_WriteScdEMARID();
	SMMU_InitGlobalReg(g_DtsConfig.SmmuPageBaseAddr);

	SCDDRV_ExitSleep();

	VDMHAL_ExitSleep();

	BPDDRV_ExitSleep();

	CurTime = OSAL_GetTimeInMs();
	dprint(KERN_CRIT, "Vfmw resume totally take %d ms\n", TIME_PERIOD(EntrTime, CurTime));

	return;
}

static  int VCTRL_MFDE_ISR( int irq, void *dev_id)
{
	VDMHAL_ISR(g_DtsConfig.VdhRegBaseAddr);
	return 1;
}

static  int VCTRL_SCD_ISR( int irq, void *dev_id)
{
    SCDDRV_ISR();
	return 1;
}

static  int VCTRL_SMMU_ISR( int irq, void *dev_id)
{
	SMMU_IntServProc();
	return 1;
}

static  int VCTRL_RequestIrq(unsigned int MfdeIrqNum, unsigned int ScdIrqNum, unsigned int SmmuIrqNum)
{
#if !defined(VDM_BUSY_WAITTING)
	if (request_irq(MfdeIrqNum, (irq_handler_t)VCTRL_MFDE_ISR, 0x00000020, "vfmw_vdh_irq", NULL) != 0) {
		dprint(KERN_CRIT, "Request mfde irq %d failed\n", MfdeIrqNum);
		return -1;
	}
#endif

#if !defined(SCD_BUSY_WAITTING)
	if (request_irq(ScdIrqNum, (irq_handler_t)VCTRL_SCD_ISR, 0x00000020, "vfmw_scd_irq", NULL) != 0) {
		dprint(KERN_CRIT, "Request scd irq %d failed\n", ScdIrqNum);
		return -1;
	}
#endif

#ifdef HIVDEC_SMMU_SUPPORT
#if !defined(SMMU_BUSY_WAITTING)
	if (request_irq(SmmuIrqNum, (irq_handler_t)VCTRL_SMMU_ISR, 0x00000020, "vfmw_smmu_irq", NULL) != 0) {
		dprint(KERN_CRIT, "Request smmu irq %d failed\n", SmmuIrqNum);
		return -1;
	}
#endif
#endif

	return 0;
}

static void VCTRL_FreeIrq(unsigned int MfdeIrqNum, unsigned int ScdIrqNum, unsigned int SmmuIrqNum)
{
#if !defined(VDM_BUSY_WAITTING)
	free_irq(MfdeIrqNum, NULL);    
#endif

#if !defined(SCD_BUSY_WAITTING)
	free_irq(ScdIrqNum, NULL);
#endif

#ifdef HIVDEC_SMMU_SUPPORT
#if !defined(SMMU_BUSY_WAITTING)
	free_irq(SmmuIrqNum, NULL);
#endif
#endif
}

static  int VCTRL_HalInit(void)
{
#ifdef HIVDEC_SMMU_SUPPORT
	if (SMMU_Init(g_DtsConfig.VdhRegBaseAddr) != SMMU_OK) {
		dprint(KERN_CRIT, "SMMU_Init failed\n");
		return -1;
	}
#endif
    BPD_Init() ;
	SCDDRV_init();
	VDMHAL_IMP_Init(g_DtsConfig.VdhRegBaseAddr , g_DtsConfig.PERICRG_RegBaseAddr);
	SMMU_InitGlobalReg(g_DtsConfig.SmmuPageBaseAddr);
	return 0;
}

static void VCTRL_HalDeInit(void)
{
#ifdef HIVDEC_SMMU_SUPPORT
	SMMU_DeInit();
#endif
	VDMHAL_IMP_DeInit();
	SCDDRV_DeInit();
}

int VCTRL_OpenDrivers(void)
{
	MEM_RECORD_S *pstMem;
	 int ret   = -1;

	pstMem = &g_RegsBaseAddr.stVdhReg;
	if (MEM_MapRegisterAddr(g_DtsConfig.VdhRegBaseAddr, 64 * 1024, pstMem) == 0) {
		if (MEM_AddMemRecord(pstMem->PhyAddr, pstMem->VirAddr, pstMem->Length) != 0) {
			dprint(KERN_ERR, "%s %d MEM_AddMemRecord failed\n", __func__, __LINE__);
			goto exit;
		}
	} else {
		dprint(KERN_CRIT, "Map vdh register failed\n");
		goto exit;
	}

	pstMem = &g_RegsBaseAddr.stCrgReg;
	if (MEM_MapRegisterAddr(g_DtsConfig.PERICRG_RegBaseAddr, 0x100, pstMem) == 0) {
		if (MEM_AddMemRecord(pstMem->PhyAddr, pstMem->VirAddr, pstMem->Length) != 0) {
			dprint(KERN_ERR, "%s %d MEM_AddMemRecord failed\n", __func__, __LINE__);
			goto exit;
		}
	} else {
		dprint(KERN_CRIT, "Map crg register failed\n");
		goto exit;
	}

	ret = VCTRL_RequestIrq(g_DtsConfig.MfdeIrqNum, g_DtsConfig.ScdIrqNum, g_DtsConfig.SmmuIrqNum);
	if (ret != 0) {
		dprint(KERN_CRIT, "VCTRL_RequestIrq failed\n");
		goto exit;
	}

	if (VCTRL_HalInit() != 0) {
		dprint(KERN_CRIT, "VCTRL_HalInit failed\n");
		goto exit;
	}

	return 0;

exit:
    VCTRL_CloseVfmw();
    return -1;
}

 int VCTRL_OpenVfmw(void)
{
	memset(&g_RegsBaseAddr, 0, sizeof(g_RegsBaseAddr));

	MEM_InitMemManager();
	if (VCTRL_OpenDrivers() != 0) {
		dprint(KERN_CRIT, "OpenDrivers fail\n");
		return -1;
	}

	return 0;
}

 int VCTRL_CloseVfmw(void)
{
	MEM_RECORD_S *pstMem;

	VCTRL_HalDeInit();

	pstMem = &g_RegsBaseAddr.stVdhReg;
	if (pstMem->Length != 0) {
		MEM_UnmapRegisterAddr(pstMem->PhyAddr, pstMem->VirAddr, pstMem->Length);
		memset(pstMem, 0, sizeof(*pstMem));
	}
    
	MEM_DelMemRecord(pstMem->PhyAddr, pstMem->VirAddr, pstMem->Length);
	pstMem = &g_RegsBaseAddr.stCrgReg;
	if (pstMem->Length != 0) {
		MEM_UnmapRegisterAddr(pstMem->PhyAddr, pstMem->VirAddr, pstMem->Length);
		memset(pstMem, 0, sizeof(*pstMem));
	}
    
	MEM_DelMemRecord(pstMem->PhyAddr, pstMem->VirAddr, pstMem->Length);
	VCTRL_FreeIrq(g_DtsConfig.MfdeIrqNum, g_DtsConfig.ScdIrqNum, g_DtsConfig.SmmuIrqNum);

	return 0;
}

 int VCTRL_VDMHal_Process(OMXVDH_REG_CFG_S *pVdmRegCfg, VDMHAL_BACKUP_S *pVdmRegState)
{
	int Ret = 0;
	VDMDRV_SLEEP_STAGE_E sleepState;
	CONFIG_VDH_CMD cmd = pVdmRegCfg->vdh_cmd;

	sleepState = VDMHAL_GetSleepStage();
	if (VDMDRV_SLEEP_STAGE_SLEEP == sleepState) {
		dprint(KERN_ALERT, "vdm sleep state\n");
		return -1;
	}

	if (pVdmRegCfg->vdh_reset_flag)
		VDMHAL_IMP_ResetVdm(g_DtsConfig.VdhRegBaseAddr);

	switch (cmd) {
	case CONFIG_VDH_AfterDec_CMD:
		VDMHAL_AfterDec(pVdmRegCfg , g_DtsConfig.VdhRegBaseAddr);
		break;

	case CONFIG_VDH_ACTIVATEDEC_CMD:
		VDMHAL_ActivateVDH(pVdmRegCfg , g_DtsConfig.VdhRegBaseAddr);
		break;

	default:
		dprint(KERN_CRIT, " %s  , cmd type unknown:%d\n", __func__, cmd);
		return -1;
		break;
	}

	Ret = VDMHAL_WaitEvent(1000);
	if (Ret == 0) {
		VDMHAL_GetRegState(pVdmRegState);
	} else {
		dprint(KERN_CRIT, "WaitEvent wait time out\n");
		VDMHAL_IMP_ResetVdm(g_DtsConfig.VdhRegBaseAddr);
	}

	sleepState = VDMHAL_GetSleepStage();
	if (sleepState == VDMDRV_SLEEP_STAGE_PREPARE)
		VDMHAL_SetSleepStage(VDMDRV_SLEEP_STAGE_SLEEP);

	return Ret;
}

 int VCTRL_SCDHal_Process(OMXSCD_REG_CFG_S *pScdRegCfg , SCD_STATE_REG_S *pScdStateReg)
{
	int Ret = 0;
	SCDDRV_SLEEP_STAGE_E sleepState;
	CONFIG_SCD_CMD cmd = pScdRegCfg->cmd;

	sleepState = SCDDRV_GetSleepStage();
	if (SCDDRV_SLEEP_STAGE_SLEEP == sleepState) {
		dprint(KERN_ALERT, "SCD sleep state\n");
		return -1;
	}

	if (pScdRegCfg->SResetFlag) {
		if (SCDDRV_ResetSCD(g_DtsConfig.VdhRegBaseAddr) != 0) {
			dprint(KERN_CRIT, "VDEC_IOCTL_SCD_WAIT_HW_DONE  Reset SCD failed\n");
			return -1;
		}
	}

	switch (cmd) {
	case CONFIG_SCD_REG_CMD:
		Ret = SCDDRV_WriteReg(&pScdRegCfg->SmCtrlReg);
		if (Ret != 0) {
			dprint(KERN_CRIT, "SCD busy\n");
			return -1;
		}

		Ret = SCDDRV_WaitEvent(1000);
		if (Ret == 0) {
			SCDDRV_GetRegState(pScdStateReg);
		} else {
			dprint(KERN_ALERT, "VDEC_IOCTL_SCD_WAIT_HW_DONE  wait time out\n");
			SCDDRV_ResetSCD(g_DtsConfig.VdhRegBaseAddr);
		}

		sleepState = SCDDRV_GetSleepStage();
		if (sleepState == SCDDRV_SLEEP_STAGE_PREPARE) {
			SCDDRV_SetSleepStage(SCDDRV_SLEEP_STAGE_SLEEP);
		}
		break;

	default:
		dprint(KERN_ALERT, " cmd type unknown:%d\n", cmd);
		return -1;
	}

	return Ret;
}

 int VCTRL_BPDHal_Process(OMXBPD_REG_S *pBpdReg)
{
	int Ret = 0;
	BPDDRV_SLEEP_STAGE_E sleepState = BPDDRV_GetSleepStage();
	if (sleepState == BPDDRV_SLEEP_STAGE_SLEEP) {
		dprint(KERN_ALERT, "BPD sleep state\n");
		return -1;
	}

	if (BPD_ConfigReg(pBpdReg , g_DtsConfig.VdhRegBaseAddr)) {
		dprint(KERN_ALERT, "omxvdec_config_bpd:  failed\n");
		Ret = -1;
	}

	sleepState = BPDDRV_GetSleepStage();
	if (sleepState == BPDDRV_SLEEP_STAGE_PREPARE)
		BPDDRV_SetSleepStage(BPDDRV_SLEEP_STAGE_SLEEP);

	return Ret;
}

 int VCTRL_VDMHAL_IsRun(void)
{
	return VDMHAL_IsVdmRun();
}

int VCTRL_Scen_Ident(void)
{
	return (RD_SCDREG(0x828) == 1) ? 1 : 0;
}

int VFMW_DRV_ModInit(void)
{
	MEM_InitLock() ;
#ifdef MODULE
#ifndef HI_ADVCA_FUNCTION_RELEASE
	printk(KERN_ALERT "Load hi_vfmw.ko (2017032400) success.\n");
#endif
#endif
	return 0;
}

void VFMW_DRV_ModExit(void)
{
#ifdef MODULE
#ifndef HI_ADVCA_FUNCTION_RELEASE
	printk(KERN_ALERT "Unload hi_vfmw.ko (2017032400) success.\n");
#endif
#endif
	return;
}

module_init(VFMW_DRV_ModInit);
module_exit(VFMW_DRV_ModExit);

MODULE_AUTHOR("gaoyajun");
MODULE_LICENSE("GPL");
