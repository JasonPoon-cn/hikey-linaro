/*
 *      Video Encode driver for hikey960  Set Register Value
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

#include "drv_venc.h"
#include <linux/hisi/hisi-iommu.h>
#include <linux/iommu.h>


enum {
	VEDU_H265	= 0,
	VEDU_H264   = 1
};

void VENC_HAL_ClrAllInt(S_HEVC_AVC_REGS_TYPE *pVeduReg)
{
	pVeduReg->VEDU_VCPI_REGS.VEDU_INTCLR_NS.u32 = 0xFFFFFFFF;
}

void VENC_HAL_DisableAllInt(S_HEVC_AVC_REGS_TYPE *pVeduReg)
{
	pVeduReg->VEDU_VCPI_REGS.VEDU_INTMASK_NS.u32 = 0;
}

void VENC_HAL_SetSmmuAddr(S_HEVC_AVC_REGS_TYPE *pVeduReg , unsigned int RdAddr , unsigned int WrAddr)
{
	pVeduReg->VEDU_COMN1_REGS.COMN1_SMMU_ERR_RDADDR = RdAddr;
	pVeduReg->VEDU_COMN1_REGS.COMN1_SMMU_ERR_WRADDR = WrAddr;
}

void VENC_HAL_StartEncode(S_HEVC_AVC_REGS_TYPE *pVeduReg)
{
	if (pVeduReg) {
		pVeduReg->VEDU_VCPI_REGS.VEDU_START.u32 = 0;
		pVeduReg->VEDU_VCPI_REGS.VEDU_START.u32 = 1;
	}
}

void VENC_HAL_Get_CfgRegSimple(VENC_REG_INFO_S *pVeduReg , S_HEVC_AVC_REGS_TYPE * pAllReg)
{
	pVeduReg->VEDU_VCPI_REGS.VEDU_RAWINT_NS.bits.rint_ve_eop_ns = pAllReg->VEDU_VCPI_REGS.VEDU_RAWINT_NS.bits.rint_ve_eop_ns;
	pVeduReg->VEDU_VCPI_REGS.VEDU_RAWINT_NS.bits.rint_ve_buffull_ns = pAllReg->VEDU_VCPI_REGS.VEDU_RAWINT_NS.bits.rint_ve_buffull_ns;

	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR00.bits.slc_len0 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR00.bits.slc_len0;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR10.bits.slc_len1 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR11.bits.invalidnum1;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR20.bits.slc_len2 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR20.bits.slc_len2;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR30.bits.slc_len3 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR30.bits.slc_len3;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR40.bits.slc_len4 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR40.bits.slc_len4;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR50.bits.slc_len5 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR50.bits.slc_len5;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR60.bits.slc_len6 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR60.bits.slc_len6;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR70.bits.slc_len7 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR70.bits.slc_len7;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR80.bits.slc_len8 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR80.bits.slc_len8;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR90.bits.slc_len9 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR90.bits.slc_len9;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR100.bits.slc_len10 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR100.bits.slc_len10;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR110.bits.slc_len11 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR110.bits.slc_len11;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR120.bits.slc_len12 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR120.bits.slc_len12;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR130.bits.slc_len13 =  pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR130.bits.slc_len13;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR140.bits.slc_len14 =  pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR140.bits.slc_len14;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR150.bits.slc_len15 =  pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR150.bits.slc_len15;

	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR01.bits.invalidnum0 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR01.bits.invalidnum0;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR11.bits.invalidnum1 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR11.bits.invalidnum1;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR21.bits.invalidnum2 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR21.bits.invalidnum2;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR31.bits.invalidnum3 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR31.bits.invalidnum3;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR41.bits.invalidnum4 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR41.bits.invalidnum4;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR51.bits.invalidnum5 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR51.bits.invalidnum5;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR61.bits.invalidnum6 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR61.bits.invalidnum6;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR71.bits.invalidnum7 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR71.bits.invalidnum7;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR81.bits.invalidnum8 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR81.bits.invalidnum8;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR91.bits.invalidnum9 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR91.bits.invalidnum9;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR101.bits.invalidnum10 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR101.bits.invalidnum10;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR111.bits.invalidnum11 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR111.bits.invalidnum11;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR121.bits.invalidnum12 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR121.bits.invalidnum12;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR131.bits.invalidnum13 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR131.bits.invalidnum13 ;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR141.bits.invalidnum14 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR141.bits.invalidnum14;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR151.bits.invalidnum15 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR151.bits.invalidnum15;
}

void VENC_HAL_Get_Reg_Venc(VENC_REG_INFO_S *pVeduReg , S_HEVC_AVC_REGS_TYPE * pAllReg)
{
	pVeduReg->VEDU_VCPI_REGS.VEDU_RAWINT_NS.bits.rint_ve_eop_ns = pAllReg->VEDU_VCPI_REGS.VEDU_RAWINT_NS.bits.rint_ve_eop_ns;
	pVeduReg->VEDU_VCPI_REGS.VEDU_RAWINT_NS.bits.rint_ve_buffull_ns = pAllReg->VEDU_VCPI_REGS.VEDU_RAWINT_NS.bits.rint_ve_buffull_ns;

	pVeduReg->VEDU_CABAC_REGS.CABAC_PIC_STRMSIZE = pAllReg->VEDU_CABAC_REGS.CABAC_PIC_STRMSIZE;
	pVeduReg->VEDU_SEL_REGS.SEL_OPT_4X4_CNT.bits.opt_4x4_cnt = pAllReg->VEDU_SEL_REGS.SEL_OPT_4X4_CNT.bits.opt_4x4_cnt;
	pVeduReg->VEDU_SEL_REGS.SEL_INTRA_OPT_8X8_CNT.bits.intra_opt_8x8_cnt = pAllReg->VEDU_SEL_REGS.SEL_INTRA_OPT_8X8_CNT.bits.intra_opt_8x8_cnt;
	pVeduReg->VEDU_SEL_REGS.SEL_INTRA_OPT_16X16_CNT.bits.intra_opt_16x16_cnt = pAllReg->VEDU_SEL_REGS.SEL_INTRA_OPT_16X16_CNT.bits.intra_opt_16x16_cnt;
	pVeduReg->VEDU_SEL_REGS.SEL_INTRA_OPT_32X32_CNT.bits.intra_opt_32x32_cnt = pAllReg->VEDU_SEL_REGS.SEL_INTRA_OPT_32X32_CNT.bits.intra_opt_32x32_cnt;

	pVeduReg->VEDU_VLC_REGS.VLC_PIC_TOTAL_BITS = pAllReg->VEDU_VLC_REGS.VLC_PIC_TOTAL_BITS;
	pVeduReg->VEDU_SEL_REGS.SEL_INTRA_PCM_OPT_8X8_CNT.bits.pcm_opt_8x8_cnt = pAllReg->VEDU_SEL_REGS.SEL_INTRA_PCM_OPT_8X8_CNT.bits.pcm_opt_8x8_cnt;
	pVeduReg->VEDU_SEL_REGS.SEL_INTRA_OPT_16X16_CNT.bits.intra_opt_16x16_cnt = pAllReg->VEDU_SEL_REGS.SEL_INTRA_OPT_16X16_CNT.bits.intra_opt_16x16_cnt;

	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR00.bits.slc_len0 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR00.bits.slc_len0;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR10.bits.slc_len1 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR10.bits.slc_len1;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR20.bits.slc_len2 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR20.bits.slc_len2;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR30.bits.slc_len3 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR30.bits.slc_len3;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR40.bits.slc_len4 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR40.bits.slc_len4;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR50.bits.slc_len5 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR50.bits.slc_len5;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR60.bits.slc_len6 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR60.bits.slc_len6;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR70.bits.slc_len7 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR70.bits.slc_len7;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR80.bits.slc_len8 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR80.bits.slc_len8;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR90.bits.slc_len9 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR90.bits.slc_len9;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR100.bits.slc_len10 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR100.bits.slc_len10;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR110.bits.slc_len11 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR110.bits.slc_len11;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR120.bits.slc_len12 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR120.bits.slc_len12;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR130.bits.slc_len13 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR130.bits.slc_len13;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR140.bits.slc_len14 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR140.bits.slc_len14 ;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR150.bits.slc_len15 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR150.bits.slc_len15;

	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR01.bits.invalidnum0 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR01.bits.invalidnum0;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR11.bits.invalidnum1 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR11.bits.invalidnum1;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR21.bits.invalidnum2 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR21.bits.invalidnum2;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR31.bits.invalidnum3 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR31.bits.invalidnum3;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR41.bits.invalidnum4 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR41.bits.invalidnum4;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR51.bits.invalidnum5 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR51.bits.invalidnum5;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR61.bits.invalidnum6 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR61.bits.invalidnum6;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR71.bits.invalidnum7 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR71.bits.invalidnum7;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR81.bits.invalidnum8 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR81.bits.invalidnum8;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR91.bits.invalidnum9 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR91.bits.invalidnum9;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR101.bits.invalidnum10 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR101.bits.invalidnum10;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR111.bits.invalidnum11 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR111.bits.invalidnum11;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR121.bits.invalidnum12 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR121.bits.invalidnum12;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR131.bits.invalidnum13 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR131.bits.invalidnum13;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR141.bits.invalidnum14 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR141.bits.invalidnum14;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR151.bits.invalidnum15 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR151.bits.invalidnum15;

	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR01.bits.islastslc0 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR01.bits.islastslc0;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR11.bits.islastslc1 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR11.bits.islastslc1;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR21.bits.islastslc2 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR21.bits.islastslc2;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR31.bits.islastslc3 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR31.bits.islastslc3;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR41.bits.islastslc4 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR41.bits.islastslc4;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR51.bits.islastslc5 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR51.bits.islastslc5;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR61.bits.islastslc6 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR61.bits.islastslc6;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR71.bits.islastslc7 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR71.bits.islastslc7;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR81.bits.islastslc8 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR81.bits.islastslc8;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR91.bits.islastslc9 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR91.bits.islastslc9;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR101.bits.islastslc10 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR101.bits.islastslc10;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR111.bits.islastslc11 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR111.bits.islastslc11;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR121.bits.islastslc12 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR121.bits.islastslc12;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR131.bits.islastslc13 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR131.bits.islastslc13;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR141.bits.islastslc14 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR141.bits.islastslc14;
	pVeduReg->VEDU_VLCST_REGS.VLCST_DSRPTR151.bits.islastslc15 = pAllReg->VEDU_VLCST_REGS.VLCST_DSRPTR151.bits.islastslc15;

	pVeduReg->VEDU_SAO_REGS.SAO_LCU_CNT.bits.sao2vcpi_lcu_cnt = pAllReg->VEDU_SAO_REGS.SAO_LCU_CNT.bits.sao2vcpi_lcu_cnt;
	pVeduReg->VEDU_SAO_REGS.SAO_OFF_NUM.bits.sao2vcpi_saooff_num_chroma = pAllReg->VEDU_SAO_REGS.SAO_OFF_NUM.bits.sao2vcpi_saooff_num_chroma;
	pVeduReg->VEDU_SAO_REGS.SAO_OFF_NUM.bits.sao2vcpi_saooff_num_luma = pAllReg->VEDU_SAO_REGS.SAO_OFF_NUM.bits.sao2vcpi_saooff_num_luma;

	pVeduReg->VEDU_VCPI_REGS.VEDU_TIMER = pAllReg->VEDU_VCPI_REGS.VEDU_TIMER;
	pVeduReg->VEDU_VCPI_REGS.VEDU_IDLE_TIMER = pAllReg->VEDU_VCPI_REGS.VEDU_IDLE_TIMER;

}
void VeduHal_CfgRegSimple( VENC_REG_INFO_S *channelcfg , S_HEVC_AVC_REGS_TYPE * pAllReg)
{
	unsigned int vcpi_protocol;
	vcpi_protocol = channelcfg->VEDU_MMU1_REGS.MST_VEDU_MODE_MMU.bits.vcpi_protocol;

	pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_idr_pic = channelcfg->vcpi_idr_pic ;
	pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_frame_type = channelcfg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_frame_type;
	pAllReg->VEDU_VCPI_REGS.VEDU_QPCFG.bits.vcpi_frm_qp = channelcfg->VEDU_VCPI_REGS.VEDU_QPCFG.bits.vcpi_frm_qp;
	pAllReg->VEDU_QPG_REGS.QPG_AVERAGE_LCU_BITS.bits.ave_lcu_bits = channelcfg->ave_lcu_bits;
	pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_sao_chroma = channelcfg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_sao_chroma;
	pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_sao_luma = channelcfg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_sao_luma;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_MODE_MMU.bits.vcpi_protocol = channelcfg->VEDU_MMU1_REGS.MST_VEDU_MODE_MMU.bits.vcpi_protocol;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_MODE_MMU.bits.vcpi_str_fmt = channelcfg->VEDU_MMU1_REGS.MST_VEDU_MODE_MMU.bits.vcpi_str_fmt;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_NBI_MVST_ADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_NBI_MVST_ADDR_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_NBI_MVST_ADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_NBI_MVST_ADDR_END;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_NBI_MVLD_ADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_NBI_MVLD_ADDR_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_NBI_MVLD_ADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_NBI_MVLD_ADDR_END;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_PMEST_ADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_PMEST_ADDR_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_PMEST_ADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_PMEST_ADDR_END;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_PMELD_ADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_PMELD_ADDR_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_PMELD_ADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_PMELD_ADDR_END;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_YH_ADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REC_YH_ADDR_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_YH_ADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REC_YH_ADDR_END;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_CH_ADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REC_CH_ADDR_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_CH_ADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REC_CH_ADDR_END;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_YADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REC_YADDR_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_YADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REC_YADDR_END;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_CADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REC_CADDR_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_CADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REC_CADDR_END;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_YH_ADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REF_YH_ADDR_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_YH_ADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REF_YH_ADDR_END;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_CH_ADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REF_CH_ADDR_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_CH_ADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REF_CH_ADDR_END;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_YADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REF_YADDR_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_YADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REF_YADDR_END;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_CADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REF_CADDR_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_CADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_REF_CADDR_END;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_SRC_YADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_SRC_YADDR_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_SRC_YADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_SRC_YADDR_END;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_SRC_CADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_SRC_CADDR_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_SRC_CADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_SRC_CADDR_END;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_SRC_VADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_SRC_VADDR_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_SRC_VADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_SRC_VADDR_END ;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_LAT_ADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_LAT_ADDR_STR;//add by gaoyajun
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_LAT_ADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_LAT_ADDR_END;

	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR0_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR0_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR1_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR1_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR2_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR2_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR3_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR3_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR4_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR4_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR5_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR5_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR6_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR6_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR7_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR7_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR8_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR8_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR9_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR9_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR10_STR =channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR10_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR11_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR11_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR12_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR12_STR ;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR13_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR13_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR14_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR14_STR;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR15_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR15_STR;


	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR0_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR0_END;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR1_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR1_END;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR2_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR2_END;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR3_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR3_END;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR4_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR4_END;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR5_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR5_END ;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR6_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR6_END;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR7_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR7_END;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR8_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR8_END;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR9_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR9_END;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR10_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR10_END;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR11_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR11_END;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR12_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR12_END;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR13_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR13_END;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR14_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR14_END;
	pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR15_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR15_END;

    // reg cfg end
    /*-----------------------------------------  µØÖ·¶îÍâÅäÖÃ l00214825  ---------------------------------------------*/
	pAllReg->VEDU_VCPI_REGS.VEDU_TUNLCELL_ADDR = channelcfg->VEDU_VCPI_REGS.VEDU_TUNLCELL_ADDR;
	pAllReg->VEDU_VCPI_REGS.VEDU_SRC_YADDR     = channelcfg->VEDU_VCPI_REGS.VEDU_SRC_YADDR  ;
	pAllReg->VEDU_VCPI_REGS.VEDU_SRC_CADDR     = channelcfg->VEDU_VCPI_REGS.VEDU_SRC_CADDR ;
	pAllReg->VEDU_VCPI_REGS.VEDU_SRC_VADDR     = channelcfg->VEDU_VCPI_REGS.VEDU_SRC_VADDR  ;
	pAllReg->VEDU_VCPI_REGS.VEDU_YH_ADDR       = channelcfg->VEDU_VCPI_REGS.VEDU_YH_ADDR;
	pAllReg->VEDU_VCPI_REGS.VEDU_CH_ADDR       = channelcfg->VEDU_VCPI_REGS.VEDU_CH_ADDR ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_package_sel  = channelcfg->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_package_sel;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_str_fmt = channelcfg->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_str_fmt;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_store_mode  = channelcfg->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_store_mode ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRIDE.bits.curld_y_stride = channelcfg->VEDU_VCPI_REGS.VEDU_STRIDE.bits.curld_y_stride ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRIDE.bits.curld_c_stride = channelcfg->VEDU_VCPI_REGS.VEDU_STRIDE.bits.curld_c_stride;

    // curld yh Êµ¼ÊÃ»ÓÐÆð×÷ÓÃ
	pAllReg->VEDU_VCPI_REGS.VEDU_HEAD_STRIDE.bits.curld_yh_stride = channelcfg->VEDU_VCPI_REGS.VEDU_STRIDE.bits.curld_y_stride*2;
	pAllReg->VEDU_VCPI_REGS.VEDU_HEAD_STRIDE.bits.curld_ch_stride = channelcfg->VEDU_VCPI_REGS.VEDU_STRIDE.bits.curld_c_stride*2;

	pAllReg->VEDU_VCPI_REGS.VEDU_REC_YADDR  = channelcfg->VEDU_VCPI_REGS.VEDU_REC_YADDR ;
	pAllReg->VEDU_VCPI_REGS.VEDU_REC_CADDR   = channelcfg->VEDU_VCPI_REGS.VEDU_REC_CADDR;

    /* Define the union U_VEDU_REC_STRIDE */
    pAllReg->VEDU_VCPI_REGS.VEDU_REC_STRIDE.u32 = channelcfg->VEDU_VCPI_REGS.VEDU_REC_STRIDE.u32 ;

    // REC_YH_ADDR
	pAllReg->VEDU_VCPI_REGS.VEDU_REC_YH_ADDR  = channelcfg->VEDU_VCPI_REGS.VEDU_REC_YH_ADDR ;
    //REC_CH_ADDR
	pAllReg->VEDU_VCPI_REGS.VEDU_REC_CH_ADDR = channelcfg->VEDU_VCPI_REGS.VEDU_REC_CH_ADDR ;
    //REC_HEAD_STRIDE
	pAllReg->VEDU_VCPI_REGS.VEDU_REC_HEAD_STRIDE.u32 = channelcfg->VEDU_VCPI_REGS.VEDU_REC_HEAD_STRIDE.u32;

     //VEDU_LUMA_REF_ADDR
	pAllReg->VEDU_VCPI_REGS.VEDU_REF_YADDR = channelcfg->VEDU_VCPI_REGS.VEDU_REF_YADDR ;
     //VEDU_CHROMA_REF_ADDR
	pAllReg->VEDU_VCPI_REGS.VEDU_REF_CADDR = channelcfg->VEDU_VCPI_REGS.VEDU_REF_CADDR;

     /* Define the union U_VEDU_VSTRIDE */
	pAllReg->VEDU_VCPI_REGS.VEDU_REF_STRIDE.u32 = channelcfg->VEDU_VCPI_REGS.VEDU_REF_STRIDE.u32;

     //VEDU_REF_YH_ADDR
	pAllReg->VEDU_VCPI_REGS.VEDU_REF_YH_ADDR  = channelcfg->VEDU_VCPI_REGS.VEDU_REF_YH_ADDR ;
     //VEDU_REF_CH_ADDR
	pAllReg->VEDU_VCPI_REGS.VEDU_REF_CH_ADDR  =channelcfg->VEDU_VCPI_REGS.VEDU_REF_CH_ADDR ;
     /* Define the union U_VEDU_REF_HSTRIDE */

	pAllReg->VEDU_VCPI_REGS.VEDU_REF_HSTRIDE.u32 = channelcfg->VEDU_VCPI_REGS.VEDU_REF_HSTRIDE.u32;

     ///////VEDU_PMELD_ADDR
	pAllReg->VEDU_VCPI_REGS.VEDU_PMELD_ADDR  = channelcfg->VEDU_VCPI_REGS.VEDU_PMELD_ADDR ;
	pAllReg->VEDU_VCPI_REGS.VEDU_PMEST_ADDR  = channelcfg->VEDU_VCPI_REGS.VEDU_PMEST_ADDR ;
	pAllReg->VEDU_VCPI_REGS.VEDU_NBI_MVST_ADDR   = channelcfg->VEDU_VCPI_REGS.VEDU_NBI_MVST_ADDR ;
	pAllReg->VEDU_VCPI_REGS.VEDU_NBI_MVLD_ADDR   = channelcfg->VEDU_VCPI_REGS.VEDU_NBI_MVLD_ADDR ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR0  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR0 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR1  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR1;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR2  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR2;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR3  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR3 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR4  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR4;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR5  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR5;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR6  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR6;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR7  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR7 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR8  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR8;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR9  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR9 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR10  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR10;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR11  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR11;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR12  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR12;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR13  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR13;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR14  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR14;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR15  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMADDR15 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN0  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN0 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN1  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN1 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN2  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN2 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN3  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN3 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN4  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN4 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN5  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN5 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN6  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN6;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN7  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN7 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN8  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN8 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN9  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN9 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN10  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN10 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN11  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN11;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN12  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN12 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN13  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN13  ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN14  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN14 ;
	pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN15  = channelcfg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN15 ;

#ifdef HARDWARE_SPLIT_SPS_PPS_EN
	pAllReg->VEDU_VCPI_REGS.VEDU_CURLD_LOWDLY.bits.vcpi_para_sprat_en = 1 ;

        if (channelcfg->bFirstNal2Send)
        {
            pAllReg->VEDU_VCPI_REGS.VEDU_PARA_ADDR = channelcfg->VEDU_VCPI_REGS.VEDU_PARA_ADDR;
            pAllReg->VEDU_MMU1_REGS.MST_VEDU_PPS_ADDR_STR = channelcfg->VEDU_MMU1_REGS.MST_VEDU_PPS_ADDR_STR ;
            pAllReg->VEDU_MMU1_REGS.MST_VEDU_PPS_ADDR_END = channelcfg->VEDU_MMU1_REGS.MST_VEDU_PPS_ADDR_END;
        }
#else
	pAllReg->VEDU_VCPI_REGS.VEDU_CURLD_LOWDLY.bits.vcpi_para_sprat_en = 0;
#endif

     /*-----------------------------------------  地址额外配置end l00214825  ---------------------------------------------*/

     /*-----------------------------------------  slice 寄存器额外配置l00214825  ---------------------------------------------*/

    /* Define the union U_CABAC_GLB_CFG */
    {
        pAllReg->VEDU_CABAC_REGS.CABAC_GLB_CFG.u32 = channelcfg->VEDU_CABAC_REGS.CABAC_GLB_CFG.u32;
    }

    /* Define the union U_CABAC_SLCHDR_SIZE */
    {
       pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_SIZE.u32 = channelcfg->VEDU_CABAC_REGS.CABAC_SLCHDR_SIZE.u32;
    }
    /* Define the union U_CABAC_SLCHDR_PART1 */
    {
       pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART1.u32 = channelcfg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART1.u32;
    }

    /* Define the union U_CABAC_SLCHDR_PART2_SEG1 */

    if (vcpi_protocol== VEDU_H265) //add by m00218451
    {
        pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG1 = channelcfg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG1;
        if (channelcfg->slchdr_size_part2 > 32)
        {
            pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG2   = channelcfg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG2 ;
        }
        pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG3   = channelcfg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG3 ;
        pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG4   = channelcfg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG4 ;
	 pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG5   = channelcfg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG5;
        pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG6   = channelcfg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG6;
        pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG7   = channelcfg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG7;
        pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG8   = channelcfg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG8;
    }
    else
    {
        pAllReg->VEDU_VLC_REGS.VLC_SlcHdrStrm0 = channelcfg->VEDU_VLC_REGS.VLC_SlcHdrStrm0;
        pAllReg->VEDU_VLC_REGS.VLC_SlcHdrStrm1 = channelcfg->VEDU_VLC_REGS.VLC_SlcHdrStrm1;
        pAllReg->VEDU_VLC_REGS.VLC_SlcHdrStrm2 = channelcfg->VEDU_VLC_REGS.VLC_SlcHdrStrm2;
        pAllReg->VEDU_VLC_REGS.VLC_SlcHdrStrm3 = channelcfg->VEDU_VLC_REGS.VLC_SlcHdrStrm3 ;
        pAllReg->VEDU_VLC_REGS.VLC_ReorderStrm0 = channelcfg->VEDU_VLC_REGS.VLC_ReorderStrm0;
        pAllReg->VEDU_VLC_REGS.VLC_ReorderStrm1  = channelcfg->VEDU_VLC_REGS.VLC_ReorderStrm1;
        pAllReg->VEDU_VLC_REGS.VLC_MarkingStrm0  = channelcfg->VEDU_VLC_REGS.VLC_MarkingStrm0;
        pAllReg->VEDU_VLC_REGS.VLC_MarkingStrm1 = channelcfg->VEDU_VLC_REGS.VLC_MarkingStrm1;
        pAllReg->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_parabit = channelcfg->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_parabit ;
        pAllReg->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_reorderbit = channelcfg->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_reorderbit;
        pAllReg->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_markingbit = channelcfg->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_markingbit;
    }
	 pAllReg->VEDU_VLCST_REGS.VLCST_PARAMETER.bits.vlcst_para_set_en = 0;

#ifdef HARDWARE_SPLIT_SPS_PPS_EN
    if (channelcfg->vlcst_para_set_en == 1)
    {
        pAllReg->VEDU_VLCST_REGS.VLCST_PARAMETER.bits.vlcst_para_set_en = channelcfg->VEDU_VLCST_REGS.VLCST_PARAMETER.bits.vlcst_para_set_en;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARAMETER.bits.vlcst_para_set_len = channelcfg->VEDU_VLCST_REGS.VLCST_PARAMETER.bits.vlcst_para_set_len;
        //pEncPara_channel->vlcst_para_set_en = 0;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA0 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA0 ;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA1 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA1;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA2 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA2;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA3 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA3;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA4 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA3;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA5 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA5;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA6 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA6;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA7 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA7;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA8 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA8;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA9 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA9;

        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA10 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA10;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA11 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA11;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA12 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA12;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA13 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA13 ;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA14 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA14;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA15 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA15;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA16 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA16;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA17 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA17;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA18 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA18;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA19 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA19;

        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA20 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA20;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA21 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA21;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA22 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA22;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA23 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA23;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA24 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA24;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA25 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA25;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA26 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA26;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA27 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA27;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA28 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA28;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA29 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA29;

        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA30 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA30;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA31 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA31;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA32 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA32;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA33 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA33;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA34 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA34;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA35 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA35;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA36 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA36;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA37 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA37;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA38 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA38;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA39 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA39;

        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA40 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA40 ;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA41 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA41;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA42 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA42;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA43 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA43;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA44 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA44;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA45 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA45 ;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA46 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA46;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA47 = channelcfg->VEDU_VLCST_REGS.VLCST_PARA_DATA47 ;
    }
#endif
}

void VeduHal_CfgReg( VENC_REG_INFO_S * regcfginfo , S_HEVC_AVC_REGS_TYPE * pAllReg  , unsigned int RdAddr , unsigned int WrAddr , HI_U64 phy_pgd_base)
{
    unsigned int vcpi_protocol;
    
    vcpi_protocol = regcfginfo->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_protocol;

    pAllReg->VEDU_VCPI_REGS.VEDU_INTMASK_NS.bits.enable_cfg_err_ns        = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTMASK_NS.bits.enable_vedu_timeout_ns   = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTMASK_NS.bits.enable_vedu_step_ns      = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTMASK_NS.bits.enable_vedu_brkpt_ns     = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTMASK_NS.bits.enable_vedu_slice_end_ns = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTMASK_NS.bits.enable_ve_pbitsover_ns   = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTMASK_NS.bits.enable_ve_buffull_ns     = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTMASK_NS.bits.enable_ve_eop_ns         = 1;

    pAllReg->VEDU_VCPI_REGS.VEDU_INTCLR_NS.bits.clr_cfg_err_ns        = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTCLR_NS.bits.clr_vedu_timeout_ns   = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTCLR_NS.bits.clr_vedu_step_ns      = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTCLR_NS.bits.clr_vedu_brkpt_ns     = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTCLR_NS.bits.clr_vedu_slice_end_ns = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTCLR_NS.bits.clr_ve_pbitsover_ns   = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTCLR_NS.bits.clr_ve_buffull_ns     = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTCLR_NS.bits.clr_ve_eop_ns         = 0;

    pAllReg->VEDU_VCPI_REGS.VEDU_START.bits.vcpi_vstep  = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_START.bits.vcpi_vstart = 0;

    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_dbgmod  = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_bkp_en  = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_time_en = 0;

    if (VEDU_H265 == vcpi_protocol)
    {
        pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_lcu_size = 2;
    }
    else if (VEDU_H264 == vcpi_protocol)
    {
        pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_lcu_size = 0;
    }
    else
    {
        pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_lcu_size = 3;
    }

    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_tiles_en   = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_ref_cmp_en = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_trans_mode = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_pskp_en    = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_trans_mode = regcfginfo->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_trans_mode;

    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_idr_pic        = regcfginfo->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_idr_pic;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_frame_type     = regcfginfo->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_frame_type;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_entropy_mode   = regcfginfo->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_entropy_mode;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_img_improve_en = regcfginfo->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_img_improve_en;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_rec_cmp_en     = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_sao_chroma     = regcfginfo->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_sao_chroma;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_sao_luma       = regcfginfo->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_sao_luma;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_slice_int_en   = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_protocol       = regcfginfo->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_protocol;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_cfg_mode       = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_lcu_time_sel   = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_MODE.bits.vcpi_vedsel         = 0;

    pAllReg->VEDU_VCPI_REGS.VEDU_TILE_SIZE.bits.vcpi_tile_height = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_TILE_SIZE.bits.vcpi_tile_width  = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_SLICE.bits.vcpi_multislc_en     = regcfginfo->VEDU_VCPI_REGS.VEDU_SLICE.bits.vcpi_multislc_en;
    pAllReg->VEDU_VCPI_REGS.VEDU_SLICE.bits.vcpi_slcspilt_mod    = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_SLICE.bits.vcpi_slice_size      = regcfginfo->VEDU_VCPI_REGS.VEDU_SLICE.bits.vcpi_slice_size;

    pAllReg->VEDU_VCPI_REGS.VEDU_QPCFG.bits.vcpi_frm_qp          = regcfginfo->VEDU_VCPI_REGS.VEDU_QPCFG.bits.vcpi_frm_qp;
    pAllReg->VEDU_VCPI_REGS.VEDU_QPCFG.bits.vcpi_cb_qp_offset    = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_QPCFG.bits.vcpi_cr_qp_offset    = 0;

    pAllReg->VEDU_VCPI_REGS.VEDU_CNTCLR  = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_FRAMENO = 0xffffffff;
    pAllReg->VEDU_VCPI_REGS.VEDU_DBLKCFG.bits.vcpi_dblk_filter_flag = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_DBLKCFG.bits.vcpi_dblk_alpha       = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_DBLKCFG.bits.vcpi_dblk_beta        = 0;

    pAllReg->VEDU_VCPI_REGS.VEDU_LOW_POWER.bits.vcpi_protocol_clkgate_en = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_LOW_POWER.bits.vcpi_mem_clkgate_en = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_LOW_POWER.bits.vcpi_clkgate_en     = 2;
    pAllReg->VEDU_VCPI_REGS.VEDU_LOW_POWER.bits.fme_gtck_en         = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_LOW_POWER.bits.mrg_gtck_en         = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_LOW_POWER.bits.tqitq_gtck_en       = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_LOW_POWER.bits.vcpi_ime_lowpow     = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_LOW_POWER.bits.vcpi_fme_lowpow     = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_LOW_POWER.bits.vcpi_intra_lowpow   = 1;

    pAllReg->VEDU_VCPI_REGS.VEDU_PICSIZE_PIX.bits.vcpi_imgheight_pix  = regcfginfo->VEDU_VCPI_REGS.VEDU_PICSIZE_PIX.bits.vcpi_imgheight_pix;
    pAllReg->VEDU_VCPI_REGS.VEDU_PICSIZE_PIX.bits.vcpi_imgwidth_pix   = regcfginfo->VEDU_VCPI_REGS.VEDU_PICSIZE_PIX.bits.vcpi_imgwidth_pix;
    pAllReg->VEDU_VCPI_REGS.VEDU_BP_POS.bits.vcpi_bp_lcu_y            = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_BP_POS.bits.vcpi_bp_lcu_x            = 0;

    pAllReg->VEDU_VCPI_REGS.VEDU_TIMEOUT                    = 0xffffffff;
    pAllReg->VEDU_VCPI_REGS.VEDU_OUTSTD.bits.vcpi_w_outstanding = 7;
    pAllReg->VEDU_VCPI_REGS.VEDU_OUTSTD.bits.vcpi_r_outstanding = 31;

    pAllReg->VEDU_VCPI_REGS.VEDU_SW_SIZE.bits.vcpi_sw_height = regcfginfo->VEDU_VCPI_REGS.VEDU_SW_SIZE.bits.vcpi_sw_height;
    pAllReg->VEDU_VCPI_REGS.VEDU_SW_SIZE.bits.vcpi_sw_width  = regcfginfo->VEDU_VCPI_REGS.VEDU_SW_SIZE.bits.vcpi_sw_width;

    pAllReg->VEDU_VCPI_REGS.VEDU_MV_LOAD.bits.vcpi_tmv_wr_rd_avail = 3;

    pAllReg->VEDU_VCPI_REGS.VEDU_CROSS_TILE_SLC.bits.vcpi_cross_tile  = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_CROSS_TILE_SLC.bits.vcpi_cross_slice = 1;

    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd_en     = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd7_absqp = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd6_absqp = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd5_absqp = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd4_absqp = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd3_absqp = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd2_absqp = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd1_absqp = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd0_absqp = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd7_en    = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd6_en    = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd5_en    = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd4_en    = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd3_en    = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd2_en    = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd1_en    = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_ENABLE.bits.osd0_en    = 0;

    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_0.bits.osd0_y = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_0.bits.osd0_x = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_1.bits.osd1_y = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_1.bits.osd1_x = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_2.bits.osd2_y = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_2.bits.osd2_x = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_3.bits.osd3_y = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_3.bits.osd3_x = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_4.bits.osd4_y = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_4.bits.osd4_x = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_5.bits.osd5_y = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_5.bits.osd5_x = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_6.bits.osd6_y = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_6.bits.osd6_x = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_7.bits.osd7_y = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_POS_7.bits.osd7_x = 0;

    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_0.bits.osd0_h = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_0.bits.osd0_w = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_1.bits.osd1_h = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_1.bits.osd1_w = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_2.bits.osd2_h = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_2.bits.osd2_w = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_3.bits.osd3_h = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_3.bits.osd3_w = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_4.bits.osd4_h = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_4.bits.osd4_w = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_5.bits.osd5_h = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_5.bits.osd5_w = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_6.bits.osd6_h = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_6.bits.osd6_w = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_7.bits.osd7_h = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_SIZE_7.bits.osd7_w = 0;

    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_LAYERID.bits.osd7_layer_id = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_LAYERID.bits.osd6_layer_id = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_LAYERID.bits.osd5_layer_id = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_LAYERID.bits.osd4_layer_id = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_LAYERID.bits.osd3_layer_id = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_LAYERID.bits.osd2_layer_id = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_LAYERID.bits.osd1_layer_id = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_LAYERID.bits.osd0_layer_id = 0;

    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_QP0.bits.osd3_qp = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_QP0.bits.osd2_qp = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_QP0.bits.osd1_qp = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_QP0.bits.osd0_qp = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_QP1.bits.osd7_qp = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_QP1.bits.osd6_qp = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_QP1.bits.osd5_qp = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_OSD_QP1.bits.osd4_qp = 0;

    pAllReg->VEDU_VCPI_REGS.VEDU_TUNLCELL_ADDR   = regcfginfo->VEDU_VCPI_REGS.VEDU_TUNLCELL_ADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_SRC_YADDR       = regcfginfo->VEDU_VCPI_REGS.VEDU_SRC_YADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_SRC_CADDR       = regcfginfo->VEDU_VCPI_REGS.VEDU_SRC_CADDR ;
    pAllReg->VEDU_VCPI_REGS.VEDU_SRC_VADDR       = regcfginfo->VEDU_VCPI_REGS.VEDU_SRC_VADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_YH_ADDR         = regcfginfo->VEDU_VCPI_REGS.VEDU_YH_ADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_CH_ADDR         = regcfginfo->VEDU_VCPI_REGS.VEDU_CH_ADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRIDE.bits.curld_y_stride       = regcfginfo->VEDU_VCPI_REGS.VEDU_STRIDE.bits.curld_y_stride ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRIDE.bits.curld_c_stride       = regcfginfo->VEDU_VCPI_REGS.VEDU_STRIDE.bits.curld_c_stride;
    pAllReg->VEDU_VCPI_REGS.VEDU_HEAD_STRIDE.bits.curld_yh_stride = regcfginfo->VEDU_VCPI_REGS.VEDU_HEAD_STRIDE.bits.curld_yh_stride;
    pAllReg->VEDU_VCPI_REGS.VEDU_HEAD_STRIDE.bits.curld_ch_stride = regcfginfo->VEDU_VCPI_REGS.VEDU_HEAD_STRIDE.bits.curld_ch_stride;

    pAllReg->VEDU_VCPI_REGS.VEDU_REC_YADDR     = regcfginfo->VEDU_VCPI_REGS.VEDU_REC_YADDR ;
    pAllReg->VEDU_VCPI_REGS.VEDU_REC_CADDR     = regcfginfo->VEDU_VCPI_REGS.VEDU_REC_CADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_REC_YH_ADDR   = regcfginfo->VEDU_VCPI_REGS.VEDU_REC_YH_ADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_REC_CH_ADDR   = regcfginfo->VEDU_VCPI_REGS.VEDU_REC_CH_ADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_REF_YADDR     = regcfginfo->VEDU_VCPI_REGS.VEDU_REF_YADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_REF_CADDR     = regcfginfo->VEDU_VCPI_REGS.VEDU_REF_CADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_REF_YH_ADDR   = regcfginfo->VEDU_VCPI_REGS.VEDU_REF_YH_ADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_REF_CH_ADDR   = regcfginfo->VEDU_VCPI_REGS.VEDU_REF_CH_ADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_PMELD_ADDR    = regcfginfo->VEDU_VCPI_REGS.VEDU_PMELD_ADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_PMEST_ADDR    = regcfginfo->VEDU_VCPI_REGS.VEDU_PMEST_ADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_NBI_MVST_ADDR = regcfginfo->VEDU_VCPI_REGS.VEDU_NBI_MVST_ADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_NBI_MVLD_ADDR = regcfginfo->VEDU_VCPI_REGS.VEDU_NBI_MVLD_ADDR ;

    pAllReg->VEDU_VCPI_REGS.VEDU_CURLD_LOWDLY.bits.curld_lowdly_en      = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_CURLD_LOWDLY.bits.curld_read_interval  = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_package_sel           = regcfginfo->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_package_sel ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_str_fmt               = regcfginfo->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_str_fmt;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_blk_type              = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_store_mode            = regcfginfo->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_store_mode;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_scale_en              = regcfginfo->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_scale_en;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_crop_en               = regcfginfo->VEDU_VCPI_REGS.VEDU_STRFMT.bits.vcpi_crop_en;
    pAllReg->VEDU_VCPI_REGS.VEDU_CROP_START.bits.vcpi_crop_ystart       = regcfginfo->VEDU_VCPI_REGS.VEDU_CROP_START.bits.vcpi_crop_ystart ;
    pAllReg->VEDU_VCPI_REGS.VEDU_CROP_START.bits.vcpi_crop_xstart       = regcfginfo->VEDU_VCPI_REGS.VEDU_CROP_START.bits.vcpi_crop_xstart;
    pAllReg->VEDU_VCPI_REGS.VEDU_CROP_END.bits.vcpi_crop_yend           = regcfginfo->VEDU_VCPI_REGS.VEDU_CROP_END.bits.vcpi_crop_yend;
    pAllReg->VEDU_VCPI_REGS.VEDU_CROP_END.bits.vcpi_crop_xend           = regcfginfo->VEDU_VCPI_REGS.VEDU_CROP_END.bits.vcpi_crop_xend;
    pAllReg->VEDU_VCPI_REGS.VEDU_SCALE_PARA.bits.vcpi_yscale            = regcfginfo->VEDU_VCPI_REGS.VEDU_SCALE_PARA.bits.vcpi_yscale ;
    pAllReg->VEDU_VCPI_REGS.VEDU_SCALE_PARA.bits.vcpi_xscale            = regcfginfo->VEDU_VCPI_REGS.VEDU_SCALE_PARA.bits.vcpi_xscale;
    pAllReg->VEDU_VCPI_REGS.VEDU_ORI_PICSIZE.bits.vcpi_oriheight_pix    = regcfginfo->VEDU_VCPI_REGS.VEDU_ORI_PICSIZE.bits.vcpi_oriheight_pix;
    pAllReg->VEDU_VCPI_REGS.VEDU_ORI_PICSIZE.bits.vcpi_oriwidth_pix     = regcfginfo->VEDU_VCPI_REGS.VEDU_ORI_PICSIZE.bits.vcpi_oriwidth_pix;

    pAllReg->VEDU_VCPI_REGS.VEDU_INTRA_INTER_CU_EN.bits.vcpi_mrg_cu_en  = regcfginfo->VEDU_VCPI_REGS.VEDU_INTRA_INTER_CU_EN.bits.vcpi_mrg_cu_en ;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTRA_INTER_CU_EN.bits.vcpi_fme_cu_en  = regcfginfo->VEDU_VCPI_REGS.VEDU_INTRA_INTER_CU_EN.bits.vcpi_fme_cu_en;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTRA_INTER_CU_EN.bits.vcpi_ipcm_en    = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_INTRA_INTER_CU_EN.bits.vcpi_intra_cu_en = regcfginfo->VEDU_VCPI_REGS.VEDU_INTRA_INTER_CU_EN.bits.vcpi_intra_cu_en;
    pAllReg->VEDU_VCPI_REGS.VEDU_VLC_CONFIG.bits.vcpi_byte_stuffing      = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_VLC_CONFIG.bits.vcpi_num_refidx         = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_VLC_CONFIG.bits.vcpi_cabac_init_idc     = 0;
    pAllReg->VEDU_VCPI_REGS.VEDU_VLC_CONFIG.bits.vcpi_ref_idc            = 3;//pEncPara->vcpi_ref_idc;
    pAllReg->VEDU_VCPI_REGS.VEDU_PRE_JUDGE_EXT_EN.bits.vcpi_ext_edge_en  = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_PRE_JUDGE_EXT_EN.bits.vcpi_force_inter  = 0;

    // need to know protocol,if we  use const value, added by hongtaoxu
    pAllReg->VEDU_VCPI_REGS.VEDU_PRE_JUDGE_EXT_EN.bits.vcpi_pblk_pre_en  = regcfginfo->VEDU_VCPI_REGS.VEDU_PRE_JUDGE_EXT_EN.bits.vcpi_pblk_pre_en;
    pAllReg->VEDU_VCPI_REGS.VEDU_PRE_JUDGE_EXT_EN.bits.vcpi_iblk_pre_en  = regcfginfo->VEDU_VCPI_REGS.VEDU_PRE_JUDGE_EXT_EN.bits.vcpi_iblk_pre_en;

    pAllReg->VEDU_VCPI_REGS.VEDU_PRE_JUDGE_COST_THR.bits.vcpi_pblk_pre_cost_thr = regcfginfo->VEDU_VCPI_REGS.VEDU_PRE_JUDGE_COST_THR.bits.vcpi_pblk_pre_cost_thr;
    pAllReg->VEDU_VCPI_REGS.VEDU_PRE_JUDGE_COST_THR.bits.vcpi_iblk_pre_cost_thr = regcfginfo->VEDU_VCPI_REGS.VEDU_PRE_JUDGE_COST_THR.bits.vcpi_iblk_pre_cost_thr ;

    pAllReg->VEDU_VCPI_REGS.VEDU_IBLK_PRE_MV_THR.bits.vcpi_iblk_pre_mvy_thr     = regcfginfo->VEDU_VCPI_REGS.VEDU_IBLK_PRE_MV_THR.bits.vcpi_iblk_pre_mvy_thr;
    pAllReg->VEDU_VCPI_REGS.VEDU_IBLK_PRE_MV_THR.bits.vcpi_iblk_pre_mvx_thr     = regcfginfo->VEDU_VCPI_REGS.VEDU_IBLK_PRE_MV_THR.bits.vcpi_iblk_pre_mvx_thr;
    pAllReg->VEDU_VCPI_REGS.VEDU_IBLK_PRE_MV_THR.bits.vcpi_iblk_pre_mv_dif_thr1 = regcfginfo->VEDU_VCPI_REGS.VEDU_IBLK_PRE_MV_THR.bits.vcpi_iblk_pre_mv_dif_thr1;
    pAllReg->VEDU_VCPI_REGS.VEDU_IBLK_PRE_MV_THR.bits.vcpi_iblk_pre_mv_dif_thr0 = regcfginfo->VEDU_VCPI_REGS.VEDU_IBLK_PRE_MV_THR.bits.vcpi_iblk_pre_mv_dif_thr0;

    pAllReg->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE0.bits.vcpi_win0_height             = regcfginfo->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE0.bits.vcpi_win0_height;
    pAllReg->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE0.bits.vcpi_win0_width              = regcfginfo->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE0.bits.vcpi_win0_width;
    pAllReg->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE1.bits.vcpi_win1_height             = regcfginfo->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE1.bits.vcpi_win1_height;
    pAllReg->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE1.bits.vcpi_win1_width              = regcfginfo->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE1.bits.vcpi_win1_width ;
    pAllReg->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE2.bits.vcpi_win2_height             = regcfginfo->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE2.bits.vcpi_win2_height;
    pAllReg->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE2.bits.vcpi_win2_width              = regcfginfo->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE2.bits.vcpi_win2_width;
    pAllReg->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE3.bits.vcpi_win3_height             = regcfginfo->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE3.bits.vcpi_win3_height ;
    pAllReg->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE3.bits.vcpi_win3_width              = regcfginfo->VEDU_VCPI_REGS.VEDU_WINDOW_SIZE3.bits.vcpi_win3_width ;

    pAllReg->VEDU_VCPI_REGS.VEDU_PIC_STRONG_EN.bits.vcpi_static_en   = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_PIC_STRONG_EN.bits.vcpi_highedge_en = 1;
    pAllReg->VEDU_VCPI_REGS.VEDU_PIC_STRONG_EN.bits.vcpi_skin_en     = 1;

    pAllReg->VEDU_VCPI_REGS.VEDU_DOWN_FREQ_EN.bits.down_freq_en = 1;

    pAllReg->VEDU_VCPI_REGS.VEDU_PBLK_PRE_MV_THR.bits.vcpi_pblk_pre_mvy_thr = regcfginfo->VEDU_VCPI_REGS.VEDU_PBLK_PRE_MV_THR.bits.vcpi_pblk_pre_mvy_thr;//l00214825 0716
    pAllReg->VEDU_VCPI_REGS.VEDU_PBLK_PRE_MV_THR.bits.vcpi_pblk_pre_mvx_thr = regcfginfo->VEDU_VCPI_REGS.VEDU_PBLK_PRE_MV_THR.bits.vcpi_pblk_pre_mvx_thr;//l00214825 0716

    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.vcpi_roi_en     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.vcpi_roi_en;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region7keep     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region7keep;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region6keep     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region6keep;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region5keep     =  regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region5keep;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region4keep     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region4keep;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region3keep     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region3keep;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region2keep     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region2keep;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region1keep     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region1keep;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region0keep     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region0keep;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp7          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp7 ;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp6          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp6;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp5          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp5;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp4          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp4;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp3          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp3;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp2          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp2;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp1          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp1;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp0          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.absqp0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region7en       = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region7en;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region6en       = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region6en;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region5en       = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region5en;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region4en       = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region4en;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region3en       = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region3en;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region2en       = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region2en;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region1en       = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region1en;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region0en       = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG0.bits.region0en;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG1.bits.roiqp3          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG1.bits.roiqp3;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG1.bits.roiqp2          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG1.bits.roiqp2;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG1.bits.roiqp1          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG1.bits.roiqp1;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG1.bits.roiqp0          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG1.bits.roiqp0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG2.bits.roiqp7          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG2.bits.roiqp7;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG2.bits.roiqp6          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG2.bits.roiqp6;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG2.bits.roiqp5          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG2.bits.roiqp5;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_CFG2.bits.roiqp4          = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_CFG2.bits.roiqp4;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_0.bits.size0_roiheight       = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_0.bits.size0_roiheight;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_0.bits.size0_roiwidth        = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_0.bits.size0_roiwidth;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_0.bits.start0_roistarty     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_0.bits.start0_roistarty;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_0.bits.start0_roistartx     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_0.bits.start0_roistartx;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_1.bits.size1_roiheight       = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_1.bits.size1_roiheight;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_1.bits.size1_roiwidth        = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_1.bits.size1_roiwidth;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_1.bits.start1_roistarty     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_1.bits.start1_roistarty;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_1.bits.start1_roistartx     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_1.bits.start1_roistartx;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_2.bits.size2_roiheight       =  regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_2.bits.size2_roiheight;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_2.bits.size2_roiwidth        = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_2.bits.size2_roiwidth;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_2.bits.start2_roistarty     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_2.bits.start2_roistarty;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_2.bits.start2_roistartx     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_2.bits.start2_roistartx ;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_3.bits.size3_roiheight       = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_3.bits.size3_roiheight;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_3.bits.size3_roiwidth        = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_3.bits.size3_roiwidth ;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_3.bits.start3_roistarty     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_3.bits.start3_roistarty;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_3.bits.start3_roistartx     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_3.bits.start3_roistartx;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_4.bits.size4_roiheight       = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_4.bits.size4_roiheight;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_4.bits.size4_roiwidth        = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_4.bits.size4_roiwidth;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_4.bits.start4_roistarty     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_4.bits.start4_roistarty;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_4.bits.start4_roistartx     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_4.bits.start4_roistartx;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_5.bits.size5_roiheight       = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_5.bits.size5_roiheight;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_5.bits.size5_roiwidth        = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_5.bits.size5_roiwidth;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_5.bits.start5_roistarty     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_5.bits.start5_roistarty;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_5.bits.start5_roistartx     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_5.bits.start5_roistartx;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_6.bits.size6_roiheight       = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_6.bits.size6_roiheight ;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_6.bits.size6_roiwidth        = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_6.bits.size6_roiwidth;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_6.bits.start6_roistarty     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_6.bits.start6_roistarty;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_6.bits.start6_roistartx     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_6.bits.start6_roistartx;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_7.bits.size7_roiheight       = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_7.bits.size7_roiheight;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_7.bits.size7_roiwidth        = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_SIZE_7.bits.size7_roiwidth;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_7.bits.start7_roistarty     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_7.bits.start7_roistarty;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_ROI_START_7.bits.start7_roistartx     = regcfginfo->VEDU_VCTRL_REGS.VCTRL_ROI_START_7.bits.start7_roistartx;

    pAllReg->VEDU_VCTRL_REGS.VCTRL_LCU_TARGET_BIT.bits.lcu_target_bit          = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NARROW_THRESHOLD.bits.narrow_tile_width     = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_LCU_BASELINE.bits.lcu_performance_baseline  = 0;
    // hongtaoxuOK

    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORM_TR32X32_COEFF_DENOISE.bits.norm32_tr1_denois_max_num  = 4;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORM_TR32X32_COEFF_DENOISE.bits.norm32_coeff_protect_num   = 255;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORM_TR16X16_COEFF_DENOISE.bits.norm16_tr1_denois_max_num  = 3;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORM_TR16X16_COEFF_DENOISE.bits.norm16_coeff_protect_num   = 64;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_TR32X32_COEFF_DENOISE.bits.skin32_tr1_denois_max_num  = 8;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_TR32X32_COEFF_DENOISE.bits.skin32_coeff_protect_num   = 32;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_TR16X16_COEFF_DENOISE.bits.skin16_tr1_denois_max_num  = 6;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_TR16X16_COEFF_DENOISE.bits.skin16_coeff_protect_num   = 16;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_STATIC_TR32X32_COEFF_DENOISE.bits.static32_tr1_denois_max_num = 8;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_STATIC_TR32X32_COEFF_DENOISE.bits.static32_coeff_protect_num  = 16;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_STATIC_TR16X16_COEFF_DENOISE.bits.static16_tr1_denois_max_num = 6;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_STATIC_TR16X16_COEFF_DENOISE.bits.static16_coeff_protect_num = 8;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HEDGE_TR32X32_COEFF_DENOISE.bits.hedge32_tr1_denois_max_num  = 8;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HEDGE_TR32X32_COEFF_DENOISE.bits.hedge32_coeff_protect_num   = 32;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HEDGE_TR16X16_COEFF_DENOISE.bits.hedge16_tr1_denois_max_num  = 6;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HEDGE_TR16X16_COEFF_DENOISE.bits.hedge16_coeff_protect_num   = 16;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORMAL_INTRA_RDO_FACTOR.bits.norm_intra_cu32_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORMAL_INTRA_RDO_FACTOR.bits.norm_intra_cu16_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORMAL_INTRA_RDO_FACTOR.bits.norm_intra_cu8_rdcost_offset  = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORMAL_INTRA_RDO_FACTOR.bits.norm_intra_cu4_rdcost_offset  = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORMAL_MRG_RDO_FACTOR.bits.norm_mrg_cu64_rdcost_offset     = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORMAL_MRG_RDO_FACTOR.bits.norm_mrg_cu32_rdcost_offset     = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORMAL_MRG_RDO_FACTOR.bits.norm_mrg_cu16_rdcost_offset     = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORMAL_MRG_RDO_FACTOR.bits.norm_mrg_cu8_rdcost_offset      = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORMAL_FME_RDO_FACTOR.bits.norm_fme_cu64_rdcost_offset     = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORMAL_FME_RDO_FACTOR.bits.norm_fme_cu32_rdcost_offset     = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORMAL_FME_RDO_FACTOR.bits.norm_fme_cu16_rdcost_offset     = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_NORMAL_FME_RDO_FACTOR.bits.norm_fme_cu8_rdcost_offset      = 0;
	// OK
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_REGION_INTRA_RDO_FACTOR.bits.skin_intra_cu32_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_REGION_INTRA_RDO_FACTOR.bits.skin_intra_cu16_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_REGION_INTRA_RDO_FACTOR.bits.skin_intra_cu8_rdcost_offset  = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_REGION_INTRA_RDO_FACTOR.bits.skin_intra_cu4_rdcost_offset  = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_REGION_MRG_RDO_FACTOR.bits.skin_mrg_cu64_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_REGION_MRG_RDO_FACTOR.bits.skin_mrg_cu32_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_REGION_MRG_RDO_FACTOR.bits.skin_mrg_cu16_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_REGION_MRG_RDO_FACTOR.bits.skin_mrg_cu8_rdcost_offset  = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_REGION_FME_RDO_FACTOR.bits.skin_fme_cu64_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_REGION_FME_RDO_FACTOR.bits.skin_fme_cu32_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_REGION_FME_RDO_FACTOR.bits.skin_fme_cu16_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_SKIN_REGION_FME_RDO_FACTOR.bits.skin_fme_cu8_rdcost_offset  = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HIGH_EDGE_INTRA_RDO_FACTOR.bits.hedge_intra_cu32_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HIGH_EDGE_INTRA_RDO_FACTOR.bits.hedge_intra_cu16_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HIGH_EDGE_INTRA_RDO_FACTOR.bits.hedge_intra_cu8_rdcost_offset  = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HIGH_EDGE_INTRA_RDO_FACTOR.bits.hedge_intra_cu4_rdcost_offset  = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HIGH_EDGE_MRG_RDO_FACTOR.bits.hedge_mrg_cu64_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HIGH_EDGE_MRG_RDO_FACTOR.bits.hedge_mrg_cu32_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HIGH_EDGE_MRG_RDO_FACTOR.bits.hedge_mrg_cu16_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HIGH_EDGE_MRG_RDO_FACTOR.bits.hedge_mrg_cu8_rdcost_offset  = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HIGH_EDGE_FME_RDO_FACTOR.bits.hedge_fme_cu64_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HIGH_EDGE_FME_RDO_FACTOR.bits.hedge_fme_cu32_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HIGH_EDGE_FME_RDO_FACTOR.bits.hedge_fme_cu16_rdcost_offset = 0;
    pAllReg->VEDU_VCTRL_REGS.VCTRL_HIGH_EDGE_FME_RDO_FACTOR.bits.hedge_fme_cu8_rdcost_offset  = 0;

    pAllReg->VEDU_PME_REGS.PME_SW_EN.bits.vcpi_new_cost_en    = 1;
    pAllReg->VEDU_PME_REGS.PME_SW_EN.bits.vcpi_psw_adapt_en   = 1;
    pAllReg->VEDU_PME_REGS.PME_SW_TR0.bits.vcpi_psw_thr0      = 20;
    pAllReg->VEDU_PME_REGS.PME_SW_TR1.bits.vcpi_psw_thr1      = 50;// if H265 Lowpwr1, this value should be 1200
    pAllReg->VEDU_PME_REGS.PME_SW_TR2.bits.vcpi_psw_thr2      = 150;// if H265 Lowpwr1, this value should be 1500 
    pAllReg->VEDU_PME_REGS.PME_TR_WEIGHTX.bits.tr_weightx_2   = 12;
    pAllReg->VEDU_PME_REGS.PME_TR_WEIGHTX.bits.tr_weightx_1   = 8;
    pAllReg->VEDU_PME_REGS.PME_TR_WEIGHTX.bits.tr_weightx_0   = 4;
    pAllReg->VEDU_PME_REGS.PME_TR_WEIGHTY.bits.tr_weighty_2   = 12;
    pAllReg->VEDU_PME_REGS.PME_TR_WEIGHTY.bits.tr_weighty_1   = 8;
    pAllReg->VEDU_PME_REGS.PME_TR_WEIGHTY.bits.tr_weighty_0   = 4;
    pAllReg->VEDU_PME_REGS.PME_SR_WEIGHT.bits.sr_weight_2     = 2;// if H265 Lowpwr1, this value should be 4
    pAllReg->VEDU_PME_REGS.PME_SR_WEIGHT.bits.sr_weight_1     = 4;// if H265 Lowpwr1, this value should be 6
    pAllReg->VEDU_PME_REGS.PME_SR_WEIGHT.bits.sr_weight_0     = 6;// if H265 Lowpwr1, this value should be 8
    pAllReg->VEDU_PME_REGS.pme_skin_thr.bits.skin_v_min_thr   = 135;
    pAllReg->VEDU_PME_REGS.pme_skin_thr.bits.skin_v_max_thr   = 160;
    pAllReg->VEDU_PME_REGS.pme_skin_thr.bits.skin_u_min_thr   = 100;
    pAllReg->VEDU_PME_REGS.pme_skin_thr.bits.skin_u_max_thr   = 127;
    pAllReg->VEDU_PME_REGS.pme_high_edge.bits.still_scene_thr = 0;
    pAllReg->VEDU_PME_REGS.pme_high_edge.bits.high_edge_cnt   = 6;
    pAllReg->VEDU_PME_REGS.pme_high_edge.bits.high_edge_thr   = 40;
    pAllReg->VEDU_PME_REGS.pme_high_edge.bits.skin_num        = (vcpi_protocol == VEDU_H265 ? 200 : 64);

    pAllReg->VEDU_REFID_REGS.REFLD_CMD_CFG.bits.refld_vcpi2cont_title_en = 1;
    pAllReg->VEDU_REFID_REGS.REFLD_CMD_CFG.bits.ddr_cross_idx            = 148;

    pAllReg->VEDU_IME_REGS.INTER_MODE.bits.vcpi_high3pre_en = 0;
    pAllReg->VEDU_IME_REGS.INTER_MODE.bits.vcpi_rect3_mod   = 1;
    pAllReg->VEDU_IME_REGS.INTER_MODE.bits.vcpi_rect2_mod   = 1;
    pAllReg->VEDU_IME_REGS.INTER_MODE.bits.vcpi_rect1_mod   = 1;
    pAllReg->VEDU_IME_REGS.INTER_MODE.bits.vcpi_inter8x8_en = 1;
    pAllReg->VEDU_IME_REGS.IME_CFG1.bits.vcpi_rect0_vstep   = 0;
    pAllReg->VEDU_IME_REGS.IME_CFG1.bits.vcpi_rect0_hstep   = 0;
    pAllReg->VEDU_IME_REGS.IME_CFG2.bits.vcpi_rect1_vstep   = 1;
    pAllReg->VEDU_IME_REGS.IME_CFG2.bits.vcpi_rect1_hstep   = 1;
    pAllReg->VEDU_IME_REGS.IME_CFG3.bits.vcpi_rect2_vstep   = 1;
    pAllReg->VEDU_IME_REGS.IME_CFG3.bits.vcpi_rect2_hstep   = 1;
    pAllReg->VEDU_IME_REGS.IME_CFG4.bits.vcpi_rect3_vstep   = 0;
    pAllReg->VEDU_IME_REGS.IME_CFG4.bits.vcpi_rect3_hstep   = 0;
    pAllReg->VEDU_IME_REGS.IME_THR0.bits.vcpi_start_thr = 1500;
    pAllReg->VEDU_IME_REGS.IME_THR1.bits.vcpi_intra_thr = 4096;
    pAllReg->VEDU_IME_REGS.IME_RDOCFG.bits.vcpi_lambdaoff16 = 0;
    pAllReg->VEDU_IME_REGS.IME_RDOCFG.bits.vcpi_lambdaoff8  = 0;
    pAllReg->VEDU_IME_REGS.IME_FME_LPOW_THR.bits.vcpi_lowpow_fme_thr1     = regcfginfo->VEDU_IME_REGS.IME_FME_LPOW_THR.bits.vcpi_lowpow_fme_thr1;
    pAllReg->VEDU_IME_REGS.IME_FME_LPOW_THR.bits.vcpi_lowpow_fme_thr0     = regcfginfo->VEDU_IME_REGS.IME_FME_LPOW_THR.bits.vcpi_lowpow_fme_thr0;

    pAllReg->VEDU_QPG_REGS.QPG_MAX_MIN_QP.bits.max_qp                     = regcfginfo->VEDU_QPG_REGS.QPG_MAX_MIN_QP.bits.max_qp;
    pAllReg->VEDU_QPG_REGS.QPG_MAX_MIN_QP.bits.min_qp                     = regcfginfo->VEDU_QPG_REGS.QPG_MAX_MIN_QP.bits.min_qp;
    pAllReg->VEDU_QPG_REGS.QPG_AVERAGE_LCU_BITS.bits.ave_lcu_bits         = regcfginfo->VEDU_QPG_REGS.QPG_AVERAGE_LCU_BITS.bits.ave_lcu_bits;
    pAllReg->VEDU_QPG_REGS.QPG_QP_DELTA.bits.qp_delta                     = regcfginfo->VEDU_QPG_REGS.QPG_QP_DELTA.bits.qp_delta;
    pAllReg->VEDU_QPG_REGS.QPG_CU_QP_DELTA_THRESH_REG0.bits.cu_qp_delta_thresh3  = 18;
    pAllReg->VEDU_QPG_REGS.QPG_CU_QP_DELTA_THRESH_REG0.bits.cu_qp_delta_thresh2  = 15;
    pAllReg->VEDU_QPG_REGS.QPG_CU_QP_DELTA_THRESH_REG0.bits.cu_qp_delta_thresh1  = 4;
    pAllReg->VEDU_QPG_REGS.QPG_CU_QP_DELTA_THRESH_REG0.bits.cu_qp_delta_thresh0  = 1;
    pAllReg->VEDU_QPG_REGS.QPG_CU_QP_DELTA_THRESH_REG1.bits.cu_qp_delta_thresh7  = 44;
    pAllReg->VEDU_QPG_REGS.QPG_CU_QP_DELTA_THRESH_REG1.bits.cu_qp_delta_thresh6  = 34;
    pAllReg->VEDU_QPG_REGS.QPG_CU_QP_DELTA_THRESH_REG1.bits.cu_qp_delta_thresh5  = 27;
    pAllReg->VEDU_QPG_REGS.QPG_CU_QP_DELTA_THRESH_REG1.bits.cu_qp_delta_thresh4  = 22;
    pAllReg->VEDU_QPG_REGS.QPG_CU_QP_DELTA_THRESH_REG2.bits.cu_qp_delta_thresh11 = 255;
    pAllReg->VEDU_QPG_REGS.QPG_CU_QP_DELTA_THRESH_REG2.bits.cu_qp_delta_thresh10 = 255;
    pAllReg->VEDU_QPG_REGS.QPG_CU_QP_DELTA_THRESH_REG2.bits.cu_qp_delta_thresh9  = 255;
    pAllReg->VEDU_QPG_REGS.QPG_CU_QP_DELTA_THRESH_REG2.bits.cu_qp_delta_thresh8  = 255;
    pAllReg->VEDU_QPG_REGS.QPG_STRONG_EDGE_QP_DELTA.bits.strong_edge_qp_delta    = 2;
    pAllReg->VEDU_QPG_REGS.QPG_SKIN_QP_DELTA.bits.skin_qp_delta                  = 3;

    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG00.bits.lambda00 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG00.bits.lambda00;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG01.bits.lambda01 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG01.bits.lambda01;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG02.bits.lambda02 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG02.bits.lambda02;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG03.bits.lambda03 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG03.bits.lambda03;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG04.bits.lambda04 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG04.bits.lambda04;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG05.bits.lambda05 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG05.bits.lambda05;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG06.bits.lambda06 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG06.bits.lambda06;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG07.bits.lambda07 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG07.bits.lambda07;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG08.bits.lambda08 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG08.bits.lambda08;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG09.bits.lambda09 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG09.bits.lambda09;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG10.bits.lambda10 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG10.bits.lambda10;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG11.bits.lambda11 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG11.bits.lambda11;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG12.bits.lambda12 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG12.bits.lambda12;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG13.bits.lambda13 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG13.bits.lambda13;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG14.bits.lambda14 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG14.bits.lambda14;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG15.bits.lambda15 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG15.bits.lambda15;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG16.bits.lambda16 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG16.bits.lambda16;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG17.bits.lambda17 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG17.bits.lambda17;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG18.bits.lambda18 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG18.bits.lambda18;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG19.bits.lambda19 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG19.bits.lambda19;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG20.bits.lambda20 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG20.bits.lambda20;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG21.bits.lambda21 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG21.bits.lambda21;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG22.bits.lambda22 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG22.bits.lambda22;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG23.bits.lambda23 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG23.bits.lambda23;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG24.bits.lambda24 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG24.bits.lambda24;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG25.bits.lambda25 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG25.bits.lambda25;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG26.bits.lambda26 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG26.bits.lambda26;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG27.bits.lambda27 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG27.bits.lambda27;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG28.bits.lambda28 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG28.bits.lambda28;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG29.bits.lambda29 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG29.bits.lambda29;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG30.bits.lambda30 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG30.bits.lambda30;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG31.bits.lambda31 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG31.bits.lambda31;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG32.bits.lambda32 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG32.bits.lambda32;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG33.bits.lambda33 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG33.bits.lambda33;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG34.bits.lambda34 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG34.bits.lambda34;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG35.bits.lambda35 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG35.bits.lambda35;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG36.bits.lambda36 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG36.bits.lambda36;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG37.bits.lambda37 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG37.bits.lambda37;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG38.bits.lambda38 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG38.bits.lambda38;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG39.bits.lambda39 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG39.bits.lambda39;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG40.bits.lambda40 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG40.bits.lambda40;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG41.bits.lambda41 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG41.bits.lambda41;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG42.bits.lambda42 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG42.bits.lambda42;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG43.bits.lambda43 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG43.bits.lambda43;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG44.bits.lambda44 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG44.bits.lambda44;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG45.bits.lambda45 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG45.bits.lambda45;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG46.bits.lambda46 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG46.bits.lambda46;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG47.bits.lambda47 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG47.bits.lambda47;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG48.bits.lambda48 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG48.bits.lambda48;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG49.bits.lambda49 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG49.bits.lambda49;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG50.bits.lambda50 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG50.bits.lambda50;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG51.bits.lambda51 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG51.bits.lambda51;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG52.bits.lambda52 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG52.bits.lambda52;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG53.bits.lambda53 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG53.bits.lambda53;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG54.bits.lambda54 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG54.bits.lambda54;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG55.bits.lambda55 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG55.bits.lambda55;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG56.bits.lambda56 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG56.bits.lambda56;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG57.bits.lambda57 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG57.bits.lambda57;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG58.bits.lambda58 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG58.bits.lambda58;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG59.bits.lambda59 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG59.bits.lambda59;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG60.bits.lambda60 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG60.bits.lambda60;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG61.bits.lambda61 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG61.bits.lambda61;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG62.bits.lambda62 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG62.bits.lambda62;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG63.bits.lambda63 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG63.bits.lambda63;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG64.bits.lambda64 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG64.bits.lambda64;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG65.bits.lambda65 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG65.bits.lambda65;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG66.bits.lambda66 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG66.bits.lambda66;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG67.bits.lambda67 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG67.bits.lambda67;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG68.bits.lambda68 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG68.bits.lambda68;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG69.bits.lambda69 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG69.bits.lambda69;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG70.bits.lambda70 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG70.bits.lambda70;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG71.bits.lambda71 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG71.bits.lambda71;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG72.bits.lambda72 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG72.bits.lambda72;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG73.bits.lambda73 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG73.bits.lambda73;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG74.bits.lambda74 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG74.bits.lambda74;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG75.bits.lambda75 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG75.bits.lambda75;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG76.bits.lambda76 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG76.bits.lambda76;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG77.bits.lambda77 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG77.bits.lambda77;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG78.bits.lambda78 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG78.bits.lambda78;
    pAllReg->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG79.bits.lambda79 = regcfginfo->VEDU_QPG_REGS.QPG_QP_LAMBDA_CTRL_REG79.bits.lambda79;

    pAllReg->VEDU_INTRA_REGS.INTER_AVAIL.bits.inter_avail           = 0;
    pAllReg->VEDU_INTRA_REGS.INTRA_SMOOTH.bits.intra_smooth         = 1;
    pAllReg->VEDU_INTRA_REGS.INTRA_BIT_WEIGHT.bits.intra_bit_weight = 13;
    pAllReg->VEDU_INTRA_REGS.INTRA_CHNL4_ANG_1EN.u32  = 0xFFFFFFFF;
    pAllReg->VEDU_INTRA_REGS.INTRA_CHNL8_ANG_1EN.u32  = 0xFFFFFFFF;
    pAllReg->VEDU_INTRA_REGS.INTRA_CHNL16_ANG_1EN.u32 = 0xFFFFFFFF;
    pAllReg->VEDU_INTRA_REGS.INTRA_CHNL32_ANG_1EN.u32 = 0xFFFFFFFF;

    pAllReg->VEDU_PMV_REGS.PMV_TMV_EN.bits.tmv_en =  1;

    pAllReg->VEDU_TQITQ_REGS.TQITQ_QSCALINGLIST.bits.q_scaling_waddr = 0;
    pAllReg->VEDU_TQITQ_REGS.TQITQ_QSCALINGLIST.bits.q_scaling_wdata = 0;

    pAllReg->VEDU_PACK_REGS.PACK_CU_PARAMETER.bits.vcpi2cu_qp_min_cu_size         = 2;
    pAllReg->VEDU_PACK_REGS.PACK_CU_PARAMETER.bits.vcpi2cu_tq_bypass_enabled_flag = 0;
    pAllReg->VEDU_PACK_REGS.PACK_PCM_PARAMETER.bits.vcpi2pu_log2_max_ipcm_cbsizey = 3;
    pAllReg->VEDU_PACK_REGS.PACK_PCM_PARAMETER.bits.vcpi2pu_log2_min_ipcm_cbsizey = 3;
    pAllReg->VEDU_PACK_REGS.PACK_TF_SKIP_FLAG.bits.vcpi2res_tf_skip_enabled_flag  = 0;

    pAllReg->VEDU_CABAC_REGS.CABAC_GLB_CFG.bits.nal_unit_head     = 0;// can be deleted hongtaoxu
    pAllReg->VEDU_CABAC_REGS.CABAC_GLB_CFG.bits.max_num_mergecand = 0;// can be deleted hongtaoxu
    pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG1 = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG1;
    pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG2 = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG2;
    pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG3 = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG3;
    pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG4 = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG4;
    pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG5 = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG5;
    pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG6 = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG6;
    pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG7 = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG7;
    pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG8 = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG8 ;

    pAllReg->VEDU_RECST_REGS.RECST_DDR_CROSS.bits.ddr_cross_idx = 148;     //0725

    pAllReg->VEDU_VLCST_REGS.VLCST_PTBITS_EN.bits.ptbits_en     = 0;
    pAllReg->VEDU_VLCST_REGS.VLCST_PTBITS                       = 0;

    pAllReg->VEDU_VLC_REGS.VLC_SlcHdrStrm0                      = regcfginfo->VEDU_VLC_REGS.VLC_SlcHdrStrm0;
    pAllReg->VEDU_VLC_REGS.VLC_SlcHdrStrm1                      = regcfginfo->VEDU_VLC_REGS.VLC_SlcHdrStrm1;
    pAllReg->VEDU_VLC_REGS.VLC_SlcHdrStrm2                      = regcfginfo->VEDU_VLC_REGS.VLC_SlcHdrStrm2;
    pAllReg->VEDU_VLC_REGS.VLC_SlcHdrStrm3                      = regcfginfo->VEDU_VLC_REGS.VLC_SlcHdrStrm3;
    pAllReg->VEDU_VLC_REGS.VLC_ReorderStrm0                     = regcfginfo->VEDU_VLC_REGS.VLC_ReorderStrm0;
    pAllReg->VEDU_VLC_REGS.VLC_ReorderStrm1                     = regcfginfo->VEDU_VLC_REGS.VLC_ReorderStrm1;
    pAllReg->VEDU_VLC_REGS.VLC_MarkingStrm0                     = regcfginfo->VEDU_VLC_REGS.VLC_MarkingStrm0;
    pAllReg->VEDU_VLC_REGS.VLC_MarkingStrm1                     = regcfginfo->VEDU_VLC_REGS.VLC_MarkingStrm1;
    pAllReg->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_parabit     = regcfginfo->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_parabit ;
    pAllReg->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_reorderbit  = regcfginfo->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_reorderbit;
    pAllReg->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_markingbit  = regcfginfo->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_markingbit;
    pAllReg->VEDU_VLC_REGS.VLC_SVC.bits.vcpi_svc_strm           = regcfginfo->VEDU_VLC_REGS.VLC_SVC.bits.vcpi_svc_strm;
    pAllReg->VEDU_VLC_REGS.VLC_SVC.bits.vcpi_svc_en             = regcfginfo->VEDU_VLC_REGS.VLC_SVC.bits.vcpi_svc_en;

    pAllReg->VEDU_CURPRE_REGS.VEDU_FILTER_PARAMETER0.bits.curld_filter_hshift   = 2;
    pAllReg->VEDU_CURPRE_REGS.VEDU_FILTER_PARAMETER0.bits.curld_filter_hrnd     = 2;
    pAllReg->VEDU_CURPRE_REGS.VEDU_FILTER_PARAMETER0.bits.curld_filter_h3       = 1;
    pAllReg->VEDU_CURPRE_REGS.VEDU_FILTER_PARAMETER0.bits.curld_filter_h2       = 1;
    pAllReg->VEDU_CURPRE_REGS.VEDU_FILTER_PARAMETER0.bits.curld_filter_h1       = 1;
    pAllReg->VEDU_CURPRE_REGS.VEDU_FILTER_PARAMETER0.bits.curld_filter_h0       = 1;
    pAllReg->VEDU_CURPRE_REGS.VEDU_FILTER_PARAMETER1.bits.curld_filter_vshift   = 2;
    pAllReg->VEDU_CURPRE_REGS.VEDU_FILTER_PARAMETER1.bits.curld_filter_vrnd     = 2;
    pAllReg->VEDU_CURPRE_REGS.VEDU_FILTER_PARAMETER1.bits.curld_filter_v3       = 1;
    pAllReg->VEDU_CURPRE_REGS.VEDU_FILTER_PARAMETER1.bits.curld_filter_v2       = 1;
    pAllReg->VEDU_CURPRE_REGS.VEDU_FILTER_PARAMETER1.bits.curld_filter_v1       = 1;
    pAllReg->VEDU_CURPRE_REGS.VEDU_FILTER_PARAMETER1.bits.curld_filter_v0       = 1;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER0.bits.curld_y_filt_01coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER0.bits.curld_y_filt_01coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER0.bits.curld_y_filt_00coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER0.bits.curld_y_filt_00coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER1.bits.curld_y_filt_03coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER1.bits.curld_y_filt_03coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER1.bits.curld_y_filt_02coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER1.bits.curld_y_filt_02coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER2.bits.curld_y_filt_11coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER2.bits.curld_y_filt_11coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER2.bits.curld_y_filt_10coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER2.bits.curld_y_filt_10coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER3.bits.curld_y_filt_13coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER3.bits.curld_y_filt_13coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER3.bits.curld_y_filt_12coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER3.bits.curld_y_filt_12coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER4.bits.curld_y_filt_21coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER4.bits.curld_y_filt_21coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER4.bits.curld_y_filt_20coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER4.bits.curld_y_filt_20coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER5.bits.curld_y_filt_23coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER5.bits.curld_y_filt_23coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER5.bits.curld_y_filt_22coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER5.bits.curld_y_filt_22coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER6.bits.curld_y_filt_31coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER6.bits.curld_y_filt_31coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER6.bits.curld_y_filt_30coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER6.bits.curld_y_filt_30coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER7.bits.curld_y_filt_33coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER7.bits.curld_y_filt_33coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER7.bits.curld_y_filt_32coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER7.bits.curld_y_filt_32coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER8.bits.curld_c_filt_01coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER8.bits.curld_c_filt_01coef ;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER8.bits.curld_c_filt_00coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER8.bits.curld_c_filt_00coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER9.bits.curld_c_filt_03coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER9.bits.curld_c_filt_03coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER9.bits.curld_c_filt_02coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER9.bits.curld_c_filt_02coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER10.bits.curld_c_filt_11coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER10.bits.curld_c_filt_11coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER10.bits.curld_c_filt_10coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER10.bits.curld_c_filt_10coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER11.bits.curld_c_filt_13coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER11.bits.curld_c_filt_13coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER11.bits.curld_c_filt_12coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER11.bits.curld_c_filt_12coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER12.bits.curld_c_filt_21coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER12.bits.curld_c_filt_21coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER12.bits.curld_c_filt_20coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER12.bits.curld_c_filt_20coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER13.bits.curld_c_filt_23coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER13.bits.curld_c_filt_23coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER13.bits.curld_c_filt_22coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER13.bits.curld_c_filt_22coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER14.bits.curld_c_filt_31coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER14.bits.curld_c_filt_31coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER14.bits.curld_c_filt_30coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER14.bits.curld_c_filt_30coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER15.bits.curld_c_filt_33coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER15.bits.curld_c_filt_33coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER15.bits.curld_c_filt_32coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_PARAMETER15.bits.curld_c_filt_32coef;

    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER0.bits.curld_y_filt_col_01coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER0.bits.curld_y_filt_col_01coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER0.bits.curld_y_filt_col_00coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER0.bits.curld_y_filt_col_00coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER1.bits.curld_y_filt_col_03coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER1.bits.curld_y_filt_col_03coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER1.bits.curld_y_filt_col_02coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER1.bits.curld_y_filt_col_02coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER2.bits.curld_y_filt_col_11coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER2.bits.curld_y_filt_col_11coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER2.bits.curld_y_filt_col_10coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER2.bits.curld_y_filt_col_10coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER3.bits.curld_y_filt_col_13coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER3.bits.curld_y_filt_col_13coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER3.bits.curld_y_filt_col_12coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER3.bits.curld_y_filt_col_12coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER4.bits.curld_y_filt_col_21coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER4.bits.curld_y_filt_col_21coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER4.bits.curld_y_filt_col_20coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER4.bits.curld_y_filt_col_20coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER5.bits.curld_y_filt_col_23coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER5.bits.curld_y_filt_col_23coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER5.bits.curld_y_filt_col_22coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER5.bits.curld_y_filt_col_22coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER6.bits.curld_y_filt_col_31coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER6.bits.curld_y_filt_col_31coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER6.bits.curld_y_filt_col_30coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER6.bits.curld_y_filt_col_30coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER7.bits.curld_y_filt_col_33coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER7.bits.curld_y_filt_col_33coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER7.bits.curld_y_filt_col_32coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER7.bits.curld_y_filt_col_32coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER8.bits.curld_c_filt_col_01coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER8.bits.curld_c_filt_col_01coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER8.bits.curld_c_filt_col_00coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER8.bits.curld_c_filt_col_00coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER9.bits.curld_c_filt_col_03coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER9.bits.curld_c_filt_col_03coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER9.bits.curld_c_filt_col_02coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER9.bits.curld_c_filt_col_02coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER10.bits.curld_c_filt_col_11coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER10.bits.curld_c_filt_col_11coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER10.bits.curld_c_filt_col_10coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER10.bits.curld_c_filt_col_10coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER11.bits.curld_c_filt_col_13coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER11.bits.curld_c_filt_col_13coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER11.bits.curld_c_filt_col_12coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER11.bits.curld_c_filt_col_12coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER12.bits.curld_c_filt_col_21coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER12.bits.curld_c_filt_col_21coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER12.bits.curld_c_filt_col_20coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER12.bits.curld_c_filt_col_20coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER13.bits.curld_c_filt_col_23coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER13.bits.curld_c_filt_col_23coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER13.bits.curld_c_filt_col_22coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER13.bits.curld_c_filt_col_22coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER14.bits.curld_c_filt_col_31coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER14.bits.curld_c_filt_col_31coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER14.bits.curld_c_filt_col_30coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER14.bits.curld_c_filt_col_30coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER15.bits.curld_c_filt_col_33coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER15.bits.curld_c_filt_col_33coef;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER15.bits.curld_c_filt_col_32coef = regcfginfo->VEDU_CURPRE_REGS.VEDU_CUR_SCALE_COL_PARAMETER15.bits.curld_c_filt_col_32coef;

    pAllReg->VEDU_CURPRE_REGS.VEDU_CUR_GCFG.bits.curld_clip_en = 0;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CLIP_THX.bits.clip_chrm_max = 235;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CLIP_THX.bits.clip_chrm_min = 16;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CLIP_THX.bits.clip_luma_max = 240;
    pAllReg->VEDU_CURPRE_REGS.VEDU_CLIP_THX.bits.clip_luma_min = 16;

    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_COEF0.bits.vcpi_coeff_ry = regcfginfo->VEDU_CURPRE_REGS.VEDU_RGB_COEF0.bits.vcpi_coeff_ry;
    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_COEF0.bits.vcpi_coeff_gy = regcfginfo->VEDU_CURPRE_REGS.VEDU_RGB_COEF0.bits.vcpi_coeff_gy;
    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_COEF1.bits.vcpi_coeff_by = regcfginfo->VEDU_CURPRE_REGS.VEDU_RGB_COEF1.bits.vcpi_coeff_by;
    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_COEF1.bits.vcpi_coeff_rcb = regcfginfo->VEDU_CURPRE_REGS.VEDU_RGB_COEF1.bits.vcpi_coeff_rcb;
    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_COEF2.bits.vcpi_coeff_gcb = regcfginfo->VEDU_CURPRE_REGS.VEDU_RGB_COEF2.bits.vcpi_coeff_gcb;
    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_COEF2.bits.vcpi_coeff_bcb = regcfginfo->VEDU_CURPRE_REGS.VEDU_RGB_COEF2.bits.vcpi_coeff_bcb;
    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_COEF3.bits.vcpi_coeff_rcr = regcfginfo->VEDU_CURPRE_REGS.VEDU_RGB_COEF3.bits.vcpi_coeff_rcr;
    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_COEF3.bits.vcpi_coeff_gcr = regcfginfo->VEDU_CURPRE_REGS.VEDU_RGB_COEF3.bits.vcpi_coeff_gcr;
    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_COEF4.bits.vcpi_coeff_bcr = regcfginfo->VEDU_CURPRE_REGS.VEDU_RGB_COEF4.bits.vcpi_coeff_bcr;

    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_COEF5.bits.vcpi_rgb_shift      = regcfginfo->VEDU_CURPRE_REGS.VEDU_RGB_COEF5.bits.vcpi_rgb_shift ;
    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_COEF5.bits.vcpi_rgb_rndy       = regcfginfo->VEDU_CURPRE_REGS.VEDU_RGB_COEF5.bits.vcpi_rgb_rndy ;
    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_COEF6.bits.vcpi_rgb_rndcb      = regcfginfo->VEDU_CURPRE_REGS.VEDU_RGB_COEF6.bits.vcpi_rgb_rndcb;
    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_COEF6.bits.vcpi_rgb_rndcr      = regcfginfo->VEDU_CURPRE_REGS.VEDU_RGB_COEF6.bits.vcpi_rgb_rndcr;
    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_CLIP.bits.vcpi_rgb_clip_en     = 0;
    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_CLIP.bits.vcpi_rgb_clpmax      = 235;
    pAllReg->VEDU_CURPRE_REGS.VEDU_RGB_CLIP.bits.vcpi_rgb_clpmin      = 16;
    pAllReg->VEDU_CURPRE_REGS.VEDU_WIDE_NARROW_EN.bits.curld_y_wide_narrow_en  = 0 ;

    pAllReg->VEDU_EMAR_REGS.EMAR_WAIT_TIM_OUT.bits.vcpi_wtmax      = 0xff;
    pAllReg->VEDU_EMAR_REGS.EMAR_WAIT_TIM_OUT.bits.vcpi_rtmax      = 0xff;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH0.bits.vcpi_ch00_rrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH0.bits.vcpi_ch01_rrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH0.bits.vcpi_ch02_rrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH0.bits.vcpi_ch03_rrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH0.bits.vcpi_ch04_rrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH0.bits.vcpi_ch05_rrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH1.bits.vcpi_ch06_rrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH1.bits.vcpi_ch07_rrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH1.bits.vcpi_ch08_rrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH1.bits.vcpi_ch09_rrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH1.bits.vcpi_ch10_rrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH1.bits.vcpi_ch11_rrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH2.bits.vcpi_ch12_rrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH3.bits.vcpi_ch00_wrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH3.bits.vcpi_ch01_wrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH3.bits.vcpi_ch02_wrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH3.bits.vcpi_ch03_wrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH3.bits.vcpi_ch04_wrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH3.bits.vcpi_ch05_wrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH4.bits.vcpi_ch06_wrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH4.bits.vcpi_ch07_wrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH4.bits.vcpi_ch08_wrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH4.bits.vcpi_ch09_wrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH4.bits.vcpi_ch10_wrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH4.bits.vcpi_ch11_wrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH5.bits.vcpi_ch12_wrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH5.bits.vcpi_ch13_wrmax  = 0xf;
    pAllReg->VEDU_EMAR_REGS.EMAR_RCH_RPT_TH5.bits.vcpi_ch14_wrmax  = 0xf;

     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SCR.bits.ptw_mid       = 0;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SCR.bits.ptw_pf        = 3;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SCR.bits.smr_rd_shadow = 0;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SCR.bits.wqos          = 0;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SCR.bits.rqos          = 0;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SCR.bits.cache_l2_en   = 1;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SCR.bits.cache_l1_en   = 1;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SCR.bits.int_en        = 1;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SCR.bits.wqos_en       = 1;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SCR.bits.rqos_en       = 1;

    pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SCR.bits.glb_bypass = regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SCR.bits.glb_bypass;

     pAllReg->VEDU_COMN1_REGS.COMN1_SMMU_CB_TTBR0 = (unsigned int)phy_pgd_base;
     pAllReg->VEDU_COMN1_REGS.COMN1_SMMU_CB_TTBR1 = (unsigned int)phy_pgd_base;

     pAllReg->VEDU_COMN1_REGS.COMN1_SMMU_ERR_RDADDR = RdAddr;
     pAllReg->VEDU_COMN1_REGS.COMN1_SMMU_ERR_WRADDR = WrAddr;

      pAllReg->VEDU_MMU0_REGS.MST_GLB_SCR.bits.glb_scr = regcfginfo->VEDU_MMU0_REGS.MST_GLB_SCR.bits.glb_scr;
      pAllReg->VEDU_MMU0_REGS.MST_GLB_BYPASS.bits.glb_bypass = 0;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid0_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid0_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid1_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid1_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid2_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid2_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid3_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid3_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid4_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid4_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid5_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid5_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid6_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid6_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid7_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid7_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid8_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid8_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid9_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid9_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid10_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid10_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid11_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid11_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid12_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid12_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid13_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid13_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid14_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid14_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid15_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid15_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid16_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid16_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid17_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid17_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid18_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid18_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid19_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid19_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid20_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid20_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid21_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid21_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid22_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid22_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid23_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid23_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid24_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid24_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid25_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid25_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid26_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid26_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid27_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid27_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid28_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid28_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid29_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid29_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid30_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid30_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid31_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID0_BYPASS.bits.sid31_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID1_BYPASS.bits.sid32_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID1_BYPASS.bits.sid32_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID1_BYPASS.bits.sid33_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID1_BYPASS.bits.sid33_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID1_BYPASS.bits.sid34_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID1_BYPASS.bits.sid34_bypass;
      pAllReg->VEDU_MMU0_REGS.MST_SID1_BYPASS.bits.sid35_bypass = regcfginfo->VEDU_MMU0_REGS.MST_SID1_BYPASS.bits.sid35_bypass;

    pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_INTMASK_NS.bits.intns_ptw_ns_msk       = 0;
    pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_INTMASK_NS.bits.intns_ptw_invalid_msk  = 0;
    pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_INTMASK_NS.bits.intns_ptw_trans_msk    = 0;
    pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_INTMASK_NS.bits.intns_tlbmiss_msk      = 0;
    pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_INTMASK_NS.bits.intns_ext_msk          = 0;
    pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_INTMASK_NS.bits.intns_permis_msk       = 0;
    //pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR0_NS.bits.smr0_ptw_qos              =  pEncPara->comn0_smr0_ptw_qos;

    pAllReg->VEDU_COMN1_REGS.COMN1_SMMU_CB_TTBCR.bits.cb_ttbcr_des   =  1;// l00214825 0104

    if (regcfginfo->bSecureFlag)
    {
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR0_S.bits.smr0_nscfg   =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR0_S.bits.smr0_nscfg ;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR1_S.bits.smr1_nscfg   =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR1_S.bits.smr1_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR2_S.bits.smr2_nscfg   =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR2_S.bits.smr2_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR3_S.bits.smr3_nscfg   =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR3_S.bits.smr3_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR4_S.bits.smr4_nscfg   =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR4_S.bits.smr4_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR5_S.bits.smr5_nscfg   =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR5_S.bits.smr5_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR6_S.bits.smr6_nscfg   =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR6_S.bits.smr6_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR7_S.bits.smr7_nscfg   =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR7_S.bits.smr7_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR8_S.bits.smr8_nscfg   =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR8_S.bits.smr8_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR9_S.bits.smr9_nscfg   =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR9_S.bits.smr9_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR10_S.bits.smr10_nscfg =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR10_S.bits.smr10_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR11_S.bits.smr11_nscfg =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR11_S.bits.smr11_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR12_S.bits.smr12_nscfg =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR12_S.bits.smr12_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR13_S.bits.smr13_nscfg =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR13_S.bits.smr13_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR14_S.bits.smr14_nscfg =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR14_S.bits.smr14_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR15_S.bits.smr15_nscfg =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR15_S.bits.smr15_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR16_S.bits.smr16_nscfg =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR16_S.bits.smr16_nscfg;
        pAllReg->VEDU_COMN2_REGS.COMN2_SMMU_SMR17_S.bits.smr17_nscfg =  regcfginfo->VEDU_COMN2_REGS.COMN2_SMMU_SMR17_S.bits.smr17_nscfg;

        pAllReg->VEDU_COMN3_REGS.COMN3_SMMU_INTMAS_S.bits.ints_ptw_ns_msk      = regcfginfo->COMN3_SMMU_INTMAS_S.bits.ints_ptw_ns_msk;
        pAllReg->VEDU_COMN3_REGS.COMN3_SMMU_INTMAS_S.bits.ints_ptw_invalid_msk = regcfginfo->COMN3_SMMU_INTMAS_S.bits.ints_ptw_invalid_msk;
        pAllReg->VEDU_COMN3_REGS.COMN3_SMMU_INTMAS_S.bits.ints_ptw_trans_msk   = regcfginfo->COMN3_SMMU_INTMAS_S.bits.ints_ptw_trans_msk;
        pAllReg->VEDU_COMN3_REGS.COMN3_SMMU_INTMAS_S.bits.ints_tlbmiss_msk     = regcfginfo->COMN3_SMMU_INTMAS_S.bits.ints_tlbmiss_msk;
        pAllReg->VEDU_COMN3_REGS.COMN3_SMMU_INTMAS_S.bits.ints_ext_msk         = regcfginfo->COMN3_SMMU_INTMAS_S.bits.ints_ext_msk;
        pAllReg->VEDU_COMN3_REGS.COMN3_SMMU_INTMAS_S.bits.ints_permis_msk      = regcfginfo->COMN3_SMMU_INTMAS_S.bits.ints_permis_msk;
    }

     // smr 0 - 35
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR0_NS.bits.smr0_ptw_qos    =  pEncPara->comn0_smr0_ptw_qos ;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR0_NS.bits.smr0_bypass     =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR0_NS.bits.smr0_bypass  ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR1_NS.bits.smr1_ptw_qos    =  pEncPara->comn0_smr1_ptw_qos ;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR1_NS.bits.smr1_bypass     =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR1_NS.bits.smr1_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR2_NS.bits.smr2_ptw_qos    =  pEncPara->comn0_smr2_ptw_qos ;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR2_NS.bits.smr2_bypass     =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR2_NS.bits.smr2_bypass  ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR3_NS.bits.smr3_ptw_qos    =  pEncPara->comn0_smr3_ptw_qos ;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR3_NS.bits.smr3_bypass     =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR3_NS.bits.smr3_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR4_NS.bits.smr4_ptw_qos    =  pEncPara->comn0_smr4_ptw_qos ;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR4_NS.bits.smr4_bypass     =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR4_NS.bits.smr4_bypass  ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR5_NS.bits.smr5_ptw_qos    =  pEncPara->comn0_smr5_ptw_qos ;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR5_NS.bits.smr5_bypass     =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR5_NS.bits.smr5_bypass  ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR6_NS.bits.smr6_ptw_qos    =  pEncPara->comn0_smr6_ptw_qos ;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR6_NS.bits.smr6_bypass     =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR6_NS.bits.smr6_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR7_NS.bits.smr7_ptw_qos    =  pEncPara->comn0_smr7_ptw_qos ;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR7_NS.bits.smr7_bypass     =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR7_NS.bits.smr7_bypass  ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR8_NS.bits.smr8_ptw_qos    =  pEncPara->comn0_smr8_ptw_qos ;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR8_NS.bits.smr8_bypass     =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR8_NS.bits.smr8_bypass  ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR9_NS.bits.smr9_ptw_qos    =  pEncPara->comn0_smr9_ptw_qos ;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR9_NS.bits.smr9_bypass     =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR9_NS.bits.smr9_bypass  ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR10_NS.bits.smr10_ptw_qos  =  pEncPara->comn0_smr10_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR10_NS.bits.smr10_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR10_NS.bits.smr10_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR11_NS.bits.smr11_ptw_qos  =  pEncPara->comn0_smr11_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR11_NS.bits.smr11_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR11_NS.bits.smr11_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR12_NS.bits.smr12_ptw_qos  =  pEncPara->comn0_smr12_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR12_NS.bits.smr12_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR12_NS.bits.smr12_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR13_NS.bits.smr13_ptw_qos  =  pEncPara->comn0_smr13_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR13_NS.bits.smr13_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR13_NS.bits.smr13_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR14_NS.bits.smr14_ptw_qos  =  pEncPara->comn0_smr14_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR14_NS.bits.smr14_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR14_NS.bits.smr14_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR15_NS.bits.smr15_ptw_qos  =  pEncPara->comn0_smr15_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR15_NS.bits.smr15_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR15_NS.bits.smr15_bypass;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR16_NS.bits.smr16_ptw_qos  =  pEncPara->comn0_smr16_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR16_NS.bits.smr16_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR16_NS.bits.smr16_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR17_NS.bits.smr17_ptw_qos  =  pEncPara->comn0_smr17_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR17_NS.bits.smr17_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR17_NS.bits.smr17_bypass;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR18_NS.bits.smr18_ptw_qos  =  pEncPara->comn0_smr18_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR18_NS.bits.smr18_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR18_NS.bits.smr18_bypass;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR19_NS.bits.smr19_ptw_qos  =  pEncPara->comn0_smr19_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR19_NS.bits.smr19_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR19_NS.bits.smr19_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR20_NS.bits.smr20_ptw_qos  =  pEncPara->comn0_smr20_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR20_NS.bits.smr20_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR20_NS.bits.smr20_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR21_NS.bits.smr21_ptw_qos  =  pEncPara->comn0_smr21_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR21_NS.bits.smr21_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR21_NS.bits.smr21_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR22_NS.bits.smr22_ptw_qos  =  pEncPara->comn0_smr22_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR22_NS.bits.smr22_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR22_NS.bits.smr22_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR23_NS.bits.smr23_ptw_qos  =  pEncPara->comn0_smr23_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR23_NS.bits.smr23_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR23_NS.bits.smr23_bypass;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR24_NS.bits.smr24_ptw_qos  =  pEncPara->comn0_smr24_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR24_NS.bits.smr24_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR24_NS.bits.smr24_bypass;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR25_NS.bits.smr25_ptw_qos  =  pEncPara->comn0_smr25_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR25_NS.bits.smr25_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR25_NS.bits.smr25_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR26_NS.bits.smr26_ptw_qos  =  pEncPara->comn0_smr26_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR26_NS.bits.smr26_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR26_NS.bits.smr26_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR27_NS.bits.smr27_ptw_qos  =  pEncPara->comn0_smr27_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR27_NS.bits.smr27_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR27_NS.bits.smr27_bypass;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR28_NS.bits.smr28_ptw_qos  =  pEncPara->comn0_smr28_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR28_NS.bits.smr28_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR28_NS.bits.smr28_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR29_NS.bits.smr29_ptw_qos  =  pEncPara->comn0_smr29_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR29_NS.bits.smr29_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR29_NS.bits.smr29_bypass;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR30_NS.bits.smr30_ptw_qos  =  pEncPara->comn0_smr30_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR30_NS.bits.smr30_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR30_NS.bits.smr30_bypass;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR31_NS.bits.smr31_ptw_qos  =  pEncPara->comn0_smr31_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR31_NS.bits.smr31_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR31_NS.bits.smr31_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR32_NS.bits.smr32_ptw_qos  =  pEncPara->comn0_smr32_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR32_NS.bits.smr32_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR32_NS.bits.smr32_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR33_NS.bits.smr33_ptw_qos  =  pEncPara->comn0_smr33_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR33_NS.bits.smr33_bypass   = regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR33_NS.bits.smr33_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR34_NS.bits.smr34_ptw_qos  =  pEncPara->comn0_smr34_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR34_NS.bits.smr34_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR34_NS.bits.smr34_bypass ;
//     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR35_NS.bits.smr35_ptw_qos  =  pEncPara->comn0_smr35_ptw_qos;
     pAllReg->VEDU_COMN0_REGS.COMN0_SMMU_SMR35_NS.bits.smr35_bypass   =  regcfginfo->VEDU_COMN0_REGS.COMN0_SMMU_SMR35_NS.bits.smr35_bypass ;

    if(( regcfginfo->vcpi_str_fmt != 6) && ( regcfginfo->vcpi_str_fmt  != 8 ))
    {
        pAllReg->VEDU_MMU0_REGS.MST_RD6_0CFG.bits.rd6_tlb_depth   = 0x80;
        pAllReg->VEDU_MMU0_REGS.MST_RD6_0CFG.bits.rd6_tlb_base    = 0x8e;
        pAllReg->VEDU_MMU0_REGS.MST_RD6_0CFG.bits.rd6_tlb_last    = 0x10d;
        pAllReg->VEDU_MMU0_REGS.MST_RD6_1CFG.bits.rd6_tlb_nxt     = 0x18;
        pAllReg->VEDU_MMU0_REGS.MST_RD6_1CFG.bits.rd6_tlb_uw      = 0x28;
        pAllReg->VEDU_MMU0_REGS.MST_RD6_1CFG.bits.rd6_tlb_upd_dis = 0x60;
        pAllReg->VEDU_MMU0_REGS.MST_RD6_1CFG.bits.rd6_tlb_upd_len = 0x8;
    }
    else
    {
        pAllReg->VEDU_MMU0_REGS.MST_RD6_0CFG.bits.rd6_tlb_depth   = 0x100;
        pAllReg->VEDU_MMU0_REGS.MST_RD6_0CFG.bits.rd6_tlb_base    = 0x8e;
        pAllReg->VEDU_MMU0_REGS.MST_RD6_0CFG.bits.rd6_tlb_last    = 0x18d;
        pAllReg->VEDU_MMU0_REGS.MST_RD6_1CFG.bits.rd6_tlb_nxt     = 0x40;
        pAllReg->VEDU_MMU0_REGS.MST_RD6_1CFG.bits.rd6_tlb_uw      = 0x80;
        pAllReg->VEDU_MMU0_REGS.MST_RD6_1CFG.bits.rd6_tlb_upd_dis = 0xa0;
        pAllReg->VEDU_MMU0_REGS.MST_RD6_1CFG.bits.rd6_tlb_upd_len = 0x20;
    }

    pAllReg->VEDU_MMU1_REGS.MST_VEDU_MODE_MMU.bits.vcpi_protocol = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_MODE_MMU.bits.vcpi_protocol;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_MODE_MMU.bits.vcpi_str_fmt = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_MODE_MMU.bits.vcpi_str_fmt;

    pAllReg->VEDU_MMU1_REGS.MST_VEDU_NBI_MVST_ADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_NBI_MVST_ADDR_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_NBI_MVST_ADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_NBI_MVST_ADDR_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_NBI_MVLD_ADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_NBI_MVLD_ADDR_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_NBI_MVLD_ADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_NBI_MVLD_ADDR_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_PMEST_ADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_PMEST_ADDR_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_PMEST_ADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_PMEST_ADDR_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_PMELD_ADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_PMELD_ADDR_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_PMELD_ADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_PMELD_ADDR_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_YH_ADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REC_YH_ADDR_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_YH_ADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REC_YH_ADDR_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_CH_ADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REC_CH_ADDR_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_CH_ADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REC_CH_ADDR_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_YADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REC_YADDR_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_YADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REC_YADDR_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_CADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REC_CADDR_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REC_CADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REC_CADDR_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_YH_ADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REF_YH_ADDR_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_YH_ADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REF_YH_ADDR_END;

    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_CH_ADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REF_CH_ADDR_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_CH_ADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REF_CH_ADDR_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_YADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REF_YADDR_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_YADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REF_YADDR_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_CADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REF_CADDR_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_REF_CADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_REF_CADDR_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_SRC_YADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_SRC_YADDR_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_SRC_YADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_SRC_YADDR_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_SRC_CADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_SRC_CADDR_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_SRC_CADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_SRC_CADDR_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_SRC_VADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_SRC_VADDR_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_SRC_VADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_SRC_VADDR_END;

    pAllReg->VEDU_MMU1_REGS.MST_VEDU_LAT_ADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_LAT_ADDR_STR;//add by gaoyajun
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_LAT_ADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_LAT_ADDR_END;

    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR0_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR0_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR1_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR1_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR2_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR2_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR3_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR3_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR4_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR4_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR5_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR5_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR6_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR6_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR7_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR7_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR8_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR8_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR9_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR9_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR10_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR10_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR11_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR11_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR12_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR12_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR13_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR13_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR14_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR14_STR;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR15_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR15_STR;

    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR0_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR0_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR1_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR1_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR2_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR2_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR3_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR3_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR4_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR4_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR5_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR5_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR6_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR6_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR7_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR7_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR8_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR8_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR9_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR9_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR10_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR10_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR11_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR11_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR12_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR12_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR13_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR13_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR14_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR14_END;
    pAllReg->VEDU_MMU1_REGS.MST_VEDU_STRMADDR15_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_STRMADDR15_END;

    /*-----------------------------------------  µØÖ·¶îÍâÅäÖÃ l00214825  ---------------------------------------------*/
    pAllReg->VEDU_VCPI_REGS.VEDU_TUNLCELL_ADDR = regcfginfo->VEDU_VCPI_REGS.VEDU_TUNLCELL_ADDR ;
    pAllReg->VEDU_VCPI_REGS.VEDU_SRC_YADDR     = regcfginfo->VEDU_VCPI_REGS.VEDU_SRC_YADDR;
    pAllReg->VEDU_VCPI_REGS.VEDU_SRC_CADDR     = regcfginfo->VEDU_VCPI_REGS.VEDU_SRC_CADDR ;
    pAllReg->VEDU_VCPI_REGS.VEDU_SRC_VADDR     = regcfginfo->VEDU_VCPI_REGS.VEDU_SRC_VADDR  ;
    pAllReg->VEDU_VCPI_REGS.VEDU_YH_ADDR       = regcfginfo->VEDU_VCPI_REGS.VEDU_YH_ADDR ;
    pAllReg->VEDU_VCPI_REGS.VEDU_CH_ADDR       = regcfginfo->VEDU_VCPI_REGS.VEDU_CH_ADDR ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRIDE.bits.curld_y_stride = regcfginfo->VEDU_VCPI_REGS.VEDU_STRIDE.bits.curld_y_stride;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRIDE.bits.curld_c_stride = regcfginfo->VEDU_VCPI_REGS.VEDU_STRIDE.bits.curld_c_stride;

     // curld yh 实际没有起作用
    pAllReg->VEDU_VCPI_REGS.VEDU_HEAD_STRIDE.bits.curld_yh_stride = regcfginfo->VEDU_VCPI_REGS.VEDU_HEAD_STRIDE.bits.curld_yh_stride;
    pAllReg->VEDU_VCPI_REGS.VEDU_HEAD_STRIDE.bits.curld_ch_stride = regcfginfo->VEDU_VCPI_REGS.VEDU_HEAD_STRIDE.bits.curld_ch_stride;

    pAllReg->VEDU_VCPI_REGS.VEDU_REC_YADDR  = regcfginfo->VEDU_VCPI_REGS.VEDU_REC_YADDR ;
    pAllReg->VEDU_VCPI_REGS.VEDU_REC_CADDR   = regcfginfo->VEDU_VCPI_REGS.VEDU_REC_CADDR;

    /* Define the union U_VEDU_REC_STRIDE */
    {
        pAllReg->VEDU_VCPI_REGS.VEDU_REC_STRIDE.u32 = regcfginfo->VEDU_VCPI_REGS.VEDU_REC_STRIDE.u32;
    }
    // REC_YH_ADDR
    pAllReg->VEDU_VCPI_REGS.VEDU_REC_YH_ADDR  = regcfginfo->VEDU_VCPI_REGS.VEDU_REC_YH_ADDR;
    //REC_CH_ADDR
    pAllReg->VEDU_VCPI_REGS.VEDU_REC_CH_ADDR = regcfginfo->VEDU_VCPI_REGS.VEDU_REC_CH_ADDR;
    //REC_HEAD_STRIDE
    {
        pAllReg->VEDU_VCPI_REGS.VEDU_REC_HEAD_STRIDE.u32 = regcfginfo->VEDU_VCPI_REGS.VEDU_REC_HEAD_STRIDE.u32;
    }

     //VEDU_LUMA_REF_ADDR
     pAllReg->VEDU_VCPI_REGS.VEDU_REF_YADDR = regcfginfo->VEDU_VCPI_REGS.VEDU_REF_YADDR ;
     //VEDU_CHROMA_REF_ADDR
     pAllReg->VEDU_VCPI_REGS.VEDU_REF_CADDR = regcfginfo->VEDU_VCPI_REGS.VEDU_REF_CADDR ;

     /* Define the union U_VEDU_VSTRIDE */
    {
        unsigned int vcpi_imgwidth_pix ;
        U_VEDU_REF_STRIDE D32;
        D32.u32 = 0;
        vcpi_imgwidth_pix = (unsigned int)regcfginfo->VEDU_VCPI_REGS.VEDU_PICSIZE_PIX.bits.vcpi_imgwidth_pix;
        D32.bits.refld_luma_vstride   = (vcpi_imgwidth_pix + 255)/256*256;
        D32.bits.refld_chroma_vstride = (vcpi_imgwidth_pix + 255)/256*256;
        pAllReg->VEDU_VCPI_REGS.VEDU_REF_STRIDE.u32 = D32.u32;
    }

     //VEDU_REF_YH_ADDR
     pAllReg->VEDU_VCPI_REGS.VEDU_REF_YH_ADDR  = regcfginfo->VEDU_VCPI_REGS.VEDU_REF_YH_ADDR;
     //VEDU_REF_CH_ADDR
     pAllReg->VEDU_VCPI_REGS.VEDU_REF_CH_ADDR  = regcfginfo->VEDU_VCPI_REGS.VEDU_REF_CH_ADDR ;
     /* Define the union U_VEDU_REF_HSTRIDE */
    {
        unsigned int vcpi_imgwidth_pix ;
        U_VEDU_REF_HSTRIDE D32;
        D32.u32 = 0;
        vcpi_imgwidth_pix = (unsigned int)regcfginfo->VEDU_VCPI_REGS.VEDU_PICSIZE_PIX.bits.vcpi_imgwidth_pix;
        if (vcpi_protocol == VEDU_H265)//add by 
        {
            D32.bits.refld_yh_stride  = (vcpi_imgwidth_pix + 63)/64*32;
            D32.bits.refld_ch_stride  = (vcpi_imgwidth_pix + 63)/64*32;
        }
        else if (vcpi_protocol == VEDU_H264)
        {
            D32.bits.refld_yh_stride  = (vcpi_imgwidth_pix + 255)/256*32;
            D32.bits.refld_ch_stride  = (vcpi_imgwidth_pix + 255)/256*32;
        }
        pAllReg->VEDU_VCPI_REGS.VEDU_REF_HSTRIDE.u32 = D32.u32;
    }

     ///////VEDU_PMELD_ADDR
    pAllReg->VEDU_VCPI_REGS.VEDU_PMELD_ADDR  = regcfginfo->VEDU_VCPI_REGS.VEDU_PMELD_ADDR ;
    pAllReg->VEDU_VCPI_REGS.VEDU_PMEST_ADDR  = regcfginfo->VEDU_VCPI_REGS.VEDU_PMEST_ADDR ;
    pAllReg->VEDU_VCPI_REGS.VEDU_NBI_MVST_ADDR   = regcfginfo->VEDU_VCPI_REGS.VEDU_NBI_MVST_ADDR ;
    pAllReg->VEDU_VCPI_REGS.VEDU_NBI_MVLD_ADDR   = regcfginfo->VEDU_VCPI_REGS.VEDU_NBI_MVLD_ADDR ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR0  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR0 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR1  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR1 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR2  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR2 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR3  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR3 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR4  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR4 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR5  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR5 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR6  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR6 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR7  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR7 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR8  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR8 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR9  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR9 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR10  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR10 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR11  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR11 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR12  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR12;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR13  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR13 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR14  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR14;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMADDR15  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMADDR15;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN0  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN0 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN1  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN1 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN2  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN2;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN3  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN3 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN4  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN4 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN5  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN5 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN6  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN6;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN7  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN7 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN8  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN8 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN9  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN9 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN10  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN10 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN11  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN11;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN12  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN12 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN13  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN13 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN14  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN14 ;
    pAllReg->VEDU_VCPI_REGS.VEDU_STRMBUFLEN15  = regcfginfo->VEDU_VCPI_REGS.VEDU_STRMBUFLEN15;

#ifdef HARDWARE_SPLIT_SPS_PPS_EN
        pAllReg->VEDU_VCPI_REGS.VEDU_CURLD_LOWDLY.bits.vcpi_para_sprat_en = 1;
        if (regcfginfo.bFirstNal2Send )
        {
            pAllReg->VEDU_VCPI_REGS.VEDU_PARA_ADDR          = regcfginfo->VEDU_VCPI_REGS.VEDU_PARA_ADDR;
            pAllReg->VEDU_MMU1_REGS.MST_VEDU_PPS_ADDR_STR = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_PPS_ADDR_STR ;
            pAllReg->VEDU_MMU1_REGS.MST_VEDU_PPS_ADDR_END = regcfginfo->VEDU_MMU1_REGS.MST_VEDU_PPS_ADDR_END ;
        }
#else
    pAllReg->VEDU_VCPI_REGS.VEDU_CURLD_LOWDLY.bits.vcpi_para_sprat_en = 0;
#endif

     /*-----------------------------------------  地址额外配置end l00214825  ---------------------------------------------*/


     /*-----------------------------------------  slice 寄存器额外配置l00214825  ---------------------------------------------*/

    /* Define the union U_CABAC_GLB_CFG */
    {
        pAllReg->VEDU_CABAC_REGS.CABAC_GLB_CFG.u32 = regcfginfo->VEDU_CABAC_REGS.CABAC_GLB_CFG.u32;
    }

    /* Define the union U_CABAC_SLCHDR_SIZE */
    {
        pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_SIZE.u32 = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_SIZE.u32;
    }
        /* Define the union U_CABAC_SLCHDR_PART1 */
    {
        pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART1.u32 = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART1.u32;
    }

    /* Define the union U_CABAC_SLCHDR_PART2_SEG1 */

    if (vcpi_protocol== VEDU_H265) //add by 
    {

        pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG1  = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG1 ;

        if (regcfginfo->slchdr_size_part2 > 32)
        {
            pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG2   = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG2 ;
        }
        pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG3   = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG3 ;
        pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG4   = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG4 ;
        pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG5   = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG5 ;
        pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG6   = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG6;
        pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG7   = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG7 ;
        pAllReg->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG8   = regcfginfo->VEDU_CABAC_REGS.CABAC_SLCHDR_PART2_SEG8 ;
    }
    else
    {
        pAllReg->VEDU_VLC_REGS.VLC_SlcHdrStrm0 = regcfginfo->VEDU_VLC_REGS.VLC_SlcHdrStrm0;
        pAllReg->VEDU_VLC_REGS.VLC_SlcHdrStrm1 = regcfginfo->VEDU_VLC_REGS.VLC_SlcHdrStrm1;
        pAllReg->VEDU_VLC_REGS.VLC_SlcHdrStrm2 = regcfginfo->VEDU_VLC_REGS.VLC_SlcHdrStrm2;
        pAllReg->VEDU_VLC_REGS.VLC_SlcHdrStrm3 = regcfginfo->VEDU_VLC_REGS.VLC_SlcHdrStrm3;
        pAllReg->VEDU_VLC_REGS.VLC_ReorderStrm0 = regcfginfo->VEDU_VLC_REGS.VLC_ReorderStrm0;
        pAllReg->VEDU_VLC_REGS.VLC_ReorderStrm1  = regcfginfo->VEDU_VLC_REGS.VLC_ReorderStrm1;
        pAllReg->VEDU_VLC_REGS.VLC_MarkingStrm0  = regcfginfo->VEDU_VLC_REGS.VLC_MarkingStrm0;
        pAllReg->VEDU_VLC_REGS.VLC_MarkingStrm1 = regcfginfo->VEDU_VLC_REGS.VLC_MarkingStrm1;
        pAllReg->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_parabit = regcfginfo->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_parabit;
        pAllReg->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_reorderbit = regcfginfo->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_reorderbit;
        pAllReg->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_markingbit = regcfginfo->VEDU_VLC_REGS.VLC_SLCHDRPARA.bits.vcpi_markingbit;
    }

    pAllReg->VEDU_VLCST_REGS.VLCST_PARAMETER.bits.vlcst_para_set_en = 0;
#ifdef HARDWARE_SPLIT_SPS_PPS_EN
    if (regcfginfo->vlcst_para_set_en == 1)
    {
        pAllReg->VEDU_VLCST_REGS.VLCST_PARAMETER.bits.vlcst_para_set_en = regcfginfo->VEDU_VLCST_REGS.VLCST_PARAMETER.bits.vlcst_para_set_en;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARAMETER.bits.vlcst_para_set_len = regcfginfo->VEDU_VLCST_REGS.VLCST_PARAMETER.bits.vlcst_para_set_len;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA0 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA0;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA1 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA1;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA2 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA2;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA3 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA3;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA4 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA4;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA5 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA5;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA6 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA6;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA7 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA7;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA8 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA8;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA9 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA9;

        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA10 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA10;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA11 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA11;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA12 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA12;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA13 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA13;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA14 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA14;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA15 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA15;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA16 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA16;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA17 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA17;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA18 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA18;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA19 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA19;

        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA20 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA20;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA21 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA21;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA22 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA22;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA23 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA23;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA24 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA24 ;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA25 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA25;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA26 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA26;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA27 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA27;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA28 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA28;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA29 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA29;

        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA30 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA30;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA31 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA31;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA32 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA32;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA33 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA33;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA34 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA34;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA35 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA35;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA36 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA36;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA37 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA37;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA38 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA38;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA39 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA39;

        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA40 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA40;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA41 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA41;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA42 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA42;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA43 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA43;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA44 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA44;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA45 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA45;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA46 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA46;
        pAllReg->VEDU_VLCST_REGS.VLCST_PARA_DATA47 = regcfginfo->VEDU_VLCST_REGS.VLCST_PARA_DATA47;
    }

#endif
}


