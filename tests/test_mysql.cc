#include "sh_filelog.h"
#include "sh_common.h"
#include "sh_ptrlist.h"
#include "sh_systemtime.h"
#include "sh_dbsql.h"
#include "sh_include.h"
#include <event2/event-config.h>
#include <event2/event.h>
#include <event.h>

#define SIGNAL_MYSQL_USR_ID      (SIGRTMIN + 1)


struct SDbCfg
{
    sh::uint16	    wPort;
    std::string		strHost;
    std::string		strDBName;
    std::string		strUserName;
    std::string		strPassword;
};

static void on_query_result_fun(const SQueryResultParam& r)
{
    if (0 != r.dwErrorNo){
        USER_LOG_FATAL("on_query_result_fun fail");
        return;
    }

    for (MYSQL_ROW row=mysql_fetch_row(r.result); NULL!=row; row=mysql_fetch_row(r.result)) {
        sh::int32 id = sh::a2i(row[0]);
        unsigned long *lengths = mysql_fetch_lengths(r.result);
        std::string s(row[1], lengths[1]);
        printf("ansy id=%d, s=%s \r\n", id, s.c_str());
    }
}


static void
signal_cb(evutil_socket_t fd, short event, void *arg)
{
    printf("mysql event done\r\n");

    sh::IMySqlAsyncQuery *prt_asy_mysql = (sh::IMySqlAsyncQuery *)arg;
    if (NULL != prt_asy_mysql){
        prt_asy_mysql->run(256);
    }
    
    //printf("%s: got signal %d\n", __func__, EVENT_SIGNAL(signal));
}

int main()
{

    //////////////////////////////////////////////////////////////////////////

    printf("msg_min:%d\r\n", SIGRTMAX);
    printf("msg_usr:%d\r\n", SIGUSR1);
    sh::IMySqlRealQuery* pRealMysql = sh::create_mysql_real_query();
    if (NULL == pRealMysql){
        assert(false);
        USER_LOG_FATAL("create real mysql fail");
        return 0;
    }

    SDbCfg cfg;
    cfg.wPort = 3306;
    cfg.strDBName = "test";
    cfg.strHost = "192.168.1.201";
    cfg.strUserName = "root";
    cfg.strPassword = "root";
    if(!pRealMysql->init(cfg.strHost.c_str(), cfg.wPort, cfg.strUserName.c_str(), cfg.strPassword.c_str(), cfg.strDBName.c_str(), "utf8")){
        assert(false);
        USER_LOG_FATAL("init real mysql fail");
        return 0;
    }

    if(!pRealMysql->connect()){
        assert(false);
        USER_LOG_FATAL("connect real mysql fail");
        return 0;
    }

    char szSql[1024];
    const sh::int32 len = sh::safe_sprintf(szSql, "SELECT * FROM `tttt` ");
    const bool r1 = pRealMysql->real_query(szSql, len);
    MYSQL_RES* select_result = pRealMysql->get_real_result();
    if (!r1 || NULL == select_result){
        assert(false);
        USER_LOG_INFO("import log select from `gs_dir` fail");
        return false;
    }

    for (MYSQL_ROW row =mysql_fetch_row(select_result); NULL!=row; row =mysql_fetch_row(select_result)) {
        sh::int32 id = sh::a2i(row[0]);
        unsigned long *lengths = mysql_fetch_lengths(select_result);
        std::string s(row[1], lengths[1]);
        printf("real id=%d, s=%s \r\n", id, s.c_str());
    }
    free_mysql_real_query(pRealMysql);


    printf("\r\ncommon test-----------------------------------------------------\r\n");

    sh::IMySqlAsyncQuery* pAsynMysql = sh::create_mysql_async_query();
    if (NULL == pAsynMysql){
        assert(false);
        USER_LOG_FATAL("create asyn mysql fail");
        return 0;
    }

    sh::AsyncDBModuleCfg asy_cfg;
    asy_cfg.signal_id = SIGNAL_MYSQL_USR_ID;
    if(!pAsynMysql->init(asy_cfg, cfg.strHost.c_str(), cfg.wPort, cfg.strUserName.c_str(), cfg.strPassword.c_str(), cfg.strDBName.c_str(), "utf8")){
        assert(false);
        USER_LOG_FATAL("init asyn mysql fail");
        return 0;
    }

    if(!pAsynMysql->connect()){
        assert(false);
        USER_LOG_FATAL("connect asyn mysql fail");
        return 0;
    }

    if(!pAsynMysql->post_async_query_event(1, szSql, len, on_query_result_fun)){
        assert(false);
        USER_LOG_FATAL("connect asyn mysql fail");
        return 0;
    }

    struct event signal_int;
    struct event_base* base;

    /* Initalize the event library */
    base = event_base_new();

    /* Initalize one event */
    event_assign(&signal_int, base, SIGNAL_MYSQL_USR_ID, EV_SIGNAL|EV_PERSIST, signal_cb,
        pAsynMysql);

    event_add(&signal_int, NULL);

    event_base_dispatch(base);
    event_base_free(base);


    printf("mysql ..done");

    sh::free_mysql_async_query(pAsynMysql);
    

    printf("mysql done");
    //////////////////////////////////////////////////////////////////////////


    return 0;
}



