#include "sh_socket.h"
#include "sh_net_util.h"
#include "sh_include.h"
#include "sh_libevent_include.h"
#include "sh_filelog.h"
#include "sh_singleton.h"


void common_read_cb(struct bufferevent* ev,void *user_data)
{
    ((CSocketClient*)user_data)->on_read(ev);
}

void common_event_cb(struct bufferevent* ev,short error_no,void *user_data)
{
    if  (error_no == BEV_EVENT_CONNECTED) {
        USER_LOG_INFO("build connection");
        ((CSocketClient*)user_data)->on_connection(ev);
    } else {
        USER_LOG_INFO("lost connection "<<error_no);
        ((CSocketClient*)user_data)->on_disconnection(ev);
    }
}
void common_write_cb(struct bufferevent* ev,void *user_data)
{
    ((CSocketClient*)user_data)->on_write(ev);
}

CSocketClient::CSocketClient(bufferevent* bev,evutil_socket_t socket,sockaddr_in* addr):m_bev(bev),m_socket(socket)
{
    m_ip = inet_ntoa(addr->sin_addr);     
    m_port = addr->sin_port;
    m_data_buffer = new char [MAX_NET_PACKET_SIZE];   
    if (!m_data_buffer) {
        USER_LOG_ERROR("CSocketClient new data_buff failure");
    } else {
        _reset_some_flags();
    }
}

CSocketClient::CSocketClient()
{
    m_bev = CSingleton<CReactorCore>::get_instance()->get_new_socket_event();
    if (m_bev == NULL) {
        USER_LOG_ERROR("CSocketClient::CSocketClient() get_new_socket_event failure");
    }
    m_data_buffer = new char [MAX_NET_PACKET_SIZE];
    if (!m_data_buffer) {
        USER_LOG_ERROR("CSocketClient new data_buff failure");
    } else {
        _reset_some_flags();
    }
}

CSocketClient::~CSocketClient()
{
    if(m_bev != NULL) {
        bufferevent_free(m_bev);
    }
    if (m_socket > 0) {
        evutil_closesocket(m_socket);
        m_socket = 0;
    }
    if (m_data_buffer) {
        delete [] m_data_buffer;
    }
}

sh::int32 CSocketClient::check_packet_info(SPacketInfo* packet,struct evbuffer* read_buffer)
{
    if(evbuffer_get_length(read_buffer) == 0) {
        USER_LOG_INFO("read_buffer is not ready!!!");
        return 0;
    }
    //先读消息长度（4byte）和消息数据
    if (m_read_count == 1) {
        evbuffer_remove(read_buffer,(void*)&m_msg_body_len,sizeof(sh::int32));
        if (m_msg_body_len <= 0) {
            USER_LOG_ERROR("read_buffer m_msg_body_len error, msg_body_len:"<<m_msg_body_len<<"rea_count:"<<m_read_count);
            return -1;    
        }
        //网络包太大
        if (m_msg_body_len >= MAX_NET_PACKET_SIZE) {
            USER_LOG_ERROR("read_buffer m_msg_body_len is too large,msg_body_len:"<<m_msg_body_len<<" max_net_packet_size:"<<MAX_NET_PACKET_SIZE);
            return -1;
        }
        sh::int32 read_msg_body_len = evbuffer_remove(read_buffer,m_data_buffer,m_msg_body_len);
        if (read_msg_body_len <= 0) {
            USER_LOG_ERROR("read_buffer is empty,need msg_body_len:"<<m_msg_body_len<<"rea_count:"<<m_read_count);
            return -1;
        } else if (read_msg_body_len == m_msg_body_len) {
            packet->size = m_msg_body_len;
            packet->data = m_data_buffer;
            _reset_some_flags();
            return packet->size;
        } else {
            m_read_count += 1;
            m_has_read_len += read_msg_body_len;
            return 0;
        }
    } else {//再读取之前没有读取完的消息数据
        sh::int32 read_msg_body_len = evbuffer_remove(read_buffer,m_data_buffer+m_has_read_len,m_msg_body_len-m_has_read_len);
        if (read_msg_body_len <= 0) {
            USER_LOG_ERROR("read_buffer is empty,need msg_body_len:"<<m_msg_body_len<<"rea_count:"<<m_read_count);
            return -1;
        }
        m_has_read_len += read_msg_body_len;
        if (m_has_read_len == m_msg_body_len) {
            //USER_LOG_INFO("recv total packet has_read_len:"<<m_has_read_len<<" msg_body_len:"<<m_msg_body_len);
            packet->size = m_msg_body_len;
            packet->data = m_data_buffer;
            _reset_some_flags();
            return packet->size;  
        } else {
            m_read_count += 1;
        }
    }
    //USER_LOG_INFO("conntinue to read data,has_read_count:"<<m_read_count<<" has_read_len:"<<m_has_read_len);
    return 0;
}

sh::int32 CSocketClient::connect_to(const char* host_name,sh::uint32 port)
{
    m_ip = host_name;
    m_port = port;

    if (m_bev == NULL) {
        m_bev = CSingleton<CReactorCore>::get_instance()->get_new_socket_event();
        if (m_bev == NULL) {
            USER_LOG_ERROR("CSocketClient::connect_to get_new_socket_event failure");
            return -2;
        }
        m_socket = bufferevent_getfd(m_bev);
        if (m_socket < 0) {
            USER_LOG_ERROR("CSocketClient::connect_to bufferevent_getfd failre");
            return -2;
        } 
    }

    sockaddr_in addr_in;
    sh::int32 rst = init_sa_in(&addr_in, host_name,port);
    if (rst < 0) {
        USER_LOG_ERROR("init socket addr "<<host_name<<":"<<port<<" error");
        return -2;
    }
 
    if (bufferevent_socket_connect(m_bev,(struct sockaddr*)&addr_in,sizeof(addr_in)) < 0 ) {
        bufferevent_free(m_bev);
        m_bev = NULL;
        USER_LOG_ERROR("connect failed");
        return -2;
    } else {
        USER_LOG_INFO("connect to "<<host_name<<":"<<port);
    }
    init_cb();
    return 0;
}

sh::int32 CSocketClient::on_read(bufferevent* ev)
{
    evbuffer* input = bufferevent_get_input(ev); 
    SPacketInfo info;
    sh::int32 msg_len = 0;
    if ((msg_len = check_packet_info(&info,input)) > 0) {
        process_msg(info.data,info.size);
    }

    //断开链接
    if (msg_len < 0) {
        USER_LOG_ERROR("connect shutdown !");
        on_disconnection(m_bev);
        return -1;
    }
    if(msg_len == 0)
        return 0;
    return 1;
}

sh::int32 CSocketClient::on_write(bufferevent* ev)
{
    USER_LOG_ERROR("on_write function");
    return 1;
}

bool CSocketClient::send_msg(const char* data,const sh::int32 size)
{
    return (bufferevent_write(m_bev,data,size)==0)?true:false;
}

void CSocketClient::init_cb()
{
    bufferevent_setcb(m_bev,common_read_cb,NULL/*common_write_cb*/,common_event_cb,this);
    bufferevent_enable(m_bev,EV_READ|EV_PERSIST);
    bufferevent_enable(m_bev,EV_WRITE|EV_PERSIST);
    //bufferevent_enable(m_bev,EV_PERSIST);
}

void CSocketClient::on_disconnection(bufferevent* bev)
{
    on_error(bev); 
}

void CSocketClient::re_connect()
{
    connect_to(m_ip.c_str(),m_port);
}


void CSocketClient::_reset_some_flags()
{
    m_read_count = 1;
    m_has_read_len = 0;
    m_msg_body_len = 0;
}