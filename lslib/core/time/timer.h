#pragma once
#include <sys/timeb.h>

namespace lslib
{
    /***************************Time*******************************************/
    //1970-1-1--3000-12-31 23:59:59
    class LSLIB_API Time
    {
        public :
            Time();
            ~Time();

        public:
            static Time GetCurDateTime();
            static string GetCurDateStr();
            static string GetCurTimeStr(bool bMs = false);
            static string GetCurDateTimeStr(bool bMs = false);
            static Time Parser(const char* datetime, const char* fmt = "%04d-%02d-%02d %02d:%02d:%02d");

        public :
            bool operator != (const Time& value) const;
            bool operator == (const Time& value) const;
            bool operator < (const Time& value) const;
            bool operator <= (const Time& value) const;
            bool operator > (const Time& value) const;
            bool operator >= (const Time& value) const;

        public :
            long GetDateTime();
            void SetDataTime(long value);
            string GetDateStr();
            string GetTimeStr(bool bMs = false);
            string GetDateTimeStr(bool bMs = false);

        public :
            void AddMilliSec(int value);  //未做检查 因基本上不可能计算后的时间大于2038年
            void AddSecond(int value);    //未做检查 因基本上不可能计算后的时间大于2038年
            void AddMinute(int value);    //未做检查 因基本上不可能计算后的时间大于2038年
            void AddHour(int value);      //未做检查 因基本上不可能计算后的时间大于2038年
            void AddDay(int value);       //未做检查 因基本上不可能计算后的时间大于2038年
            void AddMonth(int value);
            void AddYear(int value);
            void DecreaseMilliSec(int value);  //未做检查 因基本上不可能计算后的时间小于1970年
            void DecreaseSecond(int value);    //未做检查 因基本上不可能计算后的时间小于1970年
            void DecreaseMinute(int value);    //未做检查 因基本上不可能计算后的时间小于1970年
            void DecreaseHour(int value);      //未做检查 因基本上不可能计算后的时间小于1970年
            void DecreaseDay(int value);       //未做检查 因基本上不可能计算后的时间小于1970年
            void DecreaseMonth(int value);
            void DecreaseYear(int value);

        public:
            void SetTimer();
            void SetTimer(const Time& value);
            void SetMilliSec(int value);
            void SetSecond(int value);
            void SetMinute(int value);
            void SetHour(int value);
            void SetDay(int value);
            void SetMonth(int value);
            void SetYear(int value);

        public :
            int GetMilliSec();
            int GetSecond();
            int GetMinute();
            int GetHour();
            int GetDay();
            int GetMonth();
            int GetYear();
            int GetWeekDay();  //1-7
            int GetYearDay();  //1-366

        public :    //简单的月和月，年和年...比
            int BetweenMilliSec(const Time& first, const Time& tow);
            int BetweenSecond(const Time& first, const Time& tow);
            int BetweenMinute(const Time& first, const Time& tow);
            int BetweenHour(const Time& first, const Time& tow);
            int BetweenDay(const Time& first, const Time& tow);
            int BetweenMonth(const Time& first, const Time& tow);
            int BetweenYear(const Time& first, const Time& tow);

            int BetweenMilliSec(const Time& value);
            int BetweenSecond(const Time& value);
            int BetweenMinute(const Time& value);
            int BetweenHour(const Time& value);
            int BetweenDay(const Time& value);
            int BetweenMonth(const Time& value);
            int BetweenYear(const Time& value);

        public :  //总共差多少
            int BetweenAllMilliSec(const Time& first, const Time& tow);
            int BetweenAllSecond(const Time& first, const Time& tow);
            int BetweenAllMinute(const Time& first, const Time& tow);
            int BetweenAllHour(const Time& first, const Time& tow);

            int BetweenAllMilliSec(const Time& value);
            int BetweenAllSecond(const Time& value);
            int BetweenAllMinute(const Time& value);
            int BetweenAllHour(const Time& value);

        private:
            typedef enum { e_MILLISEC, e_SECOND, e_MINUTE, e_HOUR, e_DAY, e_MONTH, e_YEAR, e_WEEKDAY, e_YEARDAY } TimeType;
            void SetTime(int value, TimeType timetype);
            int GetTime(TimeType timetype);
            int BetweenTime(const Time& first, const Time& tow, TimeType timetype);
            int BetweenAllTime(const Time& first, const Time& tow, TimeType timetype);

        private:
            _timeb m_time;
    };

} // namespace lslib
