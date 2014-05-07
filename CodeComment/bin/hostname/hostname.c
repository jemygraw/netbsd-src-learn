/* $NetBSD: hostname.c,v 1.17 2011/08/29 14:51:18 joerg Exp $ */

/*
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#ifndef lint
__COPYRIGHT("@(#) Copyright (c) 1988, 1993\
 The Regents of the University of California.  All rights reserved.");
#endif /* not lint */

#ifndef lint
#if 0
static char sccsid[] = "@(#)hostname.c	8.2 (Berkeley) 4/28/95";
#else
__RCSID("$NetBSD: hostname.c,v 1.17 2011/08/29 14:51:18 joerg Exp $");
#endif
#endif /* not lint */

#include <sys/param.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

__dead static void usage(void);

int
main(int argc, char *argv[])
{
	//定义选项字符和标记
	int ch, sflag;
	//定义存储主机名的数组和指针
	char *p, hostname[MAXHOSTNAMELEN + 1];

	setprogname(argv[0]);
	sflag = 0;
	while ((ch = getopt(argc, argv, "s")) != -1)
		switch (ch) {
		case 's':
			sflag = 1;
			break;
		case '?':
		default:
			usage();
		}
	argc -= optind;
	argv += optind;

	//如果剩余的参数多于1个，打印用法信息
	if (argc > 1)
		usage();

	//如果指定了一个主机名参数
	if (*argv) {
		//设置主机名，注意主机名设置可能会失败，比如权限不够。
		if (sethostname(*argv, strlen(*argv)))
			err(1, "sethostname");
	} else {
		//如果没有指定主机名的话，那么获取主机名
		if (gethostname(hostname, sizeof(hostname)))
			err(1, "gethostname");
		//将主机名字符串的最后一个字符设置为NULL作为字符串截止符。
		//这里你或许会想是不是哪里有问题？对的，如果主机名长度小于
		//数组长度，不是有很多数组元素的值不确定么？其实，这个问题
		//在函数gethostname里面解决了，因为它使用strlcpy来拷贝
		//主机名，所以不需要手动去管理NULL了。

		//所以这行代码是否多余？
        //其实吧，这个语句是在主机名长度超过了数组长度的时候，用来
        //保证主机名字符串有截止符的。可以参考gethostname的源码。
		hostname[sizeof(hostname) - 1] = '\0';
		//如果指定了-s选项，表示去除主机名中的域信息，这里检测
		//字符数组的第一次出现'.'的位置，然后将其设置为NULL，
		//即用NULL来截断字符串。
		if (sflag && (p = strchr(hostname, '.')))
			*p = '\0';
		(void)printf("%s\n", hostname);
	}
	exit(0);
	/* NOTREACHED */
}

static void
usage(void)
{
	(void)fprintf(stderr, "usage: %s [-s] [name-of-host]\n",
	    getprogname());
	exit(1);
	/* NOTREACHED */
}
