#include "proc_db_event.h"
#include "sh_filelog.h"
#include "sh_reactor_core.h"


template<> 
CGsProcDbEvent* CSingleton<CGsProcDbEvent>::m_instance = 0;

map_dbevent_fun_type CGsProcDbEvent::m_db_events;

void CGsProcDbEvent::on_load_player_info_test(const SQueryResultParam& r)
{
    USER_LOG_INFO("on_load_player_info_test!!!!");
}

void CGsProcDbEvent::register_db_event_init()
{
    register_db_event(CGsProcDbEvent::LOAD_PLAYER_INFO_TEST,&CGsProcDbEvent::on_load_player_info_test);
}

void CGsProcDbEvent::register_db_event(CGsProcDbEvent::DBEVENT_ID id,on_load_fun_type fun)
{
    map_dbevent_fun_type::iterator itr = m_db_events.find(id);
    if(itr != m_db_events.end()){
        USER_LOG_INFO("register_db_event same event:"<<id);
        return;
    }
    m_db_events[id] = fun;
}

void CGsProcDbEvent::on_query_complute(const SQueryResultParam& r)
{
    if (0 != r.dwErrorNo){
        USER_LOG_FATAL("sql error="<<r.strErrMsg);
    }

    if (r.dwEventId >= MAX_DBEVENT_ID) {
        assert (false && "r.dwEventId > MAX_DBEVENT_ID");
        USER_LOG_FATAL("r.dwEventId > MAX_DBEVENT_ID id="<<r.dwEventId);
        return;
    }

    on_load_fun_type pfun = m_db_events[r.dwEventId];
    if (NULL == pfun){
        return;
    }

    pfun(r);
}

bool CGsProcDbEvent::register_db_signal_event(CGsProcDbEvent::DB_SIGNAL_EVENT_ID id,sh::IMySqlAsyncQuery* p_asyn_mysql)
{
    return CSingleton<CReactorCore>::get_instance()->add_async_db_event_signal(id,p_asyn_mysql);    
}
