/*
 *      Video Decode driver for hikey960   mpeg-2 master 
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
#ifndef __VDM_HAL_MPEG2_C__
#define __VDM_HAL_MPEG2_C__

#include    "vdm_hal_api.h"
#include    "vdm_hal_local.h"

typedef struct {
	unsigned int ref_pic_type_0     :2;
	unsigned int ref_pic_type_1     :2;
	unsigned int ref_pic_type_2     :2;
	unsigned int ref_pic_type_3     :2;
	unsigned int ref_pic_type_4     :2;
	unsigned int ref_pic_type_5     :2;
	unsigned int ref_pic_type_6     :2;
	unsigned int ref_pic_type_7     :2;

	unsigned int ref_pic_type_8     :2;
	unsigned int ref_pic_type_9     :2;
	unsigned int ref_pic_type_10    :2;
	unsigned int ref_pic_type_11    :2;
	unsigned int ref_pic_type_12    :2;
	unsigned int ref_pic_type_13    :2;
	unsigned int ref_pic_type_14    :2;
	unsigned int ref_pic_type_15    :2;
} REF_PIC_TYPE;

 int MP2HAL_CfgReg(OMXVDH_REG_CFG_S *pVdhRegCfg)
{
	unsigned int datTmp;
	 int D32;

	D32 = 0;
	((BASIC_CFG0 *)(&D32))->mbamt_to_dec         = ((BASIC_CFG0 *)(&pVdhRegCfg->VdhBasicCfg0))->mbamt_to_dec;
	((BASIC_CFG0 *)(&D32))->load_qmatrix_flag    = 1;
	((BASIC_CFG0 *)(&D32))->marker_bit_detect_en = 1;
	((BASIC_CFG0 *)(&D32))->ac_last_detect_en    = 0;
	((BASIC_CFG0 *)(&D32))->coef_idx_detect_en   = 1;
	((BASIC_CFG0 *)(&D32))->vop_type_detect_en   = 0;
	((BASIC_CFG0 *)(&D32))->sec_mode_en          = 0;

	write_register_value(0  ,  VREG_BASIC_CFG0, D32);

	D32 = 0;
	((BASIC_CFG1 *)(&D32))->video_standard       = 0x3;
	((BASIC_CFG1 *)(&D32))->ddr_stride           = ((BASIC_CFG1 *)(&pVdhRegCfg->VdhBasicCfg1))->ddr_stride;
	((BASIC_CFG1 *)(&D32))->fst_slc_grp          = 1;
	((BASIC_CFG1 *)(&D32))->mv_output_en         = 1;
	((BASIC_CFG1 *)(&D32))->uv_order_en          = (1 == ((BASIC_CFG1 *)(&pVdhRegCfg->VdhBasicCfg1))->uv_order_en) ? 1: 0;
	((BASIC_CFG1 *)(&D32))->vdh_2d_en            = 1;
	((BASIC_CFG1 *)(&D32))->max_slcgrp_num       = 3;
	((BASIC_CFG1 *)(&D32))->ppfd_en              = 0;
	((BASIC_CFG1 *)(&D32))->line_num_output_en   = 0;
	((BASIC_CFG1 *)(&D32))->compress_en = ((BASIC_CFG1 *)(&pVdhRegCfg->VdhBasicCfg1))->compress_en;
	/*set uv order 0: v first; 1: u first*/
	write_register_value(0  ,  VREG_BASIC_CFG1, D32);

	D32 = 0;
	((AVM_ADDR *)(&D32))->av_msg_addr = (pVdhRegCfg->VdhAvmAddr) & 0xFFFFFFF0;
	write_register_value(0  , VREG_AVM_ADDR, D32);

	D32 = 0;
	((VAM_ADDR *)(&D32))->va_msg_addr = (pVdhRegCfg->VdhVamAddr) & 0xFFFFFFF0;
	write_register_value(0  , VREG_VAM_ADDR, D32);

	write_register_value(0  , VREG_STREAM_BASE_ADDR, pVdhRegCfg->VdhStreamBaseAddr);

	datTmp = RD_SCDREG(REG_EMAR_ID);
	if (pVdhRegCfg->VdhEmarId == 0)
		datTmp = datTmp & (~(0x10000));
	else
		datTmp = datTmp | (0x10000);
	WR_SCDREG(REG_EMAR_ID, datTmp);

	//TIME_OUT
	D32 = 0x00300C03;
	write_register_value(0  , VREG_SED_TO, D32);
	write_register_value(0  , VREG_ITRANS_TO, D32);
	write_register_value(0  , VREG_PMV_TO, D32);
	write_register_value(0  , VREG_PRC_TO, D32);
	write_register_value(0  , VREG_RCN_TO, D32);
	write_register_value(0  , VREG_DBLK_TO, D32);
	write_register_value(0  , VREG_PPFD_TO, D32);

	D32 = 0;
	((YSTADDR_1D *)(&D32))->ystaddr_1d = (pVdhRegCfg->VdhYstAddr) & 0xFFFFFFF0;
	write_register_value(0  , VREG_YSTADDR_1D, D32);
	write_register_value(0  , VREG_YSTRIDE_1D, pVdhRegCfg->VdhYstride);
	write_register_value(0  , VREG_UVOFFSET_1D, pVdhRegCfg->VdhUvoffset);

	D32 = 0;
	((REF_PIC_TYPE *)(&D32))->ref_pic_type_0 = ((REF_PIC_TYPE *)(&pVdhRegCfg->VdhRefPicType))->ref_pic_type_0;
	((REF_PIC_TYPE *)(&D32))->ref_pic_type_1 = ((REF_PIC_TYPE *)(&pVdhRegCfg->VdhRefPicType))->ref_pic_type_1;
	write_register_value(0  ,  VREG_REF_PIC_TYPE, D32);

	D32 = 0;
	((FF_APT_EN *)(&D32))->ff_apt_en = 0;//USE_FF_APT_EN;
	write_register_value(0  ,  VREG_FF_APT_EN, D32);

	//HEAD_INF_OFFSET
	write_register_value(0  , VREG_HEAD_INF_OFFSET, pVdhRegCfg->VdhHeadInfOffset);
	return 0;
}

 int MP2HAL_StartDec(OMXVDH_REG_CFG_S *pVdhRegCfg)
{
	if (MP2HAL_CfgReg(pVdhRegCfg) != 0) {
		dprint(KERN_ERR, "MP2HAL_CfgReg ERROR\n");
		return -1;
	}
	return 0;
}

#endif
