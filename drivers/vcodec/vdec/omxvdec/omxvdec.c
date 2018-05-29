/*
 *      Video Decode driver for hikey960   vdec driver 
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

#include <linux/clk.h>
#include <linux/of.h>
#include <linux/io.h>

#include "omxvdec.h"

#include "scd_drv.h"
#include "vdm_drv.h"
#include "bitplane.h"
#include "vfmw_intf.h"

#define PCTRL_PERI              (0xE8A090A4)

static int gIsNormalInit = 0;

unsigned int     g_TraceOption = (1<<OMX_FATAL) + (1<<OMX_ERR);
static int                   gIsDeviceDetected  = 0;
static struct class         *g_OmxVdecClass     = NULL;
static dev_t                 g_OmxVdecDevNum;
static OMXVDEC_ENTRY         g_OmxVdecEntry;


#ifdef CONFIG_COMPAT //Modified for 64-bit platform
typedef enum {
	T_IOCTL_ARG,
	T_USER_BUFFER,
	T_BUF_MSG,
	T_SM_CONFIG,
	T_BUTT,
} COMPAT_TYPE_E;
#endif

static int omxvdec_setup_cdev(OMXVDEC_ENTRY *omxvdec, const struct file_operations *fops)
{
	int rc;
	struct device *dev;

	g_OmxVdecClass = class_create(THIS_MODULE, "omxvdec_class");
	if (IS_ERR(g_OmxVdecClass)) {
		rc = PTR_ERR(g_OmxVdecClass);
		g_OmxVdecClass = NULL;
		OmxPrint(OMX_FATAL, "%s call class_create failed, rc : %d\n", __func__, rc);
		return rc;
	}

	rc = alloc_chrdev_region(&g_OmxVdecDevNum, 0, 1, "hisi video decoder");
	if (rc) {
		OmxPrint(OMX_FATAL, "%s call alloc_chrdev_region failed, rc : %d\n", __func__, rc);
		goto cls_destroy;
	}

	dev = device_create(g_OmxVdecClass, NULL, g_OmxVdecDevNum, NULL, "hi_vdec");
	if (IS_ERR(dev)) {
		rc = PTR_ERR(dev);
		OmxPrint(OMX_FATAL, "%s call device_create failed, rc : %d\n", __func__, rc);
		goto unregister_region;
	}

	cdev_init(&omxvdec->cdev, fops);
	omxvdec->cdev.owner = THIS_MODULE;
	omxvdec->cdev.ops = fops;
	rc = cdev_add(&omxvdec->cdev, g_OmxVdecDevNum, 1);
	if (rc < 0) {
		OmxPrint(OMX_FATAL, "%s call cdev_add failed, rc : %d\n", __func__, rc);
		goto dev_destroy;
	}

	return 0;

dev_destroy:
	device_destroy(g_OmxVdecClass, g_OmxVdecDevNum);
unregister_region:
	unregister_chrdev_region(g_OmxVdecDevNum, 1);
cls_destroy:
	class_destroy(g_OmxVdecClass);
	g_OmxVdecClass = NULL;

	return rc;
}

static int omxvdec_cleanup_cdev(OMXVDEC_ENTRY *omxvdec)
{
	if (g_OmxVdecClass == NULL) {
		OmxPrint(OMX_FATAL, "%s: invalid g_OmxVdecClass is NULL", __func__);
		return -1;
	} else {
		cdev_del(&omxvdec->cdev);
		device_destroy(g_OmxVdecClass, g_OmxVdecDevNum);
		unregister_chrdev_region(g_OmxVdecDevNum, 1);
		class_destroy(g_OmxVdecClass);
		return 0;
	}
}

static int omxvdec_open(struct inode *inode, struct file *fd)
{
	int ret = -EBUSY;
	OMXVDEC_ENTRY *omxvdec = NULL;
	omxvdec = container_of(inode->i_cdev, OMXVDEC_ENTRY, cdev);
    if(down_interruptible(&omxvdec->omxvdec_mutex)) {
        printk(KERN_CRIT "%s down_interruptible failed\n", __func__);
        return -1;
    }

	if (omxvdec->open_count < MAX_OPEN_COUNT) {
		omxvdec->open_count++;
		if (omxvdec->open_count == 1) {

			VDEC_Regulator_Enable();
			ret = VCTRL_OpenVfmw();
			if (ret != 0) {
				OmxPrint(OMX_FATAL, "%s : VCTRL_OpenVfmw failed\n", __func__);
				goto error0;
			}
			gIsNormalInit = 1;
		}

		fd->private_data = omxvdec;
		ret = 0;
	} else {
		printk(KERN_CRIT "%s open omxvdec instance too much\n", __func__);
		ret = -EBUSY;
	}

	up(&omxvdec->omxvdec_mutex);
	return ret;
error0:
	VDEC_Regulator_Disable();
	omxvdec->open_count--;
	up(&omxvdec->omxvdec_mutex);
	return ret;
}

static int omxvdec_release(struct inode *inode, struct file *fd)
{
	OMXVDEC_ENTRY *omxvdec = NULL;
	omxvdec = fd->private_data;
	if (omxvdec == NULL) {
		printk(KERN_CRIT "%s: invalid omxvdec is null\n", __func__);
		return -EFAULT;
	}

    if(down_interruptible(&omxvdec->omxvdec_mutex))  {
        printk(KERN_CRIT "%s down_interruptible failed\n", __func__);
        return -1;
    }

	if (fd->private_data == NULL) {
		OmxPrint(OMX_FATAL, "%s: invalid fd->private_data is null\n", __func__);
		up(&omxvdec->omxvdec_mutex);
		return -EFAULT;
	}

	if (omxvdec->open_count > 0)
		omxvdec->open_count--;

	if (omxvdec->open_count == 0) {
		VCTRL_CloseVfmw();
		VDEC_Regulator_Disable();
		gIsNormalInit = 0;
	}

	fd->private_data = NULL;
	printk(KERN_INFO "exit %s , open_count : %d\n", __func__, omxvdec->open_count);
	up(&omxvdec->omxvdec_mutex);
	return 0;
}

#ifdef CONFIG_COMPAT   //Modified for 64-bit platform
static int omxvdec_compat_get_data(COMPAT_TYPE_E eType, void __user *pUser, void *pData)
{
	int ret = 0;
	int s32Data = 0;
	compat_ulong_t CompatData = 0;
	if (NULL == pUser || NULL == pData) {
		OmxPrint(OMX_FATAL, "%s: invalid param is null\n", __func__);
		return -1;
	}

	switch (eType) {
	case T_IOCTL_ARG:{
		COMPAT_IOCTL_MSG __user *pCompatMsg = pUser;
		OMXVDEC_IOCTL_MSG       *pIoctlMsg  = pData;
		ret |= get_user(s32Data, &(pCompatMsg->chan_num));
		pIoctlMsg->chan_num = s32Data;
		ret |= get_user(CompatData, &(pCompatMsg->in));
		pIoctlMsg->in   = (void *) ((unsigned long)CompatData);
		ret |= get_user(CompatData, &(pCompatMsg->out));
		pIoctlMsg->out = (void *) ((unsigned long)CompatData);
	}
		break;

	case T_USER_BUFFER: {
		COMPAT_BUF_DESC __user *pCompatBuf  = pUser;
		OMXVDEC_BUF_DESC       *pIoctlBuf   = pData;

		ret |= copy_from_user(pIoctlBuf, pCompatBuf, sizeof(*pCompatBuf));
		ret |= get_user(CompatData, &(pCompatBuf->bufferaddr));
		pIoctlBuf->bufferaddr = (void *) ((unsigned long)CompatData);
		ret |= get_user(CompatData, &(pCompatBuf->client_data));
		pIoctlBuf->client_data = (void *) ((unsigned long)CompatData);
	}
		break;

	case T_SM_CONFIG: {
		COMPAT_OMXSCD_REG_CFG_S __user *pCompatCfg = pUser;
		OMXSCD_REG_CFG_S               *pIoctlCfg  = pData;
		ret |= copy_from_user(pIoctlCfg, pCompatCfg, sizeof(*pCompatCfg));
		ret |= get_user(CompatData, &(pCompatCfg->IoctlSmCtrlReg.pDownMsgVirAddr));
		pIoctlCfg->SmCtrlReg.pDownMsgVirAddr = ( int *) ((unsigned long)CompatData);
		ret |= get_user(CompatData, &(pCompatCfg->IoctlSmCtrlReg.pUpMsgVirAddr));
		pIoctlCfg->SmCtrlReg.pUpMsgVirAddr = ( int *) ((unsigned long)CompatData);
		ret |= get_user(CompatData, &(pCompatCfg->IoctlSmCtrlReg.pDspSpsMsgMemVirAddr));
		pIoctlCfg->SmCtrlReg.pDspSpsMsgMemVirAddr = ( int *) ((unsigned long)CompatData);
		ret |= get_user(CompatData, &(pCompatCfg->IoctlSmCtrlReg.pDspPpsMsgMemVirAddr));
		pIoctlCfg->SmCtrlReg.pDspPpsMsgMemVirAddr = ( int *) ((unsigned long)CompatData);
		ret |= get_user(CompatData, &(pCompatCfg->IoctlSmCtrlReg.pDvmMemVirAddr));
		pIoctlCfg->SmCtrlReg.pDvmMemVirAddr = ( int *) ((unsigned long)CompatData);
		ret |= get_user(CompatData, &(pCompatCfg->IoctlSmCtrlReg.pDspSedTopMemVirAddr));
		pIoctlCfg->SmCtrlReg.pDspSedTopMemVirAddr = ( int *) ((unsigned long)CompatData);
		ret |= get_user(CompatData, &(pCompatCfg->IoctlSmCtrlReg.pDspCaMnMemVirAddr));
		pIoctlCfg->SmCtrlReg.pDspCaMnMemVirAddr = ( int *) ((unsigned long)CompatData);
	}
		break;

	default:
		OmxPrint(OMX_FATAL, "%s: unkown type %d\n", __func__, eType);
		ret = -1;
		break;
	}

	return ret;
}
#endif

static long omxvdec_compat_ioctl(struct file *fd, unsigned int code, unsigned long arg)
{
	int ret;
	OMXVDEC_IOCTL_MSG  vdec_msg;
	OMXVDEC_ENTRY     *omxvdec = fd->private_data;
	CLK_RATE_E        clk_rate;
	OMXSCD_REG_CFG_S  scd_reg_cfg;
	SCD_STATE_REG_S   scd_state_reg;
	OMXVDH_REG_CFG_S  vdm_reg_cfg;
	VDMHAL_BACKUP_S   vdm_state_reg;
	OMXBPD_REG_S      bpd_reg;
	int            vdm_is_run;
	if (!gIsNormalInit)
		return -EIO;

	if (VCTRL_Scen_Ident()) {
		OmxPrint(OMX_ALWS, "xxx mode\n");
		return -EIO;
	}
    
#ifdef CONFIG_COMPAT
	ret = omxvdec_compat_get_data(T_IOCTL_ARG, compat_ptr(arg), &vdec_msg);
	if (ret != 0) {
		OmxPrint(OMX_FATAL, "%s: T_IOCTL_MSG get data failed\n", __func__);
		return -EIO;
	}
#else
 	if (copy_from_user(&vdec_msg, (void *)arg, sizeof(vdec_msg))) {
		OmxPrint(OMX_FATAL, "%s call copy_from_user failed \n", __func__);
		return -EFAULT;
	}   
#endif

	switch (code) {
	case VDEC_IOCTL_GET_OPEN_COUNT:
		if (NULL == omxvdec) {
			OmxPrint(OMX_FATAL, "%s: invalid omxvdec is null\n", __func__);
			return -1;
		}

        if(down_interruptible(&omxvdec->omxvdec_mutex))
        {
            printk(KERN_CRIT "%s down_interruptible failed\n", __func__);
            return -1;
        }

		ret = copy_to_user(vdec_msg.out, &(omxvdec->open_count), sizeof(omxvdec->open_count));
		if (ret != 0) {
			OmxPrint(OMX_FATAL, "VDEC_IOCTL_GET_OPEN_COUNT :  copy_to_user failed\n");
			up(&omxvdec->omxvdec_mutex);
			return -EIO;
		}

		up(&omxvdec->omxvdec_mutex);
		break;

	case VDEC_IOCTL_SET_CLK_RATE:
		ret = copy_from_user(&clk_rate, vdec_msg.in, sizeof(clk_rate));
		if (ret != 0) {
			OmxPrint(OMX_FATAL, "VDEC_IOCTL_SET_CLK_RATE : copy_from_user failed\n");
			return -EIO;
		}

		if (VDEC_Regulator_SetClkRate(clk_rate , get_dts_config()->IsFPGA) != 0) {
			OmxPrint(OMX_FATAL, "VDEC_Regulator_SetClkRate faied\n");
			return -1;
		}
        
		break;

	case VDEC_IOCTL_GET_CLK_RATE:
		if (VDEC_Regulator_GetClkRate(&clk_rate) == 0) {
			ret = copy_to_user(vdec_msg.out, &clk_rate, sizeof(clk_rate));
			if (ret != 0) {
				OmxPrint(OMX_FATAL, "VDEC_IOCTL_GET_CLK_RATE: copy_to_user failed\n");
				return -EIO;
			}
		} else {
			OmxPrint(OMX_FATAL, "VDEC_Regulator_GetClkRate faied\n");
			return -1;
		}
        
		break;

	case VDEC_IOCTL_VDM_PROC:
        if(down_interruptible(&omxvdec->vdec_mutex_vdh)) {
            printk(KERN_CRIT "%s down_interruptible failed\n", __func__);
            return -1;
        }

		ret = copy_from_user(&vdm_reg_cfg, vdec_msg.in, sizeof(vdm_reg_cfg));
		if (ret != 0) {
			OmxPrint(OMX_FATAL, "VDEC_IOCTL_VDM_PROC : copy_from_user failed\n");
			up(&omxvdec->vdec_mutex_vdh);
			return -EIO;
		}

		ret = VCTRL_VDMHal_Process(&vdm_reg_cfg, &vdm_state_reg);
		if (ret != 0) {
			OmxPrint(OMX_FATAL, " VCTRL_VDMHal_Process failed\n");
			up(&omxvdec->vdec_mutex_vdh);
			return -EIO;
		}

		ret = copy_to_user(vdec_msg.out, &vdm_state_reg, sizeof(vdm_state_reg));
		if (ret != 0) {
			OmxPrint(OMX_FATAL, "VDEC_IOCTL_VDM_PROC : copy_to_user failed\n");
			up(&omxvdec->vdec_mutex_vdh);
			return -EIO;
		}
		up(&omxvdec->vdec_mutex_vdh);
		break;

	case VDEC_IOCTL_GET_VDM_HWSTATE:
        if(down_interruptible(&omxvdec->vdec_mutex_vdh))  {
            printk(KERN_CRIT "%s down_interruptible failed\n", __func__);
            return -1;
        }

		vdm_is_run = VCTRL_VDMHAL_IsRun();
		ret = copy_to_user(vdec_msg.out, &vdm_is_run, sizeof(vdm_is_run));
		if (ret != 0) {
			OmxPrint(OMX_FATAL, "VDEC_IOCTL_GET_VDM_HWSTATE copy_to_user failed\n");
			up(&omxvdec->vdec_mutex_vdh);
			return -EIO;
		}
		up(&omxvdec->vdec_mutex_vdh);
		break;
        
    case VDEC_IOCTL_SCD_PROC:
        if(down_interruptible(&omxvdec->vdec_mutex_scd))  {
            printk(KERN_CRIT "%s down_interruptible failed\n", __func__);
            return -1;
        }

#ifdef CONFIG_COMPAT
		ret = omxvdec_compat_get_data(T_SM_CONFIG, vdec_msg.in, &scd_reg_cfg);
#else        
        ret = copy_from_user(&scd_reg_cfg, vdec_msg.in, sizeof(scd_reg_cfg));
#endif        
		if (ret != 0) {
			OmxPrint(OMX_FATAL, "VDEC_IOCTL_SCD_PROC :  omxvdec_compat_get_data failed\n");
			up(&omxvdec->vdec_mutex_scd);
			return -EIO;
		}

		ret = VCTRL_SCDHal_Process(&scd_reg_cfg, &scd_state_reg);
		if (ret != 0) {
			OmxPrint(OMX_FATAL, "omxvdec_SMHal_Process failed\n");
			up(&omxvdec->vdec_mutex_scd);
			return -EIO;
		}

		ret = copy_to_user(vdec_msg.out, &scd_state_reg, sizeof(scd_state_reg));
		if (ret != 0) {
			OmxPrint(OMX_FATAL, "VDEC_IOCTL_SCD_PROC : copy_to_user failed\n");
			up(&omxvdec->vdec_mutex_scd);
			return -EIO;
		}
		up(&omxvdec->vdec_mutex_scd);
		break;

	case VDEC_IOCTL_SET_BPD_PROC:
        if(down_interruptible(&omxvdec->vdec_mutex_bpd))  {
            printk(KERN_CRIT "%s down_interruptible failed\n", __func__);
            return -1;
        }

		ret = copy_from_user(&bpd_reg, vdec_msg.in, sizeof(bpd_reg));
		if (ret != 0) {
			OmxPrint(OMX_FATAL, "VDEC_IOCTL_SET_BPD_PROC : copy_from_user failed\n");
			up(&omxvdec->vdec_mutex_bpd);
			return -EIO;
		}

		ret = VCTRL_BPDHal_Process(&bpd_reg);
		if (ret != 0) {
			OmxPrint(OMX_FATAL, "VCTRL_BPDHal_Process failed\n");
			up(&omxvdec->vdec_mutex_bpd);
			return -EIO;
		}

		ret = copy_to_user(vdec_msg.out, &bpd_reg, sizeof(bpd_reg));
		if (ret != 0) {
			OmxPrint(OMX_FATAL, "VDEC_IOCTL_SET_BPD_PROC :  copy_to_user failed\n");
			up(&omxvdec->vdec_mutex_bpd);
			return -EIO;
		}
        
		up(&omxvdec->vdec_mutex_bpd);        
		break;
        
	case VDEC_IOCTL_PROC_CMD:    
        break ;
	default:/* could not handle ioctl */
		OmxPrint(OMX_FATAL, "%s %d: ERROR cmd : %d is not supported\n", __func__, __LINE__, _IOC_NR(code));
		return -ENOTTY;
	}
	return 0;
}

static const struct file_operations omxvdec_fops = {
	.owner          = THIS_MODULE,
	.open           = omxvdec_open,
	.unlocked_ioctl = omxvdec_compat_ioctl,
	.compat_ioctl   = omxvdec_compat_ioctl,
	.release        = omxvdec_release,
};

static int omxvdec_probe(struct platform_device *pltdev)
{
	int ret;
	int dev_exi        = 0;
	unsigned int *viraddr_pctrl = NULL;
	viraddr_pctrl         = (unsigned int *) ioremap(PCTRL_PERI, 4);
	if (viraddr_pctrl) {
		dev_exi = readl(viraddr_pctrl) & 0x4;
		iounmap(viraddr_pctrl);

		if (dev_exi == 0) {
			printk(KERN_ERR "vdec is not exist\n");
			return -1;
		}
	} else {
		printk(KERN_ERR "ioremap failed\n");
		return -1;
	}

	if (gIsDeviceDetected == 1) {
		OmxPrint(OMX_INFO, "Already probe omxvdec\n");
		return 0;
	}

	platform_set_drvdata(pltdev, NULL);

	memset(&g_OmxVdecEntry, 0, sizeof(OMXVDEC_ENTRY));
	sema_init(&g_OmxVdecEntry.omxvdec_mutex, 1);      
	sema_init(&g_OmxVdecEntry.vdec_mutex_scd, 1);      
	sema_init(&g_OmxVdecEntry.vdec_mutex_bpd, 1);      
	sema_init(&g_OmxVdecEntry.vdec_mutex_vdh, 1);      

	ret = omxvdec_setup_cdev(&g_OmxVdecEntry, &omxvdec_fops);
	if (ret < 0) {
		printk(KERN_CRIT "%s call omxvdec_setup_cdev failed\n", __func__);
		goto cleanup0;
	}

	ret = VDEC_Regulator_Probe(&pltdev->dev , get_dts_config());
	if (ret != 0) {
		printk(KERN_CRIT "%s call Regulator_Initialize failed\n", __func__);
		goto cleanup1;
	}

	g_OmxVdecEntry.device = &pltdev->dev;
	platform_set_drvdata(pltdev, &g_OmxVdecEntry);
	gIsDeviceDetected = 1;

	return 0;

cleanup1:
	omxvdec_cleanup_cdev(&g_OmxVdecEntry);

cleanup0:
	return ret;
}

static int omxvdec_remove(struct platform_device *pltdev)
{
	OMXVDEC_ENTRY *omxvdec = NULL;
	omxvdec = platform_get_drvdata(pltdev);
	if (omxvdec != NULL) {
		if (IS_ERR(omxvdec)) {
			OmxPrint(OMX_ERR, "call platform_get_drvdata err, errno : %ld\n", PTR_ERR(omxvdec));
		} else {
			omxvdec_cleanup_cdev(omxvdec);
			VDEC_Regulator_Remove(&pltdev->dev);
			platform_set_drvdata(pltdev, NULL);
			gIsDeviceDetected = 0;
		}
	}

	return 0;
}

static int omxvdec_suspend(struct platform_device *pltdev, pm_message_t state)
{
	 int ret;
	OmxPrint(OMX_ALWS, "%s enter\n", __func__);

	if (gIsNormalInit != 0)
		VCTRL_Suspend();

	ret = VDEC_Regulator_Disable();
	if (ret != 0)
		OmxPrint(OMX_FATAL, "%s disable regulator failed\n", __func__);

	OmxPrint(OMX_ALWS, "%s success\n", __func__);

	return 0;
}

static int omxvdec_resume(struct platform_device *pltdev)
{
	 int ret;
	OmxPrint(OMX_ALWS, "%s enter\n", __func__);

	if (gIsNormalInit != 0) {
		ret = VDEC_Regulator_Enable();
		if (ret != 0) {
			OmxPrint(OMX_FATAL, "%s enable regulator failed\n", __func__);
			return -1;
		}
		VCTRL_Resume();
	}

	OmxPrint(OMX_ALWS, "%s success\n", __func__);
	return 0;
}

static void omxvdec_device_release(struct device *dev)
{}

static const struct of_device_id Hisi_Vdec_Match_Table[] = { 
	{.compatible = "hisi,kirin960-vdec",},
	{ }
};

static struct platform_driver omxvdec_driver = {

	.probe   = omxvdec_probe,
	.remove  = omxvdec_remove,
	.suspend = omxvdec_suspend,
	.resume  = omxvdec_resume,
	.driver  = {
		.name  = "hi_vdec",
		.owner = THIS_MODULE,
		.of_match_table = Hisi_Vdec_Match_Table
	},
};

static struct platform_device omxvdec_device = {

	.name = "hi_vdec",
	.id   = -1,
	.dev  = {
		.platform_data = NULL,
		.release       = omxvdec_device_release,
	},
};

int __init OMXVDEC_DRV_ModInit(void)
{
	int ret;

	ret = platform_device_register(&omxvdec_device);
	if (ret < 0) {
		OmxPrint(OMX_FATAL, "%s call platform_device_register failed\n", __func__);
		return ret;
	}

	ret = platform_driver_register(&omxvdec_driver);
	if (ret < 0) {
		OmxPrint(OMX_FATAL, "%s call platform_driver_register failed\n", __func__);
		goto exit;
	}
#ifdef MODULE
	OmxPrint(OMX_ALWS, "Load hi_omxvdec.ko :%d success\n", OMXVDEC_VERSION);
#endif

	return 0;
exit:
	platform_device_unregister(&omxvdec_device);

	return ret;
}

void __exit OMXVDEC_DRV_ModExit(void)
{
	platform_driver_unregister(&omxvdec_driver);
	platform_device_unregister(&omxvdec_device);

#ifdef MODULE
	OmxPrint(OMX_ALWS, "Unload hi_omxvdec.ko : %d success\n", OMXVDEC_VERSION);
#endif

}

module_init(OMXVDEC_DRV_ModInit);
module_exit(OMXVDEC_DRV_ModExit);

MODULE_AUTHOR("gaoyajun@hisilicon.com");
MODULE_DESCRIPTION("vdec driver");
MODULE_LICENSE("GPL");
