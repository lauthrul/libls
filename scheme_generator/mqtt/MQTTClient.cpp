#include "stdafx.h"
#include "MQTTClient.h"

//////////////////////////////////////////////////////////////////////////
constexpr int   MAX_BUF                         = 260;
constexpr int   RETRY_ATTEMPTS                  = 3;

// mqtt connection status
#define mqtt_status_online                      "1"
#define mqtt_status_offline                     "0"

// mqtt device identities
// #define mqtt_identity_pc                        enum_str(mqtt_terminal, mt_pc)
// #define mqtt_identity_mobile                    "mobile"
// #define mqtt_identity_mobile_android            enum_str(mqtt_terminal, mt_mobile_android)
// #define mqtt_identity_mobile_ios                enum_str(mqtt_terminal, mt_mobile_ios)
// #define mqtt_identity_sgsrv                     enum_str(mqtt_terminal, mt_sgsrv)

// mqtt default qos
#define mqtt_qos                                0

// mqtt msg key words, which are specified in mqtt topic
// #define mqtt_msg_key_cmd                        enum_str(mqtt_msg_type, mmt_cmd)
// #define mqtt_msg_key_result                     enum_str(mqtt_msg_type, mmt_result)
// #define mqtt_msg_key_notify                     enum_str(mqtt_msg_type, mmt_notify)
// #define mqtt_msg_key_status                     enum_str(mqtt_msg_type, mmt_status)

// mqtt action definitions
enum mqtt_action
{
    ma_connect,
    ma_publish_disconnect,
    ma_disconnect,
    ma_subscribe,
    ma_unsubscribe,
    ma_publish,
};

//////////////////////////////////////////////////////////////////////////
string make_client_id(_lpcstr merchant, _lpcstr user, _lpcstr terminal)
{
    string str = strtool::format("%s/%s/%s", merchant, user, terminal);
    str = strtool::lower(str);
    return str;
}

string make_status_topic(_lpcstr client_id)
{
    string str = strtool::format("%s/%s", client_id, enum_str(mqtt_msg_type, mmt_status).c_str());
    return str;
}

bool is_status_topic(_lpcstr topic)
{
    string strkey = strtool::format("/%s", enum_str(mqtt_msg_type, mmt_status).c_str());
    return (strstr(topic, strkey.c_str()) != nullptr);
}

string make_cmd_topic_prefix(_lpcstr client_id)
{
    string str = strtool::format("%s/%s/", client_id, enum_str(mqtt_msg_type, mmt_cmd).c_str());
    return str;
}

bool is_cmd_topic(_lpcstr topic)
{
    string strkey = strtool::format("/%s/", enum_str(mqtt_msg_type, mmt_cmd).c_str());
    return (strstr(topic, strkey.c_str()) != nullptr);
}

string get_cmd_from_topic(_lpcstr topic)
{
    if (is_cmd_topic(topic))
    {
        string strkey = strtool::format("/%s/", enum_str(mqtt_msg_type, mmt_cmd).c_str());
        return strstr(topic, strkey.c_str()) + strlen(strkey.c_str());
    }
    return "";
}

string make_result_topic(_lpcstr client_id)
{
    string str = strtool::format("%s/%s", client_id, enum_str(mqtt_msg_type, mmt_result).c_str());
    return str;
}

bool is_result_topic(_lpcstr topic)
{
    string strkey = strtool::format("/%s", enum_str(mqtt_msg_type, mmt_result).c_str());
    return (strstr(topic, strkey.c_str()) != nullptr);
}

string make_notify_topic_prefix(_lpcstr client_id)
{
    string str = strtool::format("%s/%s/", client_id, enum_str(mqtt_msg_type, mmt_notify).c_str());
    return str;
}

bool is_notify_topic(_lpcstr topic)
{
    string strkey = strtool::format("/%s/", enum_str(mqtt_msg_type, mmt_notify).c_str());
    return (strstr(topic, strkey.c_str()) != nullptr);
}

string get_notify_from_topic(_lpcstr topic)
{
    if (is_notify_topic(topic))
    {
        string strkey = strtool::format("/%s/", enum_str(mqtt_msg_type, mmt_notify).c_str());
        return strstr(topic, strkey.c_str()) + strlen(strkey.c_str());
    }
    return "";
}

string make_sub_topic(_lpcstr target_client_id)
{
    string str = strtool::format("%s/#", target_client_id);
    return str;
}

mqtt_terminal get_terminal_from_topic(_lpcstr topic)
{
    if (strstr(topic, ("/" + enum_str(mqtt_terminal, mt_pc) + "/").c_str()) != nullptr)
        return mt_pc;
    else if (strstr(topic, ("/" + enum_str(mqtt_terminal, mt_mobile_android) + "/").c_str()) != nullptr)
        return mt_mobile_android;
    else if (strstr(topic, ("/" + enum_str(mqtt_terminal, mt_mobile_ios) + "/").c_str()) != nullptr)
        return mt_mobile_ios;
    else if (strstr(topic, ("/" + enum_str(mqtt_terminal, mt_sgsrv) + "/").c_str()) != nullptr)
        return mt_sgsrv;
    return mt_unknown;
}

//////////////////////////////////////////////////////////////////////////
CMQTTClient::CMQTTClient() : m_pNotifier(NULL)
{
}

CMQTTClient::~CMQTTClient()
{
}

void CMQTTClient::SetNotifier(CThread* notifier)
{
    m_pNotifier = notifier;
}

void CMQTTClient::Init()
{
    m_strClientID = enum_str(mqtt_terminal, mt_sgsrv);
    m_strStatusTopic = make_status_topic(m_strClientID.c_str());
    m_strCmdTopicPrefix = make_cmd_topic_prefix(m_strClientID.c_str());
    m_strNotifyTopicPrefix = make_notify_topic_prefix(m_strClientID.c_str());
    m_strResultTopic = make_result_topic(m_strClientID.c_str());
    m_strSubTopic = "";
}

bool CMQTTClient::IsConnected()
{
    if (m_pMqtt != nullptr)
        return m_pMqtt->is_connected();
    return false;
}

void CMQTTClient::Connect(_lpcstr host, _lpcstr user, _lpcstr pwd)
{
    Init();

    m_pMqtt = make_shared<mqtt::async_client>(host, m_strClientID.c_str());
    m_pMqtt->set_callback(*this);

    m_pConnectOpts = make_shared<mqtt::connect_options>();

    m_pConnectOpts->set_connect_timeout(30);
    m_pConnectOpts->set_keep_alive_interval(60);
    m_pConnectOpts->set_user_name(user);
    m_pConnectOpts->set_password(pwd);

    mqtt::message will_msg(m_strStatusTopic.c_str(), mqtt_status_offline, mqtt_qos, true);
    mqtt::will_options will_opt(will_msg);
    m_pConnectOpts->set_will(will_opt);

    m_pMqtt->connect(*m_pConnectOpts, (void*)ma_connect, *this);

    DEBUG_LOG(g_pLogger, "mqtt connect ...");
}

void CMQTTClient::Disconnect()
{
    if (m_pMqtt && m_pMqtt->is_connected())
    {
        DEBUG_LOG(g_pLogger, "mqtt disconnect ...");

        mqtt::message_ptr msg = mqtt::make_message(m_strStatusTopic, mqtt_status_offline, mqtt_qos, true);
        m_pMqtt->publish(msg, (void*)ma_publish_disconnect, *this);
    }
}

void CMQTTClient::Send(_lpcstr topic, _lpcstr payload, bool to_utf8 /*= true*/)
{
    if (m_pMqtt && m_pMqtt->is_connected())
    {
        DEBUG_LOG(g_pLogger, "mqtt send: [topic]: %s, [payload]: %s", topic, payload);

        string strpayload;
        if (payload) strpayload = (to_utf8 ? crypto::encoding_convert(payload, "gb2312", "utf-8") : payload);

        mqtt::message_ptr pMsg = mqtt::make_message(topic, strpayload.c_str(), mqtt_qos, false);
        m_pMqtt->publish(pMsg, (void*)ma_publish, *this);
    }
}

void CMQTTClient::SendCommand(_lpcstr cmd, _lpcstr data, bool to_utf8 /*= true*/)
{
    Send((m_strCmdTopicPrefix + cmd).c_str(), data, to_utf8);
}

void CMQTTClient::SendNotify(_lpcstr event, _lpcstr data, bool to_utf8 /*= true*/)
{
    Send((m_strNotifyTopicPrefix + event).c_str(), data, to_utf8);
}

void CMQTTClient::SendResult(_lpcstr data, bool to_utf8 /*= true*/)
{
    Send(m_strResultTopic.c_str(), data, to_utf8);
}

void CMQTTClient::reconnect()
{
    this_thread::sleep_for(chrono::milliseconds(2500));
    DEBUG_LOG(g_pLogger, "mqtt re-connect[%d] ... ", m_nConnectRetry + 1);

    try
    {
        m_pMqtt->connect(*m_pConnectOpts, (void*)ma_connect, *this);
    }
    catch (const mqtt::exception& exc)
    {
        ERROR_LOG(g_pLogger, "mqtt re-connect exception[%s]", exc.what());
    }
}

void CMQTTClient::on_failure(const mqtt::token& tok)
{
    mqtt_action eAction = (mqtt_action)(int)tok.get_user_context();
    ERROR_LOG(g_pLogger, "mqtt action[%d] fail. error[%d-%s]", eAction, tok.get_return_code(), tok.get_err_message().c_str());

    switch (eAction)
    {
        case ma_connect:
            {
                if (++m_nConnectRetry <= RETRY_ATTEMPTS)
                {
                    reconnect();
                }
                else
                {
                    ERROR_LOG(g_pLogger, "mqtt connect failed with attempts[%d]", RETRY_ATTEMPTS);
                    // notify to main thread after all (re-)connect attempts fail.
                    if (m_pNotifier) m_pNotifier->SendMessage(mqtt_sys_lost_connection, 0, 0);
                }
            }
            break;
        case ma_publish_disconnect:
            m_pMqtt->disconnect((void*)ma_disconnect, *this);
            break;
        case ma_disconnect:
            break;
        case ma_subscribe:
            break;
        case ma_unsubscribe:
            break;
        case ma_publish:
            break;
        default:
            break;
    }
}

void CMQTTClient::on_success(const mqtt::token& tok)
{
    mqtt_action eAction = (mqtt_action)(int)tok.get_user_context();
    DEBUG_LOG(g_pLogger, "mqtt action[%d] success", eAction);

    switch (eAction)
    {
        case ma_connect:
            {
                m_nConnectRetry = 0;

                if (!m_strSubTopic.empty())
                    m_pMqtt->subscribe(m_strSubTopic, mqtt_qos, (void*)ma_subscribe, *this);

                mqtt::message_ptr pPubMsg = mqtt::make_message(m_strStatusTopic, mqtt_status_online, mqtt_qos, true);
                m_pMqtt->publish(pPubMsg, (void*)ma_publish, *this);
            }
            break;
        case ma_publish_disconnect:
            m_pMqtt->disconnect((void*)ma_disconnect, *this);
            break;
        case ma_disconnect:
            // notify to main thread after below conditions:
            // 1. send device offline state to mqtt broker successfully.
            // 2. disconnect from mqtt broker successfully.
            if (m_pNotifier) m_pNotifier->SendMessage(mqtt_sys_disconnected, 0, 0);
            break;
        case ma_subscribe:
            // notify to main thread after below conditions:
            // 1. connect mqtt broker successfully.
            // 2. subscribe communication topic from mqtt broker successfully.
            if (m_pNotifier) m_pNotifier->SendMessage(mqtt_sys_connected, 0, 0);
            break;
        case ma_unsubscribe:
            break;
        case ma_publish:
            break;
        default:
            break;
    }
}

void CMQTTClient::connected(const string& cause)
{
    // TODO: on_success() will be triggered before this callback after connect successful. so just ignore this, and handle on on_success()
}

void CMQTTClient::connection_lost(const string& cause)
{
    WARN_LOG(g_pLogger, "mqtt connection lost. cause[%s]", cause.c_str());
    reconnect();
}

void CMQTTClient::message_arrived(mqtt::const_message_ptr msg)
{
    mqtt_msg_base_ptr pmqttmsg = nullptr;
    const string& topic = msg->get_topic();
    const string& payload = msg->get_payload_str();
    DEBUG_LOG(g_pLogger, "mqtt received: [topic]: %s, [payload]: %s", topic.c_str(), payload.c_str());

    if (is_status_topic(topic.c_str())) // status msg
    {
        mqtt_status_msg mqmsg;
        mqmsg.type = mmt_status;
        if (payload == mqtt_status_online)          mqmsg.status = true;
        else if (payload == mqtt_status_offline)    mqmsg.status = false;

        pmqttmsg = std::dynamic_pointer_cast<mqtt_msg_base>(std::make_shared<mqtt_status_msg>(mqmsg));
    }
    else if (is_cmd_topic(topic.c_str())) // cmd msg
    {
        mqtt_cmd_msg mqmsg;
        mqmsg.type = mmt_cmd;
        mqmsg.cmd = get_cmd_from_topic(topic.c_str());

        CJsonValue jroot;
        if (CJsonWrapper::Parse(payload.c_str(), &jroot))
        {
            CJsonValue* jnode = CJsonWrapper::GetNode(&jroot, "sid");
            if (jnode != nullptr && jnode->isString())
                mqmsg.sid = jnode->asString();

            jnode = CJsonWrapper::GetNode(&jroot, "data");
            if (jnode != nullptr) mqmsg.data = std::make_shared<CJsonValue>(*jnode);
        }

        pmqttmsg = std::dynamic_pointer_cast<mqtt_msg_base>(std::make_shared<mqtt_cmd_msg>(mqmsg));
    }
    else if (is_result_topic(topic.c_str())) // result msg
    {
        mqtt_result_msg mqmsg;
        mqmsg.type = mmt_result;

        CJsonValue jroot;
        if (CJsonWrapper::Parse(payload.c_str(), &jroot))
        {
            CJsonValue* jnode = CJsonWrapper::GetNode(&jroot, "sid");
            if (jnode != nullptr && jnode->isString())
                mqmsg.sid = jnode->asString();

            jnode = CJsonWrapper::GetNode(&jroot, "data");
            if (jnode != nullptr) mqmsg.data = std::make_shared<CJsonValue>(*jnode);
        }

        pmqttmsg = std::dynamic_pointer_cast<mqtt_msg_base>(std::make_shared<mqtt_result_msg>(mqmsg));
    }
    else if (is_notify_topic(topic.c_str())) // notify msg
    {
        mqtt_notify_msg mqmsg;
        mqmsg.type = mmt_notify;
        mqmsg.event = get_notify_from_topic(topic.c_str());

        CJsonValue jroot;
        if (CJsonWrapper::Parse(payload.c_str(), &jroot))
            mqmsg.data = std::make_shared<CJsonValue>(jroot);

        pmqttmsg = std::dynamic_pointer_cast<mqtt_msg_base>(std::make_shared<mqtt_notify_msg>(mqmsg));
    }

    if (pmqttmsg != nullptr)
    {
        pmqttmsg->raw_topic = topic;
        pmqttmsg->raw_payload = payload;
        pmqttmsg->terminal = get_terminal_from_topic(topic.c_str());

        HanldeMessage(pmqttmsg);
    }
    else
    {
        CJsonValue jv;
        jv["code"] = -1;
        jv["msg"] = "unknown message";
        SendResult(CJsonWrapper::Dumps(jv).c_str());
    }
}

void CMQTTClient::delivery_complete(mqtt::delivery_token_ptr token)
{
    // TODO: do nothing.
}

void CMQTTClient::HanldeMessage(mqtt_msg_base_ptr msg)
{
    if (msg == nullptr) return;

    switch (msg->type)
    {
        case mmt_cmd:
            HandleCmd(std::dynamic_pointer_cast<mqtt_cmd_msg>(msg));
            break;
        case mmt_result:
            HandleResult(std::dynamic_pointer_cast<mqtt_result_msg>(msg));
            break;
        case mmt_notify:
            HandleNotify(std::dynamic_pointer_cast<mqtt_notify_msg>(msg));
            break;
        case mmt_status:
            HandleStatus(std::dynamic_pointer_cast<mqtt_status_msg>(msg));
            break;
    }
}

void CMQTTClient::HandleCmd(mqtt_cmd_msg_ptr msg)
{
}

void CMQTTClient::HandleResult(mqtt_result_msg_ptr msg)
{
}

void CMQTTClient::HandleNotify(mqtt_notify_msg_ptr msg)
{
}

void CMQTTClient::HandleStatus(mqtt_status_msg_ptr msg)
{
}

//////////////////////////////////////////////////////////////////////////
CMQTTClient g_mqttClient;
