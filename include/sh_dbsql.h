#ifndef __SHARE_DB_SQL_H__
#define __SHARE_DB_SQL_H__

#include "sh_common.h"
#include <stdexcept>
#include <thread>
#include "mysql.h"

/** 
* @brief 异步数据库操作返回结果
* @param dwEventId dbid
* @param result 数据集
* @param oData 用户自定义数据
* @param dwDataLen 用户自定义数据长度
* @param dwErrorNo 错误码
* @param strErrMsg 错误信息
* @param dwPassTick 从投入db 事件， 到异步返回经历的时间
* @param qwLastInsertID 如果这次执行的是一个 insert sql 语句，且有自增字段。如果post的时候有时需要读取 这个 最后的自增值的。则返回
* @param affected_rows, 实际上是调用mysql_affected_rows() 返回上次UPDATE更改的行数，上次DELETE删除的行数，或上次INSERT语句插入的行数。对于UPDATE、DELETE或INSERT语句，可在mysql_query()后立刻调用。对于SELECT语句，mysql_affected_rows()的工作方式与mysql_num_rows()类似。
* @return 无
*/

class SQueryResultParam
{
public:
    explicit SQueryResultParam(
        const sh::uint32 _dwEventID, 
        MYSQL_RES* _result,
        const void* _oData,
        const sh::uint32 _dwDataLen,
        const sh::uint32 _dwErrorNo,
        const std::string _strErrMsg,
        const sh::uint32 _dwPassTick,
        const sh::uint64 _qwLastInsertID,
        const std::string& _sql,
        const my_ulonglong affectedrows) : 
        dwEventId(_dwEventID), 
        result(_result), 
        oData(_oData),
        dwDataLen(_dwDataLen), 
        dwErrorNo(_dwErrorNo), 
        strErrMsg(_strErrMsg), 
        dwPassTick(_dwPassTick), 
        qwLastInsertID(_qwLastInsertID),
        sql(_sql),
        affected_rows(affectedrows)
        {}

    const sh::uint32 dwEventId;
    MYSQL_RES *result;
    const void* oData;
    const sh::uint32 dwDataLen;
    const sh::uint32 dwErrorNo;
    const std::string strErrMsg;
    const sh::uint32 dwPassTick; 
    const sh::uint64 qwLastInsertID;
    const std::string& sql;
    const my_ulonglong affected_rows;
private:  
    SQueryResultParam( const SQueryResultParam& );
    const SQueryResultParam& operator=( const SQueryResultParam& );
};

/** 
* @brief 异步数据库操作返回，可以在on_query_result_fun_type 接口里做相应的事情
* @param SQueryResultParam
* @return 无
*/
typedef void (*on_query_result_fun_type)(const SQueryResultParam& r);



namespace sh
{

/// 考虑到连接的当前字符集，为了在SQL语句中使用，对字符串中的特殊字符进行转义处理。
/// 如果缓冲区不够，会抛出 std::out_of_range 异常
template <sh::uint32 bufLen>
unsigned long escape_string(MYSQL* mysql, char (&to)[bufLen], const char *from, const unsigned long from_length, const char* filename, const unsigned long line)
{
    assert(mysql);
    assert(from);
    assert(filename);
    if (from_length*2+1 > sizeof(to)){
        assert(false);
        char szErrLog[256];
        sh::safe_sprintf(szErrLog, "escape_string from_length*2+1 %u > sizeof(to) %u ,%s, %u", from_length*2+1, sizeof(to), filename, line);
        throw std::out_of_range(szErrLog);
    }
    return mysql_real_escape_string(mysql, to, from, from_length);
}


/** 
* @brief mysql 阻塞方式执行sql
*/
class IMySqlRealQuery
{
public:
	virtual ~IMySqlRealQuery() {}

	// 初始连接数据库的数据
    // 会话超时 默认 2147483 秒，大概24天
    virtual bool init(const char* servername, sh::uint16 port, const char* username, 
		const char* password, const char* dbname, const char* pCharSet="latin1", 
        const int wait_timeout=2147483)=0;

	/// 连接数据库，立即返回是否成功
	virtual bool connect()=0;

	/// 立即关闭与数据库的连接
	virtual void close()=0;

	/// 立即执行一条sql语句
	virtual bool real_query(const char* lpszSqlText, const sh::uint32 dwSqlLen)=0;

	/// 返回最后一条立即执行的sql语句的结果
	virtual MYSQL_RES* get_real_result(void)=0;

	/// 释放最后一条立即执行的sql语句的结果
	virtual void free_real_result(void)=0;

    // 返回mysql 句柄
    virtual MYSQL* get_mysql()=0;

    // 定时执行心跳sql. 用来保持mysql 连接
    virtual void keep_connect_timer(const char* lpszSqlText, const sh::uint32 dwSqlLen)=0;

    template <sh::uint32 bufLen>
    unsigned long escape_str(char (&to)[bufLen], const char *from, const unsigned long from_length, const char* filename, const unsigned long line)
    {
        return escape_string(get_mysql(), to, from, from_length, filename, line);
    }

    // 事务方法 
    virtual bool x_begin()=0;
    virtual bool x_commit()=0;
    virtual bool x_roll_back()=0;
};

/*
*@brief 一个异步mysql操作组件初始化需要的信息
*       如果sql 事件需要的缓冲区在内存池里申请不了，则会使用 c++ new 来申请
*/
struct AsyncDBModuleCfg
{
    AsyncDBModuleCfg(): db_event_pool_init_number(1000),a_sql_text_max_size(1048576),
    all_sql_text_max_size(5242880), a_data_buffer_max_size(10240),
    all_data_buffer_size(5242880),max_db_event_size(300),signal_id(0) {}

	sh::uint32  db_event_pool_init_number;   //预设sql事件在内存池的数量
	sh::uint32  a_sql_text_max_size;         //如果使用了内存池，一条sql语句的最大长度
	sh::uint32  all_sql_text_max_size;       //内存池里 全局sql语句缓冲区最大值
	sh::uint32  a_data_buffer_max_size;      //如果使用了内存池 一个sql事件里携带的用户自定义数据最大
	sh::uint32  all_data_buffer_size;        //内存池了，全局用户自定义缓冲区最大值

    // 设置本线程,最多接收 select 事件的数量. 默认300
    // 如果 在 post_async_query_event 接口里 参数 bAllowIgnoreSelect=ture, 则这个数值影响post 进来的 select 事件数量.
    // 如果 当前线程里的 select 事件数量 > 这里设置的值, 则 post_async_query_event 返回 false
    sh::uint32  max_db_event_size; 
    sh::int32   signal_id; //自定义事件ID
};

/** 
* @brief mysql 非阻塞方式执行sql
*/
class IMySqlAsyncQuery 
{
public:
	virtual ~IMySqlAsyncQuery() {}

	// 初始连接数据库的数据
    // 会话超时 默认 2147483 秒，大概24天
	virtual bool init(const AsyncDBModuleCfg& cfg, const char* servername, sh::uint16 port, const char* username, 
		const char* password, const char* dbname, const char* pCharSet="latin1",
        const int wait_timeout=2147483)=0;

	/// 连接数据库，立即返回是否连接成功
	virtual bool connect()=0;

	/// 立即关闭与数据库的连接
	virtual void close()=0;

	/// 投递一个异步执行的sql事件(语句), 成功返回true, 否则返回false
    /// 注意: 如果传入的sql语句里有多条sql语句, 那么本接口只能检测第一条语句是否是select语句, 后面的就检测不来了
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
        )=0;

	/// 逻辑线程调用，处理 IOnAsyncQuery 接口下的事情
	virtual bool run(sh::uint32 dwCount=-1)=0;

    virtual MYSQL* get_mysql()=0;

    template <sh::uint32 bufLen>
    unsigned long escape_str(char (&to)[bufLen], const char *from, const unsigned long from_length, const char* filename, const unsigned long line)
    {
        return escape_string(get_mysql(), to, from, from_length, filename, line);
    }
};

IMySqlRealQuery* create_mysql_real_query();

void free_mysql_real_query(IMySqlRealQuery* );


IMySqlAsyncQuery* create_mysql_async_query();

void free_mysql_async_query(IMySqlAsyncQuery* );


// 单独锁表与解锁表控制，主要应用与 MyISAM引擎的表
// 在构造函数里执行 锁表，在析构函数里执行 解锁表。
class CLockTable
{
public:
    CLockTable() : m_pRealMysql(NULL) {}
    explicit CLockTable(IMySqlRealQuery* pRealMysql, const char* stblname);
    ~CLockTable();
private:
    IMySqlRealQuery* m_pRealMysql;
};

// 事务提交、回滚控制，只能引用 InnoDB引擎的表，不要与锁表一起用。
// 如果真的需要与锁表一起用，请使用 CTransactionWithLock
// 如果事务没有手动执行 x_commit 执行提交，当CTransaction的实例离开作用域的时候，
// 会在析构函数里调用 x_roll_back 进行回滚事务。如果手动执行了 Xcommit提交了事务，则
// 析构函数不会 调用 x_roll_back。
class CTransaction
{
public:
    CTransaction() : m_pRealMysql(NULL), m_HasCommit(false) {}
    explicit CTransaction(IMySqlRealQuery* pRealMysql);
    ~CTransaction();
    bool x_begin();
    void x_commit();
    IMySqlRealQuery* mysql() {return m_pRealMysql;}

private:
    IMySqlRealQuery*        m_pRealMysql;
    bool                    m_HasCommit; //是否已经提交了
};

// 事务提交、回滚控制、且能有锁表、解锁表控制 只能引用 InnoDB引擎的表
// 对InnoDB引擎的表因能用 事务就用事务，迫不得已采用锁表
/*
在InnoDB下，使用表锁要注意以下两点。(摘自网络文章)
（1）使用LOCK TABLES虽然可以给InnoDB加表级锁，但必须说明的是，表锁不是由InnoDB存储引擎层管理的，而是由其上一层──MySQL Server负责的，
    仅当autocommit=0、innodb_table_locks=1（默认设置）时，InnoDB层才能知道MySQL加的表锁，MySQL Server也才能感知InnoDB加的行锁，这种情况下，
    InnoDB才能自动识别涉及表级锁的死锁；否则，InnoDB将无法自动检测并处理这种死锁。有关死锁，下一小节还会继续讨论。
（2）在用 LOCK TABLES对InnoDB表加锁时要注意，要将AUTOCOMMIT设为0，否则MySQL不会给表加锁（我在mysql 5.1 试过了，当AUTOCOMMIT=1，给InnoDB表加锁是可以的）；事务结束前，不要用UNLOCK TABLES释放表锁，
    因为UNLOCK TABLES会隐含地提交事务（这个是试过了，的确是这样）；COMMIT或ROLLBACK并不能释放用LOCK TABLES加的表级锁，必须用UNLOCK TABLES释放表锁
*/
// 在x_begin 里执行了 autocommit=0，且锁表
// 在析构函数里，会解锁表，且设置 autocommit=1
// 如果事务没有手动执行 x_commit 执行提交，当CTransaction的实例离开作用域的时候，
// 会在析构函数里调用 x_roll_back 进行回滚事务。如果手动执行了 Xcommit提交了事务，则
// 析构函数不会 调用 x_roll_back。
class CTransactionWithLock
{
public:
    CTransactionWithLock() : m_pRealMysql(NULL), m_HasCommit(false) {}
    explicit CTransactionWithLock(IMySqlRealQuery* pRealMysql, const char* locktext);

    // 在析构函数里会把 autocommit=1
    ~CTransactionWithLock();

    // 在x_begin函数里会把 autocommit=0
    bool x_begin();

    void x_commit();

    IMySqlRealQuery* mysql() {return m_pRealMysql;}

private:
    std::string             m_strLockText;
    IMySqlRealQuery*        m_pRealMysql;
    bool                    m_HasCommit; //是否已经提交了
};

// 检查一个sql语句的第一个单词是否为select(忽略大小写)
// 如果传入的sql语句里有多条sql语句, 那么本接口只能检测第一条语句是否是select语句, 后面的就检测不来了
bool is_select_sql(const char* lpszSqlText, const sh::uint32 dwSqlLen);

}
#endif //__SHARE_DB_SQL_H__

