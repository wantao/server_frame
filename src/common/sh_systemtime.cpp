#include "sh_common.h"
#include "sh_systemtime.h"

namespace sh
{

CDateTime  now()
{
    return time(NULL);
}

CDateTime::CDateTime(time_t t)
{
    localtime_safe(&m_time, &t);
}

time_t CDateTime::get_time_value()
{
    return mktime(&m_time);
}

int64 CDateTime::operator - (const CDateTime & datetime)
{
    tm t_tm = datetime.m_time;
    return (int64)difftime(mktime(&m_time), mktime(&t_tm));
}

int64 CDateTime::diff_second(const CDateTime & datetime)
{
    tm t_tm = datetime.m_time;
    return (int64)difftime(mktime(&m_time), mktime(&t_tm));
}

int64 CDateTime::diff_minute(const CDateTime & datetime)
{
    tm t_tm = datetime.m_time;
    return ((int64)difftime(mktime(&m_time), mktime(&t_tm)))/60;
}

int64 CDateTime::diff_hour(const CDateTime & datetime)
{
    tm t_tm = datetime.m_time;
    return ((int64)difftime(mktime(&m_time), mktime(&t_tm)))/(60*60);
}

int64 CDateTime::diff_day(const CDateTime & datetime)
{
    tm t_tm = datetime.m_time;
    return ((int64)difftime(mktime(&m_time), mktime(&t_tm)))/(60*60*24);
}

int64 CDateTime::diff_week(const CDateTime & datetime)
{
    tm t_tm = datetime.m_time;
    return ((int64)difftime(mktime(&m_time), mktime(&t_tm)))/(60*60*24*7);
}

int32 CDateTime::diff_month(const CDateTime & datetime)
{
    return (m_time.tm_year - datetime.m_time.tm_year) * 12 + (m_time.tm_mon - datetime.m_time.tm_mon);
}

int32 CDateTime::diff_year(const CDateTime & datetime)
{
    return m_time.tm_year - datetime.m_time.tm_year;
}

CDateTime & CDateTime::inc_year(uint32 year)
{
    m_time.tm_year += year;
    m_time.tm_isdst = -1;
    mktime(&m_time);
    return *this;
}

CDateTime & CDateTime::dec_year(uint32 year)
{
    m_time.tm_year -= year;
    m_time.tm_isdst = -1;
    mktime(&m_time);
    return *this;
}

CDateTime & CDateTime::inc_month(uint32 month)
{
    m_time.tm_mon += month;
    m_time.tm_isdst = -1;
    mktime(&m_time);
    return *this;
}

CDateTime & CDateTime::dec_month(uint32 month)
{
    m_time.tm_mon -= month;
    m_time.tm_isdst = -1;
    mktime(&m_time);
    return *this;
}

CDateTime & CDateTime::inc_day(uint32 day)
{
    m_time.tm_mday += day;
    m_time.tm_isdst = -1;
    mktime(&m_time);
    return *this;
}

CDateTime & CDateTime::dec_day(uint32 day)
{
    m_time.tm_mday -= day;
    m_time.tm_isdst = -1;
    mktime(&m_time);
    return *this;
}

CDateTime & CDateTime::inc_hour(uint32 hour)
{
    m_time.tm_hour += hour;
    m_time.tm_isdst = -1;
    mktime(&m_time);
    return *this;
}

CDateTime & CDateTime::dec_hour(uint32 hour)
{
    m_time.tm_hour -= hour;
    m_time.tm_isdst = -1;
    mktime(&m_time);
    return *this;
}

CDateTime & CDateTime::inc_minute(uint32 minute)
{
    m_time.tm_min += minute;
    m_time.tm_isdst = -1;
    mktime(&m_time);
    return *this;
}

CDateTime & CDateTime::dec_minute(uint32 minute)
{
    m_time.tm_min -= minute;
    m_time.tm_isdst = -1;
    mktime(&m_time);
    return *this;
}

CDateTime & CDateTime::inc_second(uint32 second)
{
    m_time.tm_sec += second;
    m_time.tm_isdst = -1;
    mktime(&m_time);
    return *this;
}

CDateTime & CDateTime::dec_second(uint32 second)
{
    m_time.tm_sec -= second;
    m_time.tm_isdst = -1;
    mktime(&m_time);
    return *this;
}

CDateTime & CDateTime::inc_week(uint32 week)
{
    m_time.tm_mday += 7 * week;
    m_time.tm_isdst = -1;
    mktime(&m_time);
    return *this;
}

CDateTime & CDateTime::dec_week(uint32 week)
{
    m_time.tm_mday -= 7 * week;
    m_time.tm_isdst = -1;
    mktime(&m_time);
    return *this;
}

void CDateTime::set_time_value(time_t t)
{
    tm tm_time;
    localtime_safe(&tm_time, &t);
    m_time = tm_time;
}

bool CDateTime::set_date_time(uint32 year, uint32 month, uint32 day, uint32 hours, uint32 minutes, uint32 seconds)
{
    if(_check_date(year, month, day) && _check_time(hours, minutes, seconds))
    {
        m_time.tm_year = year - 1900;
        m_time.tm_mon = month - 1;
        m_time.tm_mday = day;

        m_time.tm_hour = hours;
        m_time.tm_min = minutes;
        m_time.tm_sec = seconds;
        m_time.tm_isdst = -1;
        mktime(&m_time);
        return true;
    }
    return false;
}

bool CDateTime::set_date(uint32 year, uint32 month, uint32 day)
{
    if(_check_date(year, month, day))
    {
        m_time.tm_year = year - 1900;
        m_time.tm_mon = month - 1;
        m_time.tm_mday = day;
        m_time.tm_isdst = -1;
        mktime(&m_time);
        return true;
    }
    return false;
}

bool CDateTime::set_time(uint32 hours, uint32 minutes, uint32 seconds)
{
    if(_check_time(hours, minutes, seconds))
    {
        m_time.tm_hour = hours;
        m_time.tm_min = minutes;
        m_time.tm_sec = seconds;
        m_time.tm_isdst = -1;
        mktime(&m_time);
        return true;
    }
    return false;
}

uint32 CDateTime::get_year()
{
    return m_time.tm_year + 1900;
}

uint32 CDateTime::get_month()
{
    return m_time.tm_mon + 1;
}

uint32 CDateTime::get_day()
{
    return m_time.tm_mday;
}

uint32 CDateTime::get_hour()
{
    return m_time.tm_hour;
}

uint32 CDateTime::get_minute()
{
    return m_time.tm_min;
}

uint32 CDateTime::get_second()
{
    return m_time.tm_sec;
}

uint32 CDateTime::get_week()
{
    return m_time.tm_wday;
}

std::string CDateTime::to_string()
{
    return time2str(get_time_value());
}

bool CDateTime::_check_date(uint32 year, uint32 month, uint32 day)
{
    if(year < 1970 || year >= 2037)
    {
        return false;
    }
    if(month < 1 || month > 12)
    {
        return false;
    }
    if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
    {
        if(day < 1 || day > 31)
        {
            return false;
        }
    }
    else if(month == 4 || month == 6 || month == 9 || month == 11)
    {
        if(day < 1 || day > 30)
        {
            return false;
        }
    }
    else if(month == 2)
    {
        if(is_leap_year(year))
        {
            if(day < 1 || day > 29)
            {
                return false;
            }
        }
        else
        {
            if(day < 1 || day > 28)
            {
                return false;
            }
        }
    }
    return true;
}

bool CDateTime::_check_time(uint32 hours, uint32 minutes, uint32 seconds)
{
    if(hours < 0 || hours >= 24)
    {
        return false;
    }
    if(minutes < 0 || minutes >= 60)
    {
        return false;
    }
    if(seconds < 0 || seconds >= 60)
    {
        return false;
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_leap_year(uint32 year)
{
    if ( (year%400==0) || (year%100!=0 && year%4==0) )
    {
        return true;
    }
    return false;
}


uint64  get_system_time()
{
    assert( sizeof(uint64) == 64/8 );
    uint64 time=0;
#if (defined(WIN32) || defined(WIN64))
    FILETIME ft;
    GetSystemTimeAsFileTime( &ft);
    ULARGE_INTEGER li;
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    time = li.QuadPart/10;
#else
    struct timeval now;
    gettimeofday( &now , NULL );
    //assert( now );
    time = now.tv_sec;
    time = time*1000000;
    time += now.tv_usec;
#endif
    return time;
}


}