/*
 *      Video Encode driver for hikey960   Log
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

#include <linux/kthread.h>
#include "drv_venc.h"

static unsigned int  g_VencPrintEnable = 0xf;

char   *pszMsg[((unsigned char)VENC_ALW) + 1] = {"FATAL","ERR","WARN","IFO","DBG"}; /*lint !e785 */
char g_VencPrintMsg[1024];

void HI_PRINT(unsigned int type,char *file, int line , char * function,  char*  msg, ... )
{
	va_list args;
	unsigned int uTotalChar;

	if ( ((1 << type) & g_VencPrintEnable) == 0 && (type != VENC_ALW) ) /*lint !e701 */
		return ;

	va_start(args, msg);

	uTotalChar = vsnprintf(g_VencPrintMsg, sizeof(g_VencPrintMsg), msg, args); //������args��msg����ת���γɸ�ʽ���ַ�������������ʾ���ַ�����
	g_VencPrintMsg[sizeof(g_VencPrintMsg) - 1] = '\0';

	va_end(args);

	if (uTotalChar <= 0 || uTotalChar >= 1023) /*lint !e775 */
		return;

	printk(KERN_ALERT "%s:<%d:%s>%s \n",pszMsg[type],line,function,g_VencPrintMsg);
	return;
}

