#ifndef __SH_SERVER_APPLIACTION_H__
#define __SH_SERVER_APPLIACTION_H__
#include "sh_include.h"
#include "sh_reactor_core.h"

class CListener;
class CServerAppliaction
{
    public:
        CServerAppliaction(std::string name);
        virtual ~CServerAppliaction();
        bool start_service();
        void add_service(CListener* pservice);
    private:
        CReactorCore* m_reactor;
        std::string m_service_name;
};
#endif
