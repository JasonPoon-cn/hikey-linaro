/*
 *      Video Encode driver for hikey960   venc driver
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

#include <linux/device.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/hisi/hisi-iommu.h> 

#include "drv_venc.h"

#define VERSION_STRING         "1234"
#define IOC_TYPE_VENC	       'V'
#define SMMU_RWERRADDR_SIZE    (128)

#define CMD_VENC_GET_VENCCOUNT         _IOR(IOC_TYPE_VENC, 0x31, atomic_t)
#define CMD_VENC_START_ENCODE          _IOWR(IOC_TYPE_VENC, 0x32, VENC_REG_INFO_S)

#define PCTRL_PERI 0xE8A090A4

#ifndef VM_RESERVED	/*for kernel up to 3.7.0 version*/
# define  VM_RESERVED   (VM_DONTEXPAND | VM_DONTDUMP)
#endif


/*============Deviece===============*/
typedef struct {
	dev_t dev;
	struct device* venc_device;
	struct device* venc_device_2;
	struct cdev cdev;
	struct class* venc_class;
}VENC_ENTRY;

struct semaphore g_VencMutex;

static int g_vencOpenFlag    = 0;
static int g_vencDevDetected = 0;

//VENC device open times
static  atomic_t g_VencCount = ATOMIC_INIT(0);

 int VENC_DRV_Resume(struct platform_device *pltdev);
 int VENC_DRV_Suspend(struct platform_device *pltdev, pm_message_t state);

static  int  VENC_DRV_SetupCdev(VENC_ENTRY *venc, const struct file_operations *fops);
static  int  VENC_DRV_CleanupCdev(VENC_ENTRY *venc);
static  int  VENC_DRV_Probe(struct platform_device * pltdev);
static  int  VENC_DRV_Remove(struct platform_device *pltdev);
static void VENC_DRV_DeviceRelease1(struct device* dev);

static vedu_dts_ctx          g_vedu_dts_ctx;
static U_VEDU_RAWINT_NS      g_hw_done_type;
static VEDU_OSAL_EVENT       g_hw_done_event;

static void Venc_ISR( void )
{
	unsigned int *pINTCLR     = NULL;
	S_HEVC_AVC_REGS_TYPE *pAllReg = NULL;

	HI_DBG_VENC("enter %s ()\n", __func__);

	if (!g_vedu_dts_ctx.pRegBase) {
		HI_ERR_VENC("%s, g_vedu_dts_ctx.pRegBase invalid", __func__);
		return ;
	}
	pAllReg  = (S_HEVC_AVC_REGS_TYPE *)g_vedu_dts_ctx.pRegBase;
	pINTCLR  = (unsigned int *)&(pAllReg->VEDU_VCPI_REGS.VEDU_INTCLR_NS.u32);

	g_hw_done_type.bits.rint_vedu_timeout_ns = pAllReg->VEDU_VCPI_REGS.VEDU_RAWINT_NS.bits.rint_vedu_timeout_ns;
	g_hw_done_type.bits.rint_vedu_slice_end_ns = pAllReg->VEDU_VCPI_REGS.VEDU_RAWINT_NS.bits.rint_vedu_slice_end_ns;
	g_hw_done_type.bits.rint_ve_eop_ns = pAllReg->VEDU_VCPI_REGS.VEDU_RAWINT_NS.bits.rint_ve_eop_ns;

#ifdef VENC_SIMULATE
	pAllReg->VEDU_VCPI_REGS.VEDU_RAWINT_NS.bits.rint_ve_eop = 0;
#endif
	if (g_hw_done_type.bits.rint_vedu_timeout_ns)
	{
		*pINTCLR = 0x08000000;
        g_hw_done_event.flag = 1;
	    wake_up(&(g_hw_done_event.queue_head));
	}
    
	if (g_hw_done_type.bits.rint_vedu_slice_end_ns)
		*pINTCLR = 0x400;
	if (g_hw_done_type.bits.rint_ve_eop_ns)
	{
		*pINTCLR = 0xFFFFFFFF;
        g_hw_done_event.flag = 1;
	    wake_up(&(g_hw_done_event.queue_head));
	}
}

 static  int venc_drv_waithwdone(U_VEDU_RAWINT_NS *hw_done_type)
 {
	int Ret = (-1);
    unsigned int msWaitTime = msecs_to_jiffies(1000) ;
	if (msWaitTime != 0xffffffff) {
		Ret = wait_event_interruptible_timeout(g_hw_done_event.queue_head, (g_hw_done_event.flag != 0), (msWaitTime)); 
		g_hw_done_event.flag = 0;
		Ret = (Ret != 0) ? 0 : (-1);
	} else {
		Ret = wait_event_interruptible( g_hw_done_event.queue_head, (g_hw_done_event.flag != 0));
		g_hw_done_event.flag = 0;
		Ret = (Ret == 0) ? 0 : (-1);
	}

	if (Ret != 0) {
		hw_done_type->u32 = 0;
		g_vedu_dts_ctx.IpFree = 0;
		HI_ERR_VENC("%s,wait timeout,Ret = %d\n", __func__, Ret);
		return Ret;
	}

	*hw_done_type = g_hw_done_type;
	return Ret;
 }

static  int  venc_drv_getvenccount(atomic_t  *venc_count)
{
	memcpy((void *)venc_count, (void *)(&g_VencCount), sizeof(atomic_t));
	return 0;
}

static  int venc_drv_register_info(VENC_REG_INFO_S *regcfginfo)
{
	int Ret = 0;
	CMD_TYPE cmd  = regcfginfo->cmd;
	switch (cmd) {
	case VENC_SET_CFGREG:
		HI_DBG_VENC("%s, bResetReg : %d ,bHighSpeed : %d ,bClkCfg : %d \n", __func__, regcfginfo->bResetReg , regcfginfo->bHighSpeed , regcfginfo->bClkCfg);
		if (regcfginfo->bResetReg == 1)
		{
            Ret = Venc_Regulator_Disable(0);
	        Ret |= Venc_Regulator_Enable(&g_vedu_dts_ctx);
			if (Ret != 0)
			{
				HI_ERR_VENC("%s, CMD_VENC_RESETREG Ret:%d\n", __func__, Ret);
				break;
			}
		}

		if (regcfginfo->bClkCfg == 1)
		{
			Ret = Venc_SetClkRate(regcfginfo->bHighSpeed , &g_vedu_dts_ctx);
			if(Ret != 0)
			{
				HI_ERR_VENC("%s, CMD_VENC_SETRATE Ret:%d\n", __func__, Ret);
				break;
			}
		}

		VeduHal_CfgReg(regcfginfo , (S_HEVC_AVC_REGS_TYPE*)(g_vedu_dts_ctx.pRegBase) , g_vedu_dts_ctx.RdAddr , g_vedu_dts_ctx.WrAddr , g_vedu_dts_ctx.phy_pgd_base);
		VENC_HAL_StartEncode((S_HEVC_AVC_REGS_TYPE*)(g_vedu_dts_ctx.pRegBase));
		Ret = venc_drv_waithwdone(&regcfginfo->hw_done_type) ;
		if((Ret == 0 ) && (!regcfginfo->hw_done_type.bits.rint_vedu_timeout_ns))
		{
			VENC_HAL_Get_Reg_Venc(regcfginfo , (S_HEVC_AVC_REGS_TYPE*)(g_vedu_dts_ctx.pRegBase));
			HI_DBG_VENC("%s,  VENC_HAL_Get_Reg_Venc\n", __func__);
		}

		break;
	case VENC_SET_CFGREGSIMPLE:
		VeduHal_CfgRegSimple(regcfginfo , (S_HEVC_AVC_REGS_TYPE*)(g_vedu_dts_ctx.pRegBase));
		VENC_HAL_StartEncode((S_HEVC_AVC_REGS_TYPE*)(g_vedu_dts_ctx.pRegBase));
		Ret = venc_drv_waithwdone(&regcfginfo->hw_done_type) ;
		if((Ret == 0 ) && (!regcfginfo->hw_done_type.bits.rint_vedu_timeout_ns))
		{
			VENC_HAL_Get_Reg_Venc(regcfginfo , (S_HEVC_AVC_REGS_TYPE*)(g_vedu_dts_ctx.pRegBase));
			HI_DBG_VENC("%s,  VENC_HAL_Get_Reg_Venc\n", __func__);
		}
		break;
	default:
		HI_ERR_VENC(" cmd type unknown:%d\n", cmd);
		Ret = (-1);
		break;
	}
	return Ret;
}

static irqreturn_t VENC_DRV_irq( int Irq, void* DevID)
{
	Venc_ISR();
	return IRQ_HANDLED;    
}

static int VENC_DRV_EflOpenVedu(vedu_dts_ctx *pg_vedu_dts_ctx)
{
	int s32Ret = 0;
	MEM_BUFFER_S  MEM_SMMU_START_ADDR0;
	MEM_BUFFER_S  MEM_SMMU_START_ADDR1;
	HI_DBG_VENC("enter %s()\n", __func__);

	pg_vedu_dts_ctx->pRegBase = (unsigned int *)ioremap(pg_vedu_dts_ctx->VencRegBaseAddr, pg_vedu_dts_ctx->VencRegRange);
	if (!pg_vedu_dts_ctx->pRegBase){
		HI_ERR_VENC("%s, ioremap failed\n", __func__);
		return (-1);
	}

	HI_DBG_VENC("%s, HI_DDR_MEM_Init\n", __func__);
	if (0 != DRV_MEM_INIT()) {
		HI_ERR_VENC("DRV_MEM_INIT failed\n");
		iounmap(pg_vedu_dts_ctx->pRegBase);
		return (-1);
	}

	memset((void *)&MEM_SMMU_START_ADDR0, 0, sizeof(MEM_BUFFER_S));
	memset((void *)&MEM_SMMU_START_ADDR1, 0, sizeof(MEM_BUFFER_S));

	MEM_SMMU_START_ADDR0.u32Size = SMMU_RWERRADDR_SIZE;
	s32Ret = DRV_MEM_KAlloc("SMMU_RDERR", "OMXVENC", &MEM_SMMU_START_ADDR0);
	if (s32Ret != 0 ) {
		iounmap(pg_vedu_dts_ctx->pRegBase);
		HI_ERR_VENC("%s, call DRV_MEM_KAlloc SMMU_RDERR Mem failed\n", __func__);
		return (-1);
	}

	MEM_SMMU_START_ADDR1.u32Size = SMMU_RWERRADDR_SIZE;
	s32Ret = DRV_MEM_KAlloc("SMMU_WRERR", "OMXVENC", &MEM_SMMU_START_ADDR1);
	if (s32Ret != 0 ) {
		iounmap(pg_vedu_dts_ctx->pRegBase);
		DRV_MEM_KFree(&MEM_SMMU_START_ADDR0);
		HI_ERR_VENC("%s, DRV_MEM_KAlloc  MMU Addr failed\n", __func__);
		return (-1);
	}

	pg_vedu_dts_ctx->RdAddr = MEM_SMMU_START_ADDR0.u32StartPhyAddr;
	pg_vedu_dts_ctx->WrAddr = MEM_SMMU_START_ADDR1.u32StartPhyAddr;

	VENC_HAL_SetSmmuAddr((S_HEVC_AVC_REGS_TYPE*)(pg_vedu_dts_ctx->pRegBase) , pg_vedu_dts_ctx->RdAddr , pg_vedu_dts_ctx->WrAddr);
	VENC_HAL_DisableAllInt((S_HEVC_AVC_REGS_TYPE*)(pg_vedu_dts_ctx->pRegBase));
	VENC_HAL_ClrAllInt    ((S_HEVC_AVC_REGS_TYPE*)(pg_vedu_dts_ctx->pRegBase));
    g_hw_done_event.flag = 0;
	init_waitqueue_head(&(g_hw_done_event.queue_head));
#ifdef IRQ_EN
	if (request_irq(pg_vedu_dts_ctx->VeduIrqNum , VENC_DRV_irq , 0, "DT_device", NULL) == (-1)){
		HI_ERR_VENC("%s, request_irq failed\n", __func__);
		iounmap(pg_vedu_dts_ctx->pRegBase);
		DRV_MEM_KFree(&MEM_SMMU_START_ADDR0);
		DRV_MEM_KFree(&MEM_SMMU_START_ADDR1);
		return (-1);
	}
#endif
	/* creat thread to manage channel */
	pg_vedu_dts_ctx->StopTask    = 0;
	pg_vedu_dts_ctx->TaskRunning = 0;

	HI_DBG_VENC("exit %s()\n", __func__);
	return 0;
}

static  int VENC_DRV_Open(struct inode *finode, struct file  *ffile)
{
	int Ret = 0;
	HI_DBG_VENC("enter %s()\n", __func__);
	Ret = down_interruptible(&g_VencMutex);
	if (Ret == 0) {
		HI_DBG_VENC("down_interruptible success\n");
	}
	else {
		HI_FATAL_VENC("down_interruptible failed\n");
		return (-1);
	}

	if (atomic_inc_return(&g_VencCount) == 1) {
		Ret = Venc_Regulator_Enable(&g_vedu_dts_ctx);
		if (Ret != 0) {
			HI_FATAL_VENC("%s, Venc_Regulator_Enable failed, ret=%d\n", __func__, Ret);
			atomic_dec(&g_VencCount);
			up(&g_VencMutex);
			return (-1);
		}
		Ret = VENC_DRV_EflOpenVedu(&g_vedu_dts_ctx);
		if (Ret != 0) {
			HI_FATAL_VENC("%s, VeduEfl_OpenVedu failed, ret=%d\n", __func__, Ret);
			atomic_dec(&g_VencCount);
			Venc_Regulator_Disable(1);
			up(&g_VencMutex);
			return (-1);
		}
		g_vedu_dts_ctx.IpFree = 1;
	}

	g_vencOpenFlag = 1;
	up(&g_VencMutex);

	HI_INFO_VENC("open venc device successfull\n");
	return 0;
}

static  int VENC_DRV_Close(struct inode *finode, struct file  *ffile)
{
	 int Ret = 0;
	unsigned int TimeOutCnt = 0;

	HI_DBG_VENC("enter %s()\n", __func__);
	Ret = down_interruptible(&g_VencMutex);
	if (atomic_dec_and_test(&g_VencCount)) {
        g_vedu_dts_ctx.StopTask = 1;
        while ((g_vedu_dts_ctx.TaskRunning) && (TimeOutCnt < 100)) {
            msleep(1);
            TimeOutCnt ++;
        }

        VENC_HAL_DisableAllInt((S_HEVC_AVC_REGS_TYPE*)(g_vedu_dts_ctx.pRegBase));
        VENC_HAL_ClrAllInt((S_HEVC_AVC_REGS_TYPE*)(g_vedu_dts_ctx.pRegBase));

#ifdef IRQ_EN
        free_irq(g_vedu_dts_ctx.VeduIrqNum , NULL);
#endif

        iounmap(g_vedu_dts_ctx.pRegBase);
        DRV_MEM_EXIT();
		Venc_Regulator_Disable(1);
		g_vedu_dts_ctx.IpFree = 0;
		g_vencOpenFlag = 0;
	}

	up(&g_VencMutex);

	HI_INFO_VENC("close venc device success\n");
	return 0;
}

 int VENC_DRV_Suspend(struct platform_device *pltdev, pm_message_t state)
{
	 int Ret = 0;
	unsigned int TimeOutCnt = 0;
	HI_INFO_VENC("enter\n");

	Ret = down_interruptible(&g_VencMutex);
	g_vedu_dts_ctx.IpFree = 0;
	if (!g_vencOpenFlag) {
		HI_INFO_VENC("venc device already suspend\n");
		up(&g_VencMutex);   // 
		return 0;
	}

	g_vedu_dts_ctx.StopTask = 1;

	while ((g_vedu_dts_ctx.TaskRunning) && (TimeOutCnt < 100)) {
		msleep(1);
		++TimeOutCnt ;
	}

	g_hw_done_event.flag = 0;
	up(&g_VencMutex);

	HI_INFO_VENC("exit\n");
	return 0;
}/*lint !e715*/

 int VENC_DRV_Resume(struct platform_device *pltdev)
{
	 int Ret = 0;

	HI_INFO_VENC("enter \n");

	Ret = down_interruptible(&g_VencMutex);
	if (!g_vencOpenFlag) {
		HI_INFO_VENC("venc device already resume\n");
		up(&g_VencMutex);   // add 
		return 0;
	}

	g_vedu_dts_ctx.StopTask    = 0;
	g_vedu_dts_ctx.TaskRunning = 0;

	g_vedu_dts_ctx.IpFree = 1;
	up(&g_VencMutex);
	HI_INFO_VENC("exit \n");
	return 0;
}/*lint !e715*/

static long VENC_Ioctl(struct file *file, unsigned int ucmd, unsigned long uarg)
{
	 int  Ret;
	 int  cmd  = ( int)ucmd;
	void *arg = (void *)uarg;

	atomic_t *venc_count        = NULL;
	VENC_REG_INFO_S *regcfginfo =  NULL;

	if (g_vedu_dts_ctx.IpFree == 0) {
		HI_ERR_VENC("%s, g_vedu_dts_ctx.IpFree is 0\n", __func__);
		return (-1);
	}

	if (!arg) {
		HI_ERR_VENC("%s, uarg is NULL\n", __func__);
		return (-1);
	}

	switch (cmd) {
	case CMD_VENC_GET_VENCCOUNT:
		venc_count = (atomic_t *)arg;
		Ret = venc_drv_getvenccount(venc_count);
		HI_DBG_VENC("%s, CMD_VENC_GET_VENCCOUNT Ret:%d\n", __func__, Ret);
		break;
	case CMD_VENC_START_ENCODE:
		g_vedu_dts_ctx.TaskRunning = 1;
		regcfginfo = (VENC_REG_INFO_S *)arg;
		Ret = venc_drv_register_info(regcfginfo);
		g_vedu_dts_ctx.TaskRunning = 0;
		HI_DBG_VENC("%s, CMD_VENC_CFGREGINFO Ret:%d\n", __func__, Ret);
		break;
	default:
		HI_ERR_VENC("venc cmd unknown:%x\n", ucmd);
		Ret = (-1);
		break;
	}

	return Ret;
}
static long VENC_DRV_Ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	long Ret;

	Ret = down_interruptible(&g_VencMutex);
	if (Ret != 0)
	{
		HI_FATAL_VENC("down_interruptible failed\n");
		return Ret;
	}
	Ret = (long)HI_DRV_UserCopy(file, cmd, arg, VENC_Ioctl);
	up(&g_VencMutex);
	return Ret;
}

static struct file_operations VENC_FOPS =
{
	.owner          = THIS_MODULE,
	.open           = VENC_DRV_Open,
	.unlocked_ioctl = VENC_DRV_Ioctl,
	.compat_ioctl   = VENC_DRV_Ioctl,  //�û�̬ ���ں�̬ƽ̨λ��һ�µ�ʱ�򣬻����
	.release        = VENC_DRV_Close,
	//.mmap 		= VENC_DRV_MMap,   // reserve
};

static const struct of_device_id venc_of_match[] = {
	{ .compatible = "hisi,kirin960-venc", },
	{ }
};

static struct platform_driver Venc_driver = {
	.probe   = VENC_DRV_Probe,
	.remove  = VENC_DRV_Remove,
	.suspend = VENC_DRV_Suspend,
	.resume  = VENC_DRV_Resume,
	.driver  = {
		.name           = "hi_venc",
		.owner          = THIS_MODULE,
		.of_match_table = venc_of_match
	},
};

static struct platform_device Venc_device = {
	.name = "hi_venc",
	.id   = -1,
	.dev  = {
		.platform_data = NULL,
		.release       = VENC_DRV_DeviceRelease1,
	},
};

static  int VENC_DRV_SetupCdev(VENC_ENTRY *venc, const struct file_operations *fops)
{
	 int err = 0;

	HI_INFO_VENC("enter %s()\n", __func__);
	err = alloc_chrdev_region(&venc->dev, 0, 1, "hi_venc");

	memset((void*)&(venc->cdev), 0, sizeof(struct cdev));
	cdev_init(&(venc->cdev), &VENC_FOPS);

	venc->cdev.owner = THIS_MODULE;
	venc->cdev.ops = &VENC_FOPS;
	err = cdev_add(&(venc->cdev), venc->dev, 1);
	venc->venc_class = class_create(THIS_MODULE, "hi_venc");
	if (IS_ERR(venc->venc_class)) {
		err = PTR_ERR(venc->venc_class);
		HI_ERR_VENC("Fail to create hi_venc class\n");
		return (-1);
	}

	venc->venc_device = device_create(venc->venc_class, NULL, venc->dev, "%s", "hi_venc");
	if (IS_ERR(venc->venc_device)) {
		err = PTR_ERR(venc->venc_device);
		HI_ERR_VENC("%s, Fail to create hi_venc device\n", __func__);
		return (-1);
	}

	HI_INFO_VENC("exit %s()\n", __func__);
	return 0;
}

static  int VENC_DRV_CleanupCdev(VENC_ENTRY *venc)
{
	if (venc->venc_class) {
		device_destroy(venc->venc_class,venc->dev);
		class_destroy(venc->venc_class);
	}

	cdev_del(&(venc->cdev));
	unregister_chrdev_region(venc->dev,1);
	return 0;
}

static  int VENC_DRV_Probe(struct platform_device * pltdev)
{
	 int ret            = (-1);
	VENC_ENTRY *venc      = NULL;
	 int dev_exi        = 0;
	unsigned int *viraddr_pctrl = NULL;

	HI_INFO_VENC("%s, omxvenc prepare to probe.\n", __func__);
	sema_init(&g_VencMutex , 1);

	viraddr_pctrl = (unsigned int *)ioremap(PCTRL_PERI, 4);
	if (viraddr_pctrl) {
		dev_exi = readl(viraddr_pctrl) & 0x10;
		iounmap(viraddr_pctrl);
		viraddr_pctrl = NULL;

		if (dev_exi == 0) {
			HI_INFO_VENC("device enc is not exist\n");
			return (-1);
		}
	} else {
		HI_INFO_VENC("ioremap failed!\n");
		return (-1);
	}

	if (g_vencDevDetected) {
		HI_INFO_VENC("%s,  venc device detected already\n", __func__);
		return 0;
	}

	venc = vmalloc(sizeof(VENC_ENTRY));
	if (!venc) {
		HI_ERR_VENC("%s call vmalloc failed\n", __func__);
		return -1;
	}

	memset((void *)venc, 0, sizeof(VENC_ENTRY));
	ret = VENC_DRV_SetupCdev(venc, &VENC_FOPS);
	if (ret < 0) {
		HI_ERR_VENC("%s call hivdec_setup_cdev failed\n", __func__);
		goto free;
	}

	venc->venc_device_2 = &pltdev->dev;
	platform_set_drvdata(pltdev, venc);
	g_vencDevDetected = 1;

	ret = Venc_Regulator_Init(pltdev , &g_vedu_dts_ctx);
	if (ret < 0) {
		HI_ERR_VENC("%s, Venc_Regulator_Init failed\n", __func__);
		goto cleanup;
	}

	HI_INFO_VENC("%s, omxvenc probe ok.\n", __func__);
	return 0;

cleanup:
	VENC_DRV_CleanupCdev(venc);
free:
	vfree(venc);
	return (-1);
}

static  int VENC_DRV_Remove(struct platform_device *pltdev)
{
	VENC_ENTRY *venc = NULL;
	HI_INFO_VENC("%s, omxvenc prepare to remove.\n", __func__);

	venc = platform_get_drvdata(pltdev);
	if (venc) {
		VENC_DRV_CleanupCdev(venc);
		Venc_Regulator_Deinit(pltdev);
	}
	else {
		HI_ERR_VENC("%s, call platform_get_drvdata err\n", __func__);
	}

	platform_set_drvdata(pltdev,NULL);
	vfree(venc);
	g_vencDevDetected = 0;

	HI_INFO_VENC("%s, remove omxvenc ok.\n", __func__);
	return 0;
}

static void VENC_DRV_DeviceRelease1(struct device* dev)
{
	return;
}

 int __init VENC_DRV_ModInit(void)
{
	int ret = 0;
	HI_INFO_VENC("enter %s()\n", __func__);

	memset((void *)&g_vedu_dts_ctx, 0, sizeof(g_vedu_dts_ctx));
	ret = platform_device_register(&Venc_device);
	if (ret < 0) {
		HI_ERR_VENC("%s call platform_device_register failed\n", __func__);
		return ret;
	}

	ret = platform_driver_register(&Venc_driver);
	if (ret < 0) {
		HI_ERR_VENC("%s call platform_driver_register failed\n", __func__);
		goto exit;
	}

	HI_INFO_VENC("%s, success\n", __func__);
#ifdef MODULE
	HI_INFO_VENC("Load hi_venc.ko success.\t(%s)\n", VERSION_STRING);
#endif
	HI_INFO_VENC("exit %s()\n", __func__);
	return 0;
exit:
	platform_device_unregister(&Venc_device);
#ifdef MODULE
	HI_ERR_VENC("Load hi_venc.ko failed.\t(%s)\n", VERSION_STRING);
#endif
	return ret;
}

void VENC_DRV_ModExit(void)
{
	HI_INFO_VENC("enter %s()\n", __func__);
	platform_driver_unregister(&Venc_driver);
	platform_device_unregister(&Venc_device);

	HI_INFO_VENC("exit %s()\n", __func__);
	return;
}

module_init(VENC_DRV_ModInit);
module_exit(VENC_DRV_ModExit);

MODULE_LICENSE("Dual BSD/GPL");

