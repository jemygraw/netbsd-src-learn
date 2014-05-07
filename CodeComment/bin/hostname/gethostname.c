#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "roken.h"

#ifndef HAVE_GETHOSTNAME

#ifdef HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>
#endif

/*
 返回本机的完整主机名，最大namelen个字符。如果namelen很大的话，name将以NULL结尾。
 也就是主机名长度小于namelen的时候，以NULL来补全。
 这个函数成功时返回0，否则返回-1。
 */

int		ROKEN_LIB_FUNCTION
gethostname(char *name, int namelen)
{
#if defined(HAVE_UNAME)
	{
		struct utsname	utsname;
		int		ret;

		ret = uname(&utsname);
		if (ret < 0)
			return ret;
		strlcpy(name, utsname.nodename, namelen);
		return 0;
	}
#else
			strlcpy       (name, "some.random.host", namelen);
	return 0;
#endif
}

#endif				/** GETHOSTNAME */

/*
  utsname结构体在sys/utsname.h里面定义 
  struct utsname{
  	char sysname[_SYS_NMLN]; 系统名称 uname -s
  	char nodename[_SYS_NMLN]; 主机节点名称 uname -n
 	char release[_SYS_NMLN]; 系统发布版本 uame -r
  	char version[_SYS_NMLN]; 系统版本 uname -v
  	char machine[_SYS_NMLN]; 系统硬件架构 uname -m 
 };
 */
