/*
 *      Video Decode driver for hikey960   BPD .h file
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
 
#ifndef _BITPLANE_HEADER_
#define _BITPLANE_HEADER_

typedef enum {
	BPDDRV_SLEEP_STAGE_NONE = 0,
	BPDDRV_SLEEP_STAGE_PREPARE,
	BPDDRV_SLEEP_STAGE_SLEEP
} BPDDRV_SLEEP_STAGE_E;

typedef enum {
	BPD_IDLE = 0,
	BPD_WORKING,
} BPD_STATE_E;

typedef struct
{
	unsigned int bit_offset:                           8;
	unsigned int pic_mbwidth_mod3:                     2;
	unsigned int pic_mbheigt_mod3:                     2;
	unsigned int mvmode_en:                            1;
	unsigned int overflag_en:                          1;
	unsigned int pic_coding_type:                      2;
	unsigned int pic_structure:                        2;
	unsigned int profile:                              2;
	unsigned int safe_flag:                            1;
	unsigned int reserved:                             11;
} BPD_CFG0;

typedef struct
{
	unsigned int pic_mbwidth_m1:                       16;
	unsigned int pic_mbheight_m1:                      16;
} BPD_CFG2;

typedef struct
{
	unsigned int bp6_staddr:                           32;
} BPD_CFG10;

typedef struct
{
	unsigned int axi_id:                               4;
	unsigned int axi_rd_outstanding:                   4;
	unsigned int axi_wr_outstanding:                   4;
	unsigned int bpd_axi_sep       :                   3;
	unsigned int reserved          :                   17;
} BPD_CFG11;

typedef struct
{
	unsigned int eaten_lenth:                          32;
} BPD_OUT0;

typedef struct {
	unsigned int MVTYPEMBMode :4;
	unsigned int SKIPMBMode   :4;
	unsigned int DIRECTMBMode :4;
	unsigned int ACPREDMode   :4;
	unsigned int OVERFLAGSMode:4;
	unsigned int FIELDTXMode  :4;
	unsigned int FORWARDMBMode:4;
	unsigned int CONDOVER     :2;
	unsigned int reserved     :2;
} BPD_OUT1;

typedef struct {
	 int BpdStart;
	 int BpdIntState;
	 int BpdIntMask;
	 int BpdCfg0;
	 int BpdCfg1;
	 int BpdCfg2;
	 int BpdCfg3;
	 int BpdCfg4;
	 int BpdCfg5;
	 int BpdCfg6;
	 int BpdCfg7;
	 int BpdCfg8;
	 int BpdCfg9;
	 int BpdCfg10;
	 int BpdCfg11;
} OMXBPD_REG_CFG_S;

typedef struct {
	 int BpdIntMask;
	 int BpdState;
	unsigned int BpdOut0EatenLenth;
	unsigned int BpdOut1MvtypembMode;
	unsigned int BpdOut1SkipmbMode;
	unsigned int BpdOut1DirectmbMode;
	unsigned int BpdOut1AcpredMode;
	unsigned int BpdOut1OverflagsMode;
	unsigned int BpdOut1FieldtxMode;
	unsigned int BpdOut1ForwardmbMode;
	unsigned int BpdOut1Condover;
	unsigned int BpdOut1Reserved;
} OMXBPD_REG_STATE_S;

typedef struct {
	unsigned int GlbResetFlag;
	OMXBPD_REG_CFG_S BpsRegCfg;
	OMXBPD_REG_STATE_S BpsRegState;
} OMXBPD_REG_S;

 int BPD_Init(void) ;

 int BPD_ConfigReg(OMXBPD_REG_S *pBpdReg , unsigned int VdhRegBaseAddr);

 int BPDDRV_PrepareSleep(void);

BPDDRV_SLEEP_STAGE_E BPDDRV_GetSleepStage(void);

void BPDDRV_SetSleepStage(BPDDRV_SLEEP_STAGE_E sleepState);

void BPDDRV_ForceSleep(unsigned int VdhRegBaseAddr);

void BPDDRV_ExitSleep(void);

#endif
