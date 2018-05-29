#include "drv_mailbox_cfg.h"
#include "drv_mailbox_gut.h"

#undef	_MAILBOX_FILE_
#define _MAILBOX_FILE_	 "table"
MAILBOX_EXTERN struct mb_cfg g_mailbox_global_cfg_tbl[MAILBOX_GLOBAL_CHANNEL_NUM
						      + 1] = {
	MAILBOX_CHANNEL_COMPOSE(CCPU, HIFI, MSG),

	MAILBOX_CHANNEL_COMPOSE(ACPU, HIFI, MSG),

	MAILBOX_CHANNEL_COMPOSE(HIFI, CCPU, MSG),
	MAILBOX_CHANNEL_COMPOSE(HIFI, ACPU, MSG),

	{MAILBOX_MAILCODE_INVALID, 0, 0, 0}

};

MAILBOX_EXTERN struct mb_buff
    g_mailbox_channel_handle_pool[MAILBOX_CHANNEL_NUM];

MAILBOX_EXTERN struct mb_cb g_mailbox_user_cb_pool[MAILBOX_USER_NUM];
