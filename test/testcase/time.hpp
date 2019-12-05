#include "testcase.hpp"

void test_time()
{
    printf("===test_time===\n");

    Time tm = Time::FromStr("2019-10-12 10:21:54");
    printf("FromStr: `2019-10-12 10:21:54` -> %s\n", tm.GetDateTimeStr().c_str());

    tm = Time::FromStr("2019/10/12 10:21:54.450", true, "%04d/%02d/%02d %02d:%02d:%02d.%03d");
    printf("FromStr: `2019/10/12 10:21:54.450` -> %s\n", tm.GetDateTimeStr(true, DATE_MILLTIME_FMT).c_str());
    
    tm = Time::FromTimeStamp(1568707393);
    printf("FromTimeStamp: `1568707393` -> %s\n", tm.GetDateTimeStr().c_str());

    time_t stamp = Time::CurrentTimeStamp();
    printf("CurrentTimeStamp: %d\n", stamp);

    string datastr = Time::CurrentDateStr();
    printf("CurrentDateStr: %s\n", datastr.c_str());

    datastr = Time::CurrentDateStr("%04d/%02d/%02d");
    printf("CurrentDateStr: %s\n", datastr.c_str());

    datastr = Time::CurrentTimeStr();
    printf("CurrentTimeStr: %s\n", datastr.c_str());

    datastr = Time::CurrentTimeStr(true, "%02d:%02d:%02d.%03d");
    printf("CurrentTimeStr: %s\n", datastr.c_str());

    datastr = Time::CurrentDateTimeStr();
    printf("CurrentDateTimeStr: %s\n", datastr.c_str());

    datastr = Time::CurrentDateTimeStr(true, "%04d/%02d/%02d %02d:%02d:%02d.%03d");
    printf("CurrentDateTimeStr: %s\n", datastr.c_str());

    datastr = Time::CurrentDateTimeStr(false, "%04d%02d%02d%02d%02d%02d");
    printf("CurrentDateTimeStr: %s\n", datastr.c_str());
}
declare_test_case(test_time);
