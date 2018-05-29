/*
 *      Video Encode driver for hikey960   memory structure
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
#include <asm/uaccess.h>

#define MAX_BUFFER_SIZE        (10*1024)
#define MAX_MEM_NAME_LEN       (15)
#define MAX_KMALLOC_MEM_NODE   (16)    // 1 channel need 2 node ,there is have max 8 channels .

struct venc_mem_buf {
        char          node_name[MAX_MEM_NAME_LEN];
        char          zone_name[MAX_MEM_NAME_LEN];
        void*         virt_addr;
        unsigned int           phys_addr;
        unsigned int           size;
};

static char                *g_sbuf = NULL;
static int                  g_venc_node_num   = 0;
static struct semaphore     g_VencMemSem;
static struct venc_mem_buf  g_venc_mem_node[MAX_KMALLOC_MEM_NODE];

int DRV_MEM_INIT(void)
{
    sema_init(&g_VencMemSem , 1);	
    g_sbuf = vmalloc(MAX_BUFFER_SIZE);
    if (g_sbuf == NULL) {
        HI_ERR_VENC("%s call vmalloc failed\n", __func__);
        return -1;
    }

    memset((void *)g_sbuf, 0, MAX_BUFFER_SIZE);
    memset((void *)&g_venc_mem_node, 0, MAX_KMALLOC_MEM_NODE*sizeof(g_venc_mem_node[0]));
    g_venc_node_num = 0;
    return 0;
}

int DRV_MEM_EXIT(void)
{
    int i;
    for (i = 0; i < MAX_KMALLOC_MEM_NODE; ++i) {
        if (g_venc_mem_node[i].virt_addr != NULL) {
            kfree(g_venc_mem_node[i].virt_addr);
            memset(&g_venc_mem_node[i], 0, sizeof(g_venc_mem_node[i]));
        }
    }

    if (g_sbuf)
        vfree(g_sbuf);
    g_sbuf = NULL ;

    g_venc_node_num = 0;
    return 0;
}

int DRV_MEM_KAlloc(const  char* bufName, const  char *zone_name, MEM_BUFFER_S *psMBuf)
{
    unsigned int  i;
    void *virt_addr = NULL;

    if (down_interruptible(&g_VencMemSem)) {
        HI_ERR_VENC("down_interruptible failed\n");
        return (-1);
    }

    if (psMBuf == NULL || psMBuf->u32Size == 0) {
        HI_ERR_VENC("%s, failed to check arguments\n", __func__);
        goto err_exit;
    }

    for (i=0; i<MAX_KMALLOC_MEM_NODE; ++i) {
        if ((0 == g_venc_mem_node[i].phys_addr) && (g_venc_mem_node[i].virt_addr == NULL)) {
            break;
        }
    }

    if (i >= MAX_KMALLOC_MEM_NODE) {
        HI_ERR_VENC("%s, FATAL: No free ion mem node\n", __func__);
        goto err_exit;
    }

    virt_addr = kmalloc(psMBuf->u32Size, GFP_KERNEL | GFP_DMA);
    if (IS_ERR_OR_NULL(virt_addr)) {
        HI_ERR_VENC("%s, call kzalloc failed\n", __func__);
        goto err_exit;
    }

    psMBuf->pStartVirAddr   = virt_addr;
    psMBuf->u32StartPhyAddr = __pa(virt_addr);

    snprintf(g_venc_mem_node[i].node_name, MAX_MEM_NAME_LEN, bufName);
    g_venc_mem_node[i].node_name[MAX_MEM_NAME_LEN - 1] = '\0';

    snprintf(g_venc_mem_node[i].zone_name, MAX_MEM_NAME_LEN, zone_name);
    g_venc_mem_node[i].zone_name[MAX_MEM_NAME_LEN - 1] = '\0';

    g_venc_mem_node[i].virt_addr = psMBuf->pStartVirAddr;
    g_venc_mem_node[i].phys_addr = psMBuf->u32StartPhyAddr;
    g_venc_mem_node[i].size      = psMBuf->u32Size;

    g_venc_node_num++;

    up(&g_VencMemSem);
    return 0;

err_exit:
    up(&g_VencMemSem);
    return (-1);
}

/* kfree */
 int DRV_MEM_KFree(const MEM_BUFFER_S *psMBuf)
{
    unsigned int  i;
    if (down_interruptible(&g_VencMemSem)) {
        HI_ERR_VENC("down_interruptible failed\n");
        return (-1);
    }

    if (NULL == psMBuf || psMBuf->pStartVirAddr == NULL || psMBuf->u32StartPhyAddr == 0) {
        HI_ERR_VENC("%s, failed to check arguments\n", __func__);
        goto err_exit;
    }

    for (i=0; i<MAX_KMALLOC_MEM_NODE; ++i) {
        if ((psMBuf->u32StartPhyAddr == g_venc_mem_node[i].phys_addr) &&
            (psMBuf->pStartVirAddr == g_venc_mem_node[i].virt_addr))
        {
                break;
        }
    }

    if(i >= MAX_KMALLOC_MEM_NODE) {
        HI_ERR_VENC("%s, FATAL: No free ion mem node\n", __func__);
        goto err_exit;
    }

    kfree(g_venc_mem_node[i].virt_addr);
    memset(&g_venc_mem_node[i], 0, sizeof(g_venc_mem_node[i]));
    g_venc_node_num = (g_venc_node_num > 0)? (g_venc_node_num-1): 0;

    up(&g_VencMemSem);
    return 0;
err_exit:
    up(&g_VencMemSem);
    return (-1);
}

 int HI_DRV_UserCopy(struct file *file, unsigned int cmd, unsigned long arg,
			long (*func)(struct file *file, unsigned int cmd, unsigned long uarg))
{
	void  *parg = NULL;
	 int   err   = -EINVAL;
	/*  Copy arguments into temp kernel buffer  */
	switch (_IOC_DIR(cmd)) {
	case _IOC_NONE:
	case _IOC_READ:
	case _IOC_WRITE:
	case (_IOC_WRITE | _IOC_READ):
		if (_IOC_SIZE(cmd) <= MAX_BUFFER_SIZE) {
			parg = g_sbuf;
		} else {
			HI_FATAL_VENC("_IOC_SIZE(cmd) is too long\n");
			goto out;
		}
		err = -EFAULT;
		if (_IOC_DIR(cmd) & _IOC_WRITE) {
			if (copy_from_user(parg, (void __user*)arg, _IOC_SIZE(cmd))) {
				HI_FATAL_VENC("copy_from_user failed, when use ioctl, the para must be a address, cmd=0x%x\n", cmd);
				goto out;
			}
		}
		break;
	default:
		goto out;
	}

	err = func(file, cmd, (long)(parg)); 
	if (err == -ENOIOCTLCMD)
		err = -EINVAL;
	if (err < 0)
		goto out;

	/*  Copy results into user buffer  */
	switch (_IOC_DIR(cmd)) {
	case _IOC_READ:
	case (_IOC_WRITE | _IOC_READ):
		if (copy_to_user((void __user *)arg, parg, _IOC_SIZE(cmd))) {
			HI_FATAL_VENC("copy_to_user failed, when use ioctl, the para must be a address, cmd=0x%x\n", cmd);
			err = -EFAULT;
		}
		break;
	default:
		goto out;
	}
out:
	return err;
}




