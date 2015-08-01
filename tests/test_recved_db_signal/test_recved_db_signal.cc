#include "sh_reactor_core.h"
#include "proc_db_event.h"
#include "sh_filelog.h"

struct SDbCfg
{
    sh::uint16	    wPort;
    std::string		strHost;
    std::string		strDBName;
    std::string		strUserName;
    std::string		strPassword;
};

int main(void)
{
    if (!CSingleton<CReactorCore>::get_instance()->init()) {
        return -1;
    }
    SDbCfg cfg;
    cfg.wPort = 3306;
    cfg.strDBName = "test_db";
    cfg.strHost = "192.168.1.200";
    cfg.strUserName = "root";
    cfg.strPassword = "root";

    CSingleton<CGsProcDbEvent>::get_instance()->register_db_event_init();

    sh::IMySqlAsyncQuery* pAsynMysql = sh::create_mysql_async_query();
    if (NULL == pAsynMysql){
        assert(false);
        USER_LOG_FATAL("create asyn mysql fail");
        return 0;
    }

    sh::AsyncDBModuleCfg asy_cfg;
    asy_cfg.signal_id = CGsProcDbEvent::SIGNAL_MYSQL_USR_ID;
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

    if (!CSingleton<CReactorCore>::get_instance()->add_async_db_event_signal(CGsProcDbEvent::SIGNAL_MYSQL_USR_ID,pAsynMysql))
    {
        USER_LOG_ERROR("register_db_signal_event failure");
        return 0;
    }

    char szSql[1024];
    const sh::int32 len = sh::safe_sprintf(szSql, "SELECT * FROM `player_info` ");

    if(!pAsynMysql->post_async_query_event(CGsProcDbEvent::LOAD_PLAYER_INFO_TEST, szSql, len, &(CSingleton<CGsProcDbEvent>::get_instance()->on_query_complute))){
        assert(false);
        USER_LOG_FATAL("connect asyn mysql fail");
        return 0;
    }

    CSingleton<CReactorCore>::get_instance()->start_dispatch();

}