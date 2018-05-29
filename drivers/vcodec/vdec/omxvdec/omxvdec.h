/*
 *      Video Decode driver for hikey960   common structure
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
 
#ifndef __OMXVDEC_H__
#define __OMXVDEC_H__

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/version.h>
#include <linux/kthread.h>
#include <linux/seq_file.h>
#include <linux/semaphore.h>
#include <linux/platform_device.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <linux/ioctl.h>

#include "scd_drv.h"
#include "vfmw.h"

#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#endif

#ifndef NULL
#define NULL              (0L)
#endif

#define VDEC_IOCTL_MAGIC 'v'

#define VDEC_IOCTL_GET_OPEN_COUNT  \
	_IO(VDEC_IOCTL_MAGIC, 18)

#define VDEC_IOCTL_SET_CLK_RATE      \
	_IO(VDEC_IOCTL_MAGIC, 20)

#define VDEC_IOCTL_GET_CLK_RATE      \
	_IO(VDEC_IOCTL_MAGIC, 21)

#define VDEC_IOCTL_GET_VDM_HWSTATE      \
	_IO(VDEC_IOCTL_MAGIC, 28)

#define VDEC_IOCTL_SCD_PROC      \
	_IO(VDEC_IOCTL_MAGIC, 30)

#define VDEC_IOCTL_VDM_PROC      \
	_IO(VDEC_IOCTL_MAGIC, 31)

#define VDEC_IOCTL_SET_BPD_PROC      \
	_IO(VDEC_IOCTL_MAGIC, 32)

#define VDEC_IOCTL_PROC_CMD      \
	_IO(VDEC_IOCTL_MAGIC, 33)

#define HIVDEC_IOCTL
#define OMXVDEC_NAME                    "hi_vdec"
#define DRIVER_PATH                     "/dev/hi_vdec"

#define OMXVDEC_VERSION	 		          (2017032300)
#define MAX_OPEN_COUNT                    (32)

#define OMX_ALWS                          (30)
#define OMX_FATAL                         (0)
#define OMX_ERR                           (1)
#define OMX_WARN                          (2)
#define OMX_INFO                          (3)

extern unsigned int g_TraceOption;

#ifndef HI_ADVCA_FUNCTION_RELEASE
#define OmxPrint(flag, format,arg...) \
    do { \
        if (OMX_ALWS == flag || (0 != (g_TraceOption & (1 << flag)))) \
            printk(KERN_ALERT format, ## arg); \
    } while (0)
#else
#define OmxPrint(flag, format,arg...)    ({do{}while(0);0;})
#endif

typedef enum {
	PORT_DIR_INPUT,
	PORT_DIR_OUTPUT,
	PORT_DIR_BOTH = 0xFFFFFFFF
} ePORT_DIR;

typedef enum {
	OMX_USE_OTHER,     // use outer buffer not native (unused in Android platform)
	OMX_USE_NATIVE,	   // use outer native buffer
	OMX_ALLOCATE_DRV,  // allocate buffer in driver side
} eBUFFER_TYPE;

typedef struct {
	unsigned int phyaddr_Y;
	unsigned int phyaddr_C;
	unsigned int stride;
	unsigned int frame_width;
	unsigned int frame_height;
	unsigned int disp_width;
	unsigned int disp_height;
	unsigned int left_offset;
	unsigned int right_offset;
	unsigned int top_offset;
	unsigned int bottom_offset;
	unsigned int format;
	unsigned int act_crc[2];
} OMXVDEC_FRAME_S;

typedef struct {
	unsigned char  is_secure;
	unsigned int phyaddr;
	unsigned int buffer_len;
	unsigned int data_offset;
	unsigned int data_len;
	unsigned int flags;
	unsigned int max_frm_num;
	int share_fd;
	u64 timestamp;
	eBUFFER_TYPE    buffer_type;
	ePORT_DIR       dir;
	OMXVDEC_FRAME_S out_frame;
	u64          kerneladdr;
	void        *bufferaddr;
	void        *client_data;
} OMXVDEC_BUF_DESC;

#ifdef CONFIG_COMPAT    //Modified for 64-bit platform
typedef struct {
	unsigned int phyaddr;
	unsigned int buffer_len;
	unsigned int data_offset;
	unsigned int data_len;
	unsigned int flags;
	unsigned int max_frm_num;
	int share_fd;
	u64 timestamp;
	eBUFFER_TYPE    buffer_type;
	ePORT_DIR       dir;
	OMXVDEC_FRAME_S out_frame;
	u64          kerneladdr;
	compat_ulong_t  bufferaddr;
	compat_ulong_t  client_data;
} COMPAT_BUF_DESC;
#endif

typedef struct {
	int chan_num;
	void *in;
	void *out;
} OMXVDEC_IOCTL_MSG;

#ifdef CONFIG_COMPAT    //Modified for 64-bit platform
typedef struct {
	int chan_num;
	compat_ulong_t in;
	compat_ulong_t out;
} COMPAT_IOCTL_MSG;
#endif

typedef struct {
	unsigned int dat[4];
} CMD_OMXVDEC_MSG_DATA;

typedef struct {
	unsigned int open_count;
	atomic_t nor_chan_num;
	atomic_t sec_chan_num;
	struct semaphore omxvdec_mutex;
	struct semaphore vdec_mutex_scd;
	struct semaphore vdec_mutex_bpd;
	struct semaphore vdec_mutex_vdh;
	struct cdev cdev;
	struct device *device;
} OMXVDEC_ENTRY;

typedef enum {
	CLK_RATE_LOW = 0, 
	CLK_RATE_HIGH, 
	CLK_RATE_BUTT, 
} CLK_RATE_E;

int  VDEC_Regulator_Probe(struct device *dev , VFMW_DTS_CONFIG_S *pDtsConfig);
int  VDEC_Regulator_Remove(struct device *dev);
int  VDEC_Regulator_Enable(void);
int  VDEC_Regulator_Disable(void);
int  VDEC_Regulator_GetClkRate(CLK_RATE_E *pClkRate);
int  VDEC_Regulator_SetClkRate(CLK_RATE_E eClkRate , unsigned int IsFPGA);

#endif
