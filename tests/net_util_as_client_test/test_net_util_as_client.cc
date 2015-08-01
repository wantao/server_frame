#include "sh_reactor_core.h"
#include "sh_filelog.h"
#include "gs_client.h"
#include "sh_common.h"

#define MYSIGRTMIN    34
#define TEST_SINAL      (MYSIGRTMIN + 1)


static void
test_signal_cb(evutil_socket_t fd, short event, void *arg)
{
    printf("test_signal_cb\r\n");

    //printf("%s: got signal %d\n", __func__, EVENT_SIGNAL(signal));
}


int main(int argc,char* argv[])
{
    if (argc != 2) {
        printf("formate like this:program send_data_len");
        return -1;
    }
    if (!CSingleton<CReactorCore>::get_instance()->init()) {
        USER_LOG_ERROR("reactorCore Failure");
        return -1;
    }
    
    //添加一个永久的测试信号事件，否则dispatch会因没有事件而退出
    struct event signal_int;
    event_assign(&signal_int, CSingleton<CReactorCore>::get_instance()->get_event_base(), TEST_SINAL, EV_SIGNAL|EV_PERSIST, test_signal_cb,
        NULL);
    event_add(&signal_int, NULL);

    CGsClient gs_client;
    if (gs_client.connect_to("127.0.0.1",9995) != 0) {
        USER_LOG_ERROR("in main connect to 127.0.0.1:9995 failure");
        return -1;
    }

    std::string data_packet = gs_client.create_data_packet(sh::a2u(argv[1]));
    gs_client.send_msg(data_packet.c_str(),data_packet.size());

    CSingleton<CReactorCore>::get_instance()->start_dispatch();
    return 0;
}