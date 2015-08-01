#ifndef __SHARE_DB_SQL_H__
#define __SHARE_DB_SQL_H__

#include "sh_common.h"
#include <stdexcept>
#include <thread>
#include "mysql.h"

/** 
* @brief �첽���ݿ�������ؽ��
* @param dwEventId dbid
* @param result ���ݼ�
* @param oData �û��Զ�������
* @param dwDataLen �û��Զ������ݳ���
* @param dwErrorNo ������
* @param strErrMsg ������Ϣ
* @param dwPassTick ��Ͷ��db �¼��� ���첽���ؾ�����ʱ��
* @param qwLastInsertID ������ִ�е���һ�� insert sql ��䣬���������ֶΡ����post��ʱ����ʱ��Ҫ��ȡ ��� ��������ֵ�ġ��򷵻�
* @param affected_rows, ʵ�����ǵ���mysql_affected_rows() �����ϴ�UPDATE���ĵ��������ϴ�DELETEɾ�������������ϴ�INSERT�����������������UPDATE��DELETE��INSERT��䣬����mysql_query()�����̵��á�����SELECT��䣬mysql_affected_rows()�Ĺ�����ʽ��mysql_num_rows()���ơ�
* @return ��
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
* @brief �첽���ݿ�������أ�������on_query_result_fun_type �ӿ�������Ӧ������
* @param SQueryResultParam
* @return ��
*/
typedef void (*on_query_result_fun_type)(const SQueryResultParam& r);



namespace sh
{

/// ���ǵ����ӵĵ�ǰ�ַ�����Ϊ����SQL�����ʹ�ã����ַ����е������ַ�����ת�崦��
/// ������������������׳� std::out_of_range �쳣
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
* @brief mysql ������ʽִ��sql
*/
class IMySqlRealQuery
{
public:
	virtual ~IMySqlRealQuery() {}

	// ��ʼ�������ݿ������
    // �Ự��ʱ Ĭ�� 2147483 �룬���24��
    virtual bool init(const char* servername, sh::uint16 port, const char* username, 
		const char* password, const char* dbname, const char* pCharSet="latin1", 
        const int wait_timeout=2147483)=0;

	/// �������ݿ⣬���������Ƿ�ɹ�
	virtual bool connect()=0;

	/// �����ر������ݿ������
	virtual void close()=0;

	/// ����ִ��һ��sql���
	virtual bool real_query(const char* lpszSqlText, const sh::uint32 dwSqlLen)=0;

	/// �������һ������ִ�е�sql���Ľ��
	virtual MYSQL_RES* get_real_result(void)=0;

	/// �ͷ����һ������ִ�е�sql���Ľ��
	virtual void free_real_result(void)=0;

    // ����mysql ���
    virtual MYSQL* get_mysql()=0;

    // ��ʱִ������sql. ��������mysql ����
    virtual void keep_connect_timer(const char* lpszSqlText, const sh::uint32 dwSqlLen)=0;

    template <sh::uint32 bufLen>
    unsigned long escape_str(char (&to)[bufLen], const char *from, const unsigned long from_length, const char* filename, const unsigned long line)
    {
        return escape_string(get_mysql(), to, from, from_length, filename, line);
    }

    // ���񷽷� 
    virtual bool x_begin()=0;
    virtual bool x_commit()=0;
    virtual bool x_roll_back()=0;
};

/*
*@brief һ���첽mysql���������ʼ����Ҫ����Ϣ
*       ���sql �¼���Ҫ�Ļ��������ڴ�������벻�ˣ����ʹ�� c++ new ������
*/
struct AsyncDBModuleCfg
{
    AsyncDBModuleCfg(): db_event_pool_init_number(1000),a_sql_text_max_size(1048576),
    all_sql_text_max_size(5242880), a_data_buffer_max_size(10240),
    all_data_buffer_size(5242880),max_db_event_size(300),signal_id(0) {}

	sh::uint32  db_event_pool_init_number;   //Ԥ��sql�¼����ڴ�ص�����
	sh::uint32  a_sql_text_max_size;         //���ʹ�����ڴ�أ�һ��sql������󳤶�
	sh::uint32  all_sql_text_max_size;       //�ڴ���� ȫ��sql��仺�������ֵ
	sh::uint32  a_data_buffer_max_size;      //���ʹ�����ڴ�� һ��sql�¼���Я�����û��Զ����������
	sh::uint32  all_data_buffer_size;        //�ڴ���ˣ�ȫ���û��Զ��建�������ֵ

    // ���ñ��߳�,������ select �¼�������. Ĭ��300
    // ��� �� post_async_query_event �ӿ��� ���� bAllowIgnoreSelect=ture, �������ֵӰ��post ������ select �¼�����.
    // ��� ��ǰ�߳���� select �¼����� > �������õ�ֵ, �� post_async_query_event ���� false
    sh::uint32  max_db_event_size; 
    sh::int32   signal_id; //�Զ����¼�ID
};

/** 
* @brief mysql ��������ʽִ��sql
*/
class IMySqlAsyncQuery 
{
public:
	virtual ~IMySqlAsyncQuery() {}

	// ��ʼ�������ݿ������
    // �Ự��ʱ Ĭ�� 2147483 �룬���24��
	virtual bool init(const AsyncDBModuleCfg& cfg, const char* servername, sh::uint16 port, const char* username, 
		const char* password, const char* dbname, const char* pCharSet="latin1",
        const int wait_timeout=2147483)=0;

	/// �������ݿ⣬���������Ƿ����ӳɹ�
	virtual bool connect()=0;

	/// �����ر������ݿ������
	virtual void close()=0;

	/// Ͷ��һ���첽ִ�е�sql�¼�(���), �ɹ�����true, ���򷵻�false
    /// ע��: ��������sql������ж���sql���, ��ô���ӿ�ֻ�ܼ���һ������Ƿ���select���, ����ľͼ�ⲻ����
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
        )=0;

	/// �߼��̵߳��ã����� IOnAsyncQuery �ӿ��µ�����
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


// �����������������ƣ���ҪӦ���� MyISAM����ı�
// �ڹ��캯����ִ�� ����������������ִ�� ������
class CLockTable
{
public:
    CLockTable() : m_pRealMysql(NULL) {}
    explicit CLockTable(IMySqlRealQuery* pRealMysql, const char* stblname);
    ~CLockTable();
private:
    IMySqlRealQuery* m_pRealMysql;
};

// �����ύ���ع����ƣ�ֻ������ InnoDB����ı���Ҫ������һ���á�
// ��������Ҫ������һ���ã���ʹ�� CTransactionWithLock
// �������û���ֶ�ִ�� x_commit ִ���ύ����CTransaction��ʵ���뿪�������ʱ��
// ����������������� x_roll_back ���лع���������ֶ�ִ���� Xcommit�ύ��������
// ������������ ���� x_roll_back��
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
    bool                    m_HasCommit; //�Ƿ��Ѿ��ύ��
};

// �����ύ���ع����ơ�������������������� ֻ������ InnoDB����ı�
// ��InnoDB����ı������� ������������Ȳ����Ѳ�������
/*
��InnoDB�£�ʹ�ñ���Ҫע���������㡣(ժ����������)
��1��ʹ��LOCK TABLES��Ȼ���Ը�InnoDB�ӱ�����������˵�����ǣ�����������InnoDB�洢��������ģ�����������һ�㩤��MySQL Server����ģ�
    ����autocommit=0��innodb_table_locks=1��Ĭ�����ã�ʱ��InnoDB�����֪��MySQL�ӵı�����MySQL ServerҲ���ܸ�֪InnoDB�ӵ���������������£�
    InnoDB�����Զ�ʶ���漰����������������InnoDB���޷��Զ���Ⲣ���������������й���������һС�ڻ���������ۡ�
��2������ LOCK TABLES��InnoDB�����ʱҪע�⣬Ҫ��AUTOCOMMIT��Ϊ0������MySQL����������������mysql 5.1 �Թ��ˣ���AUTOCOMMIT=1����InnoDB������ǿ��Եģ����������ǰ����Ҫ��UNLOCK TABLES�ͷű�����
    ��ΪUNLOCK TABLES���������ύ����������Թ��ˣ���ȷ����������COMMIT��ROLLBACK�������ͷ���LOCK TABLES�ӵı�����������UNLOCK TABLES�ͷű���
*/
// ��x_begin ��ִ���� autocommit=0��������
// ������������������������ autocommit=1
// �������û���ֶ�ִ�� x_commit ִ���ύ����CTransaction��ʵ���뿪�������ʱ��
// ����������������� x_roll_back ���лع���������ֶ�ִ���� Xcommit�ύ��������
// ������������ ���� x_roll_back��
class CTransactionWithLock
{
public:
    CTransactionWithLock() : m_pRealMysql(NULL), m_HasCommit(false) {}
    explicit CTransactionWithLock(IMySqlRealQuery* pRealMysql, const char* locktext);

    // �������������� autocommit=1
    ~CTransactionWithLock();

    // ��x_begin�������� autocommit=0
    bool x_begin();

    void x_commit();

    IMySqlRealQuery* mysql() {return m_pRealMysql;}

private:
    std::string             m_strLockText;
    IMySqlRealQuery*        m_pRealMysql;
    bool                    m_HasCommit; //�Ƿ��Ѿ��ύ��
};

// ���һ��sql���ĵ�һ�������Ƿ�Ϊselect(���Դ�Сд)
// ��������sql������ж���sql���, ��ô���ӿ�ֻ�ܼ���һ������Ƿ���select���, ����ľͼ�ⲻ����
bool is_select_sql(const char* lpszSqlText, const sh::uint32 dwSqlLen);

}
#endif //__SHARE_DB_SQL_H__

