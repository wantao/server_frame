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
        std::string  folder_name;   // ��־��Ŀ¼
        std::string  log_file_head; // ��־�ļ����Ƶ��ײ�
        std::string  log_file_trail;// ��־�ļ��ĺ�׺
    };

    struct log_level_set
    {
        log_level_set() : debug(0),info(0),warn(0),error(0),fatal(0)
        {}

        uint8   debug; // ����� 0 ���ʾ��
        uint8   info;
        uint8   warn;
        uint8   error;
        uint8   fatal;
    };

    struct log_info
    {
        log_info() : fp(NULL),id(0) {}

        std::mutex              flog_mutex;    // д��־�ļ�������ÿһ����־�ļ���Ӧһ����
        std::string             filename;  // ��־���ļ�����
        FILE*                   fp;        // ��ǰ����־�ļ��ľ��
        log_level_set           levels;    // ��ǰ�����־�ļ���д��������
        path_file_head_trail    path; 
        std::string             name;
        uint16                  id;
        time_t                  last_write_time; // ���һ��д��־��ʱ��

        // ���һ��д��־��ʱ�̵�ϵͳʱ���ʾ(���ֵ��ʵ������last_write_time��ã����Ƿ�ֹ��ĳ������£�һ����д��N����¼����ʱ��
        // last_tm ��Ӧ����һ��������Ҫ������last_tm�����������һ��ռ任һ��ʱ��)
        struct tm               last_tm;     
    };

    typedef struct log_info** logs_info_type;


    file_log();
    ~file_log();

    static file_log& instance()
    {
        //�ö���Ӷ�һ��ʹ�ÿ�ʼ���������̽���������
        static file_log* flog = NULL;
        static std::mutex flog_instance_mutex;
        if (NULL == flog){
            std::lock_guard<std::mutex> lck (flog_instance_mutex);
            flog = new file_log();
        }
        return *flog;
    }

    // ��־
    void log(log_info& log, const char* log_content); 
    
    // ����Ƿ���Ҫд��־
    inline log_info* check_log(const unsigned int log_type_id, const int log_level)
    {
        if (log_type_id>=m_max_log_id) return NULL;
        // m_logs_info ����� ��־����������Ȼ�п��ܱ� reload ��д������
        // �����д���ᵼ�� log ����Ч�ԣ������Ƕ�д��x����־������д��y����־
        // �������￼�ǵ����ܣ�û�ж� m_logs_info �ķ���ʹ����
        log_info& log = *(m_logs_info[log_type_id]);
        if ((log.levels.debug && DEBUG_LOG_LEVEL_ID == log_level) || (log.levels.info && INFO_LOG_LEVEL_ID == log_level) || (log.levels.warn && WARN_LOG_LEVEL_ID == log_level) || (log.levels.error && ERROR_LOG_LEVEL_ID == log_level)|| (log.levels.fatal && FATAL_LOG_LEVEL_ID == log_level))
            return &log;
        return NULL;
    }

private:
    // �ɹ�����0
    int _init();
    // �ɹ�����0
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

//д�ı���־
void sh::share_file_log(sh::file_log_handle log_handle, const char* log_content)
{
    sh::file_log::instance().log(*((sh::file_log::log_info*)log_handle), log_content);
}

sh::file_log::file_log()
{
    m_logs_info = NULL;
    m_has_init = false;

    //_init()����д�����
    _init();
    //assert(m_has_init && "log file init fail !");
}

sh::file_log::~file_log()
{
    // file_log����������Ӧ��Ҫ��ʹ�����Ľ��̵���������һ����
    // �������������������ͷ���Դ����,��ʵ���Բ���д,��Ϊ���̹ر���, ��ԴҲ����֮�ͷ�
    // ����Ϊ�˷������˼��, �ͼ�������ͷ���Դ����. o(��_��)o 
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

    if (strcmp(full_path, log.filename.c_str())) { // linux ���ļ�Ŀ¼�����ִ�Сд��
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

    std::set<uint16> ids_for_check_repeated; //�����ж��Ƿ����ظ�id
    std::set<std::string> name_for_check_repeated;//�����ж��Ƿ����ظ�����
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

    // �������ã����ɶ�Ӧ����־Ŀ¼�Լ��ļ�
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



