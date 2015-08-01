#ifndef __SH_TIMER_H__
#define __SH_TIMER_H__
#include "sh_include.h"
#include "sh_type.h"
#include "sh_singleton.h"
#include "sh_reactor_core.h"
#include "sh_filelog.h"

void common_timeout_call(evutil_socket_t socket_t,short param,void *arg);

class CBaseTimer
{
public:
    virtual ~CBaseTimer(){}
    virtual void on_timeout() = 0;
};

template<typename T,void (T::*callback)(const sh::uint64& timer_type) = &T::on_timeout>

class CTimer:public CBaseTimer
{
    public:
        CTimer()
        {
            m_ev = evtimer_new(CSingleton<CReactorCore>::get_instance()->get_event_base(),common_timeout_call,(void*)this);
        }

        ~CTimer()
        {
            if (m_ev != NULL) {
                evtimer_del(m_ev);
            }
        }

        //设置该定时器的拥有者，超时后，调用拥有者的超时处理函数
        void set_owner(T* owner)
        {
            m_owner = owner;
        }
        //设置定时器的超时时间（ms）
        void set_expire(sh::uint64 ms)
        {
            m_timer_type = ms;
            if (m_ev != NULL) {
                struct timeval tv;
                evutil_timerclear(&tv);
                tv.tv_sec = ms/(1000);
                tv.tv_usec = (ms%1000)*1000;
                sh::int32 rst = evtimer_add(m_ev,&tv);             
                if (rst == -1) {
                    USER_LOG_ERROR("add timeer error:"<<ms);
                }
            }
        }
        //移除定时器
        void remove_timer()
        {
            if (m_ev != NULL) {
                evtimer_del(m_ev); 
                m_ev = NULL;
            }
        }

    protected:
        //超时回调函数，定时器拥有者需实现自己的on_timeout函数
        void on_timeout()
        {
            (m_owner->*callback)(m_timer_type);
        }
   
    private:
        //定时器拥有者
        T* m_owner;
        //定时器事件
        struct event* m_ev;
        //定时器的类型(单位毫秒)
        sh::uint64 m_timer_type;
};

#endif
