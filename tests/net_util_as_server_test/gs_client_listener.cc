#include "gs_client_listener.h"
#include "sh_filelog.h"

sh::int32 CClientListener::on_accept(struct bufferevent* event,evutil_socket_t fd,struct sockaddr_in* sa)
{
    CGsClient* client = new CGsClient(event,fd,sa);
    client->init_cb();
    USER_LOG_INFO("trigger client accept in fd "<<fd);
    return 1;
}

