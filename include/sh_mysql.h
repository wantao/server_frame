#ifndef __SHARE_MYSQL_H__
#define __SHARE_MYSQL_H__

#include "sh_dbsql.h"
#include "sh_ptrlist.h"
#include "sh_systemtime.h"
#include <thread>


namespace sh
{


class CDBEvent
{
public:
    explicit CDBEvent(sh::uint32 dwEventId, on_query_result_fun_type pFun):m_pResult(NULL),m_pFun(pFun),m_qwLastInsertID(0),
        affected_rows(0),m_dwErrorNo(0),m_dwPassTick(0),m_dwEventId(dwEventId),m_bNeedInsertID(false),m_bNeedAffectRows(false)
    {
    }

    ~CDBEvent()
    {
        m_pResult = NULL;
        m_dwEventId = 0;

        m_dwErrorNo = 0;   
        m_pFun = NULL;
    }

    void set_sql_text(const char* pSqlText, sh::uint32 dwSqlLen) {m_sql.assign(pSqlText, dwSqlLen);}
    void set_data_buf(const void* oData, sh::uint32 dwDataLen) {m_data.assign((const char*)oData, dwDataLen);}

    on_query_result_fun_type get_fun() {return m_pFun;}

    MYSQL_RES* get_mysql_result(void) {return m_pResult;}

    void set_mysql_result(MYSQL_RES* res) { m_pResult = res;}

    const std::string& sql_text()const {return m_sql;}

    sh::uint32 get_event_id(void)const {return m_dwEventId;}

    void* get_data_buf() {return (void*)(m_data.data());}
    sh::uint32 get_data_len() {return (sh::uint32)m_data.size();}

    sh::uint32 get_error_no() {return m_dwErrorNo;}
    void set_error_no(sh::uint32 dwErrorNo) {m_dwErrorNo = dwErrorNo;}

    void set_error_message(std::string strErrMsg) {m_strErrMsg = strErrMsg;}
    const std::string& get_error_message() {return m_strErrMsg;}

	sh::ulong get_pass_tick()const {return m_dwPassTick;}
    void set_pass_tick(sh::ulong dwTick) {m_dwPassTick = dwTick;}

    const sh::uint64& get_last_insert_id() const {return m_qwLastInsertID;}
    void set_last_insert_id(const sh::uint64& qw) {m_qwLastInsertID = qw;}

    bool has_need_insert_id()const {return m_bNeedInsertID;}
    void set_need_insert_id() {m_bNeedInsertID = true;}

    const my_ulonglong get_affect_rows()const {return affected_rows;}
    void set_affect_rows(const my_ulonglong affectrows) {affected_rows = affectrows;}

    bool has_need_affect_rows()const {return m_bNeedAffectRows;}
    void set_need_affect_rows() {m_bNeedAffectRows = true;}

private:
    MYSQL_RES               *m_pResult;
    on_query_result_fun_type m_pFun;
    sh::uint64               m_qwLastInsertID;
    my_ulonglong             affected_rows;

    std::string              m_sql;
    std::string              m_data;
    std::string              m_strErrMsg;

    sh::uint32               m_dwErrorNo;
	sh::ulong			     m_dwPassTick; //ִ�����sql ���ʹ�ö೤ʱ��, ��λ����
    sh::uint32               m_dwEventId;   

    bool                     m_bNeedInsertID;
    bool                     m_bNeedAffectRows;
};


class CMySqlBase
{
protected:
    CMySqlBase();
    ~CMySqlBase();

    bool init(const char* servername, sh::uint16 port, const char* username, 
        const char* password, const char* dbname, const char* pCharSet="latin1",
        const int wait_timeout=2147483);

    bool connect();

    void close();

    MYSQL* get_mysql() {return m_Mysql;}

protected:
    std::string     m_strServerName;
    std::string     m_strUserName;
    std::string     m_strPassword;
    std::string     m_strDbName;
    time_t          m_disconnect_time; //���ֶϿ����ӵ�ʱ��. 0 ���sql��������, �� m_disconnect_time ���ǵ��� 0 
    int             m_wait_timeout;
    sh::uint16      m_wPort;
    MYSQL		    *m_Mysql;
    bool            m_bIsClose;
};

/** 
* @brief mysql ������ʽִ��sql
*/
class CMySqlRealQuery : public IMySqlRealQuery, CMySqlBase
{
public:
    CMySqlRealQuery();
    virtual ~CMySqlRealQuery();

    // ��ʼ�������ݿ������
    virtual bool init(const char* servername, sh::uint16 port, const char* username, 
        const char* password, const char* dbname, const char* pCharSet="latin1", 
        const int wait_timeout=2147483);

    /// �������ݿ⣬���������Ƿ�ɹ�
    virtual bool connect();

    /// �����ر������ݿ������
    virtual void close();

    /// ����ִ��һ��sql���
    virtual bool real_query(const char* lpszSqlText, const sh::uint32 dwSqlLen);

    /// �������һ������ִ�е�sql���Ľ��
    virtual MYSQL_RES* get_real_result(void);

    /// �ͷ����һ������ִ�е�sql���Ľ��
    virtual void free_real_result(void);

    // ����mysql ���
    virtual MYSQL* get_mysql() {return CMySqlBase::get_mysql();}

    // ��ʱִ������sql. ��������mysql ����
    virtual void keep_connect_timer(const char* lpszSqlText, const sh::uint32 dwSqlLen);

    // ���񷽷� 
    virtual bool x_begin();
    virtual bool x_commit();
    virtual bool x_roll_back();

private:
    bool _real_query(const char* lpszSqlText, const sh::uint32 dwSqlLen); //���Զ���������

private:
    MYSQL_RES                   *m_RealQueryResult;

};

/** 
* @brief mysql ��������ʽִ��sql
*/
class CMySqlAsyncQuery : public IMySqlAsyncQuery, CMySqlBase
{
public:
    //typedef std::list<CDBEvent*>        DBEVENT_LIST_TYPE;
    typedef ptr_list   DBEVENT_LIST_TYPE;

    CMySqlAsyncQuery();

    virtual ~CMySqlAsyncQuery();

    virtual void terminate();

    // ��ʼ�������ݿ������
    virtual bool init(const AsyncDBModuleCfg& cfg, const char* servername, sh::uint16 port, const char* username, 
        const char* password, const char* dbname, const char* pCharSet="latin1", 
        const int wait_timeout=2147483);

    /// �������ݿ⣬���������Ƿ����ӳɹ�
    virtual bool connect();

    /// �����ر������ݿ������
    virtual void close();

    /// Ͷ��һ���첽ִ�е�sql�¼�(���), �ɹ�����true, ���򷵻�false
    /// ���һ��sql���ĳ��ȣ����ڻ�������Ƕ�������sql���ȣ����׳��쳣 out_of_range
    /// ����ڴ治�������׳� std::bad_alloc
    /// bAllowIgnoreSelect ��sql�¼��Ķ���,�ﵽ������Ԥ�������޺�(AsyncDBModuleCfg::max_db_event_size)
    /// �����ʱ���� select sql ������, ��ô��� bAllowIgnoreSelect=true,��ϵͳ�������ʱ�����sql���,���ҷ���false, ����
    /// bAllowIgnoreSelect=false, �򲻻������ʱ�����sql���
    /// bNeedAffectedRows=true ��ʾ�����ϴ�UPDATE���ĵ��������ϴ�DELETEɾ�������������ϴ�INSERT�����������������SELECT�����Ч Ĭ��false,����Ҫ�������ֵ                  
    virtual bool post_async_query_event(const sh::uint32 dwEventId, 
        const char* lpszSqlText, 
        const sh::uint32 dwSqlLen, 
        on_query_result_fun_type pFun=NULL, 
        const void* oData=NULL, 
        const sh::uint32 dwDataLen=0, 
        bool bNeedGetLastInsertID=false,
        bool bAllowIgnoreSelect=true,
        bool bNeedAffectedRows=false
        );

    /// �߼��̵߳��ã����� IOnAsyncQuery �ӿ��µ�����
    virtual bool run(sh::uint32 dwCount=-1);

    virtual MYSQL* get_mysql() {return CMySqlBase::get_mysql();}

private:
    void runn();

    void _push_result_list(sh::CDBEvent* ptr);

private:
    volatile bool               m_bRun;

    std::thread                 m_thread;

    CLock::CMutex               m_DBEventListMutex;
    DBEVENT_LIST_TYPE           m_DBEventList;

    CLock::CMutex               m_DBEventResultListMutex;
    DBEVENT_LIST_TYPE           m_DBEventResultList;

    CPool <CDBEvent>            m_DBEventPool;  //��ʹ�ñ�����̵߳��ȣ�ʹ���ⲿ������Ŀ���Ǽ������Ĵ���
    CLock::CMutex               m_DBEventPoolMutex;

    sh::uint32                  m_max_db_event_size;
    sh::int32                   m_signal_id; //�Զ����ź�ID

    static const sh::uint32     m_max_temp_dbevent_number=256;
    CDBEvent**                  m_p_temp_dbevent_result;
    CDBEvent**                  m_p_temp_dbevent_query;
};

}

#endif //__SHARE_MYSQL_H__

