#pragma once
#include "mqtt/async_client.h"
#include "MQTTDef.h"

//////////////////////////////////////////////////////////////////////////
class CMQTTClient : public mqtt::callback, public mqtt::iaction_listener
{
public:
    CMQTTClient();
    virtual ~CMQTTClient();

public:
    // common functions
    void SetNotifier(CThread* notifier);
    bool IsConnected();
    void Connect(_lpcstr host, _lpcstr user, _lpcstr pwd);
    void Disconnect();
    void SendCommand(_lpcstr cmd, _lpcstr data, bool to_utf8 = true);
    void SendNotify(_lpcstr event, _lpcstr data, bool to_utf8 = true);
    void SendResult(_lpcstr data, bool to_utf8 = true);

protected:
    void reconnect();

    /*implement of mqtt::iaction_listener*/
    void on_failure(const mqtt::token& tok) override;
    void on_success(const mqtt::token& tok) override;

    /*implement of mqtt::callback*/
    void connected(const string& cause) override;
    void connection_lost(const string& cause) override;
    void message_arrived(mqtt::const_message_ptr msg) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

protected:
    void Init();
    void Send(_lpcstr topic, _lpcstr payload, bool to_utf8 = true);
    void Notify();
    void HanldeMessage(mqtt_msg_base_ptr msg);
    void HandleCmd(mqtt_cmd_msg_ptr msg); // handle command from other terminals (such as mobile/android and mobile/ios)
    void HandleResult(mqtt_result_msg_ptr msg); // handle result which is request from self to other terminals
    void HandleNotify(mqtt_notify_msg_ptr msg); // handle notify from other terminals
    void HandleStatus(mqtt_status_msg_ptr msg); // handle connection status change for other terminals

private:
    CThread*                    m_pNotifier;             // callback thread handler

    string                      m_strClientID;
    string                      m_strStatusTopic;
    string                      m_strCmdTopicPrefix;
    string                      m_strNotifyTopicPrefix;
    string                      m_strResultTopic;
    string                      m_strSubTopic;
    mqtt::async_client_ptr      m_pMqtt;
    mqtt::connect_options_ptr   m_pConnectOpts;
    int                         m_nConnectRetry;
};

extern CMQTTClient g_mqttClient;
