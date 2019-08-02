#pragma once

//////////////////////////////////////////////////////////////////////////
#define CFG_MODULE_System                                       "System"

//------------------------------------------------------------------------
// 1. DB���
#define     CFG_KEY_DB_HOST                                     "DbHost" // ���ݿ��ַ
#ifdef _DEBUG
#define         CFG_DEFAULT_DB_HOST                             "192.168.169.215"
#else
#define         CFG_DEFAULT_DB_HOST                             "192.168.169.215"
#endif // _DEBUG

#define     CFG_KEY_DB_PORT                                     "DbPort" // ���ݿ�˿�
#ifdef _DEBUG
#define         CFG_DEFAULT_DB_PORT                             3306
#else
#define         CFG_DEFAULT_DB_PORT                             3306
#endif // _DEBUG

#define     CFG_KEY_DB_USER                                     "DbUser" // ���ݿ��û���
#ifdef _DEBUG
#define         CFG_DEFAULT_DB_USER                             "suser"
#else
#define         CFG_DEFAULT_DB_USER                             "suser"
#endif // _DEBUG

#define     CFG_KEY_DB_PWD                                      "DbPassword" // ���ݿ�����
#ifdef _DEBUG
#define         CFG_DEFAULT_DB_PWD                              "cyberuser"
#else
#define         CFG_DEFAULT_DB_PWD                              "cyberuser"
#endif // _DEBUG

#define     CFG_KEY_DB_DATABASE                                 "DbDataBase" // ���ݿ���
#ifdef _DEBUG
#define         CFG_DEFAULT_DB_DATABASE                         "autoservice_debug"
#else
#define         CFG_DEFAULT_DB_DATABASE                         "autoservice_debug"
#endif // _DEBUG



// 2. MQTT���
#define     CFG_KEY_MQTT_HOST                                   "MqttHost" // MQTT��ַ
#ifdef _DEBUG
#define         CFG_DEFAULT_MQTT_HOST                           "tcp://192.168.169.215:1883"
#else
#define         CFG_DEFAULT_MQTT_HOST                           "tcp://192.168.169.215:1883"
#endif // _DEBUG

#define     CFG_KEY_MQTT_USER                                   "MqttUser" // MQTT�û���
#ifdef _DEBUG
#define         CFG_DEFAULT_MQTT_USER                           "mqtt"
#else
#define         CFG_DEFAULT_MQTT_USER                           "mqtt"
#endif // _DEBUG

#define     CFG_KEY_MQTT_PWD                                    "MqttPassword" // MQTT����
#ifdef _DEBUG
#define         CFG_DEFAULT_MQTT_PWD                            "123qwe"
#else
#define         CFG_DEFAULT_MQTT_PWD                            "123qwe"
#endif // _DEBUG
