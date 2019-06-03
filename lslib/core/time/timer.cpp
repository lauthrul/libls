#include "stdafx.h"
#include "timer.h"
#include <time.h>
#include <malloc.h>
#include <memory.h>

namespace lslib
{

#define Decrease(t1, t2)    ((t1)>(t2) ? (t1)-(t2) : (t2)-(t1))

    /***************************Timer*******************************************/
    Time::Time()
    {
        _ftime( &m_time );
    }

    Time::~Time()
    {
    }

    Time Time::GetCurDateTime()
    {
        Time tCurTime;
        tCurTime.SetTimer();
        return tCurTime;
    }

    string Time::GetCurDateStr()
    {
        Time tm = GetCurDateTime();
        return tm.GetDateStr();
    }

    string Time::GetCurTimeStr(bool bMs /*= false*/)
    {
        Time tm = GetCurDateTime();
        return tm.GetTimeStr(bMs);
    }

    string Time::GetCurDateTimeStr(bool bMs /*= false*/)
    {
        return GetCurDateStr() + " " + GetCurTimeStr(bMs);
    }

    Time Time::Parser(const char* datetime, const char* fmt /*= "%04d-%02d-%02d %02d:%02d:%02d"*/)
    {
        lstring strFmt = is_empty(fmt) ? "%04d-%02d-%02d %02d:%02d:%02d" : fmt;
        Time tm;
        int year, month, day, hour, minute, second;
        if ( sscanf(datetime, strFmt, &year, &month, &day, &hour, &minute, &second) > 0)
        {
            tm.SetYear(year);
            tm.SetMonth(month);
            tm.SetDay(day);
            tm.SetHour(hour);
            tm.SetMinute(minute);
            tm.SetSecond(second);
        }
        return tm;
    }

    bool Time::operator != (const Time& value) const
    {
        if ( &value == NULL ) return true;
        if ( m_time.time != value.m_time.time && m_time.millitm != value.m_time.millitm
                && m_time.timezone != value.m_time.timezone && m_time.dstflag != value.m_time.dstflag )
            return true;
        return false;
    }

    bool Time::operator == (const Time& value) const
    {
        if ( &value == NULL ) return false;
        if ( m_time.time == value.m_time.time && m_time.millitm == value.m_time.millitm
                && m_time.timezone == value.m_time.timezone && m_time.dstflag == value.m_time.dstflag )
            return true;
        return false;
    }

    bool Time::operator < (const Time& value) const
    {
        if ( &value == NULL ) return false;
        if ( m_time.time < value.m_time.time && m_time.millitm < value.m_time.millitm
                && m_time.timezone < value.m_time.timezone && m_time.dstflag < value.m_time.dstflag )
            return true;
        return false;
    }

    bool Time::operator <= (const Time& value) const
    {
        if ( &value == NULL ) return false;
        if ( m_time.time <= value.m_time.time && m_time.millitm <= value.m_time.millitm
                && m_time.timezone <= value.m_time.timezone && m_time.dstflag <= value.m_time.dstflag )
            return true;
        return false;
    }

    bool Time::operator > (const Time& value) const
    {
        if ( &value == NULL ) return true;
        if ( m_time.time > value.m_time.time && m_time.millitm > value.m_time.millitm
                && m_time.timezone > value.m_time.timezone && m_time.dstflag > value.m_time.dstflag )
            return true;
        return false;
    }

    bool Time::operator >= (const Time& value) const
    {
        if ( &value == NULL ) return true;
        if ( m_time.time >= value.m_time.time && m_time.millitm >= value.m_time.millitm
                && m_time.timezone >= value.m_time.timezone && m_time.dstflag >= value.m_time.dstflag )
            return true;
        return false;
    }

    void Time::AddMilliSec(int value)
    {
        int sec = value / 1000;
        int millisec = value % 1000;
        if ( millisec + m_time.millitm >= 1000 )
        {
            m_time.time += ++sec;
            m_time.millitm += millisec - 1000;
        }
        else
        {
            m_time.time += sec;
            m_time.millitm += millisec;
        }
    }

    void Time::AddSecond(int value)
    {
        m_time.time += value;
    }

    void Time::AddMinute(int value)
    {
        m_time.time += value * 60;
    }

    void Time::AddHour(int value)
    {
        m_time.time += value * 60 * 60;
    }

    void Time::AddDay(int value)
    {
        m_time.time += value * 60 * 60 * 24;
    }

    void Time::AddMonth(int value)
    {
        struct tm* temptm = localtime(&(m_time.time));
        int year = value / 12;
        int month = value % 12;
        if ( month + temptm->tm_mon >= 12 )
        {
            temptm->tm_year += ++year;
            temptm->tm_mon += month - 12;
        }
        else
        {
            temptm->tm_year += year;
            temptm->tm_mon += month;
        }
        time_t ltime = mktime(temptm);
        if ( ltime > 0 )
            m_time.time = ltime;
    }

    void Time::AddYear(int value)
    {
        struct tm* temptm = localtime(&(m_time.time));
        temptm->tm_year += value;
        time_t ltime = mktime(temptm);
        if ( ltime > 0 )
            m_time.time = ltime;
    }

    void Time::DecreaseMilliSec(int value)
    {
        int sec = value / 1000;
        int millisec = value % 1000;
        if ( m_time.millitm - millisec >= 0 )
        {
            m_time.time -= sec;
            m_time.millitm -= millisec;
        }
        else
        {
            m_time.time -= --sec;
            m_time.millitm += 1000 - millisec;
        }
    }

    void Time::DecreaseSecond(int value)
    {
        m_time.time -= value;
    }

    void Time::DecreaseMinute(int value)
    {
        m_time.time -= value * 60;
    }

    void Time::DecreaseHour(int value)
    {
        m_time.time -= value * 60 * 60;
    }

    void Time::DecreaseDay(int value)
    {
        m_time.time -= value * 60 * 60 * 24;
    }

    void Time::DecreaseMonth(int value)
    {
        struct tm* temptm = localtime(&(m_time.time));
        int year = value / 12;
        int month = value % 12;
        if ( temptm->tm_mon - month >= 0 )
        {
            temptm->tm_year -= year;
            temptm->tm_mon -= month;
        }
        else
        {
            temptm->tm_year -= --year;
            temptm->tm_mon += 12 - month;
        }
        time_t ltime = mktime(temptm);
        if ( ltime > 0 )
            m_time.time = ltime;
    }

    void Time::DecreaseYear(int value)
    {
        struct tm* temptm = localtime(&(m_time.time));
        temptm->tm_year -= value;
        time_t ltime = mktime(temptm);
        if ( ltime > 0 )
            m_time.time = ltime;
    }

    void Time::SetTimer()
    {
        _ftime( &m_time );
    }

    void Time::SetTimer(const Time& value)
    {
        if ( &value == NULL )
        {
            _ftime( &m_time );
            return;
        }
        if ( this == &value ) return;
        m_time.time = value.m_time.time;
        m_time.millitm = value.m_time.millitm;
        m_time.timezone = value.m_time.timezone;
        m_time.dstflag = value.m_time.dstflag;
    }

    void Time::SetMilliSec(int value)
    {
        SetTime(value, e_MILLISEC);
    }

    void Time::SetSecond(int value)
    {
        SetTime(value, e_SECOND);
    }

    void Time::SetMinute(int value)
    {
        SetTime(value, e_MINUTE);
    }

    void Time::SetHour(int value)
    {
        SetTime(value, e_HOUR);
    }

    void Time::SetDay(int value)
    {
        SetTime(value, e_DAY);
    }

    void Time::SetMonth(int value)
    {
        SetTime(value, e_MONTH);
    }

    void Time::SetYear(int value)
    {
        SetTime(value, e_YEAR);
    }

    int Time::GetMilliSec()
    {
        return GetTime(e_MILLISEC);
    }

    int Time::GetSecond()
    {
        return GetTime(e_SECOND);
    }

    int Time::GetMinute()
    {
        return GetTime(e_MINUTE);
    }

    int Time::GetHour()
    {
        return GetTime(e_HOUR);
    }

    int Time::GetDay()
    {
        return GetTime(e_DAY);
    }

    int Time::GetMonth()
    {
        return GetTime(e_MONTH);
    }

    int Time::GetYear()
    {
        return GetTime(e_YEAR);
    }

    int Time::GetWeekDay()
    {
        return GetTime(e_WEEKDAY);
    }

    int Time::GetYearDay()
    {
        return GetTime(e_YEARDAY);
    }

    int Time::BetweenMilliSec(const Time& first, const Time& tow)
    {
        return BetweenTime(first, tow, e_MILLISEC);
    }

    int Time::BetweenSecond(const Time& first, const Time& tow)
    {
        return BetweenTime(first, tow, e_SECOND);
    }

    int Time::BetweenMinute(const Time& first, const Time& tow)
    {
        return BetweenTime(first, tow, e_MINUTE);
    }

    int Time::BetweenHour(const Time& first, const Time& tow)
    {
        return BetweenTime(first, tow, e_HOUR);
    }

    int Time::BetweenDay(const Time& first, const Time& tow)
    {
        return BetweenTime(first, tow, e_DAY);
    }

    int Time::BetweenMonth(const Time& first, const Time& tow)
    {
        return BetweenTime(first, tow, e_MONTH);
    }

    int Time::BetweenYear(const Time& first, const Time& tow)
    {
        return BetweenTime(first, tow, e_YEAR);
    }

    int Time::BetweenMilliSec(const Time& value)
    {
        return BetweenTime(value, *this, e_MILLISEC);
    }

    int Time::BetweenSecond(const Time& value)
    {
        return BetweenTime(value, *this, e_SECOND);
    }

    int Time::BetweenMinute(const Time& value)
    {
        return BetweenTime(value, *this, e_MINUTE);
    }

    int Time::BetweenHour(const Time& value)
    {
        return BetweenTime(value, *this, e_HOUR);
    }

    int Time::BetweenDay(const Time& value)
    {
        return BetweenTime(value, *this, e_DAY);
    }

    int Time::BetweenMonth(const Time& value)
    {
        return BetweenTime(value, *this, e_MONTH);
    }

    int Time::BetweenYear(const Time& value)
    {
        return BetweenTime(value, *this, e_YEAR);
    }

    int Time::BetweenAllMilliSec(const Time& first, const Time& tow)
    {
        return BetweenAllTime(first, tow, e_MILLISEC);
    }

    int Time::BetweenAllSecond(const Time& first, const Time& tow)
    {
        return BetweenAllTime(first, tow, e_SECOND);
    }

    int Time::BetweenAllMinute(const Time& first, const Time& tow)
    {
        return BetweenAllTime(first, tow, e_MINUTE);
    }

    int Time::BetweenAllHour(const Time& first, const Time& tow)
    {
        return BetweenAllTime(first, tow, e_HOUR);
    }

    int Time::BetweenAllMilliSec(const Time& value)
    {
        return BetweenAllTime(value, *this, e_MILLISEC);
    }

    int Time::BetweenAllSecond(const Time& value)
    {
        return BetweenAllTime(value, *this, e_SECOND);
    }

    int Time::BetweenAllMinute(const Time& value)
    {
        return BetweenAllTime(value, *this, e_MINUTE);
    }

    int Time::BetweenAllHour(const Time& value)
    {
        return BetweenAllTime(value, *this, e_HOUR);
    }

    void Time::SetTime(int value, TimeType timetype)
    {
        struct tm* temptm = localtime(&(m_time.time));
        if ( temptm == NULL ) return;
        switch ( timetype )
        {
            case e_MILLISEC:
                if ( value >= 1000 ) value = 999;
                if ( value <= 0 ) value = 0;
                m_time.millitm = value;
                break;
            case e_SECOND:
                if ( value >= 60 ) value = 59;
                if ( value <= 0 ) value = 0;
                temptm->tm_sec = value;
                break;
            case e_MINUTE:
                if ( value >= 60 ) value = 59;
                if ( value <= 0 ) value = 0;
                temptm->tm_min = value;
                break;
            case e_HOUR:
                if ( value >= 24 ) value = 23;
                if ( value <= 0 ) value = 0;
                temptm->tm_hour = value;
                break;
            case e_DAY:
                if ( value >= 32 ) value = 31;
                if ( value <= 0 ) value = 1;
                temptm->tm_mday = value;
                break;
            case e_MONTH:
                if ( value >= 12 ) value = 12;
                if ( value <= 1 ) value = 1;
                temptm->tm_mon = value - 1;
                break;
            case e_YEAR:
                if ( value <= 1970 ) value = 1970;
                if ( value >= 2038 ) value = 2037;
                temptm->tm_year = value - 1900;
                break;
        }
        time_t ltime = mktime(temptm);
        if ( ltime > 0 )
            m_time.time = ltime;
    }

    int Time::GetTime(TimeType timetype)
    {
        struct tm* temptm = localtime(&(m_time.time));
        int ret = 0;
        if ( temptm == NULL ) return 0;
        switch ( timetype )
        {
            case e_MILLISEC:
                ret = m_time.millitm;
                break;
            case e_SECOND:
                ret = temptm->tm_sec;
                break;
            case e_MINUTE:
                ret = temptm->tm_min;
                break;
            case e_HOUR:
                ret = temptm->tm_hour;
                break;
            case e_DAY:
                ret = temptm->tm_mday;
                break;
            case e_MONTH:
                ret = temptm->tm_mon + 1;
                break;
            case e_YEAR:
                ret = 1900 + temptm->tm_year;
                break;
            case e_WEEKDAY:
                ret = temptm->tm_wday == 0 ? 7 : temptm->tm_wday;
                break;
            case e_YEARDAY:
                ret = temptm->tm_yday + 1;
                break;
        }
        return ret;
    }

    int Time::BetweenTime(const Time& first, const Time& tow, TimeType timetype)
    {
        int ret = 0;
        struct tm* temptm1 = localtime(&(tow.m_time.time));
        if ( temptm1 == NULL ) return 0;
        struct tm* temptm2 = (struct tm*)malloc(sizeof(tm));
        if ( temptm2 == NULL ) return 0;
        memset(temptm2, 0x00, sizeof(tm));
        memcpy(temptm2, temptm1, sizeof(tm));
        temptm1 = localtime(&(first.m_time.time));
        switch ( timetype )
        {
            case e_MILLISEC:
                ret = Decrease(first.m_time.millitm, tow.m_time.millitm);
                break;
            case e_SECOND:
                ret = Decrease(temptm1->tm_sec, temptm2->tm_sec);
                break;
            case e_MINUTE:
                ret = Decrease(temptm1->tm_min, temptm2->tm_min);
                break;
            case e_HOUR:
                ret = Decrease(temptm1->tm_hour, temptm2->tm_hour);
                break;
            case e_DAY:
                ret = Decrease(temptm1->tm_mday, temptm2->tm_mday);
                break;
            case e_MONTH:
                ret = Decrease(temptm1->tm_mon, temptm2->tm_mon);
                break;
            case e_YEAR:
                ret = Decrease(temptm1->tm_year, temptm2->tm_year);
                break;
        }
        free(temptm2);
        return ret;
    }

    int Time::BetweenAllTime(const Time& first, const Time& tow, TimeType timetype)
    {
        int ret = 0;
        switch ( timetype )
        {
            case e_MILLISEC:
                ret = Decrease(first.m_time.millitm, tow.m_time.millitm) + Decrease(first.m_time.time, tow.m_time.time) * 1000;
                break;
            case e_SECOND:
                ret = Decrease(first.m_time.time, tow.m_time.time);
                break;
            case e_MINUTE:
                ret = Decrease(first.m_time.time, tow.m_time.time) / 60;
                break;
            case e_HOUR:
                ret = Decrease(first.m_time.time, tow.m_time.time) / 3600;
                break;
        }
        return ret;
    }

    long Time::GetDateTime()
    {
        return m_time.time;
    }

    void Time::SetDataTime(long value)
    {
        m_time.time = value;
    }

    string Time::GetDateStr()
    {
        char szTemp[16] = {0};
        sprintf(szTemp, "%04d-%02d-%02d", GetYear(), GetMonth(), GetDay());
        return szTemp;
    }

    string Time::GetTimeStr(bool bMs /*= false*/)
    {
        char szTemp[32] = {0};
        if (bMs)
            sprintf(szTemp, "%02d:%02d:%02d.%03d", GetHour(), GetMinute(), GetSecond(), GetMilliSec());
        else
            sprintf(szTemp, "%02d:%02d:%02d", GetHour(), GetMinute(), GetSecond());
        return szTemp;
    }

    string Time::GetDateTimeStr(bool bMs /*= false*/)
    {
        return GetDateStr() + " " + GetTimeStr(bMs);
    }

    /***************************Timer64*****************************************/


}
