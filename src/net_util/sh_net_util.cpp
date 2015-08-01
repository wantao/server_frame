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
#include<sys/stat.h>
#include "sh_net_util.h"

//格式化socket地址
sh::int32 init_sa_in(sockaddr_in* addr_in, const char* ip_str, sh::int32 port) 
{
	if (addr_in == NULL || ip_str == NULL || port < 1) {
        return -1;
    }
	memset(addr_in, 0, sizeof(sockaddr_in));
	addr_in->sin_family = AF_INET;
	addr_in->sin_port = htons(port);
	addr_in->sin_addr.s_addr = inet_addr(ip_str);
    return 0;
}

//设置socket block or not
sh::int32 set_sock_noblock(sh::int32 fd,bool blocking) 
{

	sh::int32 flag = fcntl(fd, F_GETFL);
	if (flag == -1) {
		return -1;
	}

	if (blocking) {
		flag &= (~O_NONBLOCK);
	} else {
		flag |= O_NONBLOCK;
	}

	sh::int32 rv = fcntl(fd, F_SETFL, flag);

	return rv;
}

//设置socket是否延迟发送
sh::int32 set_nodelay(sh::int32 fd, bool nodelay) 
{

	sh::int32 flag;
	if (nodelay) {
		flag = 1;
	} else {
		flag = 0;
	}

	sh::int32 rv = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));

	return rv;
}

//设置地址可重用
sh::int32 set_reuseaddr(sh::int32 fd, bool reuse) 
{
	sh::int32 flag;

	if (reuse) {
		flag = 1;
	} else {
		flag = 0;
	}

	sh::int32 rv = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

	return rv;
}

//绑定且开始监听
sh::int32 start_tcp_service(sockaddr_in* addr)
{
	if (addr == NULL) 
        return -1;
	sh::int32 sockfd = socket(AF_INET,SOCK_STREAM,0);
	if (sockfd < 0 ) 
        return -1;
	set_sock_noblock(sockfd,false);
	set_reuseaddr(sockfd,true);
	if (bind(sockfd,(sockaddr*)addr,sizeof(sockaddr))!=0) {
		close(sockfd);
		return -2;
	}
	if (listen(sockfd,1024)!=0) {
		close(sockfd);
		return -3;
	}
	return sockfd;
}



