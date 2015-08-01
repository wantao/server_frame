#include "sh_filelog.h"


int main()
{
    USER_LOG_DEBUG("test log "<<"USER_LOG_DEBUG");
    USER_LOG_INFO("test log "<<"USER_LOG_INFO");
    USER_LOG_WARN("test log "<<"USER_LOG_WARN");
    USER_LOG_ERROR("test log "<<"SHARE_LOG_ERROR");
    USER_LOG_FATAL("test log "<<"SHARE_LOG_FATAL");
    
    SHARE_LOG_DEBUG("test log "<<"SHARE_LOG_DEBUG");
    SHARE_LOG_INFO("test log "<<"SHARE_LOG_INFO");
    SHARE_LOG_WARN("test log "<<"SHARE_LOG_WARN");
    SHARE_LOG_ERROR("test log "<<"SHARE_LOG_ERROR");
    SHARE_LOG_FATAL("test log "<<"SHARE_LOG_FATAL");
    
    MEMORY_LOG("test log "<<"MEMORY_LOG");
    
    SQL_LOG_DEBUG("test log "<<"SQL_LOG_DEBUG");
    SQL_LOG_INFO("test log "<<"SQL_LOG_INFO");
    SQL_LOG_WARN("test log "<<"SQL_LOG_WARN");
    SQL_LOG_ERROR("test log "<<"SQL_LOG_ERROR");
    SQL_LOG_FATAL("test log "<<"SQL_LOG_FATAL");
    
    return 0;
}



