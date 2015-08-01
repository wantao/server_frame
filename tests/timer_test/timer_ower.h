#ifndef __TIMER_OWER_H__
#define __TIMER_OWER_H__

#include "sh_timer.h"

class CTimerOwer
{
public:
    CTimerOwer();
    ~CTimerOwer();

    //����ӵ�еĶ�ʱ����ʼ�������ó�ʱʱ��
    bool timer_init();

    //��ʱ��������
    void on_timeout(const sh::uint64& timer_type);

protected:
private:
    CTimer<CTimerOwer,&CTimerOwer::on_timeout>  m_timer_5000;
    CTimer<CTimerOwer,&CTimerOwer::on_timeout>  m_timer_10000;
};

#endif