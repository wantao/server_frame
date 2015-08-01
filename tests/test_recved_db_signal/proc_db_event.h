#ifndef __PROC_DB_EVENT_H__
#define __PROC_DB_EVENT_H__
#include <map>
#include "sh_singleton.h"
#include "sh_type.h"
#include "sh_dbsql.h"

typedef void (*on_load_fun_type)(const SQueryResultParam& r);
typedef std::map<sh::uint32, on_load_fun_type> map_dbevent_fun_type;

class CGsProcDbEvent:public CSingleton<CGsProcDbEvent>
{
public:
    enum DB_SIGNAL_EVENT_ID
    {
        SIGNAL_MYSQL_USR_ID = SIGUSR1 + 2,  
    };

    enum DBEVENT_ID
    {
        LOAD_PLAYER_INFO_TEST,



        MAX_DBEVENT_ID,  // db id 的最大值. 必须协写在这个枚举最后
    };
public:
    friend CSingleton<CGsProcDbEvent>;
public:

    void register_db_event_init();
    static void register_db_event(CGsProcDbEvent::DBEVENT_ID id,on_load_fun_type fun);
    // 执行sql回调方法
    static void on_query_complute(const SQueryResultParam& r);
    //注册db信号事件
    bool register_db_signal_event(CGsProcDbEvent::DB_SIGNAL_EVENT_ID id,sh::IMySqlAsyncQuery* p_asyn_mysql);

    static void on_load_player_info_test(const SQueryResultParam& r);

private:
    static map_dbevent_fun_type m_db_events;
};
#endif