#pragma once
#include <sys/timeb.h>

#define DATE_FMT            "%04d-%02d-%02d"
#define TIME_FMT            "%02d:%02d:%02d"
#define MILLTIME_FMT        TIME_FMT ".%3d"
#define DATE_TIME_FMT       DATE_FMT " " TIME_FMT
#define DATE_MILLTIME_FMT   DATE_FMT " " MILLTIME_FMT

namespace lslib
{
    /// @brief 时间相关
    /// @note 时间表示范围[1970-01-01 00:00:00, 2038-01-19 11:14:07]
    class LSLIB_API Time
    {
    public :
        Time();
        ~Time();

        bool operator == (const Time& value) const;
        bool operator != (const Time& value) const;
        bool operator < (const Time& value) const;
        bool operator <= (const Time& value) const;
        bool operator > (const Time& value) const;
        bool operator >= (const Time& value) const;

    public:
        void SetYear(int value);            ///< 设置年[1900,]
        void SetMonth(int value);           ///< 设置月[1,12]
        void SetDay(int value);             ///< 设置日[1,31]
        void SetHour(int value);            ///< 设置时[0,23]
        void SetMinute(int value);          ///< 设置分[0,59]
        void SetSecond(int value);          ///< 设置秒[0,59]
        void SetMilliSec(int value);        ///< 设置毫秒[0,999]
        void SetTimeStamp(time_t value);    ///< 设置时间戳

        int GetYear();                      ///< 获取年[1900,]
        int GetYearDay();                   ///< 获取年内天数[1,366]
        int GetWeekDay();                   ///< 获取星期[1,7]
        int GetMonth();                     ///< 获取月[1,12]
        int GetDay();                       ///< 获取日[1,31]
        int GetHour();                      ///< 获取时[0,23]
        int GetMinute();                    ///< 获取分[0,59]
        int GetSecond();                    ///< 获取秒[0,59]
        int GetMilliSec();                  ///< 获取毫秒[0,999]
        time_t GetTimeStamp();              ///< 获取时间戳

        /// @brief 获取日期字符串，如2019-09-17
        /// @param fmt 指定日期格式
        /// @return 返回日期字符串
        string GetDateStr(lpcstr fmt = DATE_FMT);

        /// @brief 获取当前时间字符串，如16:02:36
        /// @param fmt 指定时间格式
        /// @param ms 是否包含毫秒
        /// @return 返回时间字符串
        string GetTimeStr(bool ms = false, lpcstr fmt = TIME_FMT);

        /// @brief 获取当前日期及时间字符串，如2019-09-17 16:02:36
        /// @param fmt 指定日期及时间格式
        /// @param ms 是否包含毫秒
        /// @return 返回日期及时间字符串
        string GetDateTimeStr(bool ms = false, lpcstr fmt = DATE_TIME_FMT);

    public:
        /// @brief 根据字符串解析日期及时间
        /// @param datetime 日期及时间字符串，如2019-09-17 16:02:36
        /// @param fmt 指定日期及时间格式
        /// @param ms 是否包含毫秒
        /// @return 返回Time时间
        static Time Parse(lpcstr datetime, bool ms = false, lpcstr fmt = DATE_TIME_FMT);

        /// @brief 根据时间戳创建Time对象
        static Time FromTimeStamp(time_t stamp);

        /// @brief 获取当前时间戳，如1568707393
        static time_t CurrentTimeStamp();

        /// @brief 获取当前日期字符串，如2019-09-17
        /// @param fmt 指定日期格式
        /// @return 返回日期字符串
        static string CurrentDateStr(lpcstr fmt = DATE_FMT);

        /// @brief 获取当前时间字符串，如16:02:36
        /// @param fmt 指定时间格式
        /// @param ms 是否包含毫秒
        /// @return 返回时间字符串
        static string CurrentTimeStr(bool ms = false, lpcstr fmt = TIME_FMT);

        /// @brief 获取当前日期及时间字符串，如2019-09-17 16:02:36
        /// @param fmt 指定日期及时间格式
        /// @param ms 是否包含毫秒
        /// @return 返回日期及时间字符串
        static string CurrentDateTimeStr(bool ms = false, lpcstr fmt = DATE_TIME_FMT);

    public:
        void AddYear(int value);            ///< 增加年
        void AddMonth(int value);           ///< 增加月
        void AddDay(int value);             ///< 增加日
        void AddHour(int value);            ///< 增加时
        void AddMinute(int value);          ///< 增加分
        void AddSecond(int value);          ///< 增加秒
        void AddMilliSec(int value);        ///< 增加毫秒

        void DecreaseYear(int value);       ///< 减少年
        void DecreaseMonth(int value);      ///< 减少月
        void DecreaseDay(int value);        ///< 减少日
        void DecreaseHour(int value);       ///< 减少时
        void DecreaseMinute(int value);     ///< 减少分
        void DecreaseSecond(int value);     ///< 减少秒
        void DecreaseMilliSec(int value);   ///< 减少毫秒

    public:
        static int BetweenYear(const Time& first, const Time& second);      ///< 年份相差多少
        static int BetweenMonth(const Time& first, const Time& second);     ///< 月份相差多少
        static int BetweenDay(const Time& first, const Time& second);       ///< 天数相差多少
        static int BetweenHour(const Time& first, const Time& second);      ///< 小时相差多少
        static int BetweenMinute(const Time& first, const Time& second);    ///< 分钟相差多少
        static int BetweenSecond(const Time& first, const Time& second);    ///< 秒钟相差多少
        static int BetweenMilliSec(const Time& first, const Time& second);  ///< 毫秒相差多少

        int BetweenYear(const Time& value);     ///< 年份相差多少
        int BetweenMonth(const Time& value);    ///< 月份相差多少
        int BetweenDay(const Time& value);      ///< 天数相差多少
        int BetweenHour(const Time& value);     ///< 小时相差多少
        int BetweenMinute(const Time& value);   ///< 分钟相差多少
        int BetweenSecond(const Time& value);   ///< 秒钟相差多少
        int BetweenMilliSec(const Time& value); ///< 毫秒相差多少

    public:
        static int BetweenAllDay(const Time& first, const Time& second);        ///< 总共差多少天
        static int BetweenAllHour(const Time& first, const Time& second);       ///< 总共差多少小时
        static int BetweenAllMinute(const Time& first, const Time& second);     ///< 总共差多少分钟
        static int BetweenAllSecond(const Time& first, const Time& second);     ///< 总共差多少秒钟
        static int BetweenAllMilliSec(const Time& first, const Time& second);   ///< 总共差多少毫秒

        int BetweenAllDay(const Time& value);       ///< 总共差多少天
        int BetweenAllHour(const Time& value);      ///< 总共差多少小时
        int BetweenAllMinute(const Time& value);    ///< 总共差多少分钟
        int BetweenAllSecond(const Time& value);    ///< 总共差多少秒钟
        int BetweenAllMilliSec(const Time& value);  ///< 总共差多少毫秒

    private:
        typedef enum { e_MILLISEC, e_SECOND, e_MINUTE, e_HOUR, e_DAY, e_MONTH, e_YEAR, e_WEEKDAY, e_YEARDAY } TimeType;
        void SetTime(int value, TimeType timetype);
        int GetTime(TimeType timetype);
        static int BetweenTime(const Time& first, const Time& second, TimeType timetype);
        static int BetweenAllTime(const Time& first, const Time& second, TimeType timetype);

    private:
        timeb m_time;
    };

} // namespace lslib
