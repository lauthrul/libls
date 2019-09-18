#include "stdafx.h"
#include "timer.h"
#include <time.h>
#include <malloc.h>
#include <memory.h>

#define Decrease(t1, t2)    (int)((t1)>(t2) ? (t1)-(t2) : (t2)-(t1))

namespace lslib
{
    Time::Time()
    {
        ftime( &m_time );
    }

    Time::~Time()
    {
    }

    bool Time::operator == (const Time& value) const
    {
        return (m_time.time == value.m_time.time
                && m_time.millitm == value.m_time.millitm
                && m_time.timezone == value.m_time.timezone
                && m_time.dstflag == value.m_time.dstflag);
    }

    bool Time::operator != (const Time& value) const
    {
        return !operator == (value);
    }

    bool Time::operator < (const Time& value) const
    {
        return (m_time.time < value.m_time.time
                && m_time.millitm < value.m_time.millitm
                && m_time.timezone < value.m_time.timezone
                && m_time.dstflag < value.m_time.dstflag);
    }

    bool Time::operator <= (const Time& value) const
    {
        return operator < (value) || operator == (value);
    }

    bool Time::operator > (const Time& value) const
    {
        return !operator <= (value);
    }

    bool Time::operator >= (const Time& value) const
    {
        return !operator < (value);
    }

    void Time::SetYear(int value)         { SetTime(value, e_YEAR); }
    void Time::SetMonth(int value)        { SetTime(value, e_MONTH); }
    void Time::SetDay(int value)          { SetTime(value, e_DAY); }
    void Time::SetHour(int value)         { SetTime(value, e_HOUR); }
    void Time::SetMinute(int value)       { SetTime(value, e_MINUTE); }
    void Time::SetSecond(int value)       { SetTime(value, e_SECOND); }
    void Time::SetMilliSec(int value)     { SetTime(value, e_MILLISEC); }
    void Time::SetTimeStamp(time_t value) { m_time.time = value; }

    int Time::GetYear()                   { return GetTime(e_YEAR); }
    int Time::GetMonth()                  { return GetTime(e_MONTH); }
    int Time::GetDay()                    { return GetTime(e_DAY); }
    int Time::GetYearDay()                { return GetTime(e_YEARDAY); }
    int Time::GetWeekDay()                { return GetTime(e_WEEKDAY); }
    int Time::GetHour()                   { return GetTime(e_HOUR); }
    int Time::GetMinute()                 { return GetTime(e_MINUTE); }
    int Time::GetSecond()                 { return GetTime(e_SECOND); }
    int Time::GetMilliSec()               { return GetTime(e_MILLISEC); }
    time_t Time::GetTimeStamp()           { return m_time.time; }

    string Time::GetDateStr(_lpcstr fmt /*= DATE_FMT*/)
    {
        string strfmt = strtool::is_empty(fmt) ? DATE_FMT : fmt;
        return strtool::format(fmt, GetYear(), GetMonth(), GetDay());
    }

    string Time::GetTimeStr(bool ms /*= false*/, _lpcstr fmt /*= TIME_FMT*/)
    {
        string strfmt;
        if (ms)
        {
            strfmt = strtool::is_empty(fmt) ? MILLTIME_FMT : fmt;
            return strtool::format(fmt, GetHour(), GetMinute(), GetSecond(), GetMilliSec());
        }
        else
        {
            strfmt = strtool::is_empty(fmt) ? TIME_FMT : fmt;
            return strtool::format(fmt, GetHour(), GetMinute(), GetSecond());
        }
    }

    string Time::GetDateTimeStr(bool ms /*= false*/, _lpcstr fmt /*= DATE_TIME_FMT*/)
    {
        string strfmt;
        if (ms)
        {
            strfmt = strtool::is_empty(fmt) ? DATE_MILLTIME_FMT : fmt;
            return strtool::format(fmt, GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond(), GetMilliSec());
        }
        else
        {
            strfmt = strtool::is_empty(fmt) ? DATE_TIME_FMT : fmt;
            return strtool::format(fmt, GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond());
        }
    }

    Time Time::Parse(_lpcstr datetime, bool ms /*= false*/, _lpcstr fmt /*= DATE_TIME_FMT*/)
    {
        Time tm;
        if (strtool::is_empty(datetime)) return tm;

        int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, millsec = 0;
        string strfmt;
        int ret = 0;
        if (ms)
        {
            strfmt = strtool::is_empty(fmt) ? DATE_MILLTIME_FMT : fmt;
            ret = sscanf(datetime, strfmt.c_str(), &year, &month, &day, &hour, &minute, &second, &millsec);
        }
        else
        {
            strfmt = strtool::is_empty(fmt) ? DATE_TIME_FMT : fmt;
            ret = sscanf(datetime, strfmt.c_str(), &year, &month, &day, &hour, &minute, &second);
        }

        if (ret > 0)
        {
            tm.SetYear(year);
            tm.SetMonth(month);
            tm.SetDay(day);
            tm.SetHour(hour);
            tm.SetMinute(minute);
            tm.SetSecond(second);
            tm.SetMilliSec(millsec);
        }
        return tm;
    }

    time_t Time::CurrentTimeStamp()
    {
        return Time().GetTimeStamp();
    }

    string Time::CurrentDateStr(_lpcstr fmt /*= DATE_FMT*/)
    {
        return Time().GetDateStr(fmt);
    }

    string Time::CurrentTimeStr(bool ms /*= false*/, _lpcstr fmt /*= TIME_FMT*/)
    {
        return Time().GetTimeStr(ms, fmt);
    }

    string Time::CurrentDateTimeStr(bool ms /*= false*/, _lpcstr fmt /*= DATE_TIME_FMT*/)
    {
        return Time().GetDateTimeStr(ms, fmt);
    }

    void Time::AddYear(int value)
    {
        struct tm* temptm = localtime(&(m_time.time));
        temptm->tm_year += value;
        time_t ltime = mktime(temptm);
        if (ltime > 0)
            m_time.time = ltime;
    }

    void Time::AddMonth(int value)
    {
        struct tm* temptm = localtime(&(m_time.time));
        int year = value / 12;
        int month = value % 12;
        if (month + temptm->tm_mon >= 12)
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
        if (ltime > 0)
            m_time.time = ltime;
    }

    void Time::AddDay(int value)
    {
        m_time.time += value * 60 * 60 * 24;
    }

    void Time::AddHour(int value)
    {
        m_time.time += value * 60 * 60;
    }

    void Time::AddMinute(int value)
    {
        m_time.time += value * 60;
    }

    void Time::AddSecond(int value)
    {
        m_time.time += value;
    }

    void Time::AddMilliSec(int value)
    {
        int sec = value / 1000;
        int millisec = value % 1000;
        if (millisec + m_time.millitm >= 1000)
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

    void Time::DecreaseYear(int value)
    {
        struct tm* temptm = localtime(&(m_time.time));
        temptm->tm_year -= value;
        time_t ltime = mktime(temptm);
        if (ltime > 0)
            m_time.time = ltime;
    }

    void Time::DecreaseMonth(int value)
    {
        struct tm* temptm = localtime(&(m_time.time));
        int year = value / 12;
        int month = value % 12;
        if (temptm->tm_mon - month >= 0)
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
        if (ltime > 0)
            m_time.time = ltime;
    }

    void Time::DecreaseDay(int value)
    {
        m_time.time -= value * 60 * 60 * 24;
    }

    void Time::DecreaseHour(int value)
    {
        m_time.time -= value * 60 * 60;
    }

    void Time::DecreaseMinute(int value)
    {
        m_time.time -= value * 60;
    }

    void Time::DecreaseSecond(int value)
    {
        m_time.time -= value;
    }

    void Time::DecreaseMilliSec(int value)
    {
        int sec = value / 1000;
        int millisec = value % 1000;
        if (m_time.millitm - millisec >= 0)
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

    int Time::BetweenYear(const Time& first, const Time& second)        { return BetweenTime(first, second, e_YEAR); }
    int Time::BetweenMonth(const Time& first, const Time& second)       { return BetweenTime(first, second, e_MONTH); }
    int Time::BetweenDay(const Time& first, const Time& second)         { return BetweenTime(first, second, e_DAY); }
    int Time::BetweenHour(const Time& first, const Time& second)        { return BetweenTime(first, second, e_HOUR); }
    int Time::BetweenMinute(const Time& first, const Time& second)      { return BetweenTime(first, second, e_MINUTE); }
    int Time::BetweenSecond(const Time& first, const Time& second)      { return BetweenTime(first, second, e_SECOND); }
    int Time::BetweenMilliSec(const Time& first, const Time& second)    { return BetweenTime(first, second, e_MILLISEC); }

    int Time::BetweenYear(const Time& value)                            { return BetweenTime(value, *this, e_YEAR); }
    int Time::BetweenMonth(const Time& value)                           { return BetweenTime(value, *this, e_MONTH); }
    int Time::BetweenDay(const Time& value)                             { return BetweenTime(value, *this, e_DAY); }
    int Time::BetweenHour(const Time& value)                            { return BetweenTime(value, *this, e_HOUR); }
    int Time::BetweenMinute(const Time& value)                          { return BetweenTime(value, *this, e_MINUTE); }
    int Time::BetweenSecond(const Time& value)                          { return BetweenTime(value, *this, e_SECOND); }
    int Time::BetweenMilliSec(const Time& value)                        { return BetweenTime(value, *this, e_MILLISEC); }

    int Time::BetweenAllDay(const Time& first, const Time& second)      { return BetweenAllTime(first, second, e_DAY); }
    int Time::BetweenAllHour(const Time& first, const Time& second)     { return BetweenAllTime(first, second, e_HOUR); }
    int Time::BetweenAllMinute(const Time& first, const Time& second)   { return BetweenAllTime(first, second, e_MINUTE); }
    int Time::BetweenAllSecond(const Time& first, const Time& second)   { return BetweenAllTime(first, second, e_SECOND); }
    int Time::BetweenAllMilliSec(const Time& first, const Time& second) { return BetweenAllTime(first, second, e_MILLISEC); }

    int Time::BetweenAllDay(const Time& value)                          { return BetweenAllTime(value, *this, e_DAY); }
    int Time::BetweenAllHour(const Time& value)                         { return BetweenAllTime(value, *this, e_HOUR); }
    int Time::BetweenAllMinute(const Time& value)                       { return BetweenAllTime(value, *this, e_MINUTE); }
    int Time::BetweenAllSecond(const Time& value)                       { return BetweenAllTime(value, *this, e_SECOND); }
    int Time::BetweenAllMilliSec(const Time& value)                     { return BetweenAllTime(value, *this, e_MILLISEC); }


    void Time::SetTime(int value, TimeType timetype)
    {
        struct tm* temptm = localtime(&(m_time.time));
        if ( temptm == NULL ) return;
        switch ( timetype )
        {
            case e_MILLISEC:
                m_time.millitm = CLAMP(value, 0, 999);
                break;
            case e_SECOND:
                temptm->tm_sec = CLAMP(value, 0, 59);
                break;
            case e_MINUTE:
                temptm->tm_min = CLAMP(value, 0, 59);
                break;
            case e_HOUR:
                temptm->tm_hour = CLAMP(value, 0, 23);
                break;
            case e_DAY:
                temptm->tm_mday = CLAMP(value, 1, 31);
                break;
            case e_MONTH:
                temptm->tm_mon = CLAMP(value, 1, 12);
                break;
            case e_YEAR:
                temptm->tm_year = CLAMP(value, 1970, 2038) - 1900;
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

    int Time::BetweenTime(const Time& first, const Time& second, TimeType timetype)
    {
        int ret = 0;
        struct tm tm1, tm2, *tmp;

        tmp = localtime(&(first.m_time.time));
        if (tmp == NULL) return 0;
        tm1 = *tmp;

        tmp = localtime(&(second.m_time.time));
        if (tmp == NULL) return 0;
        tm2 = *tmp;

        switch ( timetype )
        {
            case e_MILLISEC:
                ret = Decrease(first.m_time.millitm, second.m_time.millitm);
                break;
            case e_SECOND:
                ret = Decrease(tm1.tm_sec, tm2.tm_sec);
                break;
            case e_MINUTE:
                ret = Decrease(tm1.tm_min, tm2.tm_min);
                break;
            case e_HOUR:
                ret = Decrease(tm1.tm_hour, tm2.tm_hour);
                break;
            case e_DAY:
                ret = Decrease(tm1.tm_mday, tm2.tm_mday);
                break;
            case e_MONTH:
                ret = Decrease(tm1.tm_mon, tm2.tm_mon);
                break;
            case e_YEAR:
                ret = Decrease(tm1.tm_year, tm2.tm_year);
                break;
        }
        return ret;
    }

    int Time::BetweenAllTime(const Time& first, const Time& second, TimeType timetype)
    {
        int ret = 0;
        switch ( timetype )
        {
            case e_MILLISEC:
                ret = Decrease(first.m_time.millitm, second.m_time.millitm) + Decrease(first.m_time.time, second.m_time.time) * 1000;
                break;
            case e_SECOND:
                ret = Decrease(first.m_time.time, second.m_time.time);
                break;
            case e_MINUTE:
                ret = Decrease(first.m_time.time, second.m_time.time) / 60;
                break;
            case e_HOUR:
                ret = Decrease(first.m_time.time, second.m_time.time) / (60 * 60);
                break;
            case e_DAY:
                ret = Decrease(first.m_time.time, second.m_time.time) / (60 * 60 * 24);
                break;
        }
        return ret;
    }
}
