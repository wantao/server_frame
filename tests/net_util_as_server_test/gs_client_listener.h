#ifndef __GS_CLIENT_LISTENER_H__
#define __GS_CLIENT_LISTENER_H__
#include "sh_listener.h"
#include "gs_client.h"

class CClientListener:public CListener
{
    public:
        CClientListener(const char* ip,int port):CListener(ip,port){};
        ~CClientListener(){};

    public:
        sh::int32 on_accept(struct bufferevent* event,evutil_socket_t fd,struct sockaddr_in* sa);
};
#endif
