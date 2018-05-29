/*
 *      Video Decode driver for hikey960   mpeg-4 master
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

 int MP4HAL_CfgReg(OMXVDH_REG_CFG_S *pVdhRegCfg)
{
	 int datTmp;
	 int D32;

	//BASIC_CFG0
	D32 = 0;
	D32 = (pVdhRegCfg->VdhBasicCfg0 & 0x000FFFFF) // [15:0] mbamt_to_dec
		  | ( 1 << 22 )
		  | ( 0 << 23 )
		  | ( 0 << 24 )
		  | ( 1 << 25 )
		  | ( 1 << 30 )     // ld_qmatrix_flag
		  | ( 0 << 31 );    // Normal Mode

	write_register_value(0  ,  VREG_BASIC_CFG0, D32);

	/*set uv order 0: v first; 1: u first*/
	D32 = 0x2               // [3:0] video_standard
		  | (((pVdhRegCfg->VdhBasicCfg1 >> 13) & 0x1) << 13 )     // uv_order_en
		  | ( 1 << 14 )     // [14] fst_slc_grp
		  | ( 1 << 15 )     // [15] mv_output_en
		  | ( 1 << 16 )     // [27:16] max_slcgrp_num
		  | ( 0 << 28)      // line_num_output_en
		  | ( 1 << 29)      // vdh_2d_en  //l00214825 0710
		  | (((pVdhRegCfg->VdhBasicCfg1 >> 30) & 0x1) << 30)   //compress_en
		  | ( 0 << 31 );    // [31] ppfd_en   0==not ppfd dec

	write_register_value(0  , VREG_BASIC_CFG1, D32);

	D32 = (pVdhRegCfg->VdhAvmAddr) & 0xFFFFFFF0;  // mpeg4 down msg
	write_register_value(0  , VREG_AVM_ADDR, D32);

	D32 = (pVdhRegCfg->VdhVamAddr) & 0xFFFFFFF0;  // mpeg4 up msg
	write_register_value(0  , VREG_VAM_ADDR, D32);

	write_register_value(0  , VREG_STREAM_BASE_ADDR, pVdhRegCfg->VdhStreamBaseAddr);

	datTmp = RD_SCDREG(REG_EMAR_ID);
	if (pVdhRegCfg->VdhEmarId == 0)
		datTmp = datTmp & (~(0x10000));
	else
		datTmp = datTmp | (0x10000);

	WR_SCDREG(REG_EMAR_ID, datTmp);

	D32 = 0x00300C03;
	write_register_value(0  , VREG_SED_TO,    D32);
	write_register_value(0  , VREG_ITRANS_TO, D32);
	write_register_value(0  , VREG_PMV_TO,    D32);
	write_register_value(0  , VREG_PRC_TO,    D32);
	write_register_value(0  , VREG_RCN_TO,    D32);
	write_register_value(0  , VREG_DBLK_TO,   D32);
	write_register_value(0  , VREG_PPFD_TO,   D32);

	D32 = (pVdhRegCfg->VdhYstAddr) & 0xFFFFFFF0;
	write_register_value(0  , VREG_YSTADDR_1D, D32);

	write_register_value(0  , VREG_YSTRIDE_1D, pVdhRegCfg->VdhYstride);

	write_register_value(0  , VREG_UVOFFSET_1D, pVdhRegCfg->VdhUvoffset);

	D32 = 0;
	//PRCNUM_1D_CNT
	write_register_value(0  , VREG_HEAD_INF_OFFSET, D32);

	D32 = 0;
	write_register_value(0  , VREG_FF_APT_EN, D32);
	return 0;
}

 int MP4HAL_StartDec(OMXVDH_REG_CFG_S *pVdhRegCfg)
{
	 int ret;

	ret = MP4HAL_CfgReg(pVdhRegCfg);

	if (ret != 0) {
		dprint(KERN_CRIT, "MP4HAL_CfgReg failed\n");
		return -1;
	}
	return 0;
}
