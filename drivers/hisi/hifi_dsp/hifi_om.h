/*
 * hifi om.
 *
 * Copyright (c) 2013 Hisilicon Technologies CO., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/*
 *
 * Modifications made by Cadence Design Systems, Inc.  06/21/2017
 * Copyright (C) 2017 Cadence Design Systems, Inc.All rights reserved worldwide.
 *
 */

#ifndef __HIFI_OM_H__
#define __HIFI_OM_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HIFIDEBUG_PATH                                "hifidebug"
#define LOG_PATH_HISI_LOGS "/data/hisi_logs/"
#define LOG_PATH_RUNNING_TRACE "/data/hisi_logs/running_trace/"
#define LOG_PATH_HIFI_LOG "/data/hisi_logs/running_trace/hifi_log/"
#define LOG_PATH_BSD_LOG "/data/hisi_logs/running_trace/hifi_log/bsd_log/"
#define FILE_NAME_DUMP_DSP_LOG		 "hifi.log"
#define FILE_NAME_DUMP_DSP_BIN		 "hifi.bin"
#define FILE_NAME_DUMP_DSP_PANIC_LOG "hifi_panic.log"
#define FILE_NAME_DUMP_DSP_PANIC_BIN "hifi_panic.bin"
#define FILE_NAME_DUMP_DSP_OCRAM_BIN "hifi_ocram.bin"
#define FILE_NAME_DUMP_DSP_TCM_BIN   "hifi_tcm.bin"

#define HIFIDEBUG_PATH                                "hifidebug"
#define HIFIDEBUG_LEVEL_PROC_FILE                     "debuglevel"
#define HIFIDEBUG_DSPDUMPLOG_PROC_FILE                "dspdumplog"
#define HIFIDEBUG_FAULTINJECT_PROC_FILE               "dspfaultinject"
#define HIFIDEBUG_RESETOPTION_PROC_FILE               "resetsystem"
#define HIKEY_AP_DSP_MSG_MAX_LEN 100

#ifndef LOG_TAG
#define LOG_TAG "hifi_misc "
#endif

#define HIFI_DUMP_FILE_NAME_MAX_LEN 256
#define DSP_DUMP_MAX_EFFECTS_CNT        (10)
#define ROOT_UID     0
#define SYSTEM_GID   1000
#define HIFI_OM_DIR_LIMIT		0750
#define HIFI_OM_FILE_LIMIT		0640
#define HIFI_OM_LOG_SIZE_MAX	0x400000	/* 4*1024*1024 = 4M */
#define HIFI_OM_FILE_BUFFER_SIZE_MAX	(1024)
#define HIFI_SEC_MAX_NUM 64

typedef enum {
	DUMP_DSP_LOG,
	DUMP_DSP_BIN
} DUMP_DSP_TYPE;

typedef enum {
	DSP_NORMAL,
	DSP_PANIC,
	DSP_LOG_BUF_FULL
} DSP_ERROR_TYPE;

typedef enum {
	NORMAL_LOG = 0,
	NORMAL_BIN,
	PANIC_LOG,
	PANIC_BIN,
	OCRAM_BIN,
	TCM_BIN
} DUMP_DSP_INDEX;

enum HIFI_CPU_LOAD_INFO_ENUM {
	HIFI_CPU_LOAD_REPORT   = 0,
	HIFI_CPU_LOAD_VOTE_UP,
	HIFI_CPU_LOAD_VOTE_DOWN,
	HIFI_CPU_LOAD_LACK_PERFORMANCE,
	HIFI_CPU_LOAD_INFO_BUTT
};

enum HIFI_CPU_OM_INFO_ENUM {
	HIFI_CPU_OM_LOAD_INFO = 0,
	HIFI_CPU_OM_ALGO_MCPS_INFO,
	HIFI_CPU_OM_UPDATE_BUFF_DELAY_INFO, /* audio pcm play or capture update buff delay */
	HIFI_CPU_OM_INFO_BUTT
};
enum EFFECT_ALGO_ENUM {
	ID_EFFECT_ALGO_START                = 0,
	ID_EFFECT_ALGO_FORMATER,
	ID_EFFECT_ALGO_FORTE_VOICE_SPKOUT,
	ID_EFFECT_ALGO_FORTE_VOICE_MICIN,
	ID_EFFECT_ALGO_FORTE_VOICE_SPKOUT_BWE,
	ID_EFFECT_ALGO_FORTE_VOIP_MICIN,
	ID_EFFECT_ALGO_FORTE_VOIP_SPKOUT,
	ID_EFFECT_ALGO_IN_CONVERT_I2S_GENERAL,
	ID_EFFECT_ALGO_IN_CONVERT_I2S_HI363X,
	ID_EFFECT_ALGO_INTERLACE,
	ID_EFFECT_ALGO_OUT_CONVERT_I2S_GENERAL,
	ID_EFFECT_ALGO_OUT_CONVERT_I2S_HI363X,
	ID_EFFECT_ALGO_SWAP,
	ID_EFFECT_ALGO_IMEDIA_WNR_MICIN,
	ID_EFFECT_ALGO_IMEDIA_WNR_SPKOUT,
	ID_EFFECT_ALGO_SWS_INTERFACE,
	ID_EFFECT_ALGO_DTS,
	ID_EFFECT_ALGO_DRE,
	ID_EFFECT_ALGO_CHC,
	ID_EFFECT_ALGO_SRC,
	ID_EFFECT_ALGO_TTY,
	ID_EFFECT_ALGO_KARAOKE_RECORD,
	ID_EFFECT_ALGO_KARAOKE_PLAY,
	ID_EFFECT_ALGO_MLIB_CS_VOICE_CALL_MICIN,
	ID_EFFECT_ALGO_MLIB_CS_VOICE_CALL_SPKOUT,
	ID_EFFECT_ALGO_MLIB_VOIP_CALL_MICIN,
	ID_EFFECT_ALGO_MLIB_VOIP_CALL_SPKOUT,
	ID_EFFECT_ALGO_MLIB_AUDIO_PLAY,
	ID_EFFECT_ALGO_MLIB_AUDIO_RECORD,
	ID_EFFECT_ALGO_MLIB_SIRI_MICIN,
	ID_EFFECT_ALGO_MLIB_SIRI_SPKOUT,
	ID_EFFECT_ALGO_EQ,
	ID_EFFECT_ALGO_MBDRC6402,
	ID_EFFECT_ALGO_IMEDIA_VOIP_MICIN,
	ID_EFFECT_ALGO_IMEDIA_VOIP_SPKOUT,
	ID_EFFECT_ALGO_IMEDIA_VOICE_CALL_MICIN,
	ID_EFFECT_ALGO_IMEDIA_VOICE_CALL_SPKOUT,
	ID_EFFECT_ALGO_IMEDIA_VOICE_CALL_SPKOUT_BWE,
	ID_EFFECT_ALGO_BUTT
};

enum EFFECT_STREAM_ID {
	AUDIO_STREAM_PCM_OUTPUT         = 0,
	AUDIO_STREAM_PLAYER_OUTPUT,
	AUDIO_STREAM_MIXER_OUTPUT,
	AUDIO_STREAM_VOICE_OUTPUT,
	AUDIO_STREAM_VOICEPP_OUTPUT,
	AUDIO_STREAM_OUTPUT_CNT,

	AUDIO_STREAM_PCM_INPUT          = 0x10,
	AUDIO_STREAM_VOICE_INPUT,
	AUDIO_STREAM_VOICEPP_INPUT,
	AUDIO_STREAM_INPUT_CNT,
};

enum DRV_HIFI_IMAGE_SEC_LOAD_ENUM {
	DRV_HIFI_IMAGE_SEC_LOAD_STATIC = 0,
	DRV_HIFI_IMAGE_SEC_LOAD_DYNAMIC,
	DRV_HIFI_IMAGE_SEC_UNLOAD,
	DRV_HIFI_IMAGE_SEC_UNINIT,
	DRV_HIFI_IMAGE_SEC_LOAD_BUTT,
};
typedef unsigned char DRV_HIFI_IMAGE_SEC_LOAD_ENUM_UINT8;

enum DRV_HIFI_IMAGE_SEC_TYPE_ENUM {
	DRV_HIFI_IMAGE_SEC_TYPE_CODE = 0,
	DRV_HIFI_IMAGE_SEC_TYPE_DATA,
	DRV_HIFI_IMAGE_SEC_TYPE_BSS,
	DRV_HIFI_IMAGE_SEC_TYPE_BUTT,
};
typedef unsigned char DRV_HIFI_IMAGE_SEC_TYPE_ENUM_UINT8;

struct drv_hifi_image_sec {
	unsigned short sn;
	DRV_HIFI_IMAGE_SEC_TYPE_ENUM_UINT8 type;
	DRV_HIFI_IMAGE_SEC_LOAD_ENUM_UINT8 load_attib;
	unsigned int src_offset;
	unsigned int des_addr;
	unsigned int size;
};

struct drv_hifi_image_head {
	char time_stamp[24];
	unsigned int image_size;
	unsigned int sections_num;
	struct drv_hifi_image_sec sections[HIFI_SEC_MAX_NUM];
};

struct image_partition_table {
	unsigned long phy_addr_start;
	unsigned long phy_addr_end;
	unsigned int size;
	unsigned long remap_addr;
};


struct hifi_om_s {
	struct task_struct *kdumpdsp_task;
	struct semaphore	dsp_dump_sema;

	unsigned int	debug_level;
	unsigned int	dsp_debug_level;
	unsigned int *dsp_debug_level_addr;

	unsigned int	pre_dsp_dump_timestamp;
	unsigned int *dsp_time_stamp;
	unsigned int	pre_exception_no;
	unsigned int *dsp_exception_no;

	unsigned int *dsp_panic_mark;

	unsigned int *dsp_log_cur_addr;
	char *dsp_log_addr;
	char *dsp_bin_addr;
	char			cur_dump_time[HIFI_DUMP_FILE_NAME_MAX_LEN];
	bool			first_dump_log;
	bool			force_dump_log;
	DSP_ERROR_TYPE  dsp_error_type;

	bool			dsp_loaded;
	bool			hifi3_firmware_load;
	bool			reset_system;
	unsigned int	dsp_loaded_sign;

	unsigned int *dsp_debug_kill_addr;
	unsigned int *dsp_stack_addr;
	unsigned int *dsp_loaded_indicate_addr;
	struct drv_fama_config *dsp_fama_config;

	struct device	*dev;

};

struct hifi_dsp_dump_info{
	DSP_ERROR_TYPE error_type;
	DUMP_DSP_TYPE dump_type;
	char *file_name;
	char *data_addr;
	unsigned int data_len;
};

typedef struct {
	unsigned short	effect_stream_id;
	unsigned short	effect_algo_id;
	unsigned int	effect_algo_mcps;
} hifi_effect_mcps_stru;

typedef struct {
	unsigned int	cpu_load;
	unsigned int	avg_cpu_Load;
	unsigned int	ddr_freq;
} hifi_cpu_load_info_stru;

struct hifi_om_load_info_stru {
	unsigned int	recv_msg_type;
	hifi_cpu_load_info_stru	cpu_load_info;
	unsigned int	info_type;
	unsigned int	report_interval;
};

struct hifi_om_effect_mcps_stru {
	unsigned int	recv_msg_type;
	hifi_cpu_load_info_stru	cpu_load_info;
	hifi_effect_mcps_stru	effect_mcps_info[DSP_DUMP_MAX_EFFECTS_CNT];
};

struct hifi_om_update_buff_delay_info {
	unsigned int	recv_msg_type;
	unsigned short	reserved;
	unsigned short	pcm_mode;
};

extern struct hifi_om_s g_om_data;

typedef struct {
	char level_char;
	unsigned int level_num;
} debug_level_com;

/* ...alignment for shared buffers */
#define XF_PROXY_ALIGNMENT              64
#define XF_PROXY_MESSAGE_QUEUE_LENGTH   (1 << 8)
/* ...data managed by host CPU (remote) - in case of shunt it is a IPC layer */
struct xf_proxy_host_data	{
	/* ...command queue */
	xf_proxy_message_t	command[XF_PROXY_MESSAGE_QUEUE_LENGTH];
	/* ...writing index into command queue */
	uint32_t	cmd_write_idx;
	/* ...reading index for response queue */
	uint32_t	rsp_read_idx;
};
/* ...data managed by DSP (local) */
struct xf_proxy_dsp_data	{
	/* ...response queue */
	xf_proxy_message_t	response[XF_PROXY_MESSAGE_QUEUE_LENGTH];
	/* ...writing index into response queue */
	int32_t	rsp_write_idx;
	/* ...reading index for command queue */
	uint32_t	cmd_read_idx;
};

/* ...shared memory data */
typedef struct xf_shmem_data	{
	struct xf_proxy_host_data	local;
	struct xf_proxy_dsp_data	remote;
	uint8_t	*buffer;

}	xf_shmem_data_t;

struct hifi_effect_info_stru {
	unsigned int	effect_id;
	char		effect_name[64];
};

/* voice bsd param hsm struct */
	struct voice_bsd_param_hsm {
		unsigned int data_len;
		unsigned char *pdata;
	};

	enum hifi_om_work_id {
		HIFI_OM_WORK_VOICE_BSD = 0,
		HIFI_OM_WORK_AUDIO_OM_DETECTION,
		HIFI_OM_WORK_MAX,
	};

	struct hifi_om_work_ctl {
		struct workqueue_struct *wq;
		struct work_struct work;
		spinlock_t lock;
		struct list_head list;
	};

	struct hifi_om_work_info {
		int work_id;
		char *work_name;
		work_func_t func;
		struct hifi_om_work_ctl ctl;
	};

	struct hifi_om_ap_data {
		unsigned short msg_id;
		unsigned int data_len;
		unsigned char data[0];
	};

	struct hifi_om_work {
		struct list_head om_node;
		unsigned int data_len;
		unsigned char data[0];
	};

#define HIFI_STAMP (unsigned int)readl(g_om_data.dsp_time_stamp)

#define can_reset_system() \
do {\
	if (g_om_data.reset_system) {\
		printk("soc hifi reset, reset all system by reset option");\
		BUG_ON(true);\
	} \
} while (0);

#define logd(fmt, ...) \
do {\
	if (g_om_data.debug_level >= 3) {\
		printk(LOG_TAG"[D][%u]:%s:%d: "fmt, HIFI_STAMP, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
	} \
} while (0);

#define logi(fmt, ...) \
do {\
	if (g_om_data.debug_level >= 2) {\
		printk(LOG_TAG"[I][%u]:%s:%d: "fmt, HIFI_STAMP, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
	} \
} while (0);

#define logw(fmt, ...) \
do {\
	if (g_om_data.debug_level >= 1) {\
		printk(LOG_TAG"[W][%u]:%s:%d: "fmt, HIFI_STAMP, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
	} \
} while (0);

#define loge(fmt, ...) \
do {\
		printk(LOG_TAG"[E][%u]:%s:%d: "fmt, HIFI_STAMP, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
} while (0);

#define IN_FUNCTION   logd("begin.\n");
#define OUT_FUNCTION  logd("end.\n");

int load_hifi_img_by_misc(void);
/*******************************************************************************
 * Helper macros
 ******************************************************************************/
/* ...memory barrier */
#define XF_PROXY_BARRIER()                  \
	barrier()
/* ...memory invalidation */
#define XF_PROXY_INVALIDATE(buf, length)    \
	({ barrier(); buf; })
/* ...memory flushing */
#define XF_PROXY_FLUSH(buf, length)         \
	({ barrier(); buf; })
/*******************************************************************************
 * Accessors
 ******************************************************************************/
/* ...shared memory data accessor */
#define XF_SHMEM_DATA(shmem)                \
	(shmem)
/* ...atomic reading */
#define __XF_PROXY_READ_ATOMIC(var)         \
	(var)
/* ...atomic writing */
#define __XF_PROXY_WRITE_ATOMIC(var, value) \
	((var) = (value))
/* ...accessors */
#define XF_PROXY_READ(shmem, field)          \
	__XF_PROXY_READ_##field(XF_SHMEM_DATA(shmem))
#define XF_PROXY_WRITE(shmem, field, v)      \
	__XF_PROXY_WRITE_##field(XF_SHMEM_DATA(shmem), (v))
/* ...individual fields reading */
#define __XF_PROXY_READ_cmd_write_idx(shmem)        \
	__XF_PROXY_READ_ATOMIC(shmem->local.cmd_write_idx)
#define __XF_PROXY_READ_cmd_read_idx(shmem)         \
	shmem->remote.cmd_read_idx
#define __XF_PROXY_READ_rsp_write_idx(shmem)        \
	shmem->remote.rsp_write_idx
#define __XF_PROXY_READ_rsp_read_idx(shmem)         \
	__XF_PROXY_READ_ATOMIC(shmem->local.rsp_read_idx)
/* ...individual fields writings */
#define __XF_PROXY_WRITE_cmd_write_idx(shmem, v)    \
	__XF_PROXY_WRITE_ATOMIC(shmem->local.cmd_write_idx, v)
#define __XF_PROXY_WRITE_cmd_read_idx(shmem, v)     \
	__XF_PROXY_WRITE_ATOMIC(shmem->remote.cmd_read_idx, v)
#define __XF_PROXY_WRITE_rsp_read_idx(shmem, v)     \
	__XF_PROXY_WRITE_ATOMIC(shmem->local.rsp_read_idx, v)
#define __XF_PROXY_WRITE_rsp_write_idx(shmem, v)    \
	__XF_PROXY_WRITE_ATOMIC(shmem->remote.rsp_write_idx, v)
/* ...command buffer accessor */
#define XF_PROXY_COMMAND(shmem, idx)                \
	(&XF_SHMEM_DATA(shmem)->local.command[(idx)])
/* ...response buffer accessor */
#define XF_PROXY_RESPONSE(shmem, idx)               \
	(&XF_SHMEM_DATA(shmem)->remote.response[(idx)])
/*******************************************************************************
 * Ring buffer support
 ******************************************************************************/
/* ...total length of shared memory queue (for commands and responses) */
#define XF_PROXY_MESSAGE_QUEUE_LENGTH   (1 << 8)
/* ...index mask */
#define XF_PROXY_MESSAGE_QUEUE_MASK     0xFF
#define __XF_QUEUE_IDX(idx, counter)    \
	(((idx) & XF_PROXY_MESSAGE_QUEUE_MASK) | ((counter) << 16))
/* ...retrieve ring-buffer index */
#define XF_QUEUE_IDX(idx)               \
	((idx) & XF_PROXY_MESSAGE_QUEUE_MASK)
/* ...increment ring-buffer index */
#define XF_QUEUE_ADVANCE_IDX(idx)       \
	(((idx) + 0x10001) & (0xFFFF0000 | XF_PROXY_MESSAGE_QUEUE_MASK))
/* ...test if ring buffer is empty */
#define XF_QUEUE_EMPTY(read, write)     \
	((read) == (write))
/* ...test if ring buffer is full */
#define XF_QUEUE_FULL(read, write)      \
	((write) == (read) + (XF_PROXY_MESSAGE_QUEUE_LENGTH << 16))

void hifi_om_init(struct platform_device *dev,
	unsigned char *hifi_priv_base_virt,
	unsigned char *hifi_priv_base_phy);
void hifi_om_deinit(struct platform_device *dev);
int hifi_dsp_dump_hifi(void __user *arg);
void hifi_dump_panic_log(void);
bool hifi_is_loaded(void);
void ap_ipc_int_init(wait_queue_head_t *xaf_waitq_lp);
unsigned int poll_om(struct file *filp, wait_queue_head_t *xaf_waitq,
	 poll_table *wait);
void hifi_om_effect_mcps_info_show(
	struct hifi_om_effect_mcps_stru *hifi_mcps_info);
void hifi_om_cpu_load_info_show(
	struct hifi_om_load_info_stru *hifi_om_info);
void hifi_om_update_buff_delay_info_show(
	struct hifi_om_update_buff_delay_info *info);
int hifi_get_dmesg(void __user *arg);
int hifi_om_get_voice_bsd_param(void __user *uaddr);
void hifi_om_rev_data_handle(int type, const unsigned char *addr,
	unsigned int len);
int send_pcm_data_to_dsp(void __user *buf, unsigned int size);
int xf_write(xf_proxy_message_usr_t  *xaf_msg);
ssize_t xf_read(xf_proxy_message_usr_t *xaf_msg,
		wait_queue_head_t *wq, void __user *data32);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
