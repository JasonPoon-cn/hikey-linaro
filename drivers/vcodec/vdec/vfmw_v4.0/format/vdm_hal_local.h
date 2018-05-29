/*
 *      Video Decode driver for hikey960   vdm hal structure .h file
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
 
#ifndef _VDM_HAL_LOCAL_HEADER_
#define _VDM_HAL_LOCAL_HEADER_

#include "sysconfig.h"
#include "mem_manage.h"
#include "vfmw.h"
#include "vdm_drv.h"

//control registers
#define    VREG_BASIC_CFG0                     0x008
#define    VREG_BASIC_CFG1                     0x00c
#define    VREG_AVM_ADDR                       0x010
#define    VREG_VAM_ADDR                       0x014
#define    VREG_STREAM_BASE_ADDR               0x018

#define    VREG_SED_TO                         0x03c
#define    VREG_ITRANS_TO                      0x040
#define    VREG_PMV_TO                         0x044
#define    VREG_PRC_TO                         0x048
#define    VREG_RCN_TO                         0x04c
#define    VREG_DBLK_TO                        0x050
#define    VREG_PPFD_TO                        0x054

//1d registers
#define    VREG_YSTADDR_1D                     0x060
#define    VREG_YSTRIDE_1D                     0x064
#define    VREG_UVOFFSET_1D                    0x068
//prc registers
#define    VREG_HEAD_INF_OFFSET                0x06c
//ppfd registers
#define    VREG_PPFD_BUF_ADDR                  0x080
#define    VREG_PPFD_BUF_LEN                   0x084
#define    VREG_REF_PIC_TYPE                   0x094
#define    VREG_FF_APT_EN                      0x098

typedef struct {
	unsigned int mbamt_to_dec           :20;
	unsigned int memory_clock_gating_en :1;
	unsigned int module_clock_gating_en :1;
	unsigned int marker_bit_detect_en   :1;
	unsigned int ac_last_detect_en      :1;
	unsigned int coef_idx_detect_en     :1;
	unsigned int vop_type_detect_en     :1;
	unsigned int reserved               :2;
	unsigned int luma_sum_en            :1;
	unsigned int luma_historgam_en      :1;
	unsigned int load_qmatrix_flag      :1;
	unsigned int sec_mode_en            :1;
} BASIC_CFG0;

typedef struct {
	unsigned int video_standard     :4;
	unsigned int ddr_stride         :9;
	unsigned int uv_order_en        :1;
	unsigned int fst_slc_grp        :1;
	unsigned int mv_output_en       :1;
	unsigned int max_slcgrp_num     :12;
	unsigned int line_num_output_en :1;
	unsigned int vdh_2d_en          :1;
	unsigned int compress_en        :1;
	unsigned int ppfd_en            :1;
} BASIC_CFG1;

typedef struct {
	unsigned int av_msg_addr        :32;
} AVM_ADDR;

typedef struct {
	unsigned int va_msg_addr        :32;
} VAM_ADDR;

typedef struct {
	unsigned int stream_base_addr   :32;
} STREAM_BASE_ADDR;

typedef struct {
	unsigned int ystaddr_1d         :32;
} YSTADDR_1D;

typedef struct {
	unsigned int ff_apt_en          :1;
	unsigned int reserved           :31;
} FF_APT_EN;

typedef struct {
	unsigned int ppfd_buf_addr     :32;
} PPFD_BUF_ADDR;

typedef struct {
	unsigned int ppfd_buf_len      :16;
	unsigned int reserved          :16;
} PPFD_BUF_LEN;

/************************************************************************/
/*  Register read/write interface                                       */
/************************************************************************/
void  read_register_value(unsigned int index , unsigned int reg , int *val) ;

void  write_register_value(unsigned int index , unsigned int reg , int val) ;

#endif
