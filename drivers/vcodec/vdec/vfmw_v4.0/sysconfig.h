/*
 *      Video Decode driver for hikey960   vfmw sysconfigure .h file 
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
 
#ifndef __VFMW_SYSCONFIG_HEADER__
#define __VFMW_SYSCONFIG_HEADER__

#include "vfmw.h"
#include <asm/cacheflush.h>
#include <linux/kthread.h>
#include <linux/hrtimer.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/wait.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/poll.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/ioctl.h>
#include <linux/semaphore.h>

#ifdef HI_ADVCA_FUNCTION_RELEASE
#define dprint(type, fmt, arg...)  
#else
#define dprint(type, fmt, arg...)  printk(type fmt, ##arg)
#endif

#define REG_AVS_FLAG                   (0x0000)
#define REG_EMAR_ID                    (0x0004)

/* register offset */
#define SCD_REG_OFFSET           (0xc000)
#define BPD_REG_OFFSET           (0xd000)

#define SOFTRST_REQ_OFFSET       (0xf80c)
#define SOFTRST_OK_OFFSET        (0xf810)

#define RD_SCDREG(REG)       read_scdreg_value(REG)
#define WR_SCDREG(REG, DATA)  write_scdreg_value((REG) , (DATA))

typedef struct {
	spinlock_t    irq_lock;
	unsigned long irq_lockflags;
	int           isInit;
} OSAL_IRQ_SPIN_LOCK;

typedef struct {
	wait_queue_head_t queue_head;
	int flag;
} OSAL_EVENT;

unsigned int OSAL_GetTimeInMs(void);

unsigned int read_scdreg_value(unsigned int reg) ;

void         write_scdreg_value(unsigned int reg , unsigned int value) ;

#endif
