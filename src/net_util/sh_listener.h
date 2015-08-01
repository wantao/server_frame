#ifndef __SH_SERVICE_H__
#define __SH_SERVICE_H__
#include "sh_include.h"
#include "sh_type.h"
#include "sh_libevent_include.h"


//������
class CListener
{
    public:
        CListener(const char* ip,sh::uint32 port);
        virtual ~CListener();
        
        //��ȡ���������ip
        const string& get_service_ip() const;
        //��ȡ��������Ķ˿�
        const sh::uint32& get_service_port() const;

        //�ɼ̳���ʵ�ּ�������
        virtual sh::int32 on_accept(struct bufferevent* event,evutil_socket_t fd,struct sockaddr_in* sa) = 0;

    private:
        std::string m_ip;//���������ip
        sh::uint32 m_port;//��������Ķ˿�
};

#endif
