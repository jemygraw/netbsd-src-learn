我比对了一下netbsd的echo程序和自己写的echo程序。
1. netbsd的echo程序
```c
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int, char *[]);
int
main(int argc, char *argv[])
{
	int nflag;

	if (*++argv && !strcmp(*argv, "-n")) {
		++argv;
		nflag = 1;
	}
	else
		nflag = 0;

	while (*argv) {
		(void)printf("%s", *argv);
		if (*++argv)
			(void)putchar(' ');
	}
	if (nflag == 0)
		(void)putchar('\n');
	fflush(stdout);
	if (ferror(stdout))
		exit(1);
	exit(0);
}
```
我的echo程序
```c
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    int nflag=0;
    if(*++argv && !strcmp(*argv,"-n"))
    {
        nflag=1;
    }

    if(nflag)
    {
        argv++;
    }
    while(*argv)
    {
        printf("%s",*argv);
        if(*++argv)
        {
            putchar(' ');
        }
    }
    if(!nflag){
        putchar('\n');
    }
}
```c
首先，我发现下面的程序段可以整合到第一个if语句里面。
```c
if(nflag)
{
	argv++;
}
```
这个很明显是一个可以优化的地方，另外netbsd的echo程序在每一次调用函数
的时候，如果不需要这个函数的返回值就会使用`(void)`前缀放在调用函数的
前面，表示这个返回值被丢弃了。另外netbsd程序还检查了输出错误，即输出
到stdout的函数调用也有可能失败，所以先用fflush来清空缓冲区，然后检查
输出是否成功了，如果成功了返回0，否则返回1。