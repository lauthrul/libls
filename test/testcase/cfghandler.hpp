#include "testcase.hpp"

#define CFG_FILE                    "config.dat"

#define CFG_MODULE_System           "system"
#define CFG_KEY_MQTT_HOST           "mqtt_host"
#define     CFG_DEFAULT_MQTT_HOST   "192.168.169.215"

#define CFG_KEY_MQTT_USER           "mqtt_user"
#define     CFG_DEFAULT_MQTT_USER   "mqtt"

#define CFG_KEY_MQTT_PWD            "mqtt_pwd"
#define     CFG_DEFAULT_MQTT_PWD    "123qwe"

#define CFG_KEY_LOG_LEVEL           "log_level"
#define     CFG_DEFAULT_LOG_LEVEL   7

#define CFG_KEY_PRICE               "price"
#define     CFG_DEFAULT_PRICE       0.02


void test_cfghandler()
{
    string strValue;
    int nValue;
    float fValue;

    //
    strValue = CCfgHandler::GetCfgText(CFG_MODULE_System, CFG_KEY_MQTT_HOST, CFG_DEFAULT_MQTT_HOST, CFG_FILE);
    printf("[get] mqtt_host: %s\n", strValue.c_str());

    strValue += "_xxx";
    CCfgHandler::SetCfg(CFG_MODULE_System, CFG_KEY_MQTT_HOST, strValue.c_str(), CFG_FILE);
    printf("[set] mqtt_host: %s\n", strValue.c_str());

    //
    strValue = CCfgHandler::GetCfgText(CFG_MODULE_System, CFG_KEY_MQTT_USER, CFG_DEFAULT_MQTT_USER, CFG_FILE);
    printf("[get] mqtt_user: %s\n", strValue.c_str());

    strValue += "_xxx";
    CCfgHandler::SetCfg(CFG_MODULE_System, CFG_KEY_MQTT_USER, strValue.c_str(), CFG_FILE);
    printf("[set] mqtt_user: %s\n", strValue.c_str());

    //
    strValue = CCfgHandler::GetCfgText(CFG_MODULE_System, CFG_KEY_MQTT_PWD, CFG_DEFAULT_MQTT_PWD, CFG_FILE);
    printf("[get] mqtt_pwd: %s\n", strValue.c_str());

    strValue += "_xxx";
    CCfgHandler::SetCfg(CFG_MODULE_System, CFG_KEY_MQTT_PWD, strValue.c_str(), CFG_FILE);
    printf("[set] mqtt_pwd: %s\n", strValue.c_str());

    //
    nValue = CCfgHandler::GetCfgInt(CFG_MODULE_System, CFG_KEY_LOG_LEVEL, CFG_DEFAULT_LOG_LEVEL, CFG_FILE);
    printf("[get] log_level: %d\n", nValue);

    CCfgHandler::SetCfg(CFG_MODULE_System, CFG_KEY_LOG_LEVEL, ++nValue, CFG_FILE);
    printf("[set] log_level: %d\n", nValue);

    //
    fValue = CCfgHandler::GetCfgFloat(CFG_MODULE_System, CFG_KEY_PRICE, CFG_DEFAULT_PRICE, CFG_FILE);
    printf("[get] price: %f\n", fValue);

    CCfgHandler::SetCfg(CFG_MODULE_System, CFG_KEY_PRICE, ++fValue, CFG_FILE);
    printf("[set] price: %f\n", fValue);

}

declare_test_case(test_cfghandler);
