/*	$NetBSD: expand.c,v 1.13 2009/04/12 02:51:36 lukem Exp $	*/

/*
 * Copyright (c) 1980, 1993
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
__COPYRIGHT("@(#) Copyright (c) 1980, 1993\
 The Regents of the University of California.  All rights reserved.");
#endif /* not lint */

#ifndef lint
#if 0
static char sccsid[] = "@(#)expand.c	8.1 (Berkeley) 6/9/93";
#endif
__RCSID("$NetBSD: expand.c,v 1.13 2009/04/12 02:51:36 lukem Exp $");
#endif /* not lint */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <err.h>

/*
 * expand - expand tabs to equivalent spaces
 */
size_t	nstops;
size_t	tabstops[100];

static	void	getstops(const char *);
	int	main(int, char **);
static	void	usage(void) __dead;

int
main(int argc, char *argv[])
{
	int c;
	size_t n, column;

	setprogname(argv[0]);

	//支持旧的语法格式
	//expand -20 file.txt
	while (argc > 1 &&
	    argv[1][0] == '-' && isdigit((unsigned char)argv[1][1])) {
		getstops(&argv[1][1]);
		argc--; argv++;
	}

	while ((c = getopt (argc, argv, "t:")) != -1) {
		switch (c) {
		case 't':
			getstops(optarg);
			break;
		case '?':
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	do {
		//如果指定的输入是文件，那么将文件打开，重定向到stdin
		//这样做的原因是可以和下面处理stdin输入的功能共享代码
		if (argc > 0) {
			if (freopen(argv[0], "r", stdin) == NULL)
				err(EXIT_FAILURE, "Cannot open `%s'", argv[0]);
			argc--, argv++;
		}
		column = 0;
		while ((c = getchar()) != EOF) {
			switch (c) {
			case '\t':
				//如果没有指定tabstop，则补充输出(8-column)个空格后，检查下一个字符
				if (nstops == 0) {
					do {
						putchar(' ');
						column++;
					} while (column & 07);
					continue;
				}
				//如果指定了一个tabstop，那么输出最多(tabstop-column)个空格后，检查下一个字符
				if (nstops == 1) {
					do {
						putchar(' ');
						column++;
					} while (((column - 1) % tabstops[0])
					    != (tabstops[0] - 1));
					continue;
				}
				//如果指定了多个tabstop，将指针移到tabstop大于column的位置
				for (n = 0; n < nstops; n++){
					if (tabstops[n] > column)
						break;
				}
				//如果指定的tabstop都小于column的值，那么输出一个空格后，检查下一个字符
				if (n == nstops) {
					putchar(' ');
					column++;
					continue;
				}
				//否则的话，补充输出(tabstops[n]-column)个空格
				while (column < tabstops[n]) {
					putchar(' ');
					column++;
				}
				continue;

			case '\b':
				//遇到Backplace，则行减1，然后输出'\b'
				if (column)
					column--;
				putchar('\b');
				continue;
			case '\n':
				//遇到新的一行，输出行尾，然后重置column为0
				putchar(c);
				column = 0;
				continue;
			default:
				//如果都不匹配上面的字符，那么输出这个字符，然后自增column值
				putchar(c);
				column++;
				continue;			
			}
		}
	} while (argc > 0);
	return EXIT_SUCCESS;
}

static void
getstops(const char *spec)
{
	int i;
	const char *cp = spec;

	nstops = 0;
	for (;;) {
		i = 0;
		//这个while循环在遇到空格或者逗号的时候都会停止
		//所以是每次for循环来从tablist里面获取一个tab宽度
		while (*cp >= '0' && *cp <= '9'){
			i = i * 10 + *cp++ - '0';
		}
		//检查所支持的tab宽度
		if (i <= 0 || i > 256)
			errx(EXIT_FAILURE, "Too large tab stop spec `%d'", i);

		//后面的tabstops必须大于前面的tabstops
		if (nstops > 0 && (size_t)i <= tabstops[nstops-1])
			errx(EXIT_FAILURE, "Out of order tabstop spec `%d'", i);

		//检查tabstops的数量，最多支持array_size-2个tabstop参数
		if (nstops == sizeof(tabstops) / sizeof(tabstops[0]) - 1)
			errx(EXIT_FAILURE, "Too many tabstops");
		//将每个tabstops设置进数组
		tabstops[nstops++] = i;
		//达到字符串结尾
		if (*cp == '\0')
			break;
		//如果tabstops的分隔符不是逗号或者空格，报错
		if (*cp != ',' && *cp != ' ')
			errx(EXIT_FAILURE, "Illegal tab stop spec `%s'", spec);
		cp++;
	}
}

static void
usage(void)
{

	(void)fprintf(stderr, "Usage: %s [-t tablist] [file ...]\n",
	    getprogname());
	exit(EXIT_FAILURE);
}
