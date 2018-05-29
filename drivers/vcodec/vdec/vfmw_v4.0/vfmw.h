/*
 *      Video Decode driver for hikey960   vfmw .h file 
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
 
#ifndef __VDEC_FIRMWARE_H__
#define __VDEC_FIRMWARE_H__

typedef enum {
	VFMW_START_RESERVED = 0,
	VFMW_H264           = 0,
	VFMW_VC1,
	VFMW_MPEG4,
	VFMW_MPEG2,
	VFMW_H263,
	VFMW_DIVX3,
	VFMW_AVS,
	VFMW_JPEG,
	VFMW_REAL8 = 8,
	VFMW_REAL9 = 9,
	VFMW_VP6   = 10,
	VFMW_VP6F,
	VFMW_VP6A,
	VFMW_VP8,
	VFMW_SORENSON,
	VFMW_MVC,
	VFMW_HEVC,
	VFMW_RAW,
	VFMW_USER,   
	VFMW_END_RESERVED
} VID_STD_E;

typedef struct {
    unsigned int IsFPGA;
    unsigned int MfdeIrqNum;
    unsigned int ScdIrqNum;
    unsigned int BpdIrqNum;
    unsigned int SmmuIrqNum;
    unsigned int VdhRegBaseAddr;
    unsigned int VdhRegRange;
    unsigned long long SmmuPageBaseAddr;
    unsigned int PERICRG_RegBaseAddr;
} VFMW_DTS_CONFIG_S;

#endif    // __VDEC_FIRMWARE_H__
