/*
 *      Video Decode driver for hikey960   vp8 master
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
 
#ifndef __VDM_HAL_VP8_C__
#define __VDM_HAL_VP8_C__

#include "vdm_hal_api.h"
#include "vdm_hal_local.h"

 int VP8HAL_CfgReg(OMXVDH_REG_CFG_S *pVdhRegCfg)
{
	unsigned int datTmp;
	 int D32;

	//BASIC_CFG0
	D32 = 0;
	((BASIC_CFG0 *)(&D32))->mbamt_to_dec      = ((BASIC_CFG0 *)(&pVdhRegCfg->VdhBasicCfg0))->mbamt_to_dec;
	((BASIC_CFG0 *)(&D32))->load_qmatrix_flag = 0;
	((BASIC_CFG0 *)(&D32))->sec_mode_en       = 0;

	write_register_value(0  ,  VREG_BASIC_CFG0, D32);

	//BASIC_CFG1
	/*set uv order 0: v first; 1: u first*/
	D32 = 0x20000000;
	((BASIC_CFG1 *)(&D32))->video_standard    = 0x0C;
	((BASIC_CFG1 *)(&D32))->ddr_stride        = ((BASIC_CFG1 *)(&pVdhRegCfg->VdhBasicCfg1))->ddr_stride;
	((BASIC_CFG1 *)(&D32))->fst_slc_grp       = 1;
	((BASIC_CFG1 *)(&D32))->mv_output_en      = 1;
	((BASIC_CFG1 *)(&D32))->uv_order_en       = ((BASIC_CFG1 *)(&pVdhRegCfg->VdhBasicCfg1))->uv_order_en;
	((BASIC_CFG1 *)(&D32))->vdh_2d_en         = 1;
	((BASIC_CFG1 *)(&D32))->max_slcgrp_num    = 0;
	((BASIC_CFG1 *)(&D32))->compress_en       = ((BASIC_CFG1 *)(&pVdhRegCfg->VdhBasicCfg1))->compress_en;
	((BASIC_CFG1 *)(&D32))->ppfd_en           = 0;

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
	write_register_value(0  , VREG_STREAM_BASE_ADDR, pVdhRegCfg->VdhStreamBaseAddr);

	//PPFD_BUF_ADDR
	D32 = (pVdhRegCfg->VdhPpfdBufAddr) & 0xFFFFFFF0;
	write_register_value(0  , VREG_PPFD_BUF_ADDR, D32);

	//PPFD_BUF_LEN
	write_register_value(0  , VREG_PPFD_BUF_LEN, pVdhRegCfg->VdhPpfdBufLen);

	//EMAR_ID
	datTmp = RD_SCDREG(REG_EMAR_ID);
	if (pVdhRegCfg->VdhEmarId == 0)
		datTmp = datTmp & (~(0x10000));
	else
		datTmp = datTmp | (0x10000);
	WR_SCDREG(REG_EMAR_ID, datTmp);

	//YSTADDR_1D
	D32 = 0;
	((YSTADDR_1D *)(&D32))->ystaddr_1d = (pVdhRegCfg->VdhYstAddr) & 0xFFFFFFF0; //caution
	write_register_value(0  , VREG_YSTADDR_1D, D32);

	//YSTRIDE_1D
	write_register_value(0  , VREG_YSTRIDE_1D, pVdhRegCfg->VdhYstride);

	//UVOFFSET_1D
	write_register_value(0  , VREG_UVOFFSET_1D, pVdhRegCfg->VdhUvoffset);

	write_register_value(0  , VREG_HEAD_INF_OFFSET, pVdhRegCfg->VdhHeadInfOffset);
	return 0;
}

 int VP8HAL_StartDec(OMXVDH_REG_CFG_S *pVdhRegCfg)
{
	VP8HAL_CfgReg(pVdhRegCfg);
	return 0;
}

#endif
