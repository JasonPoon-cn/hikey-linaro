/*
 * hifi om.
 *
 * Copyright (c) 2013 Hisilicon Technologies CO., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

/*
 *
 * Modifications made by Cadence Design Systems, Inc.  06/21/2017
 * Copyright (C) 2017 Cadence Design Systems, Inc.All rights reserved worldwide.
 *
 */

#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>
#include <linux/platform_device.h>
#include <linux/rtc.h>
#include <linux/syscalls.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <asm/memory.h>
#include <asm/types.h>
#include <asm/io.h>

#include <linux/time.h>
#include <linux/timex.h>
#include <linux/rtc.h>

#include "audio_hifi.h"
#include "hifi_lpp.h"
#include "hifi_om.h"
#include "drv_mailbox_msg.h"
#include <linux/hisi/hisi_rproc.h>
#include <dsm/dsm_pub.h>
#include <linux/hisi/rdr_pub.h>
#include <linux/delay.h>
#include <linux/firmware.h>
#include "bsp_drv_ipc.h"

#define HI_DECLARE_SEMAPHORE(name) \
	struct semaphore name = __SEMAPHORE_INITIALIZER(name, 0)
HI_DECLARE_SEMAPHORE(hifi_log_sema);
struct hifi_om_s g_om_data;
static struct proc_dir_entry *hifi_debug_dir;

#define MAX_LEVEL_STR_LEN 32
#define UNCONFIRM_ADDR (0)
static struct hifi_dsp_dump_info s_dsp_dump_info[] = {
	{DSP_NORMAL, DUMP_DSP_LOG, FILE_NAME_DUMP_DSP_LOG, UNCONFIRM_ADDR,
	 (DRV_DSP_UART_TO_MEM_SIZE - DRV_DSP_UART_TO_MEM_RESERVE_SIZE)},
	{DSP_NORMAL, DUMP_DSP_BIN, FILE_NAME_DUMP_DSP_BIN, UNCONFIRM_ADDR,
	 HIFI_DUMP_BIN_SIZE},
	{DSP_PANIC, DUMP_DSP_LOG, FILE_NAME_DUMP_DSP_PANIC_LOG, UNCONFIRM_ADDR,
	 (DRV_DSP_UART_TO_MEM_SIZE - DRV_DSP_UART_TO_MEM_RESERVE_SIZE)},
	{DSP_PANIC, DUMP_DSP_BIN, FILE_NAME_DUMP_DSP_PANIC_BIN, UNCONFIRM_ADDR,
	 HIFI_DUMP_BIN_SIZE},
	{DSP_PANIC, DUMP_DSP_BIN, FILE_NAME_DUMP_DSP_OCRAM_BIN, UNCONFIRM_ADDR,
	 HIFI_IMAGE_OCRAMBAK_SIZE},
	{DSP_PANIC, DUMP_DSP_BIN, FILE_NAME_DUMP_DSP_TCM_BIN, UNCONFIRM_ADDR,
	 HIFI_IMAGE_TCMBAK_SIZE},
};

static struct hifi_effect_info_stru effect_algo[] = {
	{ID_EFFECT_ALGO_FORMATER, "FORMATER"},

	{ID_EFFECT_ALGO_FORTE_VOICE_SPKOUT, "FORTE_VOICE_SPKOUT"},
	{ID_EFFECT_ALGO_FORTE_VOICE_MICIN, "FORTE_VOICE_MICIN"},
	{ID_EFFECT_ALGO_FORTE_VOICE_SPKOUT_BWE, "FORTE_VOICE_SPKOUT_BWE"},

	{ID_EFFECT_ALGO_FORTE_VOIP_MICIN, "FORTE_VOIP_MICIN"},
	{ID_EFFECT_ALGO_FORTE_VOIP_SPKOUT, "FORTE_VOIP_SPKOUT"},

	{ID_EFFECT_ALGO_IN_CONVERT_I2S_GENERAL, "IN_CONVERT_I2S_GENERAL"},
	{ID_EFFECT_ALGO_IN_CONVERT_I2S_HI363X, "IN_CONVERT_I2S_HI363X"},

	{ID_EFFECT_ALGO_INTERLACE, "INTERLACE"},

	{ID_EFFECT_ALGO_OUT_CONVERT_I2S_GENERAL, "OUT_CONVERT_I2S_GENERAL"},
	{ID_EFFECT_ALGO_OUT_CONVERT_I2S_HI363X, "OUT_CONVERT_I2S_HI363X"},

	{ID_EFFECT_ALGO_SWAP, "SWAP"},

	{ID_EFFECT_ALGO_IMEDIA_WNR_MICIN, "IMEDIA_WNR_MICIN"},
	{ID_EFFECT_ALGO_IMEDIA_WNR_SPKOUT, "IMEDIA_WNR_SPKOUT"},

	{ID_EFFECT_ALGO_SWS_INTERFACE, "SWS_INTERFACE"},
	{ID_EFFECT_ALGO_DTS, "DTS"},
	{ID_EFFECT_ALGO_DRE, "DRE"},
	{ID_EFFECT_ALGO_CHC, "CHC"},
	{ID_EFFECT_ALGO_SRC, "SRC"},
	{ID_EFFECT_ALGO_TTY, "TTY"},

	{ID_EFFECT_ALGO_KARAOKE_RECORD, "KARAOKE_RECORD"},
	{ID_EFFECT_ALGO_KARAOKE_PLAY, "KARAOKE_PLAY"},

	{ID_EFFECT_ALGO_MLIB_CS_VOICE_CALL_MICIN, "MLIB_CS_VOICE_CALL_MICIN"},
	{ID_EFFECT_ALGO_MLIB_CS_VOICE_CALL_SPKOUT, "MLIB_CS_VOICE_CALL_SPKOUT"},
	{ID_EFFECT_ALGO_MLIB_VOIP_CALL_MICIN, "MLIB_VOIP_CALL_MICIN"},
	{ID_EFFECT_ALGO_MLIB_VOIP_CALL_SPKOUT, "MLIB_VOIP_CALL_MICIN"},
	{ID_EFFECT_ALGO_MLIB_AUDIO_PLAY, "MLIB_AUDIO_PLAY"},
	{ID_EFFECT_ALGO_MLIB_AUDIO_RECORD, "MLIB_AUDIO_RECORD"},
	{ID_EFFECT_ALGO_MLIB_SIRI_MICIN, "MLIB_SIRI_MICIN"},
	{ID_EFFECT_ALGO_MLIB_SIRI_SPKOUT, "MLIB_SIRI_SPKOUT"},

	{ID_EFFECT_ALGO_EQ, "EQ"},
	{ID_EFFECT_ALGO_MBDRC6402, "MBDRC6402"},

	{ID_EFFECT_ALGO_IMEDIA_VOIP_MICIN, "IMEDIA_VOIP_MICIN"},
	{ID_EFFECT_ALGO_IMEDIA_VOIP_SPKOUT, "IMEDIA_VOIP_SPKOUT"},
	{ID_EFFECT_ALGO_IMEDIA_VOICE_CALL_MICIN, "IMEDIA_VOICE_CALL_MICIN"},
	{ID_EFFECT_ALGO_IMEDIA_VOICE_CALL_SPKOUT, "IMEDIA_VOICE_CALL_SPKOUT"},
	{ID_EFFECT_ALGO_IMEDIA_VOICE_CALL_SPKOUT_BWE,
	 "IMEDIA_VOICE_CALL_SPKOUT_BWE"},
};

static void hifi_om_voice_bsd_work_handler(struct work_struct *work);
static void hifi_om_show_audio_detect_info(struct work_struct *work);

static struct hifi_om_work_info work_info[] = {
	{HIFI_OM_WORK_VOICE_BSD, "hifi_om_work_voice_bsd",
	 hifi_om_voice_bsd_work_handler, {0} },
	{HIFI_OM_WORK_AUDIO_OM_DETECTION, "hifi_om_work_audio_om_detect",
	 hifi_om_show_audio_detect_info, {0} },
};

extern struct dsm_client *dsm_audio_client;

static void hifi_get_time_stamp(char *timestamp_buf, unsigned int len)
{
	struct timeval tv = { 0 };
	struct rtc_time tm = { 0 };

	BUG_ON(NULL == timestamp_buf);

	memset(&tv, 0, sizeof(struct timeval));
	memset(&tm, 0, sizeof(struct rtc_time));

	do_gettimeofday(&tv);
	tv.tv_sec -= sys_tz.tz_minuteswest * 60;
	rtc_time_to_tm(tv.tv_sec, &tm);

	snprintf(timestamp_buf, len, "%04d%02d%02d%02d%02d%02d",
		 tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
		 tm.tm_hour, tm.tm_min, tm.tm_sec);

	return;
}

static int hifi_chown(char *path, uid_t user, gid_t group)
{
	int ret = 0;
	mm_segment_t old_fs;

	if (NULL == path)
		return -1;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	ret = (int)sys_chown((const char __user *)path, user, group);
	if (ret) {
		loge("chown %s uid [%d] gid [%d] failed error [%d]!\n", path,
		     user, group, ret);
	}

	set_fs(old_fs);

	return ret;
}

static int hifi_create_dir(char *path)
{
	int fd = -1;
	mm_segment_t old_fs = 0;

	if (!path) {
		loge("path(%pK) is invailed\n", path);
		return -1;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	fd = sys_access(path, 0);
	if (0 != fd) {
		logi("need create dir %s.\n", path);
		fd = sys_mkdir(path, HIFI_OM_DIR_LIMIT);
		if (fd < 0) {
			set_fs(old_fs);
			loge("create dir %s fail, ret: %d.\n", path, fd);
			return fd;
		}
		logi("create dir %s successed, fd: %d.\n", path, fd);

		/* hifi_log dir limit root-system */
		if (hifi_chown(path, ROOT_UID, SYSTEM_GID)) {
			loge("chown %s failed!\n", path);
		}
	}

	set_fs(old_fs);

	return 0;
}

static int hifi_om_create_log_dir(char *path)
{
	char cur_path[HIFI_DUMP_FILE_NAME_MAX_LEN];
	int index = 0;

	if (!path || (strlen(path) + 1) > HIFI_DUMP_FILE_NAME_MAX_LEN) {
		loge("path(%pK) is invailed\n", path);
		return -1;
	}

	if (0 == sys_access(path, 0))
		return 0;

	memset(cur_path, 0, HIFI_DUMP_FILE_NAME_MAX_LEN);

	if (*path != '/')
		return -1;

	cur_path[index++] = *path++;

	while (*path != '\0') {
		if (*path == '/') {
			if (hifi_create_dir(cur_path)) {
				loge("create dir %s failed\n", cur_path);
				return -1;
			}
		}
		cur_path[index++] = *path++;
	}

	return 0;

}

int hifi_om_get_voice_bsd_param(void __user *uaddr)
{
	int ret = OK;
	int work_id = HIFI_OM_WORK_VOICE_BSD;
	struct hifi_om_work *om_work = NULL;
	unsigned char data[MAIL_LEN_MAX] = { '\0' };
	unsigned int data_len = 0;
	struct voice_bsd_param_hsm param;

	memset(&param, 0, sizeof(param));
	if (copy_from_user(&param, uaddr, sizeof(param))) {
		loge("copy_from_user failed\n");
		ret = -EFAULT;
		goto exit;
	}

	if (!param.pdata) {
		loge("user buffer is null\n");
		ret = -EINVAL;
		goto exit;
	}

	spin_lock_bh(&work_info[work_id].ctl.lock);
	if (!list_empty(&work_info[work_id].ctl.list)) {
		om_work =
		    list_entry(work_info[work_id].ctl.list.next,
			       struct hifi_om_work, om_node);

		data_len = om_work->data_len;
		memcpy(data, om_work->data, om_work->data_len);

		list_del(&om_work->om_node);
		kzfree(om_work);
	}
	spin_unlock_bh(&work_info[work_id].ctl.lock);

	if (param.data_len < data_len) {
		loge("userspace len(%u) is less than data_len(%u)\n",
		     param.data_len, data_len);
		ret = -EINVAL;
		goto exit;
	}

	if (copy_to_user((void __user *)param.pdata, data, data_len)) {
		loge("copy_to_user failed\n");
		ret = -EFAULT;
		goto exit;
	}
	logd("size(%u)copy to user success\n", data_len);

 exit:

	return ret;
}

static void hifi_om_voice_bsd_work_handler(struct work_struct *work)
{
	int retval = 0;
	char *envp[2] = { "hifi_voice_bsd_param", NULL };

	retval = kobject_uevent_env(&g_om_data.dev->kobj, KOBJ_CHANGE, envp);
	if (retval) {
		loge("send uevent failed, retval: %d\n", retval);
		return;
	}
	logi("report uevent success\n");

	return;
}

void hifi_om_rev_data_handle(int work_id, const unsigned char *addr,
			     unsigned int len)
{
	struct hifi_om_work *work = NULL;

	if (!addr || 0 == len || len > MAIL_LEN_MAX) {
		loge("addr is null or len is invaled, len: %u", len);
		return;
	}

	work = kzalloc(sizeof(*work) + len, GFP_ATOMIC);
	if (!work) {
		loge("malloc size %zu failed\n", sizeof(*work) + len);
		return;
	}
	memcpy(work->data, addr, len);
	work->data_len = len;

	spin_lock_bh(&work_info[work_id].ctl.lock);
	list_add_tail(&work->om_node, &work_info[work_id].ctl.list);
	spin_unlock_bh(&work_info[work_id].ctl.lock);

	if (!queue_work
	    (work_info[work_id].ctl.wq, &work_info[work_id].ctl.work)) {
		loge("work_id: %d, This work was already on the queue\n",
		     work_id);
	}

	return;
}

static void hifi_om_show_audio_detect_info(struct work_struct *work)
{
	int work_id = HIFI_OM_WORK_AUDIO_OM_DETECTION;
	struct hifi_om_work *om_work = NULL;
	unsigned char data[MAIL_LEN_MAX] = { '\0' };
	unsigned int data_len = 0;
	unsigned int hifi_msg_type = 0;
	struct hifi_om_load_info_stru hifi_om_info;
	struct hifi_om_effect_mcps_stru mcps_info;
	struct hifi_om_update_buff_delay_info update_buff_delay_info;

	spin_lock_bh(&work_info[work_id].ctl.lock);
	if (!list_empty(&work_info[work_id].ctl.list)) {
		om_work =
		    list_entry(work_info[work_id].ctl.list.next,
			       struct hifi_om_work, om_node);

		data_len = om_work->data_len;
		memcpy(data, om_work->data, om_work->data_len);

		list_del(&om_work->om_node);
		kzfree(om_work);
	}
	spin_unlock_bh(&work_info[work_id].ctl.lock);

	memset(&hifi_om_info, 0, sizeof(hifi_om_info));
	memset(&mcps_info, 0, sizeof(mcps_info));
	memset(&update_buff_delay_info, 0, sizeof(update_buff_delay_info));

	hifi_msg_type = *(unsigned int *)data;

	switch (hifi_msg_type) {
	case HIFI_CPU_OM_LOAD_INFO:
		if ((sizeof(hifi_om_info)) != data_len) {
			logw("unavailable data from hifi, data_len: %u\n",
			     data_len);
			return;
		}
		memcpy(&hifi_om_info, data, sizeof(hifi_om_info));

		hifi_om_cpu_load_info_show(&hifi_om_info);
		break;
	case HIFI_CPU_OM_ALGO_MCPS_INFO:
		if ((sizeof(mcps_info)) != data_len) {
			logw("unavailable data from hifi, data_len: %u\n",
			     data_len);
			return;
		}
		memcpy(&mcps_info, data, sizeof(mcps_info));

		hifi_om_effect_mcps_info_show(&mcps_info);
		break;
	case HIFI_CPU_OM_UPDATE_BUFF_DELAY_INFO:
		if ((sizeof(update_buff_delay_info)) != data_len) {
			logw("unavailable data from hifi, data_len: %u\n",
			     data_len);
			return;
		}
		memcpy(&update_buff_delay_info, data,
		       sizeof(update_buff_delay_info));

		hifi_om_update_buff_delay_info_show(&update_buff_delay_info);
		break;
	default:
		logi("type(%d), not support\n", hifi_msg_type);
		break;
	}

	return;
}

int hifi_get_dmesg(void __user *arg)
{
	int ret = OK;
	unsigned int len = 0;
	struct misc_io_dump_buf_param dump_info;
	void __user *dump_info_user_buf = NULL;

	len = (unsigned int)(*g_om_data.dsp_log_cur_addr);
	if (len > DRV_DSP_UART_TO_MEM_SIZE) {
		loge("len is larger: %d(%d), don't dump log\n", len,
		     DRV_DSP_UART_TO_MEM_SIZE);
		return 0;
	}

	if (copy_from_user
	    (&dump_info, arg, sizeof(struct misc_io_dump_buf_param))) {
		loge("copy_from_user fail, don't dump log\n");
		return 0;
	}

	if (dump_info.buf_size == 0) {
		loge("input buf size is zero, don't dump log\n");
		return 0;
	}

	if (len > dump_info.buf_size) {
		logw("input buf size smaller, input buf size: %d, log size: %d, contiue to dump using smaller size\n", dump_info.buf_size, len);
		len = dump_info.buf_size;
	}

	dump_info_user_buf =
	    INT_TO_ADDR(dump_info.user_buf_l, dump_info.user_buf_h);
	if (!dump_info_user_buf) {
		loge("input dump buff addr is null\n");
		return 0;
	}
	logi("get msg: len:%d from:%pK to:%pK.\n", len,
	     s_dsp_dump_info[0].data_addr, dump_info_user_buf);

	s_dsp_dump_info[0].data_addr = g_om_data.dsp_log_addr;

	ret =
	    copy_to_user(dump_info_user_buf, s_dsp_dump_info[0].data_addr, len);
	if (OK != ret) {
		loge("copy_to_user fail: %d\n", ret);
		len -= ret;
	}

	if (dump_info.clear) {
		*g_om_data.dsp_log_cur_addr = DRV_DSP_UART_TO_MEM_RESERVE_SIZE;
		if (s_dsp_dump_info[0].data_len > DRV_DSP_UART_TO_MEM_SIZE) {
			loge("s_dsp_dump_info[0].data_len is larger than DRV_DSP_UART_TO_MEM_SIZE\n");
			len = 0;
		} else {
			memset(s_dsp_dump_info[0].data_addr, 0,
			       s_dsp_dump_info[0].data_len);
		}
	}

	return (int)len;
}

static void hifi_dump_dsp(DUMP_DSP_INDEX index)
{
	int ret = 0;

	mm_segment_t fs = 0;
	struct file *fp = NULL;
	int file_flag = O_RDWR;
	struct kstat file_stat;
	int write_size = 0;
	unsigned int err_no = 0xFFFFFFFF;

	char tmp_buf[64] = { 0 };
	unsigned long tmp_len = 0;
	struct rtc_time cur_tm;
	struct timespec now;

	char path_name[HIFI_DUMP_FILE_NAME_MAX_LEN] = { 0 };
	char *file_name = s_dsp_dump_info[index].file_name;
	char *data_addr = NULL;
	unsigned int data_len = s_dsp_dump_info[index].data_len;

	char *is_panic = "i'm panic.\n";
	char *is_exception = "i'm exception.\n";
	char *not_panic = "i'm ok.\n";

	memset(path_name, 0, HIFI_DUMP_FILE_NAME_MAX_LEN);

	if (down_interruptible(&g_om_data.dsp_dump_sema) < 0) {
		loge("acquire the semaphore error.\n");
		return;
	}

	IN_FUNCTION;

	hifi_get_log_signal();

	s_dsp_dump_info[NORMAL_LOG].data_addr =
	    g_om_data.dsp_log_addr + DRV_DSP_UART_TO_MEM_RESERVE_SIZE;
	s_dsp_dump_info[PANIC_LOG].data_addr =
	    g_om_data.dsp_log_addr + DRV_DSP_UART_TO_MEM_RESERVE_SIZE;

	if (index == OCRAM_BIN) {
		s_dsp_dump_info[index].data_addr =
		    (unsigned char *)ioremap_wc(HIFI_OCRAM_BASE_ADDR,
						HIFI_IMAGE_OCRAMBAK_SIZE);
	}
	if (index == TCM_BIN) {
		s_dsp_dump_info[index].data_addr =
		    (unsigned char *)ioremap_wc(HIFI_TCM_BASE_ADDR,
						HIFI_IMAGE_TCMBAK_SIZE);
	}

	if (NULL == s_dsp_dump_info[index].data_addr) {
		loge("dsp log ioremap fail.\n");
		goto END;
	}

	data_addr = s_dsp_dump_info[index].data_addr;

	fs = get_fs();
	set_fs(KERNEL_DS);

	ret = hifi_om_create_log_dir(LOG_PATH_HIFI_LOG);
	if (0 != ret) {
		goto END;
	}

	snprintf(path_name, HIFI_DUMP_FILE_NAME_MAX_LEN, "%s%s",
		 LOG_PATH_HIFI_LOG, file_name);

	ret = vfs_stat(path_name, &file_stat);
	if (ret < 0) {
		logi("there isn't a dsp log file:%s, and need to create.\n",
		     path_name);
		file_flag |= O_CREAT;
	}

	fp = filp_open(path_name, file_flag, HIFI_OM_FILE_LIMIT);
	if (IS_ERR(fp)) {
		loge("open file fail: %s.\n", path_name);
		fp = NULL;
		goto END;
	}

	/*write from file start */
	vfs_llseek(fp, 0, SEEK_SET);

	/*write file head */
	if (DUMP_DSP_LOG == s_dsp_dump_info[index].dump_type) {
		/*write dump log time */
		now = current_kernel_time();
		rtc_time_to_tm(now.tv_sec, &cur_tm);

		memset(tmp_buf, 0, sizeof(tmp_buf));
		tmp_len =
		    snprintf(tmp_buf, sizeof(tmp_buf),
			     "%04d-%02d-%02d %02d:%02d:%02d.\n",
			     cur_tm.tm_year + 1900, cur_tm.tm_mon + 1,
			     cur_tm.tm_mday, cur_tm.tm_hour, cur_tm.tm_min,
			     cur_tm.tm_sec);
		vfs_write(fp, tmp_buf, tmp_len, &fp->f_pos);

		/*write exception no */
		memset(tmp_buf, 0, sizeof(tmp_buf));
		err_no = (unsigned int)(*(g_om_data.dsp_exception_no));
		if (err_no != 0xFFFFFFFF) {
			tmp_len =
			    snprintf(tmp_buf, sizeof(tmp_buf),
				     "the exception no: %u.\n", err_no);
		} else {
			tmp_len =
			    snprintf(tmp_buf, sizeof(tmp_buf), "%s",
				     "hifi is fine, just dump log.\n");
		}

		vfs_write(fp, tmp_buf, tmp_len, &fp->f_pos);

		/*write error type */
		if (0xdeadbeaf == *g_om_data.dsp_panic_mark) {
			vfs_write(fp, is_panic, strlen(is_panic), &fp->f_pos);
		} else if (0xbeafdead == *g_om_data.dsp_panic_mark) {
			vfs_write(fp, is_exception, strlen(is_exception),
				  &fp->f_pos);
		} else {
			vfs_write(fp, not_panic, strlen(not_panic), &fp->f_pos);
		}
	}

	/*write dsp info */
	if ((write_size = vfs_write(fp, data_addr, data_len, &fp->f_pos)) < 0) {
		loge("write file fail.\n");
	}

	/* hifi.log file limit root-system */
	if (hifi_chown(path_name, ROOT_UID, SYSTEM_GID)) {
		loge("chown %s failed!\n", path_name);
	}

	logi("write file size: %d.\n", write_size);

 END:
	if (fp) {
		filp_close(fp, 0);
	}
	set_fs(fs);

	if ((index == OCRAM_BIN || index == TCM_BIN)
	    && (NULL != s_dsp_dump_info[index].data_addr)) {
		iounmap(s_dsp_dump_info[index].data_addr);
		s_dsp_dump_info[index].data_addr = NULL;
	}

	hifi_release_log_signal();

	up(&g_om_data.dsp_dump_sema);
	OUT_FUNCTION;

	return;
}

static debug_level_com s_debug_level_com[4] = { {'d', 3}, {'i', 2}, {'w', 1}, {'e', 0} };

static unsigned int hifi_get_debug_level_num(char level_char)
{
	int i = 0;
	int len = sizeof(s_debug_level_com) / sizeof(s_debug_level_com[0]);

	for (i = 0; i < len; i++) {
		if (level_char == s_debug_level_com[i].level_char) {
			return s_debug_level_com[i].level_num;
		}
	}

	return 2;		/*info */
}

static char hifi_get_debug_level_char(char level_num)
{
	int i = 0;
	int len = sizeof(s_debug_level_com) / sizeof(s_debug_level_com[0]);

	for (i = 0; i < len; i++) {
		if (level_num == s_debug_level_com[i].level_num) {
			return s_debug_level_com[i].level_char;
		}
	}

	return 'i';		/*info */
}

static void hifi_set_dsp_debug_level(unsigned int level)
{
	*(unsigned int *)g_om_data.dsp_debug_level_addr = level;
}

static ssize_t hifi_debug_level_show(struct file *file, char __user *buf,
				     size_t size, loff_t *data)
{
	char level_str[MAX_LEVEL_STR_LEN] = { 0 };

	if (NULL == buf) {
		loge("Input param buf is invalid\n");
		return -EINVAL;
	}

	snprintf(level_str, MAX_LEVEL_STR_LEN, "debug level: %c.\n",
		 hifi_get_debug_level_char(g_om_data.debug_level));

	return simple_read_from_buffer(buf, size, data, level_str,
				       strlen(level_str));
}

static ssize_t hifi_debug_level_store(struct file *file,
				      const char __user *buf, size_t size,
				      loff_t *data)
{
	ssize_t ret;
	char level_str[MAX_LEVEL_STR_LEN] = { 0 };
	loff_t pos = 0;

	if (NULL == buf) {
		loge("Input param buf is invalid\n");
		return -EINVAL;
	}
	ret =
	    simple_write_to_buffer(level_str, MAX_LEVEL_STR_LEN - 1, &pos, buf,
				   size);
	if (ret != size) {
		loge("Input param buf read error, return value: %zd\n", ret);
		return -EINVAL;
	}

	if (!strchr("diwe", level_str[0])) {
		loge("Input param buf is error(valid: d,i,w,e): %s.\n",
		     level_str);
		return -EINVAL;
	}
	if (level_str[1] != '\n') {
		loge("Input param buf is error, last char is not \\n .\n");
		return -EINVAL;
	}

	g_om_data.debug_level = hifi_get_debug_level_num(level_str[0]);
	return size;
}

static const struct file_operations hifi_debug_proc_ops = {
	.owner = THIS_MODULE,
	.read = hifi_debug_level_show,
	.write = hifi_debug_level_store,
};

#define HIKEY_MSG_HEAD_PROTECT_WORD 0xffff1234
#define HIKEY_MSG_BODY_PROTECT_WORD 0xffff4321

#define HIKEY_AP2DSP_MSG_QUEUE_ADDR HIFI_HIKEY_SHARE_ADDR
#define HIKEY_AP2DSP_MSG_QUEUE_SIZE 0x1800
#define HIKEY_DSP2AP_MSG_QUEUE_ADDR (HIKEY_AP2DSP_MSG_QUEUE_ADDR + HIKEY_AP2DSP_MSG_QUEUE_SIZE)
#define HIKEY_DSP2AP_MSG_QUEUE_SIZE 0x1800
static xf_shmem_data_t        *shmem;
static xf_proxy_message_usr_t     response_msg;

wait_queue_head_t	*xaf_waitq;
static xf_proxy_t           xf_proxy;

/*******************************************************************************
 * Messages API. All functions are requiring proxy lock taken
 ******************************************************************************/

/* ...allocate new message from the pool */
static inline xf_message_t *xf_msg_alloc(xf_proxy_t *proxy)
{
	xf_message_t	*m;

	if (list_empty(&proxy->free))
		return NULL;

	m = list_first_entry(&proxy->free, xf_message_t, node);
	list_del(&m->node);
	return m;
}

/* ...get message queue head */
static inline xf_message_t *xf_msg_queue_head(xf_msg_queue_t *queue)
{
	xf_message_t	*m = list_first_entry(&queue->entry, xf_message_t, node);
	return m;
}

/* ...submit message to a queue */
static inline int xf_msg_enqueue(xf_msg_queue_t *queue, xf_message_t *m)
{
	int first = list_empty(&queue->entry);

	list_add_tail(&m->node, &queue->entry);
	return first;
}

/* ...retrieve next message from the per-task queue */
static inline xf_message_t *xf_msg_dequeue(xf_msg_queue_t *queue)
{
	xf_message_t   *m;

	if (list_empty(&queue->entry))
		return NULL;
	m = list_first_entry(&queue->entry, xf_message_t, node);
	list_del(&m->node);
	return m;
}

/* ...initialize message queue */
static inline void xf_msg_queue_init(xf_msg_queue_t *queue)
{
	INIT_LIST_HEAD(&queue->entry);
}

static inline void xf_msg_free(xf_proxy_t *proxy, xf_message_t *m)
{
	list_add(&m->node, &proxy->free);
}

static inline void xf_cmap(xf_proxy_t *proxy, xf_message_t *m)
{
	/* ...place message into local response queue */
	xf_msg_enqueue(&proxy->response, m);
}

/* ...initialize shared memory interface */
static int xf_proxy_init(void)
{
	xf_proxy_t     *proxy = &xf_proxy;
	xf_message_t   *m;
	int             i;
	/* ...create a list of all messages in a pool; set head pointer */
	INIT_LIST_HEAD(&proxy->free);
	/* ...put all messages into a single-linked list */
	for (i = 0, m = &proxy->pool[0]; i < XF_CFG_MESSAGE_POOL_SIZE - 1; i++, m++)
		list_add_tail(&m->node, &proxy->free);
	/* ...initialize proxy thread message queues */
	xf_msg_queue_init(&proxy->response);

	/*...initialize proxy mutex */
	mutex_init(&proxy->xf_mutex);
	return 0;
}

/* ...retrieve pending responses from shared memory ring-buffer */
int xf_shmem_process_responses(xf_proxy_message_t *hikey_msg)
{
	uint32_t       read_idx, write_idx;
	xf_proxy_message_t  *response;
	xf_message_t   *m;
	xf_proxy_t         *proxy = &xf_proxy;

	if (!hikey_msg) {
		loge("have no memory to save hikey msg\n");
		return -1;
	}
	mutex_lock(&proxy->xf_mutex);
	/* ...get current values of read/write pointers in response queue */
	read_idx = XF_PROXY_READ(shmem, rsp_read_idx);
	write_idx = XF_PROXY_READ(shmem, rsp_write_idx);
	while (!XF_QUEUE_EMPTY(read_idx, write_idx)) {
		/* ...allocate execution message */
		m = xf_msg_alloc(proxy);
		if (m == NULL)
			break;
		/* ...get oldest not yet processed response */
		response = XF_PROXY_RESPONSE(shmem, XF_QUEUE_IDX(read_idx));
		/*  ...synchronize memory contents */
		XF_PROXY_INVALIDATE(response, sizeof(*response));
		/* ...fill message parameters */
		m->id = response->id;
		m->opcode = response->opcode;
		m->length = response->length;
		m->address = response->address;
		m->v_address = response->v_address;
		/* ...advance local reading index copy */
		read_idx = XF_QUEUE_ADVANCE_IDX(read_idx);
		/* ...update shadow copy of reading index */
		XF_PROXY_WRITE(shmem, rsp_read_idx, read_idx);
		/* ...submit message to proper client */
		xf_cmap(proxy, m);
	}
	mutex_unlock(&proxy->xf_mutex);
	return 0;
}

unsigned int poll_om(struct file *filp, wait_queue_head_t *xaf_waitq1,
	poll_table *wait)
{
	xf_proxy_t         *proxy = &xf_proxy;
	unsigned int             mask;
	xf_proxy_message_t hikey_msg;

	/* ...register client waiting queue */
	poll_wait(filp, xaf_waitq1, wait);

	/* ...process all pending responses */
	xf_shmem_process_responses(&hikey_msg);
	mutex_lock(&proxy->xf_mutex);
	mask = (xf_msg_queue_head(&proxy->response) ? POLLIN | POLLRDNORM : 0);
	mutex_unlock(&proxy->xf_mutex);
	return mask;
}

static DECLARE_COMPLETION(msg_completion);

void hikey_ap_msg_process(xf_proxy_message_t *hikey_msg)
{
	if (!hikey_msg) {
		loge("hikey msg is null\n");
		return;
	}
	response_msg.id = hikey_msg->id;
	response_msg.opcode = hikey_msg->opcode;
	response_msg.length = hikey_msg->length;
	response_msg.address = hikey_msg->address;
	response_msg.v_address = hikey_msg->v_address;
	complete(&msg_completion);

	return;
}


static void hikey_init_share_mem(char *share_mem_addr,
	unsigned int share_mem_size)
{
	if (!share_mem_addr) {
		loge("share memory is null\n");
		return;
	}
	memset(share_mem_addr, 0, share_mem_size);
	shmem = (xf_shmem_data_t *)share_mem_addr;
	/* initialize shared memory interface */
	XF_PROXY_WRITE(shmem, cmd_read_idx, 0);
	XF_PROXY_WRITE(shmem, cmd_write_idx, 0);
	XF_PROXY_WRITE(shmem, rsp_read_idx, 0);
	XF_PROXY_WRITE(shmem, rsp_write_idx, 0);

}

static void xf_cmd_send(xf_proxy_message_t *hikey_msg)
{
	uint32_t             read_idx, write_idx;
	xf_proxy_message_t *command;
	xf_proxy_t     *proxy = &xf_proxy;

	logi("Enter %s\n", __func__);
	if (!hikey_msg) {
		loge("msg is null\n");
		return;
	}
	mutex_lock(&proxy->xf_mutex);
	write_idx = XF_PROXY_READ(shmem, cmd_write_idx);
	read_idx = XF_PROXY_READ(shmem, cmd_read_idx);
	if (XF_QUEUE_FULL(read_idx, write_idx)) {
		loge("command queue is full\n");
		mutex_unlock(&proxy->xf_mutex);
		return;
	}
	/* ...select the place for the command */
	command = XF_PROXY_COMMAND(shmem, XF_QUEUE_IDX(write_idx));
	/* ...put the response message fields */
	command->id = hikey_msg->id;
	command->opcode = hikey_msg->opcode;
	command->length = hikey_msg->length;
	command->address = hikey_msg->address;
	command->v_address = hikey_msg->v_address;
	/* ...flush the content of the caches to main memory */
	XF_PROXY_FLUSH(command, sizeof(*command));
	/* ...advance local writing index copy */
	write_idx = XF_QUEUE_ADVANCE_IDX(write_idx);
	shmem->local.cmd_write_idx = write_idx;
	mutex_unlock(&proxy->xf_mutex);
	logi("Exit %s\n", __func__);
}

/*Interrupt receiver */
#define IPC_ACPU_INT_SRC_HIFI_MSG  (1)
#define K3_SYS_IPC_CORE_HIFI (4)
typedef void (*VOIDFUNCCPTR)(unsigned int);
static void _dsp_to_ap_ipc_irq_proc(void)
{
	logi("Enter %s\n", __func__);
	/*clear interrupt */
	DRV_k3IpcIntHandler_Autoack();
	wake_up(xaf_waitq);
	logi("Exit %s\n", __func__);
}
void ap_ipc_int_init(wait_queue_head_t *xaf_waitq_lp)
{
	logi("Enter %s\n", __func__);
	xaf_waitq = xaf_waitq_lp;
	xf_proxy_init();
	IPC_IntConnect(IPC_ACPU_INT_SRC_HIFI_MSG,
			(VOIDFUNCCPTR)_dsp_to_ap_ipc_irq_proc,
			IPC_ACPU_INT_SRC_HIFI_MSG);
	IPC_IntEnable(IPC_ACPU_INT_SRC_HIFI_MSG);
	logi("Exit %s\n", __func__);
}

/* ...pass command to remote DSP */
int xf_write(xf_proxy_message_usr_t  *xaf_msg)
{
	int ret;
	xf_proxy_message_t *hikey_msg = kmalloc(sizeof(*hikey_msg), GFP_KERNEL);

	if (!hikey_msg)
		return -ENOMEM;
	if (WARN_ON(xaf_msg->length > HIFI_MUSIC_DATA_SIZE)) {
		kfree(hikey_msg);
		return -EINVAL;
	}
	hikey_msg->id      = xaf_msg->id;
	hikey_msg->opcode  = xaf_msg->opcode;
	hikey_msg->length  = xaf_msg->length;
	hikey_msg->address = xaf_msg->address;

	/* ...send asynchronous command to dsp */
	xf_cmd_send(hikey_msg);

	/* ...send interrupt to dsp */
	ret = IPC_IntSend(K3_SYS_IPC_CORE_HIFI, 0);
	if (ret < 0)	{
		loge("Interrupt error\n");
		kfree(hikey_msg);
		return ret;
	}
	kfree(hikey_msg);

	return ret;
}

static inline xf_message_t *xf_msg_received(xf_proxy_t *proxy,
	xf_msg_queue_t *queue)
{
	xf_message_t   *m;
	/* ...try to peek message from the queue */
	m = xf_msg_dequeue(queue);
	if (m == NULL) {
		/* ...queue is empty; release lock */
		logi("xf_msg queue empty\n");
	}
	/* ...if message is non-null, lock is held */
	return m;
}
static inline xf_message_t *xf_cmd_recv(xf_proxy_t *proxy,
	wait_queue_head_t *wq, xf_msg_queue_t *queue, int wait)
{
	xf_message_t   *m;
	/* ...wait for message reception (take lock on success) */
	if (wait_event_interruptible(*wq, (m = xf_msg_received(proxy, queue)) != NULL || !wait))
		return ERR_PTR(-EINTR);

	/* ...return message with a lock taken */
	return m;
}

/* ...read next response message from proxy interface */
ssize_t xf_read(xf_proxy_message_usr_t *xaf_msg,
		wait_queue_head_t *wq, void __user *data32)
{
	/* ...get proxy interface */
	xf_proxy_t     *proxy = &xf_proxy;
	xf_message_t       *m;
	xf_proxy_message_usr_t msg;

	if (!proxy)
		return 0;

	mutex_lock(&proxy->xf_mutex);
	/* ...check if we have a pending response message (do not wait) */
	m = xf_cmd_recv(proxy, wq, &proxy->response, 0);
	mutex_unlock(&proxy->xf_mutex);
	if (IS_ERR(m))	{
		pr_err("receiving failed: %d", (int)PTR_ERR(m));
		return PTR_ERR(m);
	}

	/* ...check if there is a response available */
	if (m == NULL)
		return -EAGAIN;

	/* ...prepare message parameters */
	xaf_msg->id = m->id;
	xaf_msg->opcode = m->opcode;
	xaf_msg->length = m->length;
	xaf_msg->address = m->address;

	mutex_lock(&proxy->xf_mutex);
	/* ...return the message back to a pool */
	xf_msg_free(proxy, m);
	mutex_unlock(&proxy->xf_mutex);

	logi("read[id:%08x]: (%08x,%u,%08llx)", xaf_msg->id, xaf_msg->opcode, xaf_msg->length, xaf_msg->address);

	/* ...pass message to user */
	if (copy_to_user(data32, xaf_msg, sizeof(msg)))	{
		loge("HIFI couldn't copy xf_proxy_msg\n");
		return -EFAULT;
	}
	return sizeof(msg);
}
static int hifi_send_str_todsp(const char *cmd_str, size_t size)
{
	int           ret     = OK;
	xf_proxy_message_t *hikey_msg = kmalloc(sizeof(*hikey_msg) + size,
		GFP_KERNEL);
	unsigned char *music_buf = NULL;
	if (!hikey_msg)
		return -ENOMEM;
	BUG_ON(cmd_str == NULL);
	hikey_msg->id      = ID_AP_AUDIO_STR_CMD;
	hikey_msg->length  = size+1;
	hikey_msg->address = HIFI_MUSIC_DATA_LOCATION;
	music_buf = (unsigned char *)ioremap_wc(HIFI_MUSIC_DATA_LOCATION, size);
	memcpy((char *)music_buf, cmd_str, size);
	xf_cmd_send(hikey_msg);
	ret = IPC_IntSend(K3_SYS_IPC_CORE_HIFI, 0);
	if (ret < 0)	{
		loge("Interrupt error\n");
		kfree(hikey_msg);
		return ret;
	}
	kfree(hikey_msg);
	return ret;
}
#define FAULT_INJECT_OFFSET 0x100
int send_pcm_data_to_dsp(void *buf, unsigned int size)
{
	char cmd[40];
	int           ret     = OK;
	unsigned char *music_buf = NULL;
	unsigned char *pcm_buf = NULL;

	if (WARN_ON(size > HIFI_MUSIC_DATA_SIZE))
		return -EINVAL;

	music_buf = (unsigned char *)ioremap_wc(
		HIFI_MUSIC_DATA_LOCATION+FAULT_INJECT_OFFSET,
		HIFI_MUSIC_DATA_SIZE);
	ret = copy_from_user(music_buf, buf, size);
	iounmap(music_buf);
	if (ret) {
		loge("%s: couldn't copy buffer %p from user %p\n", __func__, music_buf, buf);
		return -EINVAL;
	}

	sprintf(cmd, "pcm_gain 0x8B300100 0x%08x", size);
	ret = hifi_send_str_todsp(cmd, strlen(cmd));
	if (ret < 0)
		return ret;

	wait_for_completion(&msg_completion);
	pcm_buf  = (unsigned char *)ioremap_wc(PCM_PLAY_BUFF_LOCATION, PCM_PLAY_BUFF_SIZE);
	ret = copy_to_user(buf, pcm_buf, size);

	if (ret != 0) {
		ret = -EINVAL;
		loge("%s: couldn't copy buffer %p to user %p\n", __func__, pcm_buf, buf);
	}

	iounmap(pcm_buf);
	return ret;

}

static ssize_t hifi_dsp_fault_inject_show(struct file *file, char __user *buf,
					  size_t size, loff_t *data)
{
	char useage[] =
	    "Useage:\necho \"test_case param1 param2 ...\" > dspfaultinject\n"
	    "test_case maybe:\n" "read_mem addr\n" "write_mem addr value\n"
	    "endless_loop\n"
	    "overload [type(1:RT 2:Nomal 3:Low else:default)]\n" "auto_reset\n"
	    "param_test\n" "exit\n"
	    "dyn_mem_leak [size count type(1:DDR 2:TCM)]\n"
	    "dyn_mem_overlap [type(1:DDR 2:TCM)]\n"
	    "stack_overflow [type(1:RT 2:Nomal 3:Low else:default)]\n"
	    "isr_ipc\n" "om_report_ctrl [type(on off status)]\n"
	    "performance_leak [count]\n" "mailbox_overlap\n" "msg_leak\n"
	    "msg_deliver_err\n" "isr_malloc\n" "power_off\n" "power_on\n"
	    "watchdog_timeout\n" "ulpp_ddr\n" "mailbox_full\n"
	    "repeat_start [count]\n" "stop_dma\n"
	    "set_codec [codec_type(enum VCVOICE_TYPE_ENUM 0-9)]\n"
	    "voice_abnormal [str(all set_codec bad_frame dtx_state gcell_quality ucell_quality)]\n"
	    "diagnose_apkreport [cause]\n" "policy_null [target_index]\n"
	    "performance_check [count]\n"
	    "dma_adjust [str(in out) len(0-16000)]\n"
	    "trigger_vqi [str(ber fer)]\n"
	    "fifo_vp_test [str(start_once start_period stop)]\n"
	    "volte_encrypt [str(on off)]\n" "timer_start [timer_count]\n"
	    "test_timer [str(dump_num dump_node dump_blk dump_blk normal_oneshot normal_loop normal_clear high_oneshot high_loop high_clear callback_release)]\n"
	    "cdma_test [str(dl)]\n" "modem_loop [str(on off)]\n"
	    "rtp_cn_package [str(on off)]\n" "usbvoice_stop_audio_micin\n"
	    "usbvoice_stop_audio_spkout\n" "usbvoice_start_audio_micin\n"
	    "usbvoice_start_audio_spkout\n" "usbvoice_stop_voice_tx\n"
	    "usbvoice_stop_voice_rx\n" "usbvoice_start_voice_tx\n"
	    "usbvoice_start_voice_rx\n" "apk_trigger [str(start stop)]\n"
	    "whistle\n" "offload_effect_switch [str(on off)]\n" "\n";

	if (!buf) {
		loge("Input param buf is invalid\n");
		return -EINVAL;
	}

	return simple_read_from_buffer(buf, size, data, useage, strlen(useage));
}

#define FAULT_INJECT_CMD_STR_MAX_LEN 200
static ssize_t hifi_dsp_fault_inject_store(struct file *file,
					   const char __user *buf, size_t size,
					   loff_t *data)
{
	ssize_t len = 0;
	loff_t pos = 0;
	char cmd_str[FAULT_INJECT_CMD_STR_MAX_LEN] = { 0 };

	if (!buf) {
		loge("param buf is NULL\n");
		return -EINVAL;
	}

	if (size > FAULT_INJECT_CMD_STR_MAX_LEN) {
		loge("input size(%zd) is larger than max-len(%d)\n", size,
		     FAULT_INJECT_CMD_STR_MAX_LEN);
		return -EINVAL;
	}

	memset(cmd_str, 0, sizeof(cmd_str));
	len =
	    simple_write_to_buffer(cmd_str, (FAULT_INJECT_CMD_STR_MAX_LEN - 1),
				   &pos, buf, size);
	if (len != size) {
		loge("write to buffer fail: %zd\n", len);
		return -EINVAL;
	}

	hifi_send_str_todsp(cmd_str, size);

	return size;
}

static const struct file_operations hifi_dspfaultinject_proc_ops = {
	.owner = THIS_MODULE,
	.read = hifi_dsp_fault_inject_show,
	.write = hifi_dsp_fault_inject_store,
};

struct image_partition_table addr_remap_tables[] = {
	{HIFI_RUN_DDR_REMAP_BASE, HIFI_RUN_DDR_REMAP_BASE + HIFI_RUN_SIZE,
	 HIFI_RUN_SIZE, HIFI_RUN_LOCATION},
	{HIFI_TCM_PHY_BEGIN_ADDR, HIFI_TCM_PHY_END_ADDR, HIFI_TCM_SIZE,
	 HIFI_IMAGE_TCMBAK_LOCATION},
	{HIFI_OCRAM_PHY_BEGIN_ADDR, HIFI_OCRAM_PHY_END_ADDR, HIFI_OCRAM_SIZE,
	 HIFI_IMAGE_OCRAMBAK_LOCATION}
};

extern void *memcpy64(void *dst, const void *src, unsigned len);
extern void *memcpy128(void *dst, const void *src, unsigned len);

void *memcpy_aligned(void *_dst, const void *_src, unsigned len)
{
	unsigned char *dst = _dst;
	const unsigned char *src = _src;
	unsigned int length = len;
	unsigned int cpy_len;

	if (((unsigned long)dst % 16 == 0) && ((unsigned long)src % 16 == 0)
	    && (length >= 16)) {
		cpy_len = length & 0xFFFFFFF0;
		memcpy128(dst, src, cpy_len);
		length = length % 16;
		dst = dst + cpy_len;
		src = src + cpy_len;

		if (length == 0)
			return _dst;
	}

	if (((unsigned long)dst % 8 == 0) && ((unsigned long)src % 8 == 0)
	    && (length >= 8)) {
		cpy_len = length & 0xFFFFFFF8;
		memcpy64(dst, src, cpy_len);
		length = length % 8;
		dst = dst + cpy_len;
		src = src + cpy_len;
		if (length == 0)
			return _dst;
	}

	if (((unsigned long)dst % 4 == 0) && ((unsigned long)src % 4 == 0)) {
		cpy_len = length >> 2;
		while (cpy_len-- > 0) {
			*(unsigned int *)dst = *(unsigned int *)src;
			dst += 4;
			src += 4;
		}
		length = length % 4;
		if (length == 0)
			return _dst;
	}

	while (length-- > 0)
		*dst++ = *src++;

	return _dst;
}

int load_hifi_img_by_misc(void)
{
	unsigned int i = 0;
	char *img_buf = NULL;
	struct drv_hifi_image_head *hifi_img = NULL;
	const struct firmware *hifi_firmware;
	xf_proxy_t         *proxy = &xf_proxy;

	mutex_lock(&proxy->xf_mutex);
	if (g_om_data.hifi3_firmware_load == true) {
		mutex_unlock(&proxy->xf_mutex);
		return 0;
	}

	loge("load hifi image now\n");

	if (request_firmware(&hifi_firmware, "hifi/hifi.img", g_om_data.dev) < 0) {
		loge("could not find firmware file hifi/hifi.img\n");
		mutex_unlock(&proxy->xf_mutex);
		return -ENOENT;
	}

	img_buf = (char *)hifi_firmware->data;

	hifi_img = (struct drv_hifi_image_head *)img_buf;
	logi("sections_num:%u, image_size:%u\n", hifi_img->sections_num,
	     hifi_img->image_size);

	for (i = 0; i < hifi_img->sections_num; i++) {
		unsigned int index = 0;
		unsigned long remap_dest_addr = 0;

		logi("sections_num:%u, i:%u\n", hifi_img->sections_num, i);
		logi("des_addr:0x%x, load_attib:%u, size:%u, sn:%hu, src_offset:%x, type:%u\n", hifi_img->sections[i].des_addr, hifi_img->sections[i].load_attib, hifi_img->sections[i].size, hifi_img->sections[i].sn, hifi_img->sections[i].src_offset, hifi_img->sections[i].type);

		remap_dest_addr = (unsigned long)hifi_img->sections[i].des_addr;
		if (remap_dest_addr >= HIFI_OCRAM_PHY_BEGIN_ADDR
		    && remap_dest_addr <= HIFI_OCRAM_PHY_END_ADDR) {
			index = 2;
		} else if (remap_dest_addr >= HIFI_TCM_PHY_BEGIN_ADDR
			   && remap_dest_addr <= HIFI_TCM_PHY_END_ADDR) {
			index = 1;
		} else {	/*(remap_addr >= HIFI_DDR_PHY_BEGIN_ADDR && remap_addr <= HIFI_DDR_PHY_END_ADDR) */
			index = 0;
		}
		remap_dest_addr -=
		    addr_remap_tables[index].phy_addr_start -
		    addr_remap_tables[index].remap_addr;

		if (hifi_img->sections[i].type != DRV_HIFI_IMAGE_SEC_TYPE_BSS) {
			unsigned int *iomap_dest_addr = NULL;

			if (hifi_img->sections[i].load_attib ==
			    (unsigned char)DRV_HIFI_IMAGE_SEC_UNLOAD) {
				logi("unload section\n");
				continue;
			}

			iomap_dest_addr =
			    (unsigned int *)ioremap(remap_dest_addr,
						    hifi_img->sections[i].size);
			if (!iomap_dest_addr) {
				loge("ioremap failed\n");
				release_firmware(hifi_firmware);
				mutex_unlock(&proxy->xf_mutex);
				return -1;
			}
			memcpy_aligned((void *)(iomap_dest_addr),
				       (void *)((char *)hifi_img +
						hifi_img->
						sections[i].src_offset),
				       hifi_img->sections[i].size);
			iounmap(iomap_dest_addr);
		}
	}

	g_om_data.dsp_loaded = true;
	g_om_data.hifi3_firmware_load = true;
	release_firmware(hifi_firmware);
	mutex_unlock(&proxy->xf_mutex);
	return 0;
}

#define RESET_OPTION_LEN 100

static ssize_t hifi_dsp_reset_option_show(struct file *file, char __user *buf,
					  size_t size, loff_t *data)
{
	char reset_option[RESET_OPTION_LEN] = { 0 };

	if (NULL == buf) {
		loge("Input param buf is invalid\n");
		return -EINVAL;
	}
	if (load_hifi_img_by_misc() == 0) {
		g_om_data.dsp_loaded = true;
		loge("g_om_data.dsp_loaded:%d\n", (int)g_om_data.dsp_loaded);
	}
	snprintf(reset_option, RESET_OPTION_LEN,
		 "reset_option: 0(reset mediasever) 1(reset system) current:%d.\n",
		 g_om_data.reset_system);

	return simple_read_from_buffer(buf, size, data, reset_option,
				       strlen(reset_option));
}

static ssize_t hifi_dsp_reset_option_store(struct file *file,
					   const char __user *buf, size_t size,
					   loff_t *data)
{
	ssize_t ret;
	char reset_option[RESET_OPTION_LEN] = { 0 };
	loff_t pos = 0;

	if (NULL == buf) {
		loge("Input param buf is invalid\n");
		return -EINVAL;
	}
	ret =
	    simple_write_to_buffer(reset_option, RESET_OPTION_LEN - 1, &pos,
				   buf, size);
	if (ret != size) {
		loge("Input param buf read error, return value: %zd\n", ret);
		return -EINVAL;
	}

	if (!strchr("01", reset_option[0])) {
		loge("Input param buf is error(valid: d,i,w,e): %s.\n",
		     reset_option);
		return -EINVAL;
	}
	if (reset_option[1] != '\n') {
		loge("Input param buf is error, last char is not \\n .\n");
		return -EINVAL;
	}

	g_om_data.reset_system = (reset_option[0] == '0') ? false : true;
	return size;
}

static const struct file_operations hifi_reset_option_proc_ops = {
	.owner = THIS_MODULE,
	.read = hifi_dsp_reset_option_show,
	.write = hifi_dsp_reset_option_store,
};

static ssize_t hifi_dsp_dump_log_show(struct file *file, char __user *buf,
				      size_t size, loff_t *ppos)
{
	ssize_t ret = 0;

	if (NULL == buf) {
		loge("Input param buf is invalid\n");
		return -EINVAL;
	}

	ret = simple_read_from_buffer(buf, size, ppos,
				      LOG_PATH_HIFI_LOG,
				      (strlen(LOG_PATH_HIFI_LOG) + 1));
	if (ret == (ssize_t) (strlen(LOG_PATH_HIFI_LOG) + 1)) {
		g_om_data.force_dump_log = true;
		up(&hifi_log_sema);
	}

	return ret;
}

static const struct file_operations hifi_dspdumplog_proc_ops = {
	.owner = THIS_MODULE,
	.read = hifi_dsp_dump_log_show,
};

static void create_hifidebug_proc_file(void)
{
	struct proc_dir_entry *ent_debuglevel;
	struct proc_dir_entry *ent_dspdumplog;
	struct proc_dir_entry *ent_dspfaultinject;
	struct proc_dir_entry *poc_reset_option;

	/* Creating read/write "status" entry */
	ent_debuglevel =
	    proc_create(HIFIDEBUG_LEVEL_PROC_FILE, S_IRUGO | S_IWUSR | S_IWGRP,
			hifi_debug_dir, &hifi_debug_proc_ops);
	ent_dspdumplog =
	    proc_create(HIFIDEBUG_DSPDUMPLOG_PROC_FILE, S_IRUGO, hifi_debug_dir,
			&hifi_dspdumplog_proc_ops);
	if ((ent_debuglevel == NULL) || (ent_dspdumplog == NULL)) {
		remove_proc_entry("hifidebug", 0);
		loge("create proc file fail\n");
		return;
	}
	ent_dspfaultinject =
	    proc_create(HIFIDEBUG_FAULTINJECT_PROC_FILE,
			S_IRUGO | S_IWUSR | S_IWGRP, hifi_debug_dir,
			&hifi_dspfaultinject_proc_ops);
	if (ent_dspfaultinject == NULL) {
		remove_proc_entry("hifidebug", 0);
		loge("create proc file fail\n");
	}

	poc_reset_option =
	    proc_create(HIFIDEBUG_RESETOPTION_PROC_FILE,
			S_IRUGO | S_IWUSR | S_IWGRP, hifi_debug_dir,
			&hifi_reset_option_proc_ops);
	if (poc_reset_option == NULL) {
		remove_proc_entry("hifidebug", 0);
		loge("create proc file fail\n");
	}

}

static void remove_hifidebug_proc_file(void)
{
	remove_proc_entry(HIFIDEBUG_LEVEL_PROC_FILE, hifi_debug_dir);
	remove_proc_entry(HIFIDEBUG_DSPDUMPLOG_PROC_FILE, hifi_debug_dir);
	remove_proc_entry(HIFIDEBUG_FAULTINJECT_PROC_FILE, hifi_debug_dir);
	remove_proc_entry(HIFIDEBUG_RESETOPTION_PROC_FILE, hifi_debug_dir);
}

static void hifi_create_procfs(void)
{
#ifdef ENABLE_HIFI_DEBUG
	hifi_debug_dir = proc_mkdir(HIFIDEBUG_PATH, NULL);
	if (hifi_debug_dir == NULL) {
		loge("Unable to create /proc/hifidebug directory\n");
		return;
	}
	create_hifidebug_proc_file();
#endif
}

static void hifi_remove_procfs(void)
{
#ifdef ENABLE_HIFI_DEBUG
	remove_hifidebug_proc_file();
	remove_proc_entry("hifidebug", 0);
#endif
}

static int hifi_dump_dsp_thread(void *p)
{
#define HIFI_TIME_STAMP_1S	  32768
#define HIFI_DUMPLOG_TIMESPAN (10 * HIFI_TIME_STAMP_1S)

	unsigned int exception_no = 0;
	unsigned int time_now = 0;
	unsigned int time_diff = 0;
	unsigned int *hifi_info_addr = NULL;
	unsigned int hifi_stack_addr = 0;
	int i;

	IN_FUNCTION;

	while (!kthread_should_stop()) {
		if (down_interruptible(&hifi_log_sema) != 0) {
			loge("hifi_dump_dsp_thread wake up err.\n");
		}
		time_now = (unsigned int)readl(g_om_data.dsp_time_stamp);
		time_diff = time_now - g_om_data.pre_dsp_dump_timestamp;
		g_om_data.pre_dsp_dump_timestamp = time_now;
		hifi_info_addr = g_om_data.dsp_stack_addr;

		exception_no = *(unsigned int *)(hifi_info_addr + 3);
		hifi_stack_addr = *(unsigned int *)(hifi_info_addr + 4);
		logi("errno:%x pre_errno:%x is_first:%d is_force:%d time_diff:%d ms.\n", exception_no, g_om_data.pre_exception_no, g_om_data.first_dump_log, g_om_data.force_dump_log, (time_diff * 1000) / HIFI_TIME_STAMP_1S);

		hifi_get_time_stamp(g_om_data.cur_dump_time,
				    HIFI_DUMP_FILE_NAME_MAX_LEN);

		if (exception_no < 40
		    && (exception_no != g_om_data.pre_exception_no)) {
			logi("panic addr:0x%x, cur_pc:0x%x, pre_pc:0x%x, cause:0x%x\n", *(unsigned int *)(hifi_info_addr), *(unsigned int *)(hifi_info_addr + 1), *(unsigned int *)(hifi_info_addr + 2), *(unsigned int *)(hifi_info_addr + 3));
			for (i = 0;
			     i <
			     (DRV_DSP_STACK_TO_MEM_SIZE / 2) / sizeof(int) / 4;
			     i += 4) {
				logi("0x%x: 0x%08x 0x%08x 0x%08x 0x%08x\n",
				     (hifi_stack_addr + i * 4),
				     *(hifi_info_addr + i),
				     *(hifi_info_addr + 1 + i),
				     *(hifi_info_addr + 2 + i),
				     *(hifi_info_addr + i + 3));
			}

			hifi_dump_dsp(PANIC_LOG);
			hifi_dump_dsp(PANIC_BIN);

			g_om_data.pre_exception_no = exception_no;
		} else if (g_om_data.first_dump_log || g_om_data.force_dump_log
			   || time_diff > HIFI_DUMPLOG_TIMESPAN) {
			hifi_dump_dsp(NORMAL_LOG);
			if (DSP_LOG_BUF_FULL != g_om_data.dsp_error_type) {	/*needn't dump bin when hifi log buffer full */
				hifi_dump_dsp(NORMAL_BIN);
			}
			g_om_data.first_dump_log = false;
		}

		hifi_info_addr = NULL;
	}
	OUT_FUNCTION;
	return 0;
}

void hifi_dump_panic_log(void)
{
	if (!g_om_data.dsp_loaded) {
		loge("hifi isn't loaded, errno: 0x%x .\n",
		     g_om_data.dsp_loaded_sign);
		return;
	}
	up(&hifi_log_sema);
	return;
}

static bool hifi_check_img_loaded(void)
{
	bool dsp_loaded = false;
	g_om_data.dsp_loaded_sign = *(g_om_data.dsp_loaded_indicate_addr);

	if (0xA5A55A5A == g_om_data.dsp_loaded_sign) {
		loge("hifi img is not be loaded.\n");
	} else if (g_om_data.dsp_loaded_sign > 0) {
		loge("hifi img is loaded fail: 0x%x.\n",
		     g_om_data.dsp_loaded_sign);
	} else {
		logi("hifi img be loaded.\n");
		dsp_loaded = true;
	}

	return dsp_loaded;
}

bool hifi_is_loaded(void)
{
	if (!g_om_data.dsp_loaded) {
		loge("hifi isn't load, errno is 0x%x.\n",
		     g_om_data.dsp_loaded_sign);
	}
	return g_om_data.dsp_loaded;
}

int hifi_dsp_dump_hifi(void __user *arg)
{
	unsigned int err_type = 0;

	if (!arg) {
		loge("arg is null\n");
		return -1;
	}

	if (copy_from_user(&err_type, arg, sizeof(err_type))) {
		loge("copy_from_user fail, don't dump log\n");
		return -1;
	}
	g_om_data.dsp_error_type = err_type;
	g_om_data.force_dump_log = true;
	up(&hifi_log_sema);

	return 0;
}

void hifi_om_init(struct platform_device *pdev,
		  unsigned char *hifi_priv_base_virt,
		  unsigned char *hifi_priv_base_phy)
{
	int i = 0;
	BUG_ON(NULL == pdev);

	BUG_ON(NULL == hifi_priv_base_virt);
	BUG_ON(NULL == hifi_priv_base_phy);

	memset(&g_om_data, 0, sizeof(struct hifi_om_s));

	g_om_data.dev = &pdev->dev;
	g_om_data.debug_level = 1;	/* warning level */
	g_om_data.reset_system = false;

	g_om_data.dsp_time_stamp =
	    (unsigned int *)ioremap(SYS_TIME_STAMP_REG, 0x4);
	if (NULL == g_om_data.dsp_time_stamp) {
		pr_err("time stamp reg ioremap Error.\n");	/*can't use logx */
		return;
	}

	IN_FUNCTION;

	g_om_data.dsp_debug_level = 2;	/*info level */
	g_om_data.first_dump_log = true;

	g_om_data.dsp_panic_mark =
	    (unsigned int *)(hifi_priv_base_virt +
			     (DRV_DSP_PANIC_MARK - HIFI_UNSEC_BASE_ADDR));
	g_om_data.dsp_bin_addr =
	    (char *)(hifi_priv_base_virt +
		     (HIFI_DUMP_BIN_ADDR - HIFI_UNSEC_BASE_ADDR));
	g_om_data.dsp_exception_no =
	    (unsigned int *)(hifi_priv_base_virt +
			     (DRV_DSP_EXCEPTION_NO - HIFI_UNSEC_BASE_ADDR));
	g_om_data.dsp_log_cur_addr =
	    (unsigned int *)(hifi_priv_base_virt +
			     (DRV_DSP_UART_TO_MEM_CUR_ADDR -
			      HIFI_UNSEC_BASE_ADDR));
	g_om_data.dsp_log_addr =
	    (char *)(hifi_priv_base_virt +
		     (DRV_DSP_UART_TO_MEM - HIFI_UNSEC_BASE_ADDR));
	*g_om_data.dsp_log_cur_addr = DRV_DSP_UART_TO_MEM_RESERVE_SIZE;

	g_om_data.dsp_debug_level_addr =
	    (unsigned int *)(hifi_priv_base_virt +
			     (DRV_DSP_UART_LOG_LEVEL - HIFI_UNSEC_BASE_ADDR));
	g_om_data.dsp_debug_kill_addr =
	    (unsigned int *)(hifi_priv_base_virt +
			     (DRV_DSP_KILLME_ADDR - HIFI_UNSEC_BASE_ADDR));
	g_om_data.dsp_fama_config =
	    (struct drv_fama_config *)(hifi_priv_base_virt +
				       (DRV_DSP_SOCP_FAMA_CONFIG_ADDR -
					HIFI_UNSEC_BASE_ADDR));
	g_om_data.dsp_stack_addr =
	    (unsigned int *)(hifi_priv_base_virt +
			     (DRV_DSP_STACK_TO_MEM - HIFI_UNSEC_BASE_ADDR));
	g_om_data.dsp_loaded_indicate_addr =
	    (unsigned int *)(hifi_priv_base_virt +
			     (DRV_DSP_LOADED_INDICATE - HIFI_UNSEC_BASE_ADDR));

	*(g_om_data.dsp_exception_no) = ~0;
	g_om_data.pre_exception_no = ~0;
	hikey_init_share_mem((char *)(hifi_priv_base_virt +
				      (HIFI_HIKEY_SHARE_MEM_ADDR -
				       HIFI_UNSEC_BASE_ADDR)),
			     (unsigned int)HIFI_HIKEY_SHARE_SIZE);
	g_om_data.dsp_fama_config->head_magic = DRV_DSP_SOCP_FAMA_HEAD_MAGIC;
	g_om_data.dsp_fama_config->flag = DRV_DSP_FAMA_OFF;
	g_om_data.dsp_fama_config->rear_magic = DRV_DSP_SOCP_FAMA_REAR_MAGIC;

	s_dsp_dump_info[NORMAL_BIN].data_addr = g_om_data.dsp_bin_addr;
	s_dsp_dump_info[PANIC_BIN].data_addr = g_om_data.dsp_bin_addr;

	g_om_data.dsp_loaded = hifi_check_img_loaded();
	g_om_data.hifi3_firmware_load = false;
	hifi_set_dsp_debug_level(g_om_data.dsp_debug_level);

	sema_init(&g_om_data.dsp_dump_sema, 1);

	g_om_data.kdumpdsp_task =
	    kthread_create(hifi_dump_dsp_thread, 0, "dspdumplog");
	if (IS_ERR(g_om_data.kdumpdsp_task)) {
		loge("creat hifi dump log thread fail.\n");
	} else {
		wake_up_process(g_om_data.kdumpdsp_task);
	}

	hifi_create_procfs();

	for (i = 0; i < ARRAY_SIZE(work_info); i++) {
		work_info[i].ctl.wq =
		    create_singlethread_workqueue(work_info[i].work_name);
		if (!work_info[i].ctl.wq) {
			pr_err("%s(%u):workqueue create failed!\n",
			       __FUNCTION__, __LINE__);
		} else {
			INIT_WORK(&work_info[i].ctl.work, work_info[i].func);
			spin_lock_init(&work_info[i].ctl.lock);
			INIT_LIST_HEAD(&work_info[i].ctl.list);
		}
	}

	OUT_FUNCTION;
	return;
}

void hifi_om_deinit(struct platform_device *dev)
{
	int i = 0;

	IN_FUNCTION;

	BUG_ON(NULL == dev);

	up(&g_om_data.dsp_dump_sema);
	kthread_stop(g_om_data.kdumpdsp_task);

	if (NULL != g_om_data.dsp_time_stamp) {
		iounmap(g_om_data.dsp_time_stamp);
		g_om_data.dsp_time_stamp = NULL;
	}

	hifi_remove_procfs();

	for (i = 0; i < ARRAY_SIZE(work_info); i++) {
		if (work_info[i].ctl.wq) {
			flush_workqueue(work_info[i].ctl.wq);
			destroy_workqueue(work_info[i].ctl.wq);
			work_info[i].ctl.wq = NULL;
		}
	}

	OUT_FUNCTION;

	return;
}

void hifi_om_cpu_load_info_show(struct hifi_om_load_info_stru *hifi_om_info)
{
	switch (hifi_om_info->info_type) {
	case HIFI_CPU_LOAD_VOTE_UP:
	case HIFI_CPU_LOAD_VOTE_DOWN:
		logi("CpuUtilization:%d%%, Vote DDR to %dM\n",
		     hifi_om_info->cpu_load_info.cpu_load,
		     hifi_om_info->cpu_load_info.ddr_freq);
		break;

	case HIFI_CPU_LOAD_LACK_PERFORMANCE:
		logw("DDRFreq: %dM, CpuUtilization:%d%%, Lack of performance!!!\n", hifi_om_info->cpu_load_info.ddr_freq, hifi_om_info->cpu_load_info.cpu_load);
		/*upload totally 16 times in every 16 times in case of flushing msg */
		/*stop upload because it's nothing but waste resource
		   if (unlikely((dsm_notify_limit <= 0x100) && (dsm_notify_limit & 0xF))) {
		   if (!dsm_client_ocuppy(dsm_audio_client)) {
		   dsm_client_record(dsm_audio_client, "DSM_SOC_HIFI_HIGH_CPU\n");
		   dsm_client_notify(dsm_audio_client, DSM_SOC_HIFI_HIGH_CPU);
		   }
		   }
		   dsm_notify_limit++;
		 */
		break;

	default:
		break;
	}
}

void hifi_om_effect_mcps_info_show(struct hifi_om_effect_mcps_stru
				   *hifi_mcps_info)
{
	unsigned int i;

	logw("DDRFreq: %dM, CpuUtilization:%d%%\n",
	     hifi_mcps_info->cpu_load_info.ddr_freq,
	     hifi_mcps_info->cpu_load_info.cpu_load);

	for (i = 0;
	     i <
	     (sizeof(hifi_mcps_info->effect_mcps_info) /
	      sizeof(hifi_effect_mcps_stru)); i++) {
		if (hifi_mcps_info->effect_mcps_info[i].effect_algo_id <
		    ID_EFFECT_ALGO_BUTT
		    && hifi_mcps_info->effect_mcps_info[i].effect_algo_id >
		    ID_EFFECT_ALGO_START) {
			switch (hifi_mcps_info->effect_mcps_info[i].
				effect_stream_id) {
			case AUDIO_STREAM_PCM_OUTPUT:
				logw("Algorithm: %s, Mcps: %d, Stream: PCM_OUTPUT \n", effect_algo[hifi_mcps_info->effect_mcps_info[i].effect_algo_id - 1].effect_name, hifi_mcps_info->effect_mcps_info[i].effect_algo_mcps);
				break;

			case AUDIO_STREAM_PLAYER_OUTPUT:
				logw("Algorithm: %s, Mcps: %d, Stream: PLAYER_OUTPUT \n", effect_algo[hifi_mcps_info->effect_mcps_info[i].effect_algo_id - 1].effect_name, hifi_mcps_info->effect_mcps_info[i].effect_algo_mcps);
				break;

			case AUDIO_STREAM_MIXER_OUTPUT:
				logw("Algorithm: %s, Mcps: %d, Stream: MIXER_OUTPUT \n", effect_algo[hifi_mcps_info->effect_mcps_info[i].effect_algo_id - 1].effect_name, hifi_mcps_info->effect_mcps_info[i].effect_algo_mcps);
				break;

			case AUDIO_STREAM_VOICE_OUTPUT:
				logw("Algorithm: %s, Mcps: %d, Stream: VOICE_OUTPUT \n", effect_algo[hifi_mcps_info->effect_mcps_info[i].effect_algo_id - 1].effect_name, hifi_mcps_info->effect_mcps_info[i].effect_algo_mcps);
				break;

			case AUDIO_STREAM_VOICEPP_OUTPUT:
				logw("Algorithm: %s, Mcps: %d, Stream: VOICEPP_OUTPUT \n", effect_algo[hifi_mcps_info->effect_mcps_info[i].effect_algo_id - 1].effect_name, hifi_mcps_info->effect_mcps_info[i].effect_algo_mcps);
				break;

			case AUDIO_STREAM_PCM_INPUT:
				logw("Algorithm: %s, Mcps: %d, Stream: PCM_INPUT \n", effect_algo[hifi_mcps_info->effect_mcps_info[i].effect_algo_id - 1].effect_name, hifi_mcps_info->effect_mcps_info[i].effect_algo_mcps);
				break;

			case AUDIO_STREAM_VOICE_INPUT:
				logw("Algorithm: %s, Mcps: %d, Stream: VOICE_INPUT \n", effect_algo[hifi_mcps_info->effect_mcps_info[i].effect_algo_id - 1].effect_name, hifi_mcps_info->effect_mcps_info[i].effect_algo_mcps);
				break;

			case AUDIO_STREAM_VOICEPP_INPUT:
				logw("Algorithm: %s, Mcps: %d, Stream: VOICEPP_INPUT \n", effect_algo[hifi_mcps_info->effect_mcps_info[i].effect_algo_id - 1].effect_name, hifi_mcps_info->effect_mcps_info[i].effect_algo_mcps);
				break;

			default:
				break;
			}
		}
	}
}

void hifi_om_update_buff_delay_info_show(struct hifi_om_update_buff_delay_info
					 *info)
{

	logw("Hifi continuous update play/capture buff delay : %d(0-play, 1-capture)\n", info->pcm_mode);
}
