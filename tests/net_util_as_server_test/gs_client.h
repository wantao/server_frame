#ifndef __GS_CLIENT_H__
#define __GS_CLIENT_H__

#include "sh_socket.h"
#include "sh_dbsql.h"

class CGsClient: public CSocketClient
{
    public:
        //bufferevent* bev,int socket,sockaddr_in& addr
        CGsClient(bufferevent* bev,evutil_socket_t socket,sockaddr_in* addr
                ):CSocketClient(bev,socket,addr)
        {
            init();
        }
        virtual ~CGsClient();

        void init();

    public:
        sh::int32 process_msg(const char* data, const sh::int32 data_size);
        void on_connection(bufferevent* bev);
        void on_error(bufferevent* ev);

};

#endif
