#ifndef __SHARE_FILE_LOG_H__
#define __SHARE_FILE_LOG_H__

#include <string>
#include <sstream>


namespace sh
{
typedef void* file_log_handle;

// 日志等级名称
static const char*  DEBUG_LOG_LEVEL_NAME=   "debug";
static const char*  INFO_LOG_LEVEL_NAME=    "info";
static const char*  WARN_LOG_LEVEL_NAME=    "warn";
static const char*  ERROR_LOG_LEVEL_NAME=   "error";
static const char*  FATAL_LOG_LEVEL_NAME=   "fatal";

//每个日志等级名称对应的id值
static const int  DEBUG_LOG_LEVEL_ID=   1;
static const int  INFO_LOG_LEVEL_ID=    2;
static const int  WARN_LOG_LEVEL_ID=    3;
static const int  ERROR_LOG_LEVEL_ID=   4;
static const int  FATAL_LOG_LEVEL_ID=   5;

static const char* sg_log_id_text[] = {
    "",       //index=0, empty name
    DEBUG_LOG_LEVEL_NAME, //index=DEBUG_LOG_LEVEL_ID
    INFO_LOG_LEVEL_NAME,  //index=INFO_LOG_LEVEL_ID
    WARN_LOG_LEVEL_NAME,  //index=WARN_LOG_LEVEL_ID
    ERROR_LOG_LEVEL_NAME, //index=ERROR_LOG_LEVEL_ID
    FATAL_LOG_LEVEL_NAME, //index=FATAL_LOG_LEVEL_ID
};

//系统独占默认的日志类型
static const int  SH_LOG_TYPE=0;
static const int  USER_LOG_TYPE=1;
static const int  MEMORY_LOG_TYPE=2;
static const int  SQL_LOG_TYPE=3;


//FAST_FLOG_SHARE_FILE_LOG__使用。定义一条日志的最大字节长度， 这个值可以根据实际情况修改
static const int  FAST_LOG_BUFFER_SIZE_SHARE_FILE_LOG = 1024;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//// log, 简单的文本日志；数据库日志
//////////////////////////////////////////////////////////////////////////
// 配置文件的存放目录必须是:  运行目录/share_config/share_log_config.xml
// 如果没有配置文件, sh log 自动使用默认配置
/* 下面是默认配置。
<root>
    <!--
    日志类型对应的日志目录，日志文件名称头和尾
    每类日志对应的记录级别
    日志类型id，最好按照从小到大往下排序。[0-65535]。不能有重复
    日志名称name 也不能有重复
    -->
    <logtype>
        <type id="0" name="ShareLog"    folder_name="ShareLog"   log_file_head="sh"  log_file_trail=".log">
            <item key="debug" value="yes" />
            <item key="info"  value="yes" />
            <item key="warn"  value="yes" />
            <item key="error" value="yes" />
            <item key="fatal" value="yes" />
        </type>
        <type id="1" name="UserLog"     folder_name="UserLog"    log_file_head="user"   log_file_trail=".log">
            <item key="debug" value="yes" />
            <item key="info"  value="yes" />
            <item key="warn"  value="yes" />
            <item key="error" value="yes" />
            <item key="fatal" value="yes" />
        </type>
        <type id="2" name="MemoryLog"    folder_name="MemoryLog"   log_file_head="memory"  log_file_trail=".log">
            <item key="debug" value="yes" />
            <item key="info"  value="yes" />
            <item key="warn"  value="yes" />
            <item key="error" value="yes" />
            <item key="fatal" value="yes" />
        </type>
        <type id="3" name="SqlLog"     folder_name="SqlLog"    log_file_head="sql"   log_file_trail=".log">
            <item key="debug" value="yes" />
            <item key="info"  value="yes" />
            <item key="warn"  value="yes" />
            <item key="error" value="yes" />
            <item key="fatal" value="yes" />
        </type>
    </logtype>
</root>
*/

// 检查是否写这个类型、等级的日志
// 如果需要写，则返回这个指向这个log对象的句柄
extern file_log_handle share_check_file_log(const unsigned int log_type_id, const int log_level);

//写文本日志
extern void share_file_log(file_log_handle log_handle, const char* log_content);
}



#define  FLOG_SHARE_FILE_LOG__(logtype, loglevel, lpmsg) for(int _loglevel=loglevel; _loglevel; ){\
    ::sh::file_log_handle log_handle = ::sh::share_check_file_log(logtype,loglevel);\
    if (NULL == log_handle) break;\
    std::ostringstream oss; \
    oss<<"["<<::sh::sg_log_id_text[loglevel]<<"]"\
    <<"["<<__FILE__<<"]"<<"["<<__LINE__<<"]:"<<lpmsg;\
    ::sh::share_file_log(log_handle, oss.str().c_str());_loglevel=0;} //正常情况下loglevel>0, 所以for循环的第一次一定会运行,后面_loglevel=0, for 循环绝对不会有第二次及第二次以上运行

//////////////////////////////////////////////////////////////////////////
/// 本库预定义好的几个日志宏
/// SHARE_LOG_, SHARE_FAST_LOG_ 开头的日志宏,一般是记录sh库内部日志
#define SHARE_LOG_DEBUG(lpmsg) FLOG_SHARE_FILE_LOG__(::sh::SH_LOG_TYPE, ::sh::DEBUG_LOG_LEVEL_ID, lpmsg)
#define SHARE_LOG_INFO(lpmsg) FLOG_SHARE_FILE_LOG__(::sh::SH_LOG_TYPE, ::sh::INFO_LOG_LEVEL_ID, lpmsg)
#define SHARE_LOG_WARN(lpmsg) FLOG_SHARE_FILE_LOG__(::sh::SH_LOG_TYPE, ::sh::WARN_LOG_LEVEL_ID, lpmsg)
#define SHARE_LOG_ERROR(lpmsg) FLOG_SHARE_FILE_LOG__(::sh::SH_LOG_TYPE, ::sh::ERROR_LOG_LEVEL_ID, lpmsg)
#define SHARE_LOG_FATAL(lpmsg) FLOG_SHARE_FILE_LOG__(::sh::SH_LOG_TYPE, ::sh::FATAL_LOG_LEVEL_ID, lpmsg)


/// USER_LOG_, USER_FAST_LOG_ 开头的日志宏,一般是记录用户级日志
#define USER_LOG_DEBUG(lpmsg) FLOG_SHARE_FILE_LOG__(::sh::USER_LOG_TYPE, ::sh::DEBUG_LOG_LEVEL_ID, lpmsg)
#define USER_LOG_INFO(lpmsg) FLOG_SHARE_FILE_LOG__(::sh::USER_LOG_TYPE, ::sh::INFO_LOG_LEVEL_ID, lpmsg)
#define USER_LOG_WARN(lpmsg) FLOG_SHARE_FILE_LOG__(::sh::USER_LOG_TYPE, ::sh::WARN_LOG_LEVEL_ID, lpmsg)
#define USER_LOG_ERROR(lpmsg) FLOG_SHARE_FILE_LOG__(::sh::USER_LOG_TYPE, ::sh::ERROR_LOG_LEVEL_ID, lpmsg)
#define USER_LOG_FATAL(lpmsg) FLOG_SHARE_FILE_LOG__(::sh::USER_LOG_TYPE, ::sh::FATAL_LOG_LEVEL_ID, lpmsg)

#define MEMORY_LOG(lpmsg)       FLOG_SHARE_FILE_LOG__(::sh::MEMORY_LOG_TYPE, ::sh::INFO_LOG_LEVEL_ID, lpmsg)

#define SQL_LOG_DEBUG(lpmsg)        FLOG_SHARE_FILE_LOG__(::sh::SQL_LOG_TYPE, ::sh::DEBUG_LOG_LEVEL_ID, lpmsg)
#define SQL_LOG_INFO(lpmsg)        FLOG_SHARE_FILE_LOG__(::sh::SQL_LOG_TYPE, ::sh::INFO_LOG_LEVEL_ID, lpmsg)
#define SQL_LOG_WARN(lpmsg)        FLOG_SHARE_FILE_LOG__(::sh::SQL_LOG_TYPE, ::sh::WARN_LOG_LEVEL_ID, lpmsg)
#define SQL_LOG_ERROR(lpmsg)        FLOG_SHARE_FILE_LOG__(::sh::SQL_LOG_TYPE, ::sh::ERROR_LOG_LEVEL_ID, lpmsg)
#define SQL_LOG_FATAL(lpmsg)        FLOG_SHARE_FILE_LOG__(::sh::SQL_LOG_TYPE, ::sh::FATAL_LOG_LEVEL_ID, lpmsg)
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
/// 用户可以增加自己定义的日志类型。增加的类型，必须在配置文件 share_log_config.xml 里有配置
//////////////////////////////////////////////////////////////////////////


#endif //__SHARE_FILE_LOG_H__

