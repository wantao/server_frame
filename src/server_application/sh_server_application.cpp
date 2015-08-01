#include "sh_server_application.h"
#include "sh_listener.h"
#include "sh_filelog.h"

CServerAppliaction::CServerAppliaction(std::string name)
{
    m_service_name = name;
    m_reactor = CSingleton<CReactorCore>::get_instance();
    m_reactor->init(); 
}

CServerAppliaction::~CServerAppliaction()
{
    USER_LOG_INFO("delete server application");
}

bool CServerAppliaction::start_service()
{
    event_base_dispatch(m_reactor->get_event_base());
    return false;
}

void CServerAppliaction::add_service(CListener *pservice)
{
    if(m_reactor->add_listener_event(pservice->get_service_ip(),pservice->get_service_port(),pservice) < 0)
    {
        USER_LOG_INFO("add server"<<m_service_name.c_str()<<" port:"<<pservice->get_service_port()<<"not success"); 
    }
    USER_LOG_INFO("add CListener "<<m_service_name.c_str()<<" listen in "<<pservice->get_service_ip()<<":"<<pservice->get_service_port());
}

