#include "sh_listener.h"


CListener::CListener(const char* ip,sh::uint32 port):m_ip(ip),m_port(port)
{

}

CListener::~CListener()
{

}

const string& CListener::get_service_ip() const
{
    return m_ip; 
}

const sh::uint32& CListener::get_service_port() const
{
    return m_port; 
}