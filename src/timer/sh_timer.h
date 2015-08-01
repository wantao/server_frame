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

        //���øö�ʱ����ӵ���ߣ���ʱ�󣬵���ӵ���ߵĳ�ʱ������
        void set_owner(T* owner)
        {
            m_owner = owner;
        }
        //���ö�ʱ���ĳ�ʱʱ�䣨ms��
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
        //�Ƴ���ʱ��
        void remove_timer()
        {
            if (m_ev != NULL) {
                evtimer_del(m_ev); 
                m_ev = NULL;
            }
        }

    protected:
        //��ʱ�ص���������ʱ��ӵ������ʵ���Լ���on_timeout����
        void on_timeout()
        {
            (m_owner->*callback)(m_timer_type);
        }
   
    private:
        //��ʱ��ӵ����
        T* m_owner;
        //��ʱ���¼�
        struct event* m_ev;
        //��ʱ��������(��λ����)
        sh::uint64 m_timer_type;
};

#endif
