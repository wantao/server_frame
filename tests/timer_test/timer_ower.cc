#include "timer_ower.h"

CTimerOwer::CTimerOwer()
{
}

CTimerOwer::~CTimerOwer()
{

}

bool CTimerOwer::timer_init()
{
    //5�붨ʱ����ʼ��
    m_timer_5000.set_owner(this);  
    m_timer_5000.set_expire(5000);

    //10�붨ʱ����ʼ��
    m_timer_10000.set_owner(this);  
    m_timer_10000.set_expire(10000);
    return true;
}


void CTimerOwer::on_timeout(const sh::uint64& timer_type)
{
    USER_LOG_INFO("timer_test timer_type:"<<timer_type);
    if (timer_type == 5000) {
        //5�볬ʱ����begin

        //5�볬ʱ����end
        //�����Ҫ�ö�ʱ���������³�ʼ��
        m_timer_5000.set_owner(this);
        m_timer_5000.set_expire(5000);
    } else if (timer_type == 10000) {
        //10�볬ʱ����begin

        //10�볬ʱ����end
        //�����Ҫ�ö�ʱ���������³�ʼ��
        m_timer_10000.set_owner(this);
        m_timer_10000.set_expire(10000);
    }
    
}