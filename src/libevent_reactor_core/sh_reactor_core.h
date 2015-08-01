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
        //��ʼ��event_base
        bool init();
        //��ʼdispatchѭ��
        void start_dispatch();
        //��ȡevent_base
        event_base* get_event_base(){return m_base;}
        //��Ӽ��������ϵ������¼�
        sh::int32         add_listener_event(string ip,sh::int32 port,CListener* ls);
        //��ȡһ��socket��bufferevent
        bufferevent* get_new_socket_event();
        //���db֪ͨ�ź��¼���
        /*
        param 1:�Զ����db֪ͨ�źţ���SIGUSR+2
        param 2:db���
        */
        bool         add_async_db_event_signal(const evutil_socket_t& db_event_signal,sh::IMySqlAsyncQuery* p_async_db);
        //db֪ͨ�źŻص�
        static void async_db_event_signal_cb(evutil_socket_t fd, short event, void *arg);

    private:
        event_base* m_base;
        struct event m_db_signal_event;
        std::vector<evconnlistener*> m_all_listener; 

};

#endif
