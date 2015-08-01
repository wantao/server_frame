#ifndef __SHARE_INCLUDE_H__
#define __SHARE_INCLUDE_H__

//////////////////////////////////////////////////////////////////////////
//// include
//////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdarg.h>

#include <new>
#include <vector>
#include <string>
#include <iostream>
#include <list>
#include <algorithm>
#include <map>
#include <set>
#include <sstream>

//#if (defined(WIN32) || defined(WIN64))
//    #include <hash_map>
//    namespace std{ using namespace stdext;}
//#else
//    #define hash_map map
//#endif


#include "MarkupSTL.h"

#if (defined(WIN32) || defined(WIN64))
#include <process.h>
#include <WinSock2.h>
#include <MSWSock.h>
#include <io.h> 
#include <Mstcpip.h>
#pragma comment(lib, "WS2_32.lib")
#else
#include <wchar.h>
#include <wctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#endif

using namespace std;

#endif //__SHARE_INCLUDE_H__


