#ifndef __SH_NET_UTIL_H__
#define __SH_NET_UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/prctl.h>
#include <cstddef>
#include <stdint.h>
#include <stdarg.h>
#include <sys/param.h>
#include <sys/stat.h>
#include "sh_type.h"

//格式化socket地址
sh::int32 init_sa_in(sockaddr_in* addr_in, const char* ip_str, sh::int32 port) ;

//设置socket block or not
sh::int32 set_sock_noblock(sh::int32 fd,bool blocking) ;

//设置socket是否延迟发送
sh::int32 set_nodelay(sh::int32 fd, bool nodelay) ;

//设置地址可重用
sh::int32 set_reuseaddr(sh::int32 fd, bool reuse) ;

//绑定且开始监听
sh::int32 start_tcp_service(sockaddr_in* addr);

#endif /* NET_UTIL_H_ */
