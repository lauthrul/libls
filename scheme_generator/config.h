#pragma once

//////////////////////////////////////////////////////////////////////////
#define CFG_MODULE_System                                       "System"

//------------------------------------------------------------------------
// 1. DB相关
#define     CFG_KEY_DB_HOST                                     "DbHost" // 数据库地址
#ifdef _DEBUG
#define         CFG_DEFAULT_DB_HOST                             "192.168.169.215"
#else
#define         CFG_DEFAULT_DB_HOST                             "192.168.169.215"
#endif // _DEBUG

#define     CFG_KEY_DB_PORT                                     "DbPort" // 数据库端口
#ifdef _DEBUG
#define         CFG_DEFAULT_DB_PORT                             3306
#else
#define         CFG_DEFAULT_DB_PORT                             3306
#endif // _DEBUG

#define     CFG_KEY_DB_USER                                     "DbUser" // 数据库用户名
#ifdef _DEBUG
#define         CFG_DEFAULT_DB_USER                             "suser"
#else
#define         CFG_DEFAULT_DB_USER                             "suser"
#endif // _DEBUG

#define     CFG_KEY_DB_PWD                                      "DbPassword" // 数据库密码
#ifdef _DEBUG
#define         CFG_DEFAULT_DB_PWD                              "cyberuser"
#else
#define         CFG_DEFAULT_DB_PWD                              "cyberuser"
#endif // _DEBUG

#define     CFG_KEY_DB_DATABASE                                 "DbDataBase" // 数据库名
#ifdef _DEBUG
#define         CFG_DEFAULT_DB_DATABASE                         "autoservice_debug"
#else
#define         CFG_DEFAULT_DB_DATABASE                         "autoservice_debug"
#endif // _DEBUG



// 2. MQTT相关
#define     CFG_KEY_MQTT_HOST                                   "MqttHost" // MQTT地址
#ifdef _DEBUG
#define         CFG_DEFAULT_MQTT_HOST                           "tcp://192.168.169.215:1883"
#else
#define         CFG_DEFAULT_MQTT_HOST                           "tcp://192.168.169.215:1883"
#endif // _DEBUG

#define     CFG_KEY_MQTT_USER                                   "MqttUser" // MQTT用户名
#ifdef _DEBUG
#define         CFG_DEFAULT_MQTT_USER                           "mqtt"
#else
#define         CFG_DEFAULT_MQTT_USER                           "mqtt"
#endif // _DEBUG

#define     CFG_KEY_MQTT_PWD                                    "MqttPassword" // MQTT密码
#ifdef _DEBUG
#define         CFG_DEFAULT_MQTT_PWD                            "123qwe"
#else
#define         CFG_DEFAULT_MQTT_PWD                            "123qwe"
#endif // _DEBUG
