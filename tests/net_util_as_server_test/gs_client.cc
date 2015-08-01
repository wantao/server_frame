#include "gs_client.h"
#include "gs_client_listener.h"
#include "sh_filelog.h"

CGsClient::~CGsClient()
{
}

sh::int32 CGsClient::process_msg(const char* data, const sh::int32 data_size)
{
    //消息id
    /*sh::int16 msg_id = ntohs(*((sh::uint16*)(data)));
    USER_LOG_DEBUG("Recive message type:"<<msg_id);
    CSingleton<CClientActionsMananger>::get_instance()->do_action(this,msg_id,data+sizeof(sh::uint16),data_size-sizeof(sh::uint16));*/
    //数据处理
    std::string ret_data_packet;
    ret_data_packet.append((char*)&data_size,sizeof(sh::int32));
    ret_data_packet.append(data,data_size);
    send_msg(ret_data_packet.c_str(),ret_data_packet.size());
    return 1;
}

void CGsClient::on_error(bufferevent* ev)
{
    //错误处理
    USER_LOG_ERROR("CGsClient::on_error");
}

void CGsClient::on_connection(bufferevent* bev)
{
    //连接处理
    USER_LOG_ERROR("CGsClient::on_connection");        
}

void CGsClient::init()
{
    USER_LOG_INFO("CGsClient::init some user info!!!");
}


