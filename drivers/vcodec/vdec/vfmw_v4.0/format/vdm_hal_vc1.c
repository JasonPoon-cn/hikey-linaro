/*
 *      Video Decode driver for hikey960   vc1 master
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

#ifndef __VDM_HAL_VC1_C__
#define __VDM_HAL_VC1_C__

#include "sysconfig.h"
#include "vdm_hal_api.h"
#include "vdm_hal_local.h"

 int VC1HAL_CfgReg(OMXVDH_REG_CFG_S *pVdhRegCfg)
{
	unsigned int datTmp;
	 int D32;

	D32 = 0;
	D32 = (pVdhRegCfg->VdhBasicCfg0 & 0x000FFFFF)  // [19:0] mbamt_to_dec
		 | ( 0 << 22 )
		 | ( 0 << 23 )
		 | ( 1 << 24 )
		 | ( 0 << 25 )
		 | ( 1 << 30 ) 	// [30] ld_qmatrix_flag
		 | ( 0 << 31 );	// [31] sec_mode, 0==Normal Mode

	write_register_value(0  ,  VREG_BASIC_CFG0, D32);

	/*set uv order 0: v first; 1: u first*/
	D32 = 0;
	((BASIC_CFG1*)(&D32))->video_standard     = 0x1;
	((BASIC_CFG1*)(&D32))->ddr_stride         = ((BASIC_CFG1*)(&pVdhRegCfg->VdhBasicCfg1))->ddr_stride;
	((BASIC_CFG1*)(&D32))->uv_order_en        = ((BASIC_CFG1*)(&pVdhRegCfg->VdhBasicCfg1))->uv_order_en;
	((BASIC_CFG1*)(&D32))->fst_slc_grp        = 1;
	((BASIC_CFG1*)(&D32))->mv_output_en       = 1;
	((BASIC_CFG1*)(&D32))->max_slcgrp_num     = 1;
	((BASIC_CFG1*)(&D32))->line_num_output_en = 0;
	((BASIC_CFG1*)(&D32))->vdh_2d_en          = 1; // y00226912 K3��1 2D output
	((BASIC_CFG1*)(&D32))->compress_en        = ((BASIC_CFG1*)(&pVdhRegCfg->VdhBasicCfg1))->compress_en;
	((BASIC_CFG1*)(&D32))->ppfd_en            = 0;

	write_register_value(0  ,  VREG_BASIC_CFG1, D32);

	write_register_value(0  , VREG_AVM_ADDR, pVdhRegCfg->VdhAvmAddr);
	write_register_value(0  , VREG_VAM_ADDR, pVdhRegCfg->VdhVamAddr);
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

	write_register_value(0  , VREG_YSTADDR_1D, pVdhRegCfg->VdhYstAddr);
	write_register_value(0  , VREG_YSTRIDE_1D, pVdhRegCfg->VdhYstride);
	write_register_value(0  , VREG_UVOFFSET_1D, pVdhRegCfg->VdhUvoffset);
	write_register_value(0  , VREG_FF_APT_EN, pVdhRegCfg->VdhFfAptEn);
	write_register_value(0  , VREG_REF_PIC_TYPE, pVdhRegCfg->VdhRefPicType);

	write_register_value(0  , VREG_HEAD_INF_OFFSET, pVdhRegCfg->VdhHeadInfOffset);
	return 0;
}

 int VC1HAL_StartDec(OMXVDH_REG_CFG_S *pVdhRegCfg)
{
	VC1HAL_CfgReg(pVdhRegCfg);
	return 0;
}

#endif
