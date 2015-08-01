#include "gs_client_listener.h"
#include "sh_reactor_core.h"
#include "sh_filelog.h"

int main(void)
{
    if (!CSingleton<CReactorCore>::get_instance()->init()) {
        USER_LOG_ERROR("reactorCore Failure");
        return -1;
    }

    CClientListener client_listener("0.0.0.0",9998);

    if (0 != CSingleton<CReactorCore>::get_instance()->add_listener_event(client_listener.get_service_ip(),client_listener.get_service_port(),&client_listener)) {
        USER_LOG_ERROR("reactorCore add_listener_event Failure");
        return -1;
    }

    CSingleton<CReactorCore>::get_instance()->start_dispatch();
    return 0;
}