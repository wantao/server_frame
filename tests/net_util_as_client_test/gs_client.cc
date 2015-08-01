#include "gs_client.h"
#include "sh_filelog.h"

CGsClient::CGsClient()
{
}

CGsClient::~CGsClient()
{
}

std::string CGsClient::create_data_packet(sh::uint32 data_len)
{
    m_send_data.clear();
    std::string data_body = "a";
    m_send_data.append((char*)&data_len,sizeof(sh::int32));
    sh::uint32 i = 0;
    while (i < data_len) {
        m_send_data.append(data_body.c_str(),data_body.size());
        ++i;
    }   
    return m_send_data;
}

sh::int32 CGsClient::process_msg(const char* data, const sh::int32 data_size)
{
    //消息id
    /*sh::int16 msg_id = ntohs(*((sh::uint16*)(data)));
    USER_LOG_DEBUG("Recive message type:"<<msg_id);
    CSingleton<CClientActionsMananger>::get_instance()->do_action(this,msg_id,data+sizeof(sh::uint16),data_size-sizeof(sh::uint16));*/
    //USER_LOG_INFO("recvMsg:"<<data);
    send_msg(m_send_data.c_str(),m_send_data.size());
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



