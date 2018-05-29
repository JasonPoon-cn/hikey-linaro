/*
 *      Video Decode driver for hikey960   real v8 master 
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

#ifndef __VDM_HAL_REAL8_C__
#define __VDM_HAL_REAL8_C__

#include "vdm_hal_api.h"
#include "vdm_hal_local.h"

typedef struct {
	unsigned int mbamt_to_dec:                         20;
	unsigned int memory_clock_gating_en:               1;
	unsigned int module_clock_gating_en:               1;
	unsigned int marker_bit_detect_en:                 1;
	unsigned int ac_last_detect_en:                    1;
	unsigned int coef_idx_detect_en:                   1;
	unsigned int vop_type_detect_en:                   1;
	unsigned int reserved:                             4;
	unsigned int ld_qmatrix_flag:                      1;
	unsigned int sec_mode_en:                          1;
} RV8_BASIC_CFG0;

typedef struct {
	unsigned int video_standard:                       4;
	unsigned int ddr_stride:                           9;
	unsigned int uv_order_en:                          1;
	unsigned int fst_slc_grp:                          1;
	unsigned int mv_output_en:                         1;
	unsigned int max_slcgrp_num:                       12;
	unsigned int line_num_output_en:                   1;
	unsigned int vdh_2d_en:                            1;
	unsigned int compress_en:                          1;
	unsigned int ppfd_en:                              1;
} RV8_BASIC_CFG1;

typedef struct {
	unsigned int av_msg_addr:                          32;
} RV8_AVM_ADDR;

typedef struct {
	unsigned int va_msg_addr:                          32;
} RV8_VAM_ADDR;

typedef struct {
	unsigned int ff_apt_en:                            1;
	unsigned int reserved:                             31;
} RV8_FF_APT_EN;

void RV8HAL_WriteReg(OMXVDH_REG_CFG_S *pVdhRegCfg)
{
	unsigned int datTmp;
	 int D32;
	dprint(KERN_INFO, "configuring VDM registers\n");
	D32 = 0;
	((RV8_BASIC_CFG0 *)(&D32))->mbamt_to_dec             = ((RV8_BASIC_CFG0 *)(&pVdhRegCfg->VdhBasicCfg0))->mbamt_to_dec;
	((RV8_BASIC_CFG0 *)(&D32))->ld_qmatrix_flag          = 0;
	((RV8_BASIC_CFG0 *)(&D32))->marker_bit_detect_en     = 0;
	((RV8_BASIC_CFG0 *)(&D32))->ac_last_detect_en        = 0;
	((RV8_BASIC_CFG0 *)(&D32))->coef_idx_detect_en       = 1;
	((RV8_BASIC_CFG0 *)(&D32))->vop_type_detect_en       = 0;
	((RV8_BASIC_CFG0 *)(&D32))->sec_mode_en              = 0;

	write_register_value(0  ,  VREG_BASIC_CFG0, D32);

	D32 = 0;
	((RV8_BASIC_CFG1 *)(&D32))->video_standard       = 0x8;
	((RV8_BASIC_CFG1 *)(&D32))->ddr_stride           = ((RV8_BASIC_CFG1 *)(&pVdhRegCfg->VdhBasicCfg1))->ddr_stride;
	((RV8_BASIC_CFG1 *)(&D32))->fst_slc_grp          = ((RV8_BASIC_CFG1 *)(&pVdhRegCfg->VdhBasicCfg1))->fst_slc_grp;
	((RV8_BASIC_CFG1 *)(&D32))->mv_output_en         = ((RV8_BASIC_CFG1 *)(&pVdhRegCfg->VdhBasicCfg1))->mv_output_en;
	((RV8_BASIC_CFG1 *)(&D32))->uv_order_en          = ((RV8_BASIC_CFG1 *)(&pVdhRegCfg->VdhBasicCfg1))->uv_order_en;
	((RV8_BASIC_CFG1 *)(&D32))->vdh_2d_en            = 1;
	((RV8_BASIC_CFG1 *)(&D32))->max_slcgrp_num       = 0;
	((RV8_BASIC_CFG1 *)(&D32))->line_num_output_en   = 0;
	((RV8_BASIC_CFG1 *)(&D32))->compress_en          = ((RV8_BASIC_CFG1 *)(&pVdhRegCfg->VdhBasicCfg1))->compress_en;
	((RV8_BASIC_CFG1 *)(&D32))->ppfd_en              = 0;

	//BASIC_CFG1
	/*set uv order 0: v first; 1: u first*/
	write_register_value(0  ,  VREG_BASIC_CFG1, D32);

	D32 = 0;
	((RV8_AVM_ADDR *)(&D32))->av_msg_addr = (pVdhRegCfg->VdhAvmAddr) & 0xFFFFFFF0;
	write_register_value(0  , VREG_AVM_ADDR, D32);

	D32 = 0;
	((RV8_VAM_ADDR *)(&D32))->va_msg_addr = (pVdhRegCfg->VdhVamAddr) & 0xFFFFFFF0;
	write_register_value(0  , VREG_VAM_ADDR, D32);

	write_register_value(0  , VREG_STREAM_BASE_ADDR, pVdhRegCfg->VdhStreamBaseAddr);

	datTmp = RD_SCDREG(REG_EMAR_ID);
	if (pVdhRegCfg->VdhEmarId == 0)
		datTmp = datTmp & (~(0x10000));
	else
		datTmp = datTmp | (0x10000);

	WR_SCDREG(REG_EMAR_ID, datTmp);

	D32 = 0x00300C03;
	write_register_value(0  , VREG_SED_TO, D32);
	write_register_value(0  , VREG_ITRANS_TO, D32);
	write_register_value(0  , VREG_PMV_TO, D32);
	write_register_value(0  , VREG_PRC_TO, D32);
	write_register_value(0  , VREG_RCN_TO, D32);
	write_register_value(0  , VREG_DBLK_TO, D32);
	write_register_value(0  , VREG_PPFD_TO, D32);

	D32 = (pVdhRegCfg->VdhYstAddr) & 0xFFFFFFF0;
	write_register_value(0  , VREG_YSTADDR_1D, D32);

	write_register_value(0  , VREG_YSTRIDE_1D, pVdhRegCfg->VdhYstride);

	write_register_value(0  , VREG_UVOFFSET_1D, pVdhRegCfg->VdhUvoffset);

	write_register_value(0  , VREG_HEAD_INF_OFFSET, pVdhRegCfg->VdhHeadInfOffset);
	D32 = 0;
	//REF_PIC_TYPE
	write_register_value(0  , VREG_REF_PIC_TYPE, D32);

	D32 = 0;
	((RV8_FF_APT_EN *)(&D32))->ff_apt_en = 0;  //Always use FrameSave Mode, USE_FF_APT_EN
	//FF_APT_EN
	write_register_value(0  ,  VREG_FF_APT_EN, D32);
}

 int RV8HAL_StartDec(OMXVDH_REG_CFG_S *pVdhRegCfg)
{
	RV8HAL_WriteReg(pVdhRegCfg);
	return 0;
}

#endif
