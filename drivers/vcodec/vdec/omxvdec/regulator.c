/*
 *      Video Decode driver for hikey960   regulator configure
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

#include "omxvdec.h"

#include <linux/hisi/hisi-iommu.h>
#include <linux/iommu.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/clk.h>

#define REGULATOR_NAME          "ldo_vdec"
#define VCODEC_CLOCK_NAME       "clk_gate_vdec"
#define VCODEC_CLK_RATE         "dec_clk_rate"
#define VDEC_QOS_MODE           0xE893000C

static unsigned int  g_VdecClkRate_l  = 200000000;
static unsigned int  g_VdecClkRate_h  = 480000000;
static unsigned int  g_CurClkRate     = 0;
static unsigned int  g_vdecQosMode    = 0x2;
static int g_VdecPowerOn    = 0;
static struct  regulator    *g_VdecRegulator  = NULL;
static struct  iommu_domain *g_VdecSmmuDomain = NULL;
static struct clk *g_PvdecClk = NULL;
static struct semaphore   g_RegulatorSem;

#ifdef HIVDEC_SMMU_SUPPORT

#if 1
/*----------------------------------------
    func: iommu enable intf
 ----------------------------------------*/
static int VDEC_Enable_Iommu(struct device *dev)
{
    int ret = -1;    
    g_VdecSmmuDomain = iommu_domain_alloc(dev->bus);
    if (NULL == g_VdecSmmuDomain) {
        OmxPrint(OMX_FATAL, "%s iommu_domain_alloc failed!\n", __func__);
        return -1;
    }

    ret = iommu_attach_device(g_VdecSmmuDomain, dev);
    if (ret) {
        OmxPrint(OMX_ERR, "iommu_attach_device failed!\n");
        goto out_free_domain;
    }
    return 0;
out_free_domain:
    iommu_domain_free(g_VdecSmmuDomain);
    return -1;  
}

/*----------------------------------------
    func: iommu disable intf
 ----------------------------------------*/
static void VDEC_Disable_Iommu(struct device *dev)
{
	if ((g_VdecSmmuDomain != NULL) && (dev != NULL))
		g_VdecSmmuDomain = NULL;
}
#else
static int VDEC_Enable_Iommu(struct device *dev)
{
    g_VdecSmmuDomain = hisi_ion_enable_iommu(NULL);
    if (NULL == g_VdecSmmuDomain) {
        printk(KERN_ERR "%s iommu_domain_alloc failed!\n", __func__);
        return -1;
    }

    return 0;
}

static void VDEC_Disable_Iommu(struct device *dev)
{
    g_VdecSmmuDomain = NULL;
    if( g_VdecSmmuDomain && dev) {
        g_VdecSmmuDomain = NULL;
    }
}
#endif

/*----------------------------------------
    func: get smmu page table base addr
 ----------------------------------------*/
static u64 VDEC_GetSmmuBasePhy(struct device *dev)
{
	struct iommu_domain_data *domain_data = NULL;
	if (VDEC_Enable_Iommu(dev) == -1)
		return 0;

	domain_data = (struct iommu_domain_data *)(g_VdecSmmuDomain->priv);
	return (u64) (domain_data->phy_pgd_base);
}

#endif

/*----------------------------------------
    func: initialize vcodec clock rate
 ----------------------------------------*/
static int VDEC_Init_ClockRate(struct device *dev)
{
	int ret;
	struct device_node *np = NULL;
	struct clk *pvdec_clk  = NULL;
	if (dev == NULL) {
		printk(KERN_CRIT "%s: invalid dev is NULL\n", __func__);
		return -1;
	}

	np = dev->of_node;
	pvdec_clk = devm_clk_get(dev, VCODEC_CLOCK_NAME);
	if (IS_ERR_OR_NULL(pvdec_clk)) {
		printk(KERN_CRIT "%s can not get clock\n", __func__);
		return -1;
	}
    
	g_PvdecClk = pvdec_clk;
	ret = of_property_read_u32_index(np, VCODEC_CLK_RATE, 1, &g_VdecClkRate_l);
	if (ret) {
		printk(KERN_CRIT "%s can not get g_VdecClkRate_l, return %d\n", __func__, ret);
		g_VdecClkRate_l = 200000000;   //200MHZ
		return -1;
	}
    
	OmxPrint(OMX_ALWS, "%s get g_VdecClkRate_l : %u\n", __func__, g_VdecClkRate_l);
	ret = of_property_read_u32_index(np, VCODEC_CLK_RATE, 0, &g_VdecClkRate_h);
	if (ret) {
		printk(KERN_CRIT "%s can not get g_VdecClkRate_h, return %d\n", __func__, ret);
		g_VdecClkRate_h = 480000000;    //480MHZ
		return -1;
	}
    
	OmxPrint(OMX_ALWS, "%s get g_VdecClkRate_h : %u\n", __func__, g_VdecClkRate_h);
	ret = clk_set_rate(pvdec_clk, g_VdecClkRate_h);
	if (ret) {
		printk(KERN_CRIT "%s Failed to clk_set_rate, return %d\n", __func__, ret);
		return -1;
	}
    
	OmxPrint(OMX_ALWS, "%s set VdecClkRate : %u\n", __func__, g_VdecClkRate_h);
	g_CurClkRate = g_VdecClkRate_h;
	return 0;
}

/*----------------------------------------
    func: get dts configure
          as reg base & irq num
 ----------------------------------------*/
static int VDEC_GetDtsConfigInfo(struct device *dev, VFMW_DTS_CONFIG_S *pDtsConfig)
{
	int ret;
	struct device_node *np_crg = NULL;
	struct device_node *np     = dev->of_node;
	struct resource res;
	if (np == NULL) {
		printk(KERN_CRIT "%s: invalid device node is null\n", __func__);
		return -1;
	}

	if (pDtsConfig == NULL) {
		printk(KERN_CRIT "%s: invalid pDtsConfig is null\n", __func__);
		return -1;
	}
	//Get irq num, return 0 if failed
	pDtsConfig->MfdeIrqNum = irq_of_parse_and_map(np, 0);
	if (pDtsConfig->MfdeIrqNum == 0) {
		printk(KERN_CRIT "%s irq_of_parse_and_map MfdeIrqNum failed\n", __func__);
		return -1;
	}

	pDtsConfig->ScdIrqNum = irq_of_parse_and_map(np, 1);
	if (pDtsConfig->ScdIrqNum == 0) {
		printk(KERN_CRIT "%s irq_of_parse_and_map ScdIrqNum failed\n", __func__);
		return -1;
	}

	pDtsConfig->BpdIrqNum = irq_of_parse_and_map(np, 2);
	if (pDtsConfig->BpdIrqNum == 0) {
		printk(KERN_CRIT "%s irq_of_parse_and_map BpdIrqNum failed\n", __func__);
		return -1;
	}

	pDtsConfig->SmmuIrqNum = irq_of_parse_and_map(np, 3);
	if (pDtsConfig->SmmuIrqNum == 0) {
		printk(KERN_CRIT "%s irq_of_parse_and_map SmmuIrqNum failed\n", __func__);
		return -1;
	}

	//Get reg base addr & size, return 0 if success
	ret = of_address_to_resource(np, 0, &res);
	if (ret) {
		printk(KERN_CRIT "%s of_address_to_resource failed return %d\n", __func__, ret);
		return -1;
	}
    
	pDtsConfig->VdhRegBaseAddr = res.start;
	pDtsConfig->VdhRegRange = resource_size(&res);
#ifdef HIVDEC_SMMU_SUPPORT
	//Get reg base addr & size, return 0 if failed
	pDtsConfig->SmmuPageBaseAddr = VDEC_GetSmmuBasePhy(dev);
	if (pDtsConfig->SmmuPageBaseAddr == 0) {
		printk(KERN_CRIT "%s Regulator_GetSmmuBasePhy failed\n", __func__);
		return -1;
	}
#endif

	np_crg = of_find_compatible_node(NULL, NULL, "hisilicon,hi3660-crgctrl");
	ret = of_address_to_resource(np_crg, 0, &res);
	if (ret) {
		printk(KERN_CRIT "%s of_address_to_resource crg failed return %d\n", __func__, ret);
		return -1;
	}
    
	pDtsConfig->PERICRG_RegBaseAddr = res.start;
	//Check if is FPGA platform
	ret = of_property_read_u32(np, "vdec_fpga", &pDtsConfig->IsFPGA);
	if (ret) {
		pDtsConfig->IsFPGA = 0;
		OmxPrint(OMX_ALWS, "Is not FPGA platform\n");
	}

	/* get vdec qos mode */
	ret = of_property_read_u32(np, "vdec_qos_mode", &g_vdecQosMode);
	if (ret) {
		g_vdecQosMode = 0x2;
		OmxPrint(OMX_WARN, "get vdec qos mode failed set default\n");
	}

	ret = VDEC_Init_ClockRate(dev);
	if (ret != 0) {
		printk(KERN_ERR "%s VDEC_Init_ClockRate failed\n", __func__);
		return -1;		
	}

	return 0;
}

/*----------------------------------------
    func: get dts configure
          as reg base & irq num
 ----------------------------------------*/
static int VDEC_Config_QOS(void)
{
	int ret = -1;
	unsigned int *qos_addr = NULL;

	qos_addr = (unsigned int *) ioremap(VDEC_QOS_MODE, 4);
	if (qos_addr) {
		writel(g_vdecQosMode, qos_addr);
		iounmap(qos_addr);
		ret = 0;
	} else {
		OmxPrint(OMX_ERR, "ioremap VDEC_QOS_MODE reg failed\n");
		ret = -1;
	}

	return ret;
}

/******************************** SHARE FUNC **********************************/

/*----------------------------------------
    func: regulator probe entry
 ----------------------------------------*/
int VDEC_Regulator_Probe(struct device *dev, VFMW_DTS_CONFIG_S *pDtsConfig)
{
	int ret;
	g_VdecRegulator = NULL;
	if (dev == NULL) {
		printk(KERN_CRIT "%s, invalid params", __func__);
		return -1;
	}

	g_VdecRegulator = devm_regulator_get(dev, REGULATOR_NAME);
	if (g_VdecRegulator == NULL) {
		printk(KERN_CRIT "%s get regulator failed\n", __func__);
		return -1;
	} else if (IS_ERR(g_VdecRegulator)) {
		OmxPrint(OMX_FATAL, "%s get regulator failed, error no = %ld\n", __func__, PTR_ERR(g_VdecRegulator));
		g_VdecRegulator = NULL;
		return -1;
	}

	memset(pDtsConfig , 0, sizeof(VFMW_DTS_CONFIG_S));
	ret = VDEC_GetDtsConfigInfo(dev, pDtsConfig);
	if (ret != 0) {
		printk(KERN_CRIT "%s Regulator_GetDtsConfigInfo failed\n", __func__);
		return -1;
	}
 
	dprint(KERN_ERR, "%s done: IsFPGA : %d, MfdeIrqNum : %d, ScdIrqNum : %d, BpdIrqNum : %d, SmmuIrqNum : %d\n",
		__func__, pDtsConfig->IsFPGA, pDtsConfig->MfdeIrqNum, pDtsConfig->ScdIrqNum, pDtsConfig->BpdIrqNum, pDtsConfig->SmmuIrqNum);   
   
	sema_init(&g_RegulatorSem, 1);
	return 0;
}

/*----------------------------------------
    func: regulator deinitialize
 ----------------------------------------*/
int VDEC_Regulator_Remove(struct device * dev)
{
    if(down_interruptible(&g_RegulatorSem)) {
        printk(KERN_CRIT "%s down_interruptible failed\n", __func__);
        return -1;
    }
 
	VDEC_Disable_Iommu(dev);
	g_VdecRegulator = NULL;
	g_PvdecClk      = NULL;
	up(&g_RegulatorSem);

	return 0;
}

int VDEC_Regulator_Enable(void)
{
	int ret;    
    if(down_interruptible(&g_RegulatorSem)) {
        printk(KERN_CRIT "%s down_interruptible failed\n", __func__);
        return -1;
    }

	if (g_VdecPowerOn == 1) {
		up(&g_RegulatorSem);
		return 0;
	}
	if (g_PvdecClk == NULL) {
		printk(KERN_CRIT "%s: invalid g_PvdecClk is NULL\n", __func__);
		goto error_exit;
	}

	if (IS_ERR_OR_NULL(g_VdecRegulator)) {
		OmxPrint(OMX_WARN, "%s ERROR: g_VdecRegulator is NULL", __func__);
		goto error_exit;
	}

	ret = clk_prepare_enable(g_PvdecClk);
	if (ret != 0) {
		printk(KERN_CRIT "%s clk_prepare_enable failed\n", __func__);
		goto error_exit;
	}

	OmxPrint(OMX_ALWS, "%s, call regulator_enable\n", __func__);
	ret = regulator_enable(g_VdecRegulator);
	if (ret != 0) {
		printk(KERN_CRIT "%s enable regulator failed\n", __func__);
		goto error_clk_unprepare;
	}

	ret = VDEC_Config_QOS();
	if (ret != 0) {
		printk(KERN_ERR "VDEC_Config_QOS failed\n");
		goto error_regulator_disable;
	}

	g_VdecPowerOn = 1;
	up(&g_RegulatorSem);
	return 0;

error_regulator_disable:
	regulator_disable(g_VdecRegulator);

error_clk_unprepare:
	clk_disable_unprepare(g_PvdecClk);

error_exit:
	up(&g_RegulatorSem);
	return -1;
}

int VDEC_Regulator_Disable(void)
{
	int ret;
    if(down_interruptible(&g_RegulatorSem)) {
        printk(KERN_CRIT "%s down_interruptible failed\n", __func__);
        return -1;
    }

	if (g_VdecPowerOn == 0) {
		up(&g_RegulatorSem);
		return 0;
	}

	if (g_PvdecClk == NULL) {
		printk(KERN_CRIT "%s: invalid g_PvdecClk is NULL\n", __func__);
		goto error_exit;
	}

	if (IS_ERR_OR_NULL(g_VdecRegulator)) {
		OmxPrint(OMX_WARN, "%s ERROR: g_VdecRegulator is NULL", __func__);
		goto error_exit;
	}

	OmxPrint(OMX_ALWS, "%s, call regulator_disable\n", __func__);
	ret = regulator_disable(g_VdecRegulator);
	if (ret != 0) {
		printk(KERN_CRIT "%s disable regulator failed\n", __func__);
		goto error_exit;
	}

	clk_disable_unprepare(g_PvdecClk);
	g_VdecPowerOn = 0;
	up(&g_RegulatorSem);
	return 0;
error_exit:
	up(&g_RegulatorSem);
	return -1;
}

/*----------------------------------------
    func: get decoder clock rate
 ----------------------------------------*/
int VDEC_Regulator_GetClkRate(CLK_RATE_E *pClkRate)
{
    if(down_interruptible(&g_RegulatorSem)) {
        printk(KERN_CRIT "%s down_interruptible failed\n", __func__);
        return -1;
    }

	if (g_CurClkRate == g_VdecClkRate_h) {
		*pClkRate = CLK_RATE_HIGH;
	} else if (g_CurClkRate == g_VdecClkRate_l) {
		*pClkRate = CLK_RATE_LOW;
	} else {
		OmxPrint(OMX_ERR, "%s: unkown clk rate %d, return CLK_RATE_HIGH\n", __func__, g_CurClkRate);
		*pClkRate = CLK_RATE_HIGH;
	}
	up(&g_RegulatorSem);

	return 0;
}

/*----------------------------------------
    func: decoder clock rate select
 ----------------------------------------*/
int VDEC_Regulator_SetClkRate(CLK_RATE_E eClkRate , unsigned int IsFPGA)
{
	int ret          = 0;
	unsigned int rate         = 0;
	unsigned char need_set_flag = 1;
    if(down_interruptible(&g_RegulatorSem)) {
        printk(KERN_CRIT "%s down_interruptible failed\n", __func__);
        return -1;
    }

	if (IsFPGA) {
		up(&g_RegulatorSem);
		return 0;
	}

	if (IS_ERR_OR_NULL(g_PvdecClk)) {
		printk(KERN_ERR "Couldn't get clk [%s]\n", __func__);
		goto error_exit;
	}

	rate = (unsigned int) clk_get_rate(g_PvdecClk);
	switch (eClkRate) {
	case CLK_RATE_LOW:
		if (g_VdecClkRate_l == rate) {
			need_set_flag = 0;
		} else {
			rate = g_VdecClkRate_l;
			need_set_flag = 1;
		}
		break;

	case CLK_RATE_HIGH:
		if (g_VdecClkRate_h == rate) {
			need_set_flag = 0;
		} else {
			rate = g_VdecClkRate_h;
			need_set_flag = 1;
		}
		break;

	default:
		printk(KERN_ERR "[%s] unsupport clk rate enum %d\n", __func__, eClkRate);
		goto error_exit;
	}

	if (need_set_flag == 1) {
		ret = clk_set_rate(g_PvdecClk, rate);
		if (ret != 0) {
			printk(KERN_ERR "Failed to clk_set_rate %u HZ[%s] ret : %d\n", rate, __func__, ret);
			goto error_exit;
		}
		g_CurClkRate = rate;
	}

	up(&g_RegulatorSem);
	return 0;

error_exit:
	up(&g_RegulatorSem);
	return -1;
}
