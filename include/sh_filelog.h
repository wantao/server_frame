#ifndef __SHARE_FILE_LOG_H__
#define __SHARE_FILE_LOG_H__

#include <string>
#include <sstream>


namespace sh
{
typedef void* file_log_handle;

// ��־�ȼ�����
static const char*  DEBUG_LOG_LEVEL_NAME=   "debug";
static const char*  INFO_LOG_LEVEL_NAME=    "info";
static const char*  WARN_LOG_LEVEL_NAME=    "warn";
static const char*  ERROR_LOG_LEVEL_NAME=   "error";
static const char*  FATAL_LOG_LEVEL_NAME=   "fatal";

//ÿ����־�ȼ����ƶ�Ӧ��idֵ
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

//ϵͳ��ռĬ�ϵ���־����
static const int  SH_LOG_TYPE=0;
static const int  USER_LOG_TYPE=1;
static const int  MEMORY_LOG_TYPE=2;
static const int  SQL_LOG_TYPE=3;


//FAST_FLOG_SHARE_FILE_LOG__ʹ�á�����һ����־������ֽڳ��ȣ� ���ֵ���Ը���ʵ������޸�
static const int  FAST_LOG_BUFFER_SIZE_SHARE_FILE_LOG = 1024;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//// log, �򵥵��ı���־�����ݿ���־
//////////////////////////////////////////////////////////////////////////
// �����ļ��Ĵ��Ŀ¼������:  ����Ŀ¼/share_config/share_log_config.xml
// ���û�������ļ�, sh log �Զ�ʹ��Ĭ������
/* ������Ĭ�����á�
<root>
    <!--
    ��־���Ͷ�Ӧ����־Ŀ¼����־�ļ�����ͷ��β
    ÿ����־��Ӧ�ļ�¼����
    ��־����id����ð��մ�С������������[0-65535]���������ظ�
    ��־����name Ҳ�������ظ�
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

// ����Ƿ�д������͡��ȼ�����־
// �����Ҫд���򷵻����ָ�����log����ľ��
extern file_log_handle share_check_file_log(const unsigned int log_type_id, const int log_level);

//д�ı���־
extern void share_file_log(file_log_handle log_handle, const char* log_content);
}



#define  FLOG_SHARE_FILE_LOG__(logtype, loglevel, lpmsg) for(int _loglevel=loglevel; _loglevel; ){\
    ::sh::file_log_handle log_handle = ::sh::share_check_file_log(logtype,loglevel);\
    if (NULL == log_handle) break;\
    std::ostringstream oss; \
    oss<<"["<<::sh::sg_log_id_text[loglevel]<<"]"\
    <<"["<<__FILE__<<"]"<<"["<<__LINE__<<"]:"<<lpmsg;\
    ::sh::share_file_log(log_handle, oss.str().c_str());_loglevel=0;} //���������loglevel>0, ����forѭ���ĵ�һ��һ��������,����_loglevel=0, for ѭ�����Բ����еڶ��μ��ڶ�����������

//////////////////////////////////////////////////////////////////////////
/// ����Ԥ����õļ�����־��
/// SHARE_LOG_, SHARE_FAST_LOG_ ��ͷ����־��,һ���Ǽ�¼sh���ڲ���־
#define SHARE_LOG_DEBUG(lpmsg) FLOG_SHARE_FILE_LOG__(::sh::SH_LOG_TYPE, ::sh::DEBUG_LOG_LEVEL_ID, lpmsg)
#define SHARE_LOG_INFO(lpmsg) FLOG_SHARE_FILE_LOG__(::sh::SH_LOG_TYPE, ::sh::INFO_LOG_LEVEL_ID, lpmsg)
#define SHARE_LOG_WARN(lpmsg) FLOG_SHARE_FILE_LOG__(::sh::SH_LOG_TYPE, ::sh::WARN_LOG_LEVEL_ID, lpmsg)
#define SHARE_LOG_ERROR(lpmsg) FLOG_SHARE_FILE_LOG__(::sh::SH_LOG_TYPE, ::sh::ERROR_LOG_LEVEL_ID, lpmsg)
#define SHARE_LOG_FATAL(lpmsg) FLOG_SHARE_FILE_LOG__(::sh::SH_LOG_TYPE, ::sh::FATAL_LOG_LEVEL_ID, lpmsg)


/// USER_LOG_, USER_FAST_LOG_ ��ͷ����־��,һ���Ǽ�¼�û�����־
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
/// �û����������Լ��������־���͡����ӵ����ͣ������������ļ� share_log_config.xml ��������
//////////////////////////////////////////////////////////////////////////


#endif //__SHARE_FILE_LOG_H__

