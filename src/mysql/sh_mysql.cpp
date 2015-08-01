#include "sh_mysql.h"
#include "sh_filelog.h"
#include "sh_systemtime.h"
#include <stdexcept>
#include <exception>

namespace sh
{

CMySqlBase::CMySqlBase()
{
    m_Mysql = NULL;
    m_bIsClose = true;

    m_wait_timeout=28800;
    m_disconnect_time = 0;
}

CMySqlBase::~CMySqlBase()
{
    close();
    if (NULL != m_Mysql) {
        delete m_Mysql;
        m_Mysql = NULL;
    }
}

bool CMySqlBase::init(const char* servername, sh::uint16 port, const char* username, 
    const char* password, const char* dbname, const char* pCharSet,const int wait_timeout)
{
    if (NULL == servername || NULL == username || NULL == password || NULL == dbname) {
        SQL_LOG_FATAL("CMySqlBase::init fail , param error");
        return false;
    }

    m_strServerName = servername;
    m_wPort = port;
    m_strUserName = username;
    m_strPassword = password;
    m_strDbName = dbname;

    if (NULL == m_Mysql){
        m_Mysql = new MYSQL;
    }

    if (NULL == mysql_init(m_Mysql)){
        SQL_LOG_FATAL("mysql_init(m_Mysql) fail!");
        delete m_Mysql;
        m_Mysql = NULL;
        return false;
    }

    //��������
    if (0 != mysql_options(m_Mysql,MYSQL_SET_CHARSET_NAME,pCharSet)){
        SQL_LOG_FATAL("mysql_options(m_Mysql,MYSQL_SET_CHARSET_NAME,pCharSet) fail!");
        delete m_Mysql;
        m_Mysql = NULL;
        return false;
    }
    m_wait_timeout = wait_timeout;
    return true;
}

bool CMySqlBase::connect()
{
    if (NULL == m_Mysql) {
        SQL_LOG_FATAL("please first call init");
        return false;
    }

    //Ӧ��mysql_init()֮���Լ�mysql_connect()��mysql_real_connect()֮ǰ����mysql_options()��
    // �����Զ�����
    my_bool opt_reconnect = 1;
    if (0 != mysql_options(m_Mysql,MYSQL_OPT_RECONNECT,&opt_reconnect)){
        SQL_LOG_FATAL("mysql_options(m_Mysql,MYSQL_OPT_RECONNECT,&opt_reconnect) fail --1 !");
        delete m_Mysql;
        m_Mysql = NULL;
        return false;
    }

    if (!mysql_real_connect(m_Mysql, m_strServerName.c_str(), m_strUserName.c_str(), m_strPassword.c_str(), m_strDbName.c_str(), m_wPort, NULL, 0)){
        std::string error_msg = mysql_error(m_Mysql);
        SQL_LOG_FATAL("connect database fail:"<<"; error msg: "<<error_msg<<m_strServerName<<","<<m_strUserName<<","<<m_strDbName<<","<<m_wPort);
        return false;
    }

    // ����һ�Σ�
    if (0 != mysql_options(m_Mysql,MYSQL_OPT_RECONNECT,&opt_reconnect)){
        SQL_LOG_FATAL("mysql_options(m_Mysql,MYSQL_OPT_RECONNECT,&opt_reconnect) fail --2 !");
        delete m_Mysql;
        m_Mysql = NULL;
        return false;
    }

    // �������ӳ�ʱ
    char szSql[1024];
    const int len1 = sh::safe_sprintf(szSql, "set session wait_timeout=%d;", m_wait_timeout);
    if (mysql_real_query(m_Mysql, szSql, len1)!=0){
        int error_no = mysql_errno(m_Mysql);
        std::string error_msg = mysql_error(m_Mysql);
        SQL_LOG_FATAL("error no: "<<error_no<<"; error msg: "<<error_msg<<"; sql: "<<szSql);
    }

    m_bIsClose = false;
    SQL_LOG_INFO("connect database success��"<<m_strServerName<<","<<m_strUserName<<","<<m_strDbName<<","<<m_wPort);
    return true;
}

void CMySqlBase::close()
{
    if (NULL != m_Mysql) {
        if (!m_bIsClose) {
            mysql_close(m_Mysql);
            m_bIsClose = true;
            SQL_LOG_INFO("close data base success��"<<m_strServerName<<","<<m_strUserName<<","<<m_strDbName<<","<<m_wPort);
        }
    }
}

bool CMySqlRealQuery::init(const char* servername, sh::uint16 port, const char* username, 
    const char* password, const char* dbname, const char* pCharSet,const int wait_timeout)
{
    return CMySqlBase::init(servername, port, username, password, dbname, pCharSet, wait_timeout);
}

bool CMySqlRealQuery::connect()
{
    return CMySqlBase::connect();
}

void CMySqlRealQuery::close()
{
    CMySqlBase::close();
}

// ��ʱִ������sql. ��������mysql ����
void CMySqlRealQuery::keep_connect_timer(const char* lpszSqlText, const sh::uint32 dwSqlLen)
{
    _real_query(lpszSqlText, dwSqlLen);
}

// ���񷽷� 
bool CMySqlRealQuery::x_begin()
{
    return _real_query("BEGIN;",(unsigned int)strlen("BEGIN;"));
}

bool CMySqlRealQuery::x_commit()
{
    return _real_query("COMMIT;",(unsigned int)strlen("COMMIT;"));
}

bool CMySqlRealQuery::x_roll_back()
{
    return _real_query("ROLLBACK;",(unsigned int)strlen("ROLLBACK;"));
}

bool CMySqlRealQuery::real_query(const char* lpszSqlText, const sh::uint32 dwSqlLen)
{
    return _real_query(lpszSqlText, dwSqlLen);
}

bool CMySqlRealQuery::_real_query(const char* lpszSqlText, const sh::uint32 dwSqlLen)
{
    if (NULL == m_Mysql || NULL == lpszSqlText || 0 == dwSqlLen) {
        SQL_LOG_FATAL("mysql null or sqltext null or sqllen = 0");
        return false;
    }

#ifdef _DEBUG
    SQL_LOG_DEBUG("sql:"<<lpszSqlText);
#endif

    if (mysql_real_query(m_Mysql, lpszSqlText, dwSqlLen) != 0) {
        int error_no = mysql_errno(m_Mysql);
        std::string error_msg = mysql_error(m_Mysql);
        SQL_LOG_FATAL("error no: "<<error_no<<"; error msg: "<<error_msg<<"; sql: "<<lpszSqlText);

        //����������ӶϿ�������������һ�Σ����ʧ���򷵻�false
        if (2006 == error_no)  {
            if (!mysql_real_connect(m_Mysql, m_strServerName.c_str(), m_strUserName.c_str(),m_strPassword.c_str(), m_strDbName.c_str(), m_wPort, NULL, 0)) {
                SQL_LOG_FATAL("connect again fali:"<<m_strServerName<<","<<m_strUserName<<","<<m_strDbName<<","<<m_wPort);
                if (0 == m_disconnect_time) {
                    m_disconnect_time = time(NULL);
                } else if (time(NULL) - m_disconnect_time > 60) { //����Ѿ��Ͽ��� 60 ��, ��Ҫ�رշ�������
                    SQL_LOG_FATAL("disconnected mysql server close this server now !!!");
                    sh::mill_sleep(100);
                    exit(0); 
                }
                return false;
            } else {
                m_disconnect_time = 0; // �������ӳɹ���
                //�������ӳɹ��ˣ��ٰѸղ�ʧ�ܵ�sql ��ִ��һ��
                SQL_LOG_INFO("connect again success :"<<m_strServerName<<","<<m_strUserName<<","<<m_strDbName<<","<<m_wPort);
                if (mysql_real_query(m_Mysql, lpszSqlText, dwSqlLen) != 0) {
                    int error_no = mysql_errno(m_Mysql);
                    std::string error_msg = mysql_error(m_Mysql);
                    SQL_LOG_FATAL("error no: "<<error_no<<"; error msg: "<<error_msg<<"; sql: "<<lpszSqlText);
                    return false;
                } else {
                    free_real_result();
                    /*
                    ���ڳɹ����������ݵ�ÿ����ѯ��SELECT��SHOW��DESCRIBE��EXPLAIN��CHECK TABLE�ȣ����������mysql_store_result()��mysql_use_result() ��
                    ����������ѯ������Ҫ����mysql_store_result()��mysql_use_result()������������κ�����¾�������mysql_store_result()����Ҳ���ᵼ���κ��˺������ܽ���
                    */
                    m_RealQueryResult = mysql_store_result(m_Mysql);
                    return true;
                }
            }
        }
        return false;
    } else {
        m_disconnect_time = 0; //sql ���ִ�гɹ�, ��ʾsql ��������, 

        free_real_result();
        /*
        ���ڳɹ����������ݵ�ÿ����ѯ��SELECT��SHOW��DESCRIBE��EXPLAIN��CHECK TABLE�ȣ����������mysql_store_result()��mysql_use_result() ��
        ����������ѯ������Ҫ����mysql_store_result()��mysql_use_result()������������κ�����¾�������mysql_store_result()����Ҳ���ᵼ���κ��˺������ܽ���
        */
        m_RealQueryResult = mysql_store_result(m_Mysql);
    }
    return true;
}

MYSQL_RES* CMySqlRealQuery::get_real_result(void)
{
    return m_RealQueryResult;
}

void CMySqlRealQuery::free_real_result(void)
{
    if (NULL != m_RealQueryResult) {
        mysql_free_result(m_RealQueryResult);
        m_RealQueryResult = NULL;
    }
}

CMySqlRealQuery::CMySqlRealQuery()
{
    m_RealQueryResult = NULL;
}

CMySqlRealQuery::~CMySqlRealQuery()
{
    free_real_result();
}

CMySqlAsyncQuery::CMySqlAsyncQuery():m_thread(std::bind(&CMySqlAsyncQuery::runn, this))
{
    m_bRun = true;
    m_max_db_event_size = 500; // һ���첽sql�¼��б�������Ŷ�500���¼�.

    assert(m_max_temp_dbevent_number);
    m_p_temp_dbevent_result = new CDBEvent*[m_max_temp_dbevent_number];
    memset(m_p_temp_dbevent_result, 0, sizeof(CDBEvent*)*m_max_temp_dbevent_number);
    m_p_temp_dbevent_query = new CDBEvent*[m_max_temp_dbevent_number];
    memset(m_p_temp_dbevent_query, 0, sizeof(CDBEvent*)*m_max_temp_dbevent_number);
}

CMySqlAsyncQuery::~CMySqlAsyncQuery()
{
    if (m_thread.joinable()){
        m_thread.join();
    }

    if (NULL != m_p_temp_dbevent_result) {
        delete []m_p_temp_dbevent_result;
        m_p_temp_dbevent_result = NULL;
    }
    if (NULL != m_p_temp_dbevent_query) {
        delete []m_p_temp_dbevent_query;
        m_p_temp_dbevent_query = NULL;
    }
}

void CMySqlAsyncQuery::terminate()
{
    m_bRun = false;
}

bool CMySqlAsyncQuery::init(const AsyncDBModuleCfg& cfg, const char* servername, sh::uint16 port, const char* username, 
                  const char* password, const char* dbname, const char* pCharSet,const int wait_timeout)
{
    if (CMySqlBase::init(servername, port, username, password, dbname, pCharSet, wait_timeout)) {
        if (m_DBEventPool.init(cfg.db_event_pool_init_number, false) == false)
            return false;

        m_max_db_event_size = cfg.max_db_event_size;
        m_signal_id = cfg.signal_id;
        return true;
    }
    return false;
}

bool CMySqlAsyncQuery::connect()
{
    return CMySqlBase::connect();
}

void CMySqlAsyncQuery::close()
{
    if (!m_bIsClose) {
        terminate();
        if (m_thread.joinable()){
            m_thread.join();
        }
    }
    CMySqlBase::close();
}

bool CMySqlAsyncQuery::post_async_query_event(const sh::uint32 dwEventId, const char* lpszSqlText, const sh::uint32 dwSqlLen, 
                                 on_query_result_fun_type pFun, const void* oData, const sh::uint32 dwDataLen, 
                                 bool bNeedGetLastInsertID, bool bAllowIgnoreSelect, bool bNeedAffectedRows)
{
    if (bAllowIgnoreSelect) {
        //CLock lock(m_DBEventListMutex); //m_DBEventListMutex �� m_DBEventList����
        const sh::uint32 cur_db_event_size = m_DBEventList.size(); //����ֻ��,���Բ�����
        if (cur_db_event_size >= m_max_db_event_size){
            if (is_select_sql(lpszSqlText, dwSqlLen)){  
                SQL_LOG_FATAL("import log cur_db_event_size >= m_max_db_event_size "<<cur_db_event_size<<", "<<m_max_db_event_size<<", sql="<<lpszSqlText);
                return false;
            }
        }
    }

    CDBEvent* pEvent = NULL;
    {
        std::lock_guard<std::mutex> lock(m_DBEventPoolMutex);
        pEvent = m_DBEventPool.malloc(dwEventId, pFun);
        if (NULL == pEvent) {
            SQL_LOG_FATAL("not enough memory !!");
            throw std::bad_alloc();
        }
    }

    if (NULL != lpszSqlText && dwSqlLen > 0){
        pEvent->set_sql_text(lpszSqlText, dwSqlLen);
    }

    if (NULL != oData && dwDataLen > 0){
        pEvent->set_data_buf(oData, dwDataLen); 
    }

    if (bNeedGetLastInsertID){
        pEvent->set_need_insert_id();
    }

    if (bNeedAffectedRows) {
        pEvent->set_need_affect_rows();
    }

    {
        std::lock_guard<std::mutex> lock(m_DBEventListMutex);
        m_DBEventList.push_back(pEvent);//m_DBEventListMutex �� m_DBEventList����
    }
    return true;
}

bool CMySqlAsyncQuery::run(sh::uint32 dwCount)
{
    CDBEvent* pEvent = NULL;
    void* pData = NULL;
    on_query_result_fun_type pFun=NULL;
    bool bReturn = false;
    sh::uint32 N=0; //��¼��ǰ�����¼����������

    if (!m_DBEventResultList.empty()) {
        N=0;
        std::lock_guard<std::mutex> lock(m_DBEventResultListMutex);
        while ((!m_DBEventResultList.empty()) && N<dwCount && N<m_max_temp_dbevent_number) {
            m_p_temp_dbevent_result[N] = (CDBEvent*)m_DBEventResultList.pop_front();
            N++;
        }
        bReturn = true;
    } else {
        return bReturn;
    }
   
    sh::uint32 i=0;
    while (i < N) {
        pEvent = m_p_temp_dbevent_result[i++];
        if (NULL == pEvent) 
            continue;
        pFun = pEvent->get_fun();
        pData = pEvent->get_data_buf();
        if (pEvent->get_data_len() < 1){
            pData = NULL;
        }

        if (NULL != pFun) {
#ifdef _DEBUG
            SQL_LOG_DEBUG("result id:"<<pEvent->get_event_id());
#endif
            const SQueryResultParam result_param(pEvent->get_event_id(), pEvent->get_mysql_result(),
                pData, pEvent->get_data_len(), pEvent->get_error_no(), pEvent->get_error_message(),
                pEvent->get_pass_tick(), pEvent->get_last_insert_id(), pEvent->sql_text(), pEvent->get_affect_rows());
            (*pFun)(result_param);

            if (NULL != pEvent->get_mysql_result()) {
                mysql_free_result(pEvent->get_mysql_result()); //�ͷ����ݼ�
            }
        }
    }

    if (N > 0) {
        std::lock_guard<std::mutex> lock(m_DBEventPoolMutex);
        for (i=0; i<N; ++i) {
            pEvent = m_p_temp_dbevent_result[i];
            m_DBEventPool.free(pEvent);
        }
    }
    
    return bReturn;
}

void CMySqlAsyncQuery::runn()
{
    CDBEvent* pEvent = NULL;

    for (;;)
    {
        if (false == m_bRun) {
            std::lock_guard<std::mutex> lock(m_DBEventListMutex);
            if (m_DBEventList.empty())
                break;
        }

        if (m_DBEventList.empty()) {
            sh::mill_sleep(1);
            continue;
        }

        sh::uint32 N = 0;
        {
            //���¼�sql�¼��б�copydao��ʱ�¼��б�
            std::lock_guard<std::mutex> lock(m_DBEventListMutex);
            while (!m_DBEventList.empty() && N < m_max_temp_dbevent_number){
                m_p_temp_dbevent_query[N] = (CDBEvent*)m_DBEventList.pop_front();
                N++;
            }
        }

        sh::uint32 i=0;
        bool success = false;
        while (i < N) {
            pEvent = m_p_temp_dbevent_query[i];
            i++;
            if (NULL == pEvent) 
                continue;

#ifdef _DEBUG
            SQL_LOG_DEBUG("event id:"<<pEvent->get_event_id()<<", query sql:"<<pEvent->sql_text());
            assert (pEvent->sql_text().size());
#endif
            const ulong dwBeginTick = sh::get_tick_count();
            if (mysql_real_query(m_Mysql, pEvent->sql_text().c_str(), (unsigned long)pEvent->sql_text().size()) != 0) {
                sh::uint32 error_no = mysql_errno(m_Mysql);
                std::string error_msg = mysql_error(m_Mysql);
                pEvent->set_error_no(error_no);
                pEvent->set_error_message(error_msg);
                pEvent->set_pass_tick(get_tick_count() - dwBeginTick);
                SQL_LOG_FATAL("error no: "<<error_no<<"; error msg: "<<error_msg<<"; sql: "<<pEvent->sql_text());

                if (2006 == error_no)  {
                    if (0 == m_disconnect_time) {
                        m_disconnect_time = time(NULL);
                    } else if (time(NULL) - m_disconnect_time > 60) { //����Ѿ��Ͽ��� 60 ��, ��Ҫ�رշ�������
                        SQL_LOG_FATAL("disconnected mysql server close this server now !!!");
                        sh::mill_sleep(100);
                        exit(0); 
                    }
                }

                {
                    _push_result_list(pEvent);
                    success = true;
                }
                continue;
            } else {
                m_disconnect_time = 0;
                MYSQL_RES* res = mysql_store_result(m_Mysql);
                pEvent->set_mysql_result(res);

                // res == NULL ��ʾ�� select ���
                if (NULL == res){
                    if (pEvent->has_need_affect_rows()) {
                        pEvent->set_affect_rows(mysql_affected_rows(m_Mysql));
                    }
                    if (pEvent->has_need_insert_id()) {
                        // ���϶�ȡ��ǰ���������idֵ
                        pEvent->set_last_insert_id(mysql_insert_id(m_Mysql));
                    }
                }

                pEvent->set_pass_tick(get_tick_count() - dwBeginTick);

                {
                    _push_result_list(pEvent);
                    success = true;
                }
            }
        }

        #if !(defined(WIN32) || defined(WIN64))
            if (m_signal_id > 0 && success){
                if(-1 == raise(m_signal_id)){
                    assert(false);
                    SQL_LOG_FATAL("raise error: signal id: "<<m_signal_id);
                    sh::mill_sleep(100);
                    exit(0); 
                }
            }
       #endif
    }
}

void CMySqlAsyncQuery::_push_result_list(sh::CDBEvent* ptr)
{
    std::lock_guard<std::mutex> lock(m_DBEventResultListMutex);
    m_DBEventResultList.push_back(ptr);//��m_DBEventResultList����
}

}



