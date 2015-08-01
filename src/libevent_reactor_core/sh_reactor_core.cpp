#include "sh_reactor_core.h"
#include "sh_net_util.h"
#include "sh_listener.h"
#include "sh_filelog.h"

template<> 
CReactorCore* CSingleton<CReactorCore>::m_instance = 0;

void common_listener_cb(struct evconnlistener* listener,evutil_socket_t fd,struct sockaddr* sa,sh::int32 socklen,void* user_data)
{
    struct bufferevent* bev;
    CListener* ls = (CListener*)user_data; 
    bev = bufferevent_socket_new(CReactorCore::get_instance()->get_event_base(), fd, BEV_OPT_CLOSE_ON_FREE);
    if(!bev)
    {
        USER_LOG_ERROR("error build bufferevent");
        event_base_loopbreak(CReactorCore::get_instance()->get_event_base());
        return;
    }
    sh::int32 rst = ls->on_accept(bev,fd,(sockaddr_in*)sa);
    if(rst < 0 )
    { 
        USER_LOG_ERROR("accept error ,free bev");
        bufferevent_free(bev);
    }
}


bool CReactorCore::init()
{
    m_base = event_base_new(); 
    if(m_base == NULL)
    {
        USER_LOG_ERROR("init event base failed");
        return false;
    }
    return true;
}

void CReactorCore::start_dispatch()
{
    if (m_base != NULL) {
        event_base_dispatch(m_base);
    }
}

sh::int32 CReactorCore::add_listener_event(std::string ip,sh::int32 port,CListener* ls)
{
    sockaddr_in addr_in;
    sh::int32 rst = init_sa_in(&addr_in, ip.c_str(),port);
    if(rst < 0)
    {
        USER_LOG_ERROR("init socket addr error");
        return -2;
    }
    evconnlistener *new_ev_ls = evconnlistener_new_bind(m_base,common_listener_cb,(void*)ls, LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,(struct sockaddr*)&addr_in,sizeof(addr_in));
    if(!new_ev_ls)
    {
        USER_LOG_ERROR("can't create listener on "<<ip.c_str()<<":"<<port);
        return -1;
    }
    m_all_listener.push_back(new_ev_ls);
    return 0;
}

bufferevent* CReactorCore::get_new_socket_event()
{
    bufferevent* bev = bufferevent_socket_new(m_base,-1,BEV_OPT_CLOSE_ON_FREE);
    if(bev == NULL)
    {
        USER_LOG_ERROR("create new socket error!");
        return NULL;
    }
    return bev;
}

bool CReactorCore::add_async_db_event_signal(const evutil_socket_t& db_event_signal,sh::IMySqlAsyncQuery* p_async_db)
{
    if (!p_async_db) {
        USER_LOG_ERROR("add_async_db_event_signal CMySqlAsyncQuery is Null!!!!");
        return false;
    }
    if (0 != event_assign(&m_db_signal_event, m_base, db_event_signal, EV_SIGNAL|EV_PERSIST, async_db_event_signal_cb,p_async_db)) {
        USER_LOG_ERROR("add_async_db_event_signal event_assign db_event_signal failure");
        return false;
    }
    if (0 != event_add(&m_db_signal_event, NULL)) {
        USER_LOG_ERROR("add_async_db_event_signal event_add m_db_signal_event failure");
        return false;
    }
    return true;
}

void CReactorCore::async_db_event_signal_cb(evutil_socket_t fd, short event, void *arg)
{
    sh::IMySqlAsyncQuery* p_async_db = (sh::IMySqlAsyncQuery*)arg;
    if (!p_async_db) {
        USER_LOG_ERROR("async_db_event_signal_cb user_data is null");
        return;
    }
    p_async_db->run(256);
}