#ifndef __SH_SERVICE_H__
#define __SH_SERVICE_H__
#include "sh_include.h"
#include "sh_type.h"
#include "sh_libevent_include.h"


//监听器
class CListener
{
    public:
        CListener(const char* ip,sh::uint32 port);
        virtual ~CListener();
        
        //获取监听服务的ip
        const string& get_service_ip() const;
        //获取监听服务的端口
        const sh::uint32& get_service_port() const;

        //由继承者实现监听函数
        virtual sh::int32 on_accept(struct bufferevent* event,evutil_socket_t fd,struct sockaddr_in* sa) = 0;

    private:
        std::string m_ip;//监听服务的ip
        sh::uint32 m_port;//监听服务的端口
};

#endif
