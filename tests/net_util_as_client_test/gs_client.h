#ifndef __GS_CLIENT_H__
#define __GS_CLIENT_H__

#include "sh_socket.h"
#include "sh_dbsql.h"

class CGsClient: public CSocketClient
{
    public:
        CGsClient();
        virtual ~CGsClient();

    public:
        std::string create_data_packet(sh::uint32 data_len);
        sh::int32 process_msg(const char* data, const sh::int32 data_size);
        void on_connection(bufferevent* bev);
        void on_error(bufferevent* ev);
private:
    std::string m_send_data;

};

#endif
