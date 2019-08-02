#pragma once

#define ptr_define(T)\
    using T##_ptr = std::shared_ptr<T>;


//////////////////////////////////////////////////////////////////////////
#define mqtt_resource_base 1000

// mqtt commands
enum mqtt_cmd
{
    mqtt_cmd_get_sign_state = mqtt_resource_base + 1000,
    mqtt_cmd_sign_in,
    mqtt_cmd_start_order,
    mqtt_cmd_stop_order,
    mqtt_cmd_cancel_order,
    mqtt_cmd_get_order_state,
    mqtt_cmd_get_order_setting,
    mqtt_cmd_set_order_setting,
    mqtt_cmd_get_orders,
    mqtt_cmd_reset_orders,
    mqtt_cmd_get_current_lottery,
    mqtt_cmd_set_current_lottery,
    mqtt_cmd_get_plan_counts,
    mqtt_cmd_get_plan,
    mqtt_cmd_set_plan,
    mqtt_cmd_quick_set_plan,
    mqtt_cmd_get_plan_status,
};
declare_enum_str(mqtt_cmd, );

// mqtt notifies
enum mqtt_notify
{
    mqtt_notify_terminal_changed = mqtt_resource_base + 2000,
    mqtt_notify_sign_state_changed,
    mqtt_notify_order_started,
    mqtt_notify_order_stopped,
    mqtt_notify_order_canceled,
    mqtt_notify_order_setting_changed,
    mqtt_notify_order_added,
    mqtt_notify_order_updated,
    mqtt_notify_orders_reset,
    mqtt_notify_lottery_changed,
    mqtt_notify_plan_updated,
    mqtt_notify_plan_status_updated,
    mqtt_notify_scheme_updated,
};
declare_enum_str(mqtt_notify, );

// mqtt system msgs
enum mqtt_sys
{
    mqtt_sys_lost_connection = mqtt_resource_base + 3000,
    mqtt_sys_connected,
    mqtt_sys_disconnected,
};
declare_enum_str(mqtt_sys, );

//////////////////////////////////////////////////////////////////////////

// mqtt terminals
enum mqtt_terminal
{
    mt_unknown = -1,
    mt_pc,
    mt_mobile_android,
    mt_mobile_ios,
    mt_sgsrv,
};
declare_enum_str(mqtt_terminal, );

// mqtt msg types
enum mqtt_msg_type
{
    mmt_unknown = -1,
    mmt_cmd,
    mmt_result,
    mmt_notify,
    mmt_status,
};
declare_enum_str(mqtt_msg_type, );

//////////////////////////////////////////////////////////////////////////
// mqtt msg related definitions

ptr_define(CJsonValue);

struct mqtt_msg_base
{
    string          raw_topic;
    string          raw_payload;
    mqtt_terminal   terminal;
    mqtt_msg_type   type;

    mqtt_msg_base() : terminal(mt_unknown), type(mmt_unknown) {}
    virtual ~mqtt_msg_base() = default;
};
ptr_define(mqtt_msg_base);

struct mqtt_cmd_msg : public mqtt_msg_base
{
    string          sid;
    string          cmd;
    CJsonValue_ptr  data;
};
ptr_define(mqtt_cmd_msg);

struct mqtt_result_msg: public mqtt_msg_base
{
    string          sid;
    CJsonValue_ptr  data;
};
ptr_define(mqtt_result_msg);

struct mqtt_notify_msg : public mqtt_msg_base
{
    string          event;
    CJsonValue_ptr  data;
};
ptr_define(mqtt_notify_msg);

struct mqtt_status_msg : public mqtt_msg_base
{
    bool            status;
};
ptr_define(mqtt_status_msg);

//////////////////////////////////////////////////////////////////////////
// mqtt cmd parameters

// param for command base
struct mqtt_cmd_param_base
{
    string          sid;

    mqtt_cmd_param_base() = default;
    mqtt_cmd_param_base(_lpcstr _sid) : sid(_sid) {}
    virtual ~mqtt_cmd_param_base() = default;
};
ptr_define(mqtt_cmd_param_base);
