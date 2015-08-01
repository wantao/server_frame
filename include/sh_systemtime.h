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
* @defgroup grouptime 时间操作
* @{
*/
class CDateTime;

/**
* @brief 获取代表当前日期时间(调用此函数的此时此刻)的CDateTime
* @return 代表当前日期时间(调用此函数的此时此刻)的CDateTime
*/

CDateTime now();

/**
* @brief 日期时间类,代表1970年午夜到2037年之间的某一个时间
*
*/
class  CDateTime
{
public:
    /**
    * @brief 构造一个CDateTime类,构造出来的时间为1970-1-1午夜
    */
    CDateTime(time_t t = 0);


    /**
    * @brief 获取本类所代表的日期时间的time_t值
    * @return 返回的time_t值
    */
    time_t get_time_value();

    /**
    * @brief 设置本类所代表的日期时间
    * @param t 设置的日期时间
    * @return void
    */
    void set_time_value(time_t t);

    /**
    * @brief 获取本类所代表的日期时间与datetime所代表的日期时间差距的秒数
    * @param datetime 一个日期时间
    * @return 差距的秒数
    */
    int64 operator - (const CDateTime &datetime);

    /**
    * @brief 获取本类所代表的日期时间与datetime所代表的日期时间差距的秒数
    * @param datetime 一个日期时间
    * @return 差距的秒数
    */
    int64 diff_second(const CDateTime &datetime);

    /**
    * @brief 获取本类所代表的日期时间与datetime所代表的日期时间差距的分钟数,差距少于1分钟的不计算在内
    * @param datetime 一个日期时间
    * @return 差距的分钟数
    */
    int64 diff_minute(const CDateTime &datetime);

    /**
    * @brief 获取本类所代表的日期时间与datetime所代表的日期时间差距的小时数,差距少于1小时的不计算在内
    * @param datetime 一个日期时间
    * @return 差距的小时数
    */
    int64 diff_hour(const CDateTime &datetime);

    /**
    * @brief 获取本类所代表的日期时间与datetime所代表的日期时间差距的天数,差距少于1天的不计算在内
    * @param datetime 一个日期时间
    * @return 差距的天数
    */
    int64 diff_day(const CDateTime &datetime);

    /**
    * @brief 获取本类所代表的日期时间与datetime所代表的日期时间差距的星期数,差距少于1星期的不计算在内
    * @param datetime 一个日期时间
    * @return 差距的星期数
    */
    int64 diff_week(const CDateTime &datetime);

    /**
    * @brief 获取本类所代表的日期时间与datetime所代表的日期时间差距的月份,差距少于1个月的不计算在内
    * @param datetime 一个日期时间
    * @return 差距的星期数
    */
    int32 diff_month(const CDateTime &datetime);

    /**
    * @brief 获取本类所代表的日期时间与datetime所代表的日期时间差距的年数,差距少于1年的不计算在内
    * @param datetime 一个日期时间
    * @return 差距的年数
    */
    int32 diff_year(const CDateTime &datetime);

    /**
    * @brief 增加本类所代表的日期时间的年数
    * @param year 增加的年数
    * @return 新的日期时间
    */
    CDateTime & inc_year(uint32 year = 1);

    /**
    * @brief 减少本类所代表的日期时间的年数
    * @param year 减少的年数
    * @return 新的日期时间
    */
    CDateTime & dec_year(uint32 year = 1);

    /**
    * @brief 增加本类所代表的日期时间的月数
    * @param month 增加的月数
    * @return 新的日期时间
    */
    CDateTime & inc_month(uint32 month = 1);

    /**
    * @brief 减少本类所代表的日期时间的月数
    * @param month 减少的月数
    * @return 新的日期时间
    */
    CDateTime & dec_month(uint32 month = 1);

    /**
    * @brief 增加本类所代表的日期时间的天数
    * @param day 增加的天数
    * @return 新的日期时间
    */
    CDateTime & inc_day(uint32 day = 1);

    /**
    * @brief 减少本类所代表的日期时间的天数
    * @param day 减少的天数
    * @return 新的日期时间
    */
    CDateTime & dec_day(uint32 day = 1);

    /**
    * @brief 增加本类所代表的日期时间的小时数
    * @param hour 增加的小时数
    * @return 新的日期时间
    */
    CDateTime & inc_hour(uint32 hour = 1);

    /**
    * @brief 减少本类所代表的日期时间的小时数
    * @param hour 减少的小时数
    * @return 新的日期时间
    */
    CDateTime & dec_hour(uint32 hour = 1);

    /**
    * @brief 增加本类所代表的日期时间的分钟数
    * @param minute 增加的分钟数
    * @return 新的日期时间
    */
    CDateTime & inc_minute(uint32 minute = 1);

    /**
    * @brief 减少本类所代表的日期时间的分钟数
    * @param minute 减少的分钟数
    * @return 新的日期时间
    */
    CDateTime & dec_minute(uint32 minute = 1);

    /**
    * @brief 增加本类所代表的日期时间的秒数
    * @param second 增加的秒数
    * @return 新的日期时间
    */
    CDateTime & inc_second(uint32 second = 1);

    /**
    * @brief 减少本类所代表的日期时间的秒数
    * @param second 减少的秒数
    * @return 新的日期时间
    */
    CDateTime & dec_second(uint32 second = 1);

    /**
    * @brief 增加本类所代表的日期时间的星期数
    * @param week 增加的星期数
    * @return 新的日期时间
    */
    CDateTime & inc_week(uint32 week = 1);

    /**
    * @brief 减少本类所代表的日期时间的星期数
    * @param week 减少的星期数
    * @return 新的日期时间
    */
    CDateTime & dec_week(uint32 week = 1);



    /**
    * @brief 设置本类所代表的日期时间
    * @param year 设置的年份[1970-2037]
    * @param month 设定的此年的哪个月[1-12]
    * @param day 设定的此月的第几天[1-31]
    * @param hours 设定的此天的第几小时[0-23]
    * @param minutes 设定的此小时的第几分钟[0-59]
    * @param seconds 设定的此分钟的第几秒[0-59]
    * @return 是否设置成功,false为没有设置成功
    */
    bool set_date_time(uint32 year, uint32 month, uint32 day, uint32 hours, uint32 minutes, uint32 seconds);

    /**
    * @brief 设置本类所代表的日期,时间不变
    * @param year 设置的年份[1970-2037]
    * @param month 设定的此年的哪个月[1-12]
    * @param day 设定的此月的第几天[1-31]
    * @return 是否设置成功,false为没有设置成功
    */
    bool set_date(uint32 year, uint32 month, uint32 day);

    /**
    * @brief 设置本类所代表的时间,日期不变
    * @param hours 设定的此天的第几小时[0-23]
    * @param minutes 设定的此小时的第几分钟[0-59]
    * @param seconds 设定的此分钟的第几秒[0-59]
    * @return 是否设置成功,false为没有设置成功
    */
    bool set_time(uint32 hours, uint32 minutes, uint32 seconds);

    /**
    * @brief 获取本类所代表的年份
    * @return 获取的年份
    */
    uint32 get_year();

    /**
    * @brief 获取本类所代表的日期时间所在的年份的第几个月
    * @return 获取的月份
    */
    uint32 get_month();

    /**
    * @brief 获取本类所代表的日期时间所在的月份的第几天
    * @return 获取的天数
    */
    uint32 get_day();

    /**
    * @brief 获取本类所代表的日期时间所在的天的第几个小时
    * @return 获取的小时数
    */
    uint32 get_hour();

    /**
    * @brief 获取本类所代表的日期时间所在的小时的分钟数
    * @return 获取的分钟数
    */
    uint32 get_minute();

    /**
    * @brief 设置本类所代表的日期时间所在的分钟的秒数
    * @return 获取的秒数
    */
    uint32 get_second();

    /**
    * @brief 设置本类所代表的日期时间所在的星期数
    * @return 获取星期数 [0,6]
    */
    uint32 get_week();

    /**
    * @brief 获取格式化后的日期时间字符串
    * @param format 格式化的格式
    * 具体格式:
    * ......
    * ......
    * @return 格式化后的日期时间字符串
    */
    std::string to_string();


private:
    /**
    * @brief 不允许操作+
    */
    CDateTime & operator + (const CDateTime & datetime);

    bool _check_date(uint32 year, uint32 month, uint32 day);

    bool _check_time(uint32 hours, uint32 minutes, uint32 seconds);

    ::tm m_time;
};



/**
* @brief 检测此年是否为闰年
* @param year 检测的年
* @return 检测的结构,TRUE为是闰年,FALSE不为闰年
*/
bool is_leap_year(uint32 year);

/**
* @brief
* 计算机系统启动到目前为止所经历的时间（此函数有效时间为系统启动后49.7天内）
* @return 计算机系统启动到目前为止所经历的时间，如果该值不可获得，返回－1
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
* 挂起当前线程的执行直到milliseconds毫秒后
* @param milliseconds : 指示睡眠的时间
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
* 获取当前的微妙时间
* @return 当前的微妙时间
*/
inline uint64  time_micro_sec()
{
    return get_system_time();
}

/**
* @brief
* 获取当前的毫秒时间
* @return 当前的毫秒时间
*/
inline uint64  time_milli_sec()
{
    return get_system_time()/1000LL;
}

/**
* @brief
* 获取当前的秒时间
* @return 当前的秒时间
*/
inline uint64  time_secs()
{
    return get_system_time()/1000000LL;
}

/** @} */


}

#endif //__SHARE_SYSTEM_TIME_H__

