#include "sh_filelog.h"
#include "sh_common.h"
#include <string>
#include <vector>
#include <set>
#include <thread> 
#include <mutex>


#define SHOW_ERROR_MSG(errormsg) {printf("error: %s, press any key to contine\n", errormsg);getchar(); }
#ifndef MAX_PATH
#define MAX_PATH 256
#endif 

namespace sh
{

class file_log
{
public:
    struct path_file_head_trail
    {
        std::string  folder_name;   // 日志的目录
        std::string  log_file_head; // 日志文件名称的首部
        std::string  log_file_trail;// 日志文件的后缀
    };

    struct log_level_set
    {
        log_level_set() : debug(0),info(0),warn(0),error(0),fatal(0)
        {}

        uint8   debug; // 如果非 0 则表示打开
        uint8   info;
        uint8   warn;
        uint8   error;
        uint8   fatal;
    };

    struct log_info
    {
        log_info() : fp(NULL),id(0) {}

        std::mutex              flog_mutex;    // 写日志文件的锁，每一个日志文件对应一个锁
        std::string             filename;  // 日志的文件名称
        FILE*                   fp;        // 当前打开日志文件的句柄
        log_level_set           levels;    // 当前这个日志文件的写级别配置
        path_file_head_trail    path; 
        std::string             name;
        uint16                  id;
        time_t                  last_write_time; // 最后一次写日志的时刻

        // 最后一次写日志的时刻的系统时间表示(这个值其实可以用last_write_time求得，但是防止在某种情况下，一秒内写了N条记录，这时候
        // last_tm 的应都是一样，就需要重新求last_tm，这里就是以一点空间换一点时间)
        struct tm               last_tm;     
    };

    typedef struct log_info** logs_info_type;


    file_log();
    ~file_log();

    static file_log& instance()
    {
        //该对象从对一次使用开始创建，进程结束后销毁
        static file_log* flog = NULL;
        static std::mutex flog_instance_mutex;
        if (NULL == flog){
            std::lock_guard<std::mutex> lck (flog_instance_mutex);
            flog = new file_log();
        }
        return *flog;
    }

    // 日志
    void log(log_info& log, const char* log_content); 
    
    // 检查是否需要写日志
    inline log_info* check_log(const unsigned int log_type_id, const int log_level)
    {
        if (log_type_id>=m_max_log_id) return NULL;
        // m_logs_info 里面的 日志级别内容虽然有可能被 reload 重写，但是
        // 这个重写不会导致 log 的有效性，最多就是多写了x条日志或者少写了y条日志
        // 所以这里考虑到性能，没有对 m_logs_info 的访问使用锁
        log_info& log = *(m_logs_info[log_type_id]);
        if ((log.levels.debug && DEBUG_LOG_LEVEL_ID == log_level) || (log.levels.info && INFO_LOG_LEVEL_ID == log_level) || (log.levels.warn && WARN_LOG_LEVEL_ID == log_level) || (log.levels.error && ERROR_LOG_LEVEL_ID == log_level)|| (log.levels.fatal && FATAL_LOG_LEVEL_ID == log_level))
            return &log;
        return NULL;
    }

private:
    // 成功返回0
    int _init();
    // 成功返回0
    int _load();

private:
    bool                m_has_init;
    logs_info_type      m_logs_info;
    uint16              m_max_log_id;
};

}

sh::file_log_handle sh::share_check_file_log(const unsigned int log_type_id, const int log_level)
{
    return sh::file_log::instance().check_log(log_type_id,log_level);
}

//写文本日志
void sh::share_file_log(sh::file_log_handle log_handle, const char* log_content)
{
    sh::file_log::instance().log(*((sh::file_log::log_info*)log_handle), log_content);
}

sh::file_log::file_log()
{
    m_logs_info = NULL;
    m_has_init = false;

    //_init()必须写在最后
    _init();
    //assert(m_has_init && "log file init fail !");
}

sh::file_log::~file_log()
{
    // file_log的生命周期应该要与使用它的进程的生命周期一样。
    // 所以这个析构函数里的释放资源操作,其实可以不用写,因为进程关闭了, 资源也会随之释放
    // 这里为了符合设计思想, 就加上这个释放资源代码. o(∩_∩)o 
    if (NULL != m_logs_info) {
        for (uint16 i=0; i<m_max_log_id; ++i) {
            if (NULL != m_logs_info[i]) {
                if (NULL != m_logs_info[i]->fp) {
                    fclose(m_logs_info[i]->fp);
                    m_logs_info[i]->fp = NULL;
                    delete m_logs_info[i];
                    m_logs_info[i] = NULL;
                }
            }
        }
        delete []m_logs_info;
        m_logs_info = NULL;
    }
}

int sh::file_log::_init()
{
    if (m_has_init) return 0;

    if (NULL != m_logs_info){
        delete []m_logs_info;
        m_logs_info = NULL;
    }

    int r = _load();

    if (0 == r) 
        m_has_init = true;

    return r;
}

void sh::file_log::log(file_log::log_info& log, const char* log_content)
{
    std::lock_guard<std::mutex> lck(log.flog_mutex);
    const time_t now_time = time(NULL);
    if (now_time != log.last_write_time) {
        log.last_write_time = now_time;
        tm stm = {0};
        localtime_r(&now_time, &stm);
        memcpy(&log.last_tm, &stm, sizeof(log.last_tm));
    }

    char full_path[MAX_PATH*2];
    snprintf(full_path, sizeof(full_path)-1, "%s/%s/%s_%d-%02d-%02d%s", sh::get_current_exe_directory(),
        log.path.folder_name.c_str(), log.path.log_file_head.c_str(), 
        log.last_tm.tm_year+1900, log.last_tm.tm_mon+1, log.last_tm.tm_mday, log.path.log_file_trail.c_str());
    full_path[sizeof(full_path)-1] = '\0';

    if (strcmp(full_path, log.filename.c_str())) { // linux 下文件目录是区分大小写的
        if (NULL != log.fp){
            fclose(log.fp);
        }

        log.fp = fopen(full_path, "a+");
        log.filename = full_path;
    }

    if (NULL != log.fp) {
        fprintf(log.fp, "[%02d:%02d:%02d]%s\n", log.last_tm.tm_hour, log.last_tm.tm_min, log.last_tm.tm_sec, log_content);
        fflush(log.fp);
    }
}

int sh::file_log::_load()
{
    char full_path[MAX_PATH]={0};
    snprintf(full_path, sizeof(full_path)-1, "%s/share_config/share_log_config.xml", sh::get_current_exe_directory());

    CMarkupSTL oxml;
    if ( ! oxml.Load(full_path) ) {
        const char *doc = "<root>\
            <logtype>\
                <type id=\"0\" name=\"ShareLog\"    folder_name=\"ShareLog\"   log_file_head=\"sh\"  log_file_trail=\".log\">\
                <item key=\"debug\" value=\"yes\" />\
                <item key=\"info\"  value=\"yes\" />\
                <item key=\"warn\"  value=\"yes\" />\
                <item key=\"error\" value=\"yes\" />\
                <item key=\"fatal\" value=\"yes\" />\
                </type>\
                <type id=\"1\" name=\"UserLog\"     folder_name=\"UserLog\"    log_file_head=\"user\"   log_file_trail=\".log\">\
                <item key=\"debug\" value=\"yes\" />\
                <item key=\"info\"  value=\"yes\" />\
                <item key=\"warn\"  value=\"yes\" />\
                <item key=\"error\" value=\"yes\" />\
                <item key=\"fatal\" value=\"yes\" />\
                </type>\
                <type id=\"2\" name=\"MemoryLog\"    folder_name=\"MemoryLog\"   log_file_head=\"memory\"  log_file_trail=\".log\">\
                <item key=\"debug\" value=\"yes\" />\
                <item key=\"info\"  value=\"yes\" />\
                <item key=\"warn\"  value=\"yes\" />\
                <item key=\"error\" value=\"yes\" />\
                <item key=\"fatal\" value=\"yes\" />\
                </type>\
                <type id=\"3\" name=\"SqlLog\"     folder_name=\"SqlLog\"    log_file_head=\"sql\"   log_file_trail=\".log\">\
                <item key=\"debug\" value=\"yes\" />\
                <item key=\"info\"  value=\"yes\" />\
                <item key=\"warn\"  value=\"yes\" />\
                <item key=\"error\" value=\"yes\" />\
                <item key=\"fatal\" value=\"yes\" />\
                </type>\
            </logtype>\
            </root>";
        if (!oxml.SetDoc(doc)) {
            SHOW_ERROR_MSG("can not read log config ok!");
            return -1;
        }
    }

    if ( ! oxml.FindElem("root") ) {
        SHOW_ERROR_MSG("can not find the root in the log config");
        return -1;
    }

    if ( ! oxml.IntoElem() ) {
        SHOW_ERROR_MSG("can not into the root");
        return -1;
    }

    std::set<uint16> ids_for_check_repeated; //用来判断是否有重复id
    std::set<std::string> name_for_check_repeated;//用来判断是否有重复名称
    std::vector<log_info*> vec_logs;
    if (oxml.FindElem("logtype")) {
        if (oxml.IntoElem()) {
            while (oxml.FindElem("type")) {
                uint16 id = (uint16)atoi(oxml.GetAttrib("id").c_str());
                if ( ! ids_for_check_repeated.insert(id).second ){
                    printf("log id repeat id=%u!\n", id);
                    assert(false);
                    continue;
                }
                std::string name = oxml.GetAttrib("name");
                if ( ! name_for_check_repeated.insert(name).second ){
                    printf("log name repeat name=%s\n", name.c_str());
                    assert(false);
                    continue;
                }

                log_info* log = new log_info();
                log->last_write_time = time(NULL);
                memset(&(log->last_tm), 0, sizeof(log->last_tm));
                log->id = id;
                log->fp = NULL;
                log->name = name;
                log->path.folder_name = oxml.GetAttrib("folder_name");
                log->path.log_file_head = oxml.GetAttrib("log_file_head");
                log->path.log_file_trail = oxml.GetAttrib("log_file_trail");

                if (oxml.IntoElem()) {
                    while (oxml.FindElem("item")) {
                        const std::string yes = "yes";
                        std::string key = oxml.GetAttrib("key");
                        std::string value = oxml.GetAttrib("value");

                        if (key==DEBUG_LOG_LEVEL_NAME && value==yes)
                            log->levels.debug = 1;
                        if (key==INFO_LOG_LEVEL_NAME && value==yes)
                            log->levels.info = 1;
                        if (key==WARN_LOG_LEVEL_NAME && value==yes)
                            log->levels.warn = 1;
                        if (key==ERROR_LOG_LEVEL_NAME && value==yes)
                            log->levels.error = 1;
                        if (key==FATAL_LOG_LEVEL_NAME && value==yes)
                            log->levels.fatal = 1;
                    }
                    oxml.OutOfElem();
                }
                vec_logs.push_back(log);
            }
            oxml.OutOfElem();
        }
    }

    time_t now_time = time(NULL);
    tm stm = {0};
    localtime_r(&now_time, &stm);

    // 根据配置，生成对应的日志目录以及文件
    uint16 max_log_type_id = 1;
    std::vector<log_info*>::iterator itr;
    for (itr=vec_logs.begin(); itr!=vec_logs.end(); ++itr) {
        log_info* log = *itr;
        log->last_write_time = now_time;
        memcpy(&(log->last_tm), &stm, sizeof(log->last_tm));

        char full_path[MAX_PATH]={0};
        snprintf(full_path, sizeof(full_path)-1, "%s/%s", sh::get_current_exe_directory(),log->path.folder_name.c_str());
        if (!opendir(full_path)) {
            if (-1 == mkdir(full_path, 0777)) {
                printf("cannot create director %s\n", full_path);
                continue;
            }
        }

        memset(full_path, 0, sizeof(full_path));
        snprintf(full_path, sizeof(full_path)-1, "%s/%s/%s_%d-%02d-%02d%s", sh::get_current_exe_directory(),
            log->path.folder_name.c_str(), log->path.log_file_head.c_str(), 
            stm.tm_year+1900, stm.tm_mon+1, stm.tm_mday, log->path.log_file_trail.c_str());

        FILE* fp = fopen(full_path, "a+");
        if (NULL == fp) {
            printf("cannot open file %s\n", full_path);
            continue;
        }

        log->filename = full_path;
        log->fp = fp;

        if (log->id > max_log_type_id)
            max_log_type_id = log->id;
    }

    m_max_log_id = max_log_type_id+1;
    m_logs_info = new log_info*[m_max_log_id];
    memset((char*)m_logs_info, 0, sizeof(log_info*)*m_max_log_id);
    for (itr=vec_logs.begin(); itr!=vec_logs.end(); ++itr) {
        log_info* log = *itr;
        m_logs_info[log->id] = log;
    }
    return 0;
}



