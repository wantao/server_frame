#ifndef __TIMER_OWER_H__
#define __TIMER_OWER_H__

#include "sh_timer.h"

class CTimerOwer
{
public:
    CTimerOwer();
    ~CTimerOwer();

    //将所拥有的定时器初始化并设置超时时间
    bool timer_init();

    //超时函数处理
    void on_timeout(const sh::uint64& timer_type);

protected:
private:
    CTimer<CTimerOwer,&CTimerOwer::on_timeout>  m_timer_5000;
    CTimer<CTimerOwer,&CTimerOwer::on_timeout>  m_timer_10000;
};

#endif