#include "sh_dbsql.h"
#include "sh_mysql.h"

#define USE_SHARE_MYSQL

namespace sh
{

#ifdef USE_SHARE_MYSQL

IMySqlRealQuery* create_mysql_real_query()
{
    return new CMySqlRealQuery();
}

void free_mysql_real_query(IMySqlRealQuery* pMySqlRealQuery)
{
    delete pMySqlRealQuery;
}

IMySqlAsyncQuery* create_mysql_async_query()
{
    return new CMySqlAsyncQuery();
}

void free_mysql_async_query(IMySqlAsyncQuery* pMySqlAsyncQuery)
{
    if (NULL != pMySqlAsyncQuery){
        pMySqlAsyncQuery->close();
        delete pMySqlAsyncQuery;
    }
}

CLockTable::CLockTable(sh::IMySqlRealQuery* pRealMysql, const char* stblname)
{
    assert(pRealMysql);
    m_pRealMysql = pRealMysql;
    char szSql[256];
    const int len = sh::safe_sprintf(szSql, "LOCK TABLES %s", stblname);
    m_pRealMysql->real_query(szSql, len);
}

CLockTable::~CLockTable()
{
    if (NULL != m_pRealMysql){
        char szSql[64];
        // 特别注意，对于使用InnoDB引擎的表，执行UNLOCK TABLES 会隐含提交事务。
        // 所以对于事务的回滚和提交都必须在 UNLOCK TABLES 前执行
        const int len = sh::safe_sprintf(szSql, "UNLOCK TABLES;");
        m_pRealMysql->real_query(szSql, len);
    }
}

CTransaction::CTransaction(sh::IMySqlRealQuery* pRealMysql) : m_pRealMysql(pRealMysql), m_HasCommit(false) 
{
    assert(m_pRealMysql);
}

bool CTransaction::x_begin()
{
    assert(m_pRealMysql);
    return m_pRealMysql->x_begin();
}

void CTransaction::x_commit()
{
    assert(m_pRealMysql);
    m_HasCommit = true;
    m_pRealMysql->x_commit();
}

CTransaction::~CTransaction()
{
    if (!m_HasCommit && NULL != m_pRealMysql){
        m_pRealMysql->x_roll_back();
    }
}

CTransactionWithLock::CTransactionWithLock(sh::IMySqlRealQuery* pRealMysql, const char* locktext)
{
    assert(pRealMysql);
    m_pRealMysql = pRealMysql;
    m_HasCommit = false;
    m_strLockText = locktext;
}

CTransactionWithLock::~CTransactionWithLock()
{
    try
    {
        if (!m_HasCommit && NULL != m_pRealMysql){
            m_pRealMysql->x_roll_back();
        }
        if (NULL != m_pRealMysql){
            char szSql[64];
            // 特别注意，对于使用InnoDB引擎的表，执行UNLOCK TABLES 会隐含提交事务。
            // 所以对于事务的回滚和提交都必须在 UNLOCK TABLES 前执行
            const int len = sh::safe_sprintf(szSql, "UNLOCK TABLES;");
            m_pRealMysql->real_query(szSql, len);
        }
    }
    catch (...)
    {	
    }
    if (NULL != m_pRealMysql)
        m_pRealMysql->real_query("set autocommit=1;", strlen("set autocommit=1;"));
}

bool CTransactionWithLock::x_begin()
{
    assert(m_pRealMysql);
    if (m_pRealMysql->x_begin()){
        m_pRealMysql->real_query("set autocommit=0;", strlen("set autocommit=0;"));
        char szSql[256];
        const int len = sh::safe_sprintf(szSql, "LOCK TABLES %s", m_strLockText.c_str());
        return m_pRealMysql->real_query(szSql, len);
    }
    return false;
}

void CTransactionWithLock::x_commit()
{
    assert(m_pRealMysql);
    m_HasCommit = true;
    m_pRealMysql->x_commit();
}


bool is_select_sql(const char* lpszSqlText, const sh::uint32 dwSqlLen)
{
    const char* start=lpszSqlText;
    if (NULL == start) 
        return false;

    int len = dwSqlLen;
    while( (NULL != start) && (len > 0) && isspace(*start) ){
        ++start;
        --len;
    }

    // 检查第一个单词是否是select
    char szSelectBuf[7]={0};
    for (int i=0; i<len && i<6 && NULL != start; ++i){
        szSelectBuf[i] = *(start+i);
    }

    if (sh::stricmp(szSelectBuf, "select") == 0){
        return true;
    }
    return false;
}


#endif

}

