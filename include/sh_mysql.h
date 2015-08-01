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
	sh::ulong			     m_dwPassTick; //执行这个sql 语句使用多长时间, 单位毫秒
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
    time_t          m_disconnect_time; //出现断开连接的时间. 0 如果sql连接正常, 则 m_disconnect_time 总是等于 0 
    int             m_wait_timeout;
    sh::uint16      m_wPort;
    MYSQL		    *m_Mysql;
    bool            m_bIsClose;
};

/** 
* @brief mysql 阻塞方式执行sql
*/
class CMySqlRealQuery : public IMySqlRealQuery, CMySqlBase
{
public:
    CMySqlRealQuery();
    virtual ~CMySqlRealQuery();

    // 初始连接数据库的数据
    virtual bool init(const char* servername, sh::uint16 port, const char* username, 
        const char* password, const char* dbname, const char* pCharSet="latin1", 
        const int wait_timeout=2147483);

    /// 连接数据库，立即返回是否成功
    virtual bool connect();

    /// 立即关闭与数据库的连接
    virtual void close();

    /// 立即执行一条sql语句
    virtual bool real_query(const char* lpszSqlText, const sh::uint32 dwSqlLen);

    /// 返回最后一条立即执行的sql语句的结果
    virtual MYSQL_RES* get_real_result(void);

    /// 释放最后一条立即执行的sql语句的结果
    virtual void free_real_result(void);

    // 返回mysql 句柄
    virtual MYSQL* get_mysql() {return CMySqlBase::get_mysql();}

    // 定时执行心跳sql. 用来保持mysql 连接
    virtual void keep_connect_timer(const char* lpszSqlText, const sh::uint32 dwSqlLen);

    // 事务方法 
    virtual bool x_begin();
    virtual bool x_commit();
    virtual bool x_roll_back();

private:
    bool _real_query(const char* lpszSqlText, const sh::uint32 dwSqlLen); //带自动重新连接

private:
    MYSQL_RES                   *m_RealQueryResult;

};

/** 
* @brief mysql 非阻塞方式执行sql
*/
class CMySqlAsyncQuery : public IMySqlAsyncQuery, CMySqlBase
{
public:
    //typedef std::list<CDBEvent*>        DBEVENT_LIST_TYPE;
    typedef ptr_list   DBEVENT_LIST_TYPE;

    CMySqlAsyncQuery();

    virtual ~CMySqlAsyncQuery();

    virtual void terminate();

    // 初始连接数据库的数据
    virtual bool init(const AsyncDBModuleCfg& cfg, const char* servername, sh::uint16 port, const char* username, 
        const char* password, const char* dbname, const char* pCharSet="latin1", 
        const int wait_timeout=2147483);

    /// 连接数据库，立即返回是否连接成功
    virtual bool connect();

    /// 立即关闭与数据库的连接
    virtual void close();

    /// 投递一个异步执行的sql事件(语句), 成功返回true, 否则返回false
    /// 如果一个sql语句的长度，大于或等于我们定义的最大sql长度，会抛出异常 out_of_range
    /// 如果内存不够，会抛出 std::bad_alloc
    /// bAllowIgnoreSelect 当sql事件的队列,达到了我们预定义上限后(AsyncDBModuleCfg::max_db_event_size)
    /// 如果此时还有 select sql 语句加入, 那么如果 bAllowIgnoreSelect=true,则系统会忽略这时加入的sql语句,并且返回false, 否则
    /// bAllowIgnoreSelect=false, 则不会忽略这时加入的sql语句
    /// bNeedAffectedRows=true 表示返回上次UPDATE更改的行数，上次DELETE删除的行数，或上次INSERT语句插入的行数。对于SELECT语句无效 默认false,不需要返回这个值                  
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

    /// 逻辑线程调用，处理 IOnAsyncQuery 接口下的事情
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

    CPool <CDBEvent>            m_DBEventPool;  //不使用本身多线程调度，使用外部锁来，目的是减少锁的次数
    CLock::CMutex               m_DBEventPoolMutex;

    sh::uint32                  m_max_db_event_size;
    sh::int32                   m_signal_id; //自定义信号ID

    static const sh::uint32     m_max_temp_dbevent_number=256;
    CDBEvent**                  m_p_temp_dbevent_result;
    CDBEvent**                  m_p_temp_dbevent_query;
};

}

#endif //__SHARE_MYSQL_H__

