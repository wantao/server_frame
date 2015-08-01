#include "timer_ower.h"

CTimerOwer::CTimerOwer()
{
}

CTimerOwer::~CTimerOwer()
{

}

bool CTimerOwer::timer_init()
{
    //5秒定时器初始化
    m_timer_5000.set_owner(this);  
    m_timer_5000.set_expire(5000);

    //10秒定时器初始化
    m_timer_10000.set_owner(this);  
    m_timer_10000.set_expire(10000);
    return true;
}


void CTimerOwer::on_timeout(const sh::uint64& timer_type)
{
    USER_LOG_INFO("timer_test timer_type:"<<timer_type);
    if (timer_type == 5000) {
        //5秒超时处理begin

        //5秒超时处理end
        //如果还要该定时器，则重新初始化
        m_timer_5000.set_owner(this);
        m_timer_5000.set_expire(5000);
    } else if (timer_type == 10000) {
        //10秒超时处理begin

        //10秒超时处理end
        //如果还要该定时器，则重新初始化
        m_timer_10000.set_owner(this);
        m_timer_10000.set_expire(10000);
    }
    
}