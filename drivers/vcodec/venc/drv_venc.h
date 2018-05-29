/*
 *      Video Encode driver for hikey960 common structure
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

#ifndef __DRV_VENC_H__
#define __DRV_VENC_H__

#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/seq_file.h>
#include <linux/vmalloc.h>
#include <linux/hisi/hisi_ion.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/rtc.h>

#include "Vedu_RegAll.h"

#ifndef _M_IX86
typedef unsigned long long      HI_U64;
#else
typedef __int64                 HI_U64;
#endif

#ifndef NULL
#define NULL             0L
#endif

typedef struct 
{
	wait_queue_head_t   queue_head;
	 int              flag;
} VEDU_OSAL_EVENT;


void     HI_PRINT(unsigned int type, char *file, int line, char *function,  char *msg, ... );
typedef enum {
	VENC_FATAL = 0,
	VENC_ERR,
	VENC_WARN,
	VENC_INFO,
	VENC_DBG,
	VENC_ALW,
} VENC_PRINT_TYPE;

#define HI_FATAL_VENC(fmt,...) HI_PRINT(VENC_FATAL,(char *)__FILE__, (int)__LINE__, (char *)__FUNCTION__, fmt, ##__VA_ARGS__)
#define HI_ERR_VENC(fmt,...)   HI_PRINT(VENC_ERR,(char *)__FILE__, (int)__LINE__, (char *)__FUNCTION__, fmt, ##__VA_ARGS__)
#define HI_WARN_VENC(fmt,...)  HI_PRINT(VENC_WARN,(char *)__FILE__, (int)__LINE__, (char *)__FUNCTION__, fmt,##__VA_ARGS__)
#define HI_INFO_VENC(fmt,...)  HI_PRINT(VENC_INFO,(char *)__FILE__, (int)__LINE__, (char *)__FUNCTION__, fmt,##__VA_ARGS__)
#define HI_DBG_VENC(fmt,...)   HI_PRINT(VENC_DBG,(char *)__FILE__, (int)__LINE__, (char *)__FUNCTION__, fmt,##__VA_ARGS__)

// DRV_MEM
typedef struct
{
    void          *pStartVirAddr;
    unsigned int   u32StartPhyAddr;
    unsigned int   u32Size;
} MEM_BUFFER_S;

 int HI_DRV_UserCopy(struct file *file, unsigned int cmd, unsigned long arg, long (*func)(struct file *file, unsigned int cmd, unsigned long uarg));
/**************************************platform.h**************************************************/
 int DRV_MEM_INIT(void);
 int DRV_MEM_EXIT(void);
 int DRV_MEM_KAlloc(const  char* bufName, const  char *zone_name, MEM_BUFFER_S *psMBuf);
 int DRV_MEM_KFree(const MEM_BUFFER_S *psMBuf);
/**************************************************************************************/


 //DRV_VENC_EFL 
typedef struct {
	unsigned int    IpFree;       /* for channel control */
	unsigned int   *pRegBase;
	unsigned int    StopTask;
    unsigned int    RdAddr;
    unsigned int    WrAddr;
    uint64_t        phy_pgd_base ;
	unsigned int    TaskRunning;  /* to block Close IP */

	unsigned int IsFPGA;          /* DTS CONFIGURE */
	unsigned int VeduIrqNum;
	unsigned int MmuIrqNum;
	unsigned int VencRegBaseAddr;
	unsigned int VencRegRange;
	unsigned int normalRate;
	unsigned int highRate;
} vedu_dts_ctx;

//VFMW_REGULATOR
 int  Venc_Regulator_Init(struct platform_device *pdev , vedu_dts_ctx *pCtx);
 void Venc_Regulator_Deinit(struct platform_device *pdev);
 int  Venc_Regulator_Enable(vedu_dts_ctx *pCtx);
 int  Venc_Regulator_Disable(int disVolthold);
 int  Venc_SetClkRate(int isHighRate , vedu_dts_ctx *pCtx);

// 
typedef enum
{
	VENC_SET_CFGREG = 100,
	VENC_SET_CFGREGSIMPLE
} CMD_TYPE;

typedef struct
{
	CMD_TYPE cmd;

	int bResetReg;
	int bClkCfg;
	int bHighSpeed;
	U_VEDU_RAWINT_NS    hw_done_type;

	int bFirstNal2Send;
	unsigned int    ave_lcu_bits;
	unsigned int	vcpi_idr_pic;
	unsigned int   bSecureFlag;
	unsigned int   vcpi_str_fmt;
	unsigned int   vlcst_para_set_en;
	unsigned int   slchdr_size_part2;

	U_REFLD_CMD_CFG		REFLD_CMD_CFG;
	S_sel_REGS_TYPE  VEDU_SEL_REGS;
	S_sao_REGS_TYPE  VEDU_SAO_REGS;
	U_PMV_TMV_EN			PMV_TMV_EN;
	U_TQITQ_QSCALINGLIST   TQITQ_QSCALINGLIST;
	U_RECST_DDR_CROSS      RECST_DDR_CROSS;
	U_COMN1_SMMU_CB_TTBCR  COMN1_SMMU_CB_TTBCR;
	U_COMN3_SMMU_INTMAS_S  COMN3_SMMU_INTMAS_S;
	S_vcpi_REGS_TYPE   VEDU_VCPI_REGS;
	S_vctrl_REGS_TYPE  VEDU_VCTRL_REGS;
	S_pme_REGS_TYPE  VEDU_PME_REGS;
	S_ime_REGS_TYPE VEDU_IME_REGS;
	S_qpg_REGS_TYPE VEDU_QPG_REGS;
	S_intra_REGS_TYPE VEDU_INTRA_REGS;
	S_pack_REGS_TYPE  VEDU_PACK_REGS;
	S_cabac_REGS_TYPE  VEDU_CABAC_REGS;
	S_curpre_REGS_TYPE  VEDU_CURPRE_REGS;
	S_emar_REGS_TYPE  VEDU_EMAR_REGS;
	S_vlcst_REGS_TYPE  VEDU_VLCST_REGS;
	S_vlc_REGS_TYPE  VEDU_VLC_REGS;
	S_comn0_REGS_TYPE VEDU_COMN0_REGS;
	S_comn2_REGS_TYPE  VEDU_COMN2_REGS;
	S_mmu0_REGS_TYPE VEDU_MMU0_REGS;
	S_mmu1_REGS_TYPE VEDU_MMU1_REGS;
} VENC_REG_INFO_S; 
 
void VENC_HAL_ClrAllInt(S_HEVC_AVC_REGS_TYPE *pVeduReg);
void VENC_HAL_DisableAllInt(S_HEVC_AVC_REGS_TYPE *pVeduReg);
void VENC_HAL_StartEncode(S_HEVC_AVC_REGS_TYPE *pVeduReg);
void VENC_HAL_Get_CfgRegSimple(VENC_REG_INFO_S *pVeduReg , S_HEVC_AVC_REGS_TYPE *pAllReg);
void VENC_HAL_Get_Reg_Venc(VENC_REG_INFO_S *pVeduReg , S_HEVC_AVC_REGS_TYPE * pAllReg);
void VeduHal_CfgRegSimple( VENC_REG_INFO_S * channelcfg , S_HEVC_AVC_REGS_TYPE * pAllReg);
void VeduHal_CfgReg( VENC_REG_INFO_S * regcfginfo , S_HEVC_AVC_REGS_TYPE * pAllReg  , unsigned int RdAddr , unsigned int WrAddr , HI_U64 phy_pgd_base);
void VENC_HAL_SetSmmuAddr(S_HEVC_AVC_REGS_TYPE *pVeduReg , unsigned int RdAddr , unsigned int WrAddr);
 
#endif /* __HI_TYPE_H__ */


