/*
 *      Video Encode driver for hikey960   regulator configure
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
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/regulator/consumer.h>
#include <linux/hisi/hisi-iommu.h>  

#include "drv_venc.h"

#define VENC_CLK_RATE       "enc_clk_rate"
#define REGULATOR_NAME      "ldo_venc"
#define VENC_CLOCK_NAME     "clk_gate_venc"  
#define VENC_CLOCK_VOLT     "venc_volt_hold"
#define VENC_QOS_MODE        0xE894000C

static  struct clk       *g_PvencClk        = NULL;
static  struct clk       *g_PvencVolt       = NULL;
static  struct  iommu_domain     *g_hisi_mmu_domain = NULL;
static  struct regulator *g_VencRegulator   = NULL;
#ifdef VENC_VOLT_HOLD
static  int g_voltHold = 0;
#endif

static unsigned int g_vencQosMode  = 0x2;
static int g_VencPowerOn = 0;

static int Venc_Enable_Iommu(struct platform_device *pdev , vedu_dts_ctx *pCtx)
{
	struct iommu_domain *hisi_domain      = NULL;
	struct iommu_domain_data* domain_data = NULL;
	struct device *dev    = NULL;
	int ret = 0;

	if ((!pdev ) || (!(&pdev->dev))){
		HI_ERR_VENC("%s,  invalid Parameters\n", __func__);
		return -1;
	}

	dev = &pdev->dev;
	hisi_domain = iommu_domain_alloc(dev->bus);
	if (!hisi_domain) {
		HI_ERR_VENC("%s, iommu_domain_alloc failed\n", __func__);
		return -1;
	}

	ret = iommu_attach_device(hisi_domain, dev);
	if (ret){
		HI_ERR_VENC("iommu_attach_device failed\n");
		goto out_free_domain;
	}
    
	g_hisi_mmu_domain = hisi_domain;
	domain_data = (struct iommu_domain_data *)(g_hisi_mmu_domain->priv);
	if (domain_data){
		pCtx->phy_pgd_base = (uint64_t)(domain_data->phy_pgd_base);
	}else{
		goto out_detach_device;
	}

	return 0;

out_detach_device:
	iommu_detach_device(g_hisi_mmu_domain, dev);
out_free_domain:
	iommu_domain_free(hisi_domain);
	return -1;
}

static int Venc_Disable_Iommu(struct platform_device *pdev)
{
	if( g_hisi_mmu_domain && pdev) {
		iommu_detach_device(g_hisi_mmu_domain, &pdev->dev);
		iommu_domain_free(g_hisi_mmu_domain);
		g_hisi_mmu_domain = NULL;
		return 0;
	}

	return -1;
}

static  int Venc_GetDtsConfigInfo(struct platform_device *pdev, vedu_dts_ctx *pCtx)
{
	unsigned int rate_h = 0;
	unsigned int rate_n = 0;
	int ret    = (-1);
	struct resource res;
	struct clk *pvenc_clk    = NULL;
	struct clk *pvenc_volt   = NULL;
	struct device_node *np   = NULL;
	struct device *dev       = &pdev->dev;
	HI_U64 SmmuPageBaseAddr  = 0 ;
	struct iommu_domain_data *domain_data = NULL;

	if (!dev){
		HI_ERR_VENC("invalid argument\n");
		return (-1);
	}

	np = dev->of_node;
	memset(&res, 0, sizeof(res));
	if ((!np) || (!pCtx)){
		HI_ERR_VENC("invalid argument np:0x%x, pCtx:0x%x \n", np, pCtx);
		return (-1);
	}

	pCtx->VeduIrqNum = irq_of_parse_and_map(np, 0);
	if (pCtx->VeduIrqNum == 0){
		HI_ERR_VENC("%s, irq_of_parse_and_map VeduIrqNum failed\n", __func__);
		return (-1);
	}

	pCtx->MmuIrqNum = irq_of_parse_and_map(np, 1);
	if (pCtx->MmuIrqNum == 0){
		HI_ERR_VENC("%s, irq_of_parse_and_map MmuIrqNum failed\n", __func__);
		return (-1);
	}

	ret = of_address_to_resource(np, 0, &res);
	if (ret){
		HI_ERR_VENC("%s of_address_to_resource failed ret = %d\n", __func__, ret);
		return (-1);
	}
    
	pCtx->VencRegBaseAddr = res.start;
	pCtx->VencRegRange    = resource_size(&res);
	pvenc_clk  = devm_clk_get(dev, VENC_CLOCK_NAME);
	pvenc_volt = devm_clk_get(dev, VENC_CLOCK_VOLT);
	if (IS_ERR_OR_NULL(pvenc_volt)|| IS_ERR_OR_NULL(pvenc_clk)){
		HI_ERR_VENC("can not get venc clock, pvenc_volt = 0x%x, pvenc_clk = 0x%x\n", pvenc_volt, pvenc_clk);
		return (-1);
	}
    
	g_PvencClk  = pvenc_clk;
	g_PvencVolt = pvenc_volt;
	ret = of_property_read_u32_index(np, VENC_CLK_RATE, 0, &rate_h);
	ret += of_property_read_u32_index(np, VENC_CLK_RATE, 1, &rate_n);
	if (ret){
		HI_ERR_VENC("%s can not get venc rate, return %d\n", __func__, ret);
		return (-1);
	}
    
	pCtx->normalRate = rate_n;
	pCtx->highRate   = rate_h;

	ret = of_property_read_u32(np, "venc_fpga", &pCtx->IsFPGA);
	if (ret)
		HI_INFO_VENC("%s, read failed\n", __func__);

	ret = of_property_read_u32(np, "venc_qos_mode", &g_vencQosMode);
	if (ret){
		g_vencQosMode = 0x2;
		HI_ERR_VENC("get venc qos mode failed set default\n");
	}

	domain_data = (struct iommu_domain_data *)(g_hisi_mmu_domain->priv);
	if (domain_data){
		SmmuPageBaseAddr = (uint64_t)(domain_data->phy_pgd_base);
	}

	if (SmmuPageBaseAddr == 0){
		HI_ERR_VENC("%s, Regulator_GetSmmuBasePhy failed\n", __func__);
		return (-1);
	}

	return 0;
}

static  int Venc_Regulator_Get(struct platform_device *pdev)
{
	HI_INFO_VENC("enter %s\n", __func__);

	g_VencRegulator = NULL;
	g_VencRegulator = devm_regulator_get(&pdev->dev, REGULATOR_NAME);
	if (IS_ERR_OR_NULL(g_VencRegulator)){
		HI_ERR_VENC("%s, get regulator failed, error no = %ld\n", __func__, PTR_ERR(g_VencRegulator));
		g_VencRegulator = NULL;

		return (-1);
	}

	return 0;
}

static  int Venc_config_QOS(void)
{
	int ret       = (-1);
	unsigned int *qos_addr = NULL;

	qos_addr = (unsigned int *)ioremap(VENC_QOS_MODE, 4); 
	if (qos_addr){
		writel(g_vencQosMode, qos_addr);
		iounmap(qos_addr); 
		qos_addr = NULL;
		ret = 0;
	}else{
		HI_ERR_VENC("ioremap VENC_QOS_MODE reg failed\n");
		ret = (-1);
	}

	return ret;
}

 int Venc_Regulator_Init(struct platform_device *pdev , vedu_dts_ctx *pCtx)
{
	int ret = 0;
	if (!pdev){
		HI_ERR_VENC("%s, invalid argument\n", __func__);
		return (-1);
	}

	/* 1 get regulator interface */
	ret = Venc_Regulator_Get(pdev);
	if (ret < 0){
		HI_ERR_VENC("%s, Venc_Regulator_Get failed\n", __func__);
		return (-1);
	}

	/* 2 create smmu domain */
	ret = Venc_Enable_Iommu(pdev , pCtx);
	if (ret < 0){
		HI_ERR_VENC("%s, VENC_Enable_Iommu failed\n", __func__);
		return (-1);
	}

	/* 3 read venc dts info from dts */
	ret = Venc_GetDtsConfigInfo(pdev, pCtx);
	if (ret != 0){
		HI_ERR_VENC("%s VENC_GetDtsConfigInfo failed.\n", __func__);
		return (-1);
	}

	HI_INFO_VENC("exit %s()\n", __func__);
	return 0;
}

void Venc_Regulator_Deinit(struct platform_device *pdev)
{
	if (pdev)
		Venc_Disable_Iommu(pdev);
	return;
}

 int Venc_Regulator_Enable(vedu_dts_ctx *pCtx)
{
	 int ret = (-1);

	if (g_VencPowerOn == 1)
		return 0;

	if (IS_ERR_OR_NULL(g_PvencClk) || IS_ERR_OR_NULL(g_PvencVolt) || IS_ERR_OR_NULL(g_VencRegulator)){
		HI_ERR_VENC("invalid_argument g_PvencClk:0x%x, g_PvencVolt:0x%x, g_VencRegulator:0x%x\n",
				g_PvencClk, g_PvencVolt, g_VencRegulator);
		return (-1);
	}

	ret = clk_set_rate(g_PvencClk, pCtx->normalRate);
	if (ret != 0){
		HI_ERR_VENC("clk_set_rate high/2 failed\n");
		return (-1);
	}

	ret = clk_prepare_enable(g_PvencClk);
	if (ret != 0){
		HI_ERR_VENC("clk_prepare_enable failed\n");
		return (-1);
	}
    
#ifdef VENC_VOLT_HOLD
	if (!g_voltHold){
		ret = clk_prepare_enable(g_PvencVolt);
		if (ret != 0){
			HI_ERR_VENC("clk_prepare_enable venc_volt_hold failed\n");
			goto on_error_clk;
		}
		g_voltHold = 1;
		HI_INFO_VENC("venc volt hold success");
	}
#endif

	ret = regulator_enable(g_VencRegulator);
	if (ret != 0){
		HI_ERR_VENC("enable regulator failed\n");
		goto on_error_volt;
	}

	ret = Venc_config_QOS();
	if (ret != 0){
		HI_ERR_VENC("Venc_config_QOS failed\n");
		goto on_error_regulator;
	}

	g_VencPowerOn = 1;
	return 0;

on_error_regulator:
	regulator_disable(g_VencRegulator);

on_error_volt:
	clk_disable_unprepare(g_PvencVolt);
#ifdef VENC_VOLT_HOLD
on_error_clk:
	clk_disable_unprepare(g_PvencClk);
#endif
	return (-1);
}

 int Venc_Regulator_Disable(int disVoltHold)
{
	int ret = (-1);
	HI_DBG_VENC("%s, Venc_Regulator_Disable g_VencRegulator:0x%x\n", __func__, g_VencRegulator);

	if (g_VencPowerOn == 0)
		return 0;

	if (IS_ERR_OR_NULL(g_PvencClk) || IS_ERR_OR_NULL(g_PvencVolt) || IS_ERR_OR_NULL(g_VencRegulator)){
		HI_ERR_VENC("invalid_argument g_PvencClk:0x%x, g_PvencVolt:0x%x, g_VencRegulator:0x%x\n",
				g_PvencClk, g_PvencVolt, g_VencRegulator);
		return (-1);
	}

	ret = regulator_disable(g_VencRegulator);
	if (ret != 0){
		HI_ERR_VENC("disable regulator failed\n");
		return (-1);
	}

	clk_disable_unprepare(g_PvencClk);
#ifdef VENC_VOLT_HOLD
	if (disVoltHold && g_voltHold){
		clk_disable_unprepare(g_PvencVolt); // always kept 0.8v, if venc exist
		g_voltHold = 0;
		HI_INFO_VENC("venc volt hold release success");
	}
#endif
	g_VencPowerOn = 0;

	return 0;
}

 int Venc_SetClkRate(int isHighRate , vedu_dts_ctx *pCtx)
{
	int ret  = 0;

	if (isHighRate){
		ret = clk_set_rate(g_PvencClk, (unsigned long)pCtx->highRate);
		if (ret != 0)
			HI_ERR_VENC("clk_set_rate high failed\n");
	}
    
	HI_DBG_VENC("clk_set_rate high success\n");
	return ret;
}

