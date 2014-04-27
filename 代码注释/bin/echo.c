/* $NetBSD: echo.c,v 1.18 2008/09/18 05:42:08 dholland Exp $	*/

/*
 * Copyright (c) 1989, 1993
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
__COPYRIGHT(
"@(#) Copyright (c) 1989, 1993\
 The Regents of the University of California.  All rights reserved.");
#endif /* not lint */

#ifndef lint
#if 0
static char sccsid[] = "@(#)echo.c	8.1 (Berkeley) 5/31/93";
#else
__RCSID("$NetBSD: echo.c,v 1.18 2008/09/18 05:42:08 dholland Exp $");
#endif
#endif /* not lint */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int, char *[]);

/* ARGSUSED */
int
main(int argc, char *argv[])
{
	int nflag;

	setprogname(argv[0]);
	(void)setlocale(LC_ALL, "");

	/* This utility may NOT do getopt(3) option parsing. */
	//将指针移到第二个参数，检查是否是-n，如果是则表示echo输出结束不输出换行
	if (*++argv && !strcmp(*argv, "-n")) {
		++argv;
		nflag = 1;
	}
	else
		nflag = 0;

	//依次输出后面的参数，以空格分开
	while (*argv) {
		(void)printf("%s", *argv);
		if (*++argv)
			(void)putchar(' ');
	}
	//输出尾部换行
	if (nflag == 0)
		(void)putchar('\n');
	fflush(stdout);
	if (ferror(stdout))
		exit(1);
	exit(0);
	/* NOTREACHED */
}
