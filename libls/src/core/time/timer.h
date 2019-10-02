#pragma once
#include <sys/timeb.h>

#define DATE_FMT            "%04d-%02d-%02d"
#define TIME_FMT            "%02d:%02d:%02d"
#define MILLTIME_FMT        TIME_FMT ".%3d"
#define DATE_TIME_FMT       DATE_FMT " " TIME_FMT
#define DATE_MILLTIME_FMT   DATE_FMT " " MILLTIME_FMT

namespace lslib
{
    /// @brief ʱ�����
    /// @note ʱ���ʾ��Χ[1970-01-01 00:00:00, 2038-01-19 11:14:07]
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
        void SetYear(int value);            ///< ������[1900,]
        void SetMonth(int value);           ///< ������[1,12]
        void SetDay(int value);             ///< ������[1,31]
        void SetHour(int value);            ///< ����ʱ[0,23]
        void SetMinute(int value);          ///< ���÷�[0,59]
        void SetSecond(int value);          ///< ������[0,59]
        void SetMilliSec(int value);        ///< ���ú���[0,999]
        void SetTimeStamp(time_t value);    ///< ����ʱ���

        int GetYear();                      ///< ��ȡ��[1900,]
        int GetYearDay();                   ///< ��ȡ��������[1,366]
        int GetWeekDay();                   ///< ��ȡ����[1,7]
        int GetMonth();                     ///< ��ȡ��[1,12]
        int GetDay();                       ///< ��ȡ��[1,31]
        int GetHour();                      ///< ��ȡʱ[0,23]
        int GetMinute();                    ///< ��ȡ��[0,59]
        int GetSecond();                    ///< ��ȡ��[0,59]
        int GetMilliSec();                  ///< ��ȡ����[0,999]
        time_t GetTimeStamp();              ///< ��ȡʱ���

        /// @brief ��ȡ�����ַ�������2019-09-17
        /// @param fmt ָ�����ڸ�ʽ
        /// @return ���������ַ���
        string GetDateStr(lpcstr fmt = DATE_FMT);

        /// @brief ��ȡ��ǰʱ���ַ�������16:02:36
        /// @param fmt ָ��ʱ���ʽ
        /// @param ms �Ƿ��������
        /// @return ����ʱ���ַ���
        string GetTimeStr(bool ms = false, lpcstr fmt = TIME_FMT);

        /// @brief ��ȡ��ǰ���ڼ�ʱ���ַ�������2019-09-17 16:02:36
        /// @param fmt ָ�����ڼ�ʱ���ʽ
        /// @param ms �Ƿ��������
        /// @return �������ڼ�ʱ���ַ���
        string GetDateTimeStr(bool ms = false, lpcstr fmt = DATE_TIME_FMT);

    public:
        /// @brief �����ַ����������ڼ�ʱ��
        /// @param datetime ���ڼ�ʱ���ַ�������2019-09-17 16:02:36
        /// @param fmt ָ�����ڼ�ʱ���ʽ
        /// @param ms �Ƿ��������
        /// @return ����Timeʱ��
        static Time Parse(lpcstr datetime, bool ms = false, lpcstr fmt = DATE_TIME_FMT);

        /// @brief ����ʱ�������Time����
        static Time FromTimeStamp(time_t stamp);

        /// @brief ��ȡ��ǰʱ�������1568707393
        static time_t CurrentTimeStamp();

        /// @brief ��ȡ��ǰ�����ַ�������2019-09-17
        /// @param fmt ָ�����ڸ�ʽ
        /// @return ���������ַ���
        static string CurrentDateStr(lpcstr fmt = DATE_FMT);

        /// @brief ��ȡ��ǰʱ���ַ�������16:02:36
        /// @param fmt ָ��ʱ���ʽ
        /// @param ms �Ƿ��������
        /// @return ����ʱ���ַ���
        static string CurrentTimeStr(bool ms = false, lpcstr fmt = TIME_FMT);

        /// @brief ��ȡ��ǰ���ڼ�ʱ���ַ�������2019-09-17 16:02:36
        /// @param fmt ָ�����ڼ�ʱ���ʽ
        /// @param ms �Ƿ��������
        /// @return �������ڼ�ʱ���ַ���
        static string CurrentDateTimeStr(bool ms = false, lpcstr fmt = DATE_TIME_FMT);

    public:
        void AddYear(int value);            ///< ������
        void AddMonth(int value);           ///< ������
        void AddDay(int value);             ///< ������
        void AddHour(int value);            ///< ����ʱ
        void AddMinute(int value);          ///< ���ӷ�
        void AddSecond(int value);          ///< ������
        void AddMilliSec(int value);        ///< ���Ӻ���

        void DecreaseYear(int value);       ///< ������
        void DecreaseMonth(int value);      ///< ������
        void DecreaseDay(int value);        ///< ������
        void DecreaseHour(int value);       ///< ����ʱ
        void DecreaseMinute(int value);     ///< ���ٷ�
        void DecreaseSecond(int value);     ///< ������
        void DecreaseMilliSec(int value);   ///< ���ٺ���

    public:
        static int BetweenYear(const Time& first, const Time& second);      ///< ���������
        static int BetweenMonth(const Time& first, const Time& second);     ///< �·�������
        static int BetweenDay(const Time& first, const Time& second);       ///< ����������
        static int BetweenHour(const Time& first, const Time& second);      ///< Сʱ������
        static int BetweenMinute(const Time& first, const Time& second);    ///< ����������
        static int BetweenSecond(const Time& first, const Time& second);    ///< ����������
        static int BetweenMilliSec(const Time& first, const Time& second);  ///< ����������

        int BetweenYear(const Time& value);     ///< ���������
        int BetweenMonth(const Time& value);    ///< �·�������
        int BetweenDay(const Time& value);      ///< ����������
        int BetweenHour(const Time& value);     ///< Сʱ������
        int BetweenMinute(const Time& value);   ///< ����������
        int BetweenSecond(const Time& value);   ///< ����������
        int BetweenMilliSec(const Time& value); ///< ����������

    public:
        static int BetweenAllDay(const Time& first, const Time& second);        ///< �ܹ��������
        static int BetweenAllHour(const Time& first, const Time& second);       ///< �ܹ������Сʱ
        static int BetweenAllMinute(const Time& first, const Time& second);     ///< �ܹ�����ٷ���
        static int BetweenAllSecond(const Time& first, const Time& second);     ///< �ܹ����������
        static int BetweenAllMilliSec(const Time& first, const Time& second);   ///< �ܹ�����ٺ���

        int BetweenAllDay(const Time& value);       ///< �ܹ��������
        int BetweenAllHour(const Time& value);      ///< �ܹ������Сʱ
        int BetweenAllMinute(const Time& value);    ///< �ܹ�����ٷ���
        int BetweenAllSecond(const Time& value);    ///< �ܹ����������
        int BetweenAllMilliSec(const Time& value);  ///< �ܹ�����ٺ���

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
