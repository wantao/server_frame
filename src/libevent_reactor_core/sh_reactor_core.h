#ifndef __SH_REATOR_CORE_H__
#define __SH_REATOR_CORE_H__
#include "sh_singleton.h"
#include "sh_include.h"
#include "sh_libevent_include.h"
#include "sh_dbsql.h"

class CListener;
void common_listener_cb(struct evconnlistener* listener,evutil_socket_t fd,struct sockaddr* sa,sh::int32 socklen,void* user_data);

class CReactorCore:public CSingleton<CReactorCore>
{
    public:
        friend CSingleton<CReactorCore>;
    public:
        //初始化event_base
        bool init();
        //开始dispatch循环
        void start_dispatch();
        //获取event_base
        event_base* get_event_base(){return m_base;}
        //添加监听网络上到来的事件
        sh::int32         add_listener_event(string ip,sh::int32 port,CListener* ls);
        //获取一个socket的bufferevent
        bufferevent* get_new_socket_event();
        //添加db通知信号事件，
        /*
        param 1:自定义的db通知信号，如SIGUSR+2
        param 2:db句柄
        */
        bool         add_async_db_event_signal(const evutil_socket_t& db_event_signal,sh::IMySqlAsyncQuery* p_async_db);
        //db通知信号回调
        static void async_db_event_signal_cb(evutil_socket_t fd, short event, void *arg);

    private:
        event_base* m_base;
        struct event m_db_signal_event;
        std::vector<evconnlistener*> m_all_listener; 

};

#endif
