#include "timer_ower.h"


int main(void)
{
    CSingleton<CReactorCore>::get_instance()->init();
    CTimerOwer timer_ower;
    timer_ower.timer_init();
    //CTimer timer(10000);
    CSingleton<CReactorCore>::get_instance()->start_dispatch();
    printf("11111\n");
}