/* SPDX-License-Identifier: GPL-2.0 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/jiffies.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/dmaengine_pcm.h>
#include <sound/initval.h>
#include <sound/soc.h>
#include <linux/interrupt.h>
#include <linux/reset.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/reset-controller.h>
#include <linux/clk.h>
#include <linux/regulator/consumer.h>
#include <sound/dmaengine_pcm.h>

/*add a virtual codec for extern codec*/
static int virtual_digital_mute(struct snd_soc_dai *codec_dai, int mute)
{
	return 0;
}

static int virtual_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params,
				struct snd_soc_dai *dai)
{
	return 0;
}

static int virtual_set_dai_sysclk(struct snd_soc_dai *codec_dai,
				 int clk_id, unsigned int freq, int dir)
{
	return 0;
}


static const struct snd_soc_dai_ops virtual_dai_ops = {
	.digital_mute	= virtual_digital_mute,
	.hw_params	= virtual_hw_params,
	.set_sysclk	= virtual_set_dai_sysclk,
};

static struct snd_soc_dai_driver virtual_dai[] = {
	{
		/* DAI PCM */
		.name	= "virtual-dai",
		.id	= 1,
		.playback = {
			.stream_name	= "PCM Playback",
			.channels_min	= 2,
			.channels_max	= 2,
			.rates		= SNDRV_PCM_RATE_48000,
			.formats	= SNDRV_PCM_FMTBIT_S16_LE |
						 SNDRV_PCM_FMTBIT_U16_LE,
		},
		.capture = {
			.stream_name	= "PCM Capture",
			.channels_min	= 2,
			.channels_max	= 2,
			.rates		= SNDRV_PCM_RATE_48000,
			.formats	= SNDRV_PCM_FMTBIT_S16_LE |
						 SNDRV_PCM_FMTBIT_U16_LE,
		},
		.ops	= &virtual_dai_ops,
	}
};

static int virtual_codec_probe(struct snd_soc_codec *codec)
{
	return 0;
}

static int virtual_codec_remove(struct snd_soc_codec *codec)
{
	return 0;
}

static int virtual_codec_set_bias_level(struct snd_soc_codec *codec,
				 enum snd_soc_bias_level level)
{
	return 0;
}

static const struct snd_soc_dapm_widget virtual_codec_widgets[] = {
	SND_SOC_DAPM_OUTPUT("I2SOUT"),
	SND_SOC_DAPM_AIF_IN("AIFIN", "Playback", 0, SND_SOC_NOPM, 0, 0),
};

static const struct snd_soc_dapm_route virtual_codec_routes[] = {
	{ "I2SOUT", NULL, "AIFIN" },
};

static struct snd_soc_codec_driver soc_codec_dev_virtual = {
	.probe		= virtual_codec_probe,
	.remove		= virtual_codec_remove,
	.set_bias_level	= virtual_codec_set_bias_level,
	.component_driver = {
		.dapm_widgets		= virtual_codec_widgets,
		.num_dapm_widgets	= ARRAY_SIZE(virtual_codec_widgets),
		.dapm_routes		= virtual_codec_routes,
		.num_dapm_routes	= ARRAY_SIZE(virtual_codec_routes),
	},
};
/*end*/
static const struct snd_pcm_hardware snd_hisi_hardware = {
	.info	= SNDRV_PCM_INFO_MMAP |
				 SNDRV_PCM_INFO_MMAP_VALID |
				 SNDRV_PCM_INFO_PAUSE |
				 SNDRV_PCM_INFO_RESUME |
				 SNDRV_PCM_INFO_INTERLEAVED |
				 SNDRV_PCM_INFO_HALF_DUPLEX,
	.period_bytes_min	= 4096,
	.period_bytes_max	= 4096,
	.periods_min	= 4,
	.periods_max	= UINT_MAX,
	.buffer_bytes_max	= SIZE_MAX,
};

static const struct snd_dmaengine_pcm_config dmaengine_pcm_config = {
	.pcm_hardware	= &snd_hisi_hardware,
	.prepare_slave_config	= snd_dmaengine_pcm_prepare_slave_config,
	.prealloc_buffer_size	= 64 * 1024,
};

static int hisi_i2s_plat_probe(struct platform_device *pdev)
{
	int ret = 0;

	ret = devm_snd_dmaengine_pcm_register(&pdev->dev,
			&dmaengine_pcm_config,
			SND_DMAENGINE_PCM_FLAG_CUSTOM_CHANNEL_NAME);
	if (ret) {
		dev_err(&pdev->dev, "Failed to register dmaengine pcm\n");
		return -EINVAL;
	}
	ret = snd_soc_register_codec(&pdev->dev, &soc_codec_dev_virtual,
				     virtual_dai, ARRAY_SIZE(virtual_dai));
	if (ret) {
		dev_err(&pdev->dev, "Failed to register codec\n");
		return -EINVAL;
	}
	return ret;
}

static int hisi_i2s_plat_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

static const struct of_device_id hisi_i2s_plat_dt_ids[] = {
	{ .compatible = "hisilicon,hisi-i2s-plat"},
	{/* sentinel */ }
};

MODULE_DEVICE_TABLE(of, hisi_i2s_dt_ids);

static struct platform_driver hisi_i2s_plat_driver = {
	.probe = hisi_i2s_plat_probe,
	.remove = hisi_i2s_plat_remove,
	.driver = {
		.name = "hisi_i2s_plat",
		.owner = THIS_MODULE,
		.of_match_table = hisi_i2s_plat_dt_ids,
	},
};

module_platform_driver(hisi_i2s_plat_driver);

MODULE_DESCRIPTION("Hisilicon I2S Plat driver");
MODULE_AUTHOR("alik.hou");
MODULE_LICENSE("GPL");

