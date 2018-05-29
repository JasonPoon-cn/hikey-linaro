/*
 *      Video Decode driver for hikey960   hevc master 
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
 
#include "sysconfig.h"
#include "vfmw.h"
#include "mem_manage.h"
#include "vdm_hal_api.h"
#include "vdm_hal_local.h"
#include "vfmw_intf.h"

typedef struct {
	unsigned int mbamt_to_dec           :20;
	unsigned int memory_clock_gating_en :1;
	unsigned int module_clock_gating_en :1;
	unsigned int marker_bit_detect_en   :1;
	unsigned int ac_last_detect_en      :1;
	unsigned int coef_idx_detect_en     :1;
	unsigned int vop_type_detect_en     :1;
	unsigned int work_mode              :2;
	unsigned int luma_sum_en            :1;
	unsigned int luma_histogram_en      :1;
	unsigned int load_qmatrix_flag      :1;
	unsigned int vdh_safe_flag          :1;
} HEVC_BASIC_CFG0;

typedef struct {
	unsigned int video_standard     :4;
	unsigned int reserved           :9;
	unsigned int uv_order_en        :1;
	unsigned int fst_slc_grp        :1;
	unsigned int mv_output_en       :1;
	unsigned int max_slcgrp_num     :12;
	unsigned int line_num_output_en :1;
	unsigned int vdh_2d_en          :1;
	unsigned int frm_cmp_en         :1;
	unsigned int ppfd_en            :1;
} HEVC_BASIC_CFG1;

 int HEVCHAL_CfgVdmReg(OMXVDH_REG_CFG_S *pVdhRegCfg)
{
	unsigned int datTmp;
	unsigned int D32;

	//BASIC_CFG0
	D32 = 0;
	((HEVC_BASIC_CFG0 *)(&D32))->marker_bit_detect_en = 0;
	((HEVC_BASIC_CFG0 *)(&D32))->ac_last_detect_en    = 0;
	((HEVC_BASIC_CFG0 *)(&D32))->coef_idx_detect_en   = 1; //(run_cnt>64) check enable switch
	((HEVC_BASIC_CFG0 *)(&D32))->vop_type_detect_en   = 0;
	((HEVC_BASIC_CFG0 *)(&D32))->load_qmatrix_flag    = ((HEVC_BASIC_CFG0 *)(&pVdhRegCfg->VdhBasicCfg0))->load_qmatrix_flag;
	((HEVC_BASIC_CFG0 *)(&D32))->luma_sum_en          = 0; //enable switch:conculate luma pixel
	((HEVC_BASIC_CFG0 *)(&D32))->luma_histogram_en    = 0; //enable switch:conculate luma histogram
	((HEVC_BASIC_CFG0 *)(&D32))->mbamt_to_dec         = ((HEVC_BASIC_CFG0 *)(&pVdhRegCfg->VdhBasicCfg0))->mbamt_to_dec;
	((HEVC_BASIC_CFG0*)(&D32))->vdh_safe_flag         = 0;

	write_register_value(0  ,  VREG_BASIC_CFG0, D32);

	//BASIC_CFG1
	/*set uv order 0: v first; 1: u first*/
	D32 = 0;
	((HEVC_BASIC_CFG1 *)(&D32))->video_standard       = 0xD;
	((HEVC_BASIC_CFG1 *)(&D32))->fst_slc_grp          = ((HEVC_BASIC_CFG1 *)(&pVdhRegCfg->VdhBasicCfg1))->fst_slc_grp;
	((HEVC_BASIC_CFG1 *)(&D32))->mv_output_en         = 1;
	((HEVC_BASIC_CFG1 *)(&D32))->uv_order_en          = ((HEVC_BASIC_CFG1 *)(&pVdhRegCfg->VdhBasicCfg1))->uv_order_en;
	((HEVC_BASIC_CFG1 *)(&D32))->vdh_2d_en            = 1;
	((HEVC_BASIC_CFG1 *)(&D32))->max_slcgrp_num       = 3;
	((HEVC_BASIC_CFG1 *)(&D32))->line_num_output_en   = 0; //enable switch:output "decodered pixel line of current frame" to DDR
	((HEVC_BASIC_CFG1 *)(&D32))->frm_cmp_en           = ((HEVC_BASIC_CFG1 *)(&pVdhRegCfg->VdhBasicCfg1))->frm_cmp_en;
	((HEVC_BASIC_CFG1 *)(&D32))->ppfd_en              = 0;
	write_register_value(0  ,  VREG_BASIC_CFG1, D32);

	//AVM_ADDR
	D32 = 0;
	((AVM_ADDR *)(&D32))->av_msg_addr = (pVdhRegCfg->VdhAvmAddr) & 0xFFFFFFF0;
	write_register_value(0  , VREG_AVM_ADDR, D32);

	//VAM_ADDR
	D32 = 0;
	((VAM_ADDR *)(&D32))->va_msg_addr = (pVdhRegCfg->VdhVamAddr) & 0xFFFFFFF0;
	write_register_value(0  , VREG_VAM_ADDR, D32);

	//STREAM_BASE_ADDR
	D32 = 0;
	((STREAM_BASE_ADDR *)(&D32))->stream_base_addr = (pVdhRegCfg->VdhStreamBaseAddr) & 0xFFFFFFF0;
	write_register_value(0  , VREG_STREAM_BASE_ADDR, D32);

	//TIME_OUT
	D32 = 0x00300C03;    // l00214825
	write_register_value(0  , VREG_SED_TO, D32);
	write_register_value(0  , VREG_ITRANS_TO, D32);
	write_register_value(0  , VREG_PMV_TO, D32);
	write_register_value(0  , VREG_PRC_TO, D32);
	write_register_value(0  , VREG_RCN_TO, D32);
	write_register_value(0  , VREG_DBLK_TO, D32);
	write_register_value(0  , VREG_PPFD_TO, D32);

	D32 = 0;
	((YSTADDR_1D *)(&D32))->ystaddr_1d = (pVdhRegCfg->VdhYstAddr) & 0xFFFFFF00;
	write_register_value(0  , VREG_YSTADDR_1D, D32);

	//YSTRIDE_1D
	write_register_value(0  , VREG_YSTRIDE_1D, pVdhRegCfg->VdhYstride);

	//UVOFFSET_1D
	write_register_value(0  , VREG_UVOFFSET_1D, pVdhRegCfg->VdhUvoffset);

	//HEAD_INF_OFFSET
	D32 = 0;
	write_register_value(0  , VREG_HEAD_INF_OFFSET, D32); 

	//PPFD_BUF_ADDR
	D32 = 0;
	((PPFD_BUF_ADDR *)(&D32))->ppfd_buf_addr = 0;     
	write_register_value(0  , VREG_PPFD_BUF_ADDR, D32);

	//PPFD_BUF_LEN
	D32 = 0;
	((PPFD_BUF_LEN *)(&D32))->ppfd_buf_len = 0;        
	write_register_value(0  , VREG_PPFD_BUF_LEN, D32);

	datTmp = RD_SCDREG(REG_EMAR_ID);
	if (pVdhRegCfg->VdhEmarId == 0)
		datTmp = datTmp & (~(0x10000));
	else
		datTmp = datTmp | (0x10000);

	WR_SCDREG(REG_EMAR_ID, datTmp);

	//FF_APT_EN
	D32 = 0x2;
	write_register_value(0  ,  VREG_FF_APT_EN, D32);
	return 0;
}

 int HEVCHAL_StartDec(OMXVDH_REG_CFG_S *pVdhRegCfg)
{
	HEVCHAL_CfgVdmReg(pVdhRegCfg);
	return 0;
}
