#ifndef __SHARE_SYSTEM_TIME_H__
#define __SHARE_SYSTEM_TIME_H__

#if !(defined(WIN32) || defined(WIN64))
#include   <sys/times.h> 
#endif

namespace sh
{

//////////////////////////////////////////////////////////////////////////
//// System Time
//////////////////////////////////////////////////////////////////////////
/**
* @defgroup grouptime ʱ�����
* @{
*/
class CDateTime;

/**
* @brief ��ȡ����ǰ����ʱ��(���ô˺����Ĵ�ʱ�˿�)��CDateTime
* @return ����ǰ����ʱ��(���ô˺����Ĵ�ʱ�˿�)��CDateTime
*/

CDateTime now();

/**
* @brief ����ʱ����,����1970����ҹ��2037��֮���ĳһ��ʱ��
*
*/
class  CDateTime
{
public:
    /**
    * @brief ����һ��CDateTime��,���������ʱ��Ϊ1970-1-1��ҹ
    */
    CDateTime(time_t t = 0);


    /**
    * @brief ��ȡ���������������ʱ���time_tֵ
    * @return ���ص�time_tֵ
    */
    time_t get_time_value();

    /**
    * @brief ���ñ��������������ʱ��
    * @param t ���õ�����ʱ��
    * @return void
    */
    void set_time_value(time_t t);

    /**
    * @brief ��ȡ���������������ʱ����datetime�����������ʱ���������
    * @param datetime һ������ʱ��
    * @return ��������
    */
    int64 operator - (const CDateTime &datetime);

    /**
    * @brief ��ȡ���������������ʱ����datetime�����������ʱ���������
    * @param datetime һ������ʱ��
    * @return ��������
    */
    int64 diff_second(const CDateTime &datetime);

    /**
    * @brief ��ȡ���������������ʱ����datetime�����������ʱ����ķ�����,�������1���ӵĲ���������
    * @param datetime һ������ʱ��
    * @return ���ķ�����
    */
    int64 diff_minute(const CDateTime &datetime);

    /**
    * @brief ��ȡ���������������ʱ����datetime�����������ʱ�����Сʱ��,�������1Сʱ�Ĳ���������
    * @param datetime һ������ʱ��
    * @return ����Сʱ��
    */
    int64 diff_hour(const CDateTime &datetime);

    /**
    * @brief ��ȡ���������������ʱ����datetime�����������ʱ���������,�������1��Ĳ���������
    * @param datetime һ������ʱ��
    * @return ��������
    */
    int64 diff_day(const CDateTime &datetime);

    /**
    * @brief ��ȡ���������������ʱ����datetime�����������ʱ�����������,�������1���ڵĲ���������
    * @param datetime һ������ʱ��
    * @return ����������
    */
    int64 diff_week(const CDateTime &datetime);

    /**
    * @brief ��ȡ���������������ʱ����datetime�����������ʱ������·�,�������1���µĲ���������
    * @param datetime һ������ʱ��
    * @return ����������
    */
    int32 diff_month(const CDateTime &datetime);

    /**
    * @brief ��ȡ���������������ʱ����datetime�����������ʱ���������,�������1��Ĳ���������
    * @param datetime һ������ʱ��
    * @return ��������
    */
    int32 diff_year(const CDateTime &datetime);

    /**
    * @brief ���ӱ��������������ʱ�������
    * @param year ���ӵ�����
    * @return �µ�����ʱ��
    */
    CDateTime & inc_year(uint32 year = 1);

    /**
    * @brief ���ٱ��������������ʱ�������
    * @param year ���ٵ�����
    * @return �µ�����ʱ��
    */
    CDateTime & dec_year(uint32 year = 1);

    /**
    * @brief ���ӱ��������������ʱ�������
    * @param month ���ӵ�����
    * @return �µ�����ʱ��
    */
    CDateTime & inc_month(uint32 month = 1);

    /**
    * @brief ���ٱ��������������ʱ�������
    * @param month ���ٵ�����
    * @return �µ�����ʱ��
    */
    CDateTime & dec_month(uint32 month = 1);

    /**
    * @brief ���ӱ��������������ʱ�������
    * @param day ���ӵ�����
    * @return �µ�����ʱ��
    */
    CDateTime & inc_day(uint32 day = 1);

    /**
    * @brief ���ٱ��������������ʱ�������
    * @param day ���ٵ�����
    * @return �µ�����ʱ��
    */
    CDateTime & dec_day(uint32 day = 1);

    /**
    * @brief ���ӱ��������������ʱ���Сʱ��
    * @param hour ���ӵ�Сʱ��
    * @return �µ�����ʱ��
    */
    CDateTime & inc_hour(uint32 hour = 1);

    /**
    * @brief ���ٱ��������������ʱ���Сʱ��
    * @param hour ���ٵ�Сʱ��
    * @return �µ�����ʱ��
    */
    CDateTime & dec_hour(uint32 hour = 1);

    /**
    * @brief ���ӱ��������������ʱ��ķ�����
    * @param minute ���ӵķ�����
    * @return �µ�����ʱ��
    */
    CDateTime & inc_minute(uint32 minute = 1);

    /**
    * @brief ���ٱ��������������ʱ��ķ�����
    * @param minute ���ٵķ�����
    * @return �µ�����ʱ��
    */
    CDateTime & dec_minute(uint32 minute = 1);

    /**
    * @brief ���ӱ��������������ʱ�������
    * @param second ���ӵ�����
    * @return �µ�����ʱ��
    */
    CDateTime & inc_second(uint32 second = 1);

    /**
    * @brief ���ٱ��������������ʱ�������
    * @param second ���ٵ�����
    * @return �µ�����ʱ��
    */
    CDateTime & dec_second(uint32 second = 1);

    /**
    * @brief ���ӱ��������������ʱ���������
    * @param week ���ӵ�������
    * @return �µ�����ʱ��
    */
    CDateTime & inc_week(uint32 week = 1);

    /**
    * @brief ���ٱ��������������ʱ���������
    * @param week ���ٵ�������
    * @return �µ�����ʱ��
    */
    CDateTime & dec_week(uint32 week = 1);



    /**
    * @brief ���ñ��������������ʱ��
    * @param year ���õ����[1970-2037]
    * @param month �趨�Ĵ�����ĸ���[1-12]
    * @param day �趨�Ĵ��µĵڼ���[1-31]
    * @param hours �趨�Ĵ���ĵڼ�Сʱ[0-23]
    * @param minutes �趨�Ĵ�Сʱ�ĵڼ�����[0-59]
    * @param seconds �趨�Ĵ˷��ӵĵڼ���[0-59]
    * @return �Ƿ����óɹ�,falseΪû�����óɹ�
    */
    bool set_date_time(uint32 year, uint32 month, uint32 day, uint32 hours, uint32 minutes, uint32 seconds);

    /**
    * @brief ���ñ��������������,ʱ�䲻��
    * @param year ���õ����[1970-2037]
    * @param month �趨�Ĵ�����ĸ���[1-12]
    * @param day �趨�Ĵ��µĵڼ���[1-31]
    * @return �Ƿ����óɹ�,falseΪû�����óɹ�
    */
    bool set_date(uint32 year, uint32 month, uint32 day);

    /**
    * @brief ���ñ����������ʱ��,���ڲ���
    * @param hours �趨�Ĵ���ĵڼ�Сʱ[0-23]
    * @param minutes �趨�Ĵ�Сʱ�ĵڼ�����[0-59]
    * @param seconds �趨�Ĵ˷��ӵĵڼ���[0-59]
    * @return �Ƿ����óɹ�,falseΪû�����óɹ�
    */
    bool set_time(uint32 hours, uint32 minutes, uint32 seconds);

    /**
    * @brief ��ȡ��������������
    * @return ��ȡ�����
    */
    uint32 get_year();

    /**
    * @brief ��ȡ���������������ʱ�����ڵ���ݵĵڼ�����
    * @return ��ȡ���·�
    */
    uint32 get_month();

    /**
    * @brief ��ȡ���������������ʱ�����ڵ��·ݵĵڼ���
    * @return ��ȡ������
    */
    uint32 get_day();

    /**
    * @brief ��ȡ���������������ʱ�����ڵ���ĵڼ���Сʱ
    * @return ��ȡ��Сʱ��
    */
    uint32 get_hour();

    /**
    * @brief ��ȡ���������������ʱ�����ڵ�Сʱ�ķ�����
    * @return ��ȡ�ķ�����
    */
    uint32 get_minute();

    /**
    * @brief ���ñ��������������ʱ�����ڵķ��ӵ�����
    * @return ��ȡ������
    */
    uint32 get_second();

    /**
    * @brief ���ñ��������������ʱ�����ڵ�������
    * @return ��ȡ������ [0,6]
    */
    uint32 get_week();

    /**
    * @brief ��ȡ��ʽ���������ʱ���ַ���
    * @param format ��ʽ���ĸ�ʽ
    * �����ʽ:
    * ......
    * ......
    * @return ��ʽ���������ʱ���ַ���
    */
    std::string to_string();


private:
    /**
    * @brief ���������+
    */
    CDateTime & operator + (const CDateTime & datetime);

    bool _check_date(uint32 year, uint32 month, uint32 day);

    bool _check_time(uint32 hours, uint32 minutes, uint32 seconds);

    ::tm m_time;
};



/**
* @brief �������Ƿ�Ϊ����
* @param year ������
* @return ���Ľṹ,TRUEΪ������,FALSE��Ϊ����
*/
bool is_leap_year(uint32 year);

/**
* @brief
* �����ϵͳ������ĿǰΪֹ��������ʱ�䣨�˺�����Чʱ��Ϊϵͳ������49.7���ڣ�
* @return �����ϵͳ������ĿǰΪֹ��������ʱ�䣬�����ֵ���ɻ�ã����أ�1
*/
inline ulong  get_tick_count()
{
#if (defined(WIN32) || defined(WIN64))
    return ::GetTickCount();
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif //
}

uint64  get_system_time();

/**
* @brief
* ����ǰ�̵߳�ִ��ֱ��milliseconds�����
* @param milliseconds : ָʾ˯�ߵ�ʱ��
* @return void
*/
inline void  mill_sleep(uint32 milliseconds)
{
#if (defined(WIN32) || defined(WIN64))
    ::Sleep((DWORD)milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}


/**
* @brief
* ��ȡ��ǰ��΢��ʱ��
* @return ��ǰ��΢��ʱ��
*/
inline uint64  time_micro_sec()
{
    return get_system_time();
}

/**
* @brief
* ��ȡ��ǰ�ĺ���ʱ��
* @return ��ǰ�ĺ���ʱ��
*/
inline uint64  time_milli_sec()
{
    return get_system_time()/1000LL;
}

/**
* @brief
* ��ȡ��ǰ����ʱ��
* @return ��ǰ����ʱ��
*/
inline uint64  time_secs()
{
    return get_system_time()/1000000LL;
}

/** @} */


}

#endif //__SHARE_SYSTEM_TIME_H__

