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

#define CFG_KEY_AVATAR              "avatar"

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

    //
    _ldword len = 0;
    _lpbyte pbuf = os::get_file_buffer("avatar.jpg", &len);
    CCfgHandler::SetCfg(CFG_MODULE_System, CFG_KEY_AVATAR, pbuf, len, CFG_FILE);
    os::release_file_buffer(pbuf);

    len = CCfgHandler::GetCfgBlob((_lpvoid&)pbuf, CFG_MODULE_System, CFG_KEY_AVATAR, CFG_FILE);
    os::save_buffer_to_file(pbuf, len, "avatar_new.jpg", 0);
    os::release_file_buffer(pbuf);

    printf("===load conifg===\n", fValue);
    SCfgData_list lst;
    CCfgHandler::LoadCfg(lst, CFG_FILE);
    string value;
    for (SCfgData_list::iterator it = lst.begin(); it != lst.end(); it++)
    {
        const SCfgData& data = *it;
        switch (data.eValueType)
        {
            case CVT_INT:
                value = strtool::format("%d", data.v_int);
                break;
            case CVT_FLOAT:
                value = strtool::format("%f", data.v_float);
                break;
            case CVT_STRING:
                value = data.v_str;
                break;
            case CVT_BLOB:
                _lbyte_array arr; arr.assign(data.v_blob.begin(), data.v_blob.end());
                value = "[hex] " + strtool::byte_array_to_hex_str(arr);
                break;
        }
        printf("%-20s %-20s %-20s %-20s\n", data.strModule.c_str(), data.strKey.c_str(), enum_str(ECfgValueType, data.eValueType).c_str(), value.c_str());
    }
}
declare_test_case(test_cfghandler);
