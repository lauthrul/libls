#pragma once
#include "stdafx.h"
#include "MQTTClient.h"

//////////////////////////////////////////////////////////////////////////
enum_str_begin(mqtt_cmd)
enum_str_item_ex(mqtt_cmd_get_sign_state, "get_sign_state")
enum_str_item_ex(mqtt_cmd_sign_in, "sign_in")
enum_str_item_ex(mqtt_cmd_start_order, "start_order")
enum_str_item_ex(mqtt_cmd_stop_order, "stop_order")
enum_str_item_ex(mqtt_cmd_cancel_order, "cancel_order")
enum_str_item_ex(mqtt_cmd_get_order_state, "get_order_state")
enum_str_item_ex(mqtt_cmd_get_order_setting, "get_order_setting")
enum_str_item_ex(mqtt_cmd_set_order_setting, "set_order_setting")
enum_str_item_ex(mqtt_cmd_get_orders, "get_orders")
enum_str_item_ex(mqtt_cmd_reset_orders, "reset_orders")
enum_str_item_ex(mqtt_cmd_get_current_lottery, "get_current_lottery")
enum_str_item_ex(mqtt_cmd_set_current_lottery, "set_current_lottery")
enum_str_item_ex(mqtt_cmd_get_plan_counts, "get_plan_counts")
enum_str_item_ex(mqtt_cmd_get_plan, "get_plan")
enum_str_item_ex(mqtt_cmd_set_plan, "set_plan")
enum_str_item_ex(mqtt_cmd_quick_set_plan, "quick_set_plan")
enum_str_item_ex(mqtt_cmd_get_plan_status, "get_plan_status")
enum_str_end(mqtt_cmd)


enum_str_begin(mqtt_notify)
enum_str_item_ex(mqtt_notify_terminal_changed, "terminal_changed")
enum_str_item_ex(mqtt_notify_sign_state_changed, "sign_state_changed")
enum_str_item_ex(mqtt_notify_order_started, "order_started")
enum_str_item_ex(mqtt_notify_order_stopped, "order_stopped")
enum_str_item_ex(mqtt_notify_order_canceled, "order_canceled")
enum_str_item_ex(mqtt_notify_order_setting_changed, "order_setting_changed")
enum_str_item_ex(mqtt_notify_order_added, "order_added")
enum_str_item_ex(mqtt_notify_order_updated, "order_updated")
enum_str_item_ex(mqtt_notify_orders_reset, "orders_reset")
enum_str_item_ex(mqtt_notify_lottery_changed, "lottery_changed")
enum_str_item_ex(mqtt_notify_plan_updated, "plan_updated")
enum_str_item_ex(mqtt_notify_plan_status_updated, "plan_status_updated")
enum_str_item_ex(mqtt_notify_scheme_updated, "scheme_updated")
enum_str_end(mqtt_notify)


enum_str_begin(mqtt_sys)
enum_str_item(mqtt_sys_lost_connection)
enum_str_item(mqtt_sys_connected)
enum_str_item(mqtt_sys_disconnected)
enum_str_end(mqtt_sys)


enum_str_begin(mqtt_terminal)
enum_str_item_ex(mt_unknown, "unknown")
enum_str_item_ex(mt_pc, "pc")
enum_str_item_ex(mt_mobile_android, "mobile/android")
enum_str_item_ex(mt_mobile_ios, "mobile/ios")
enum_str_item_ex(mt_sgsrv, "sgsrv")
enum_str_end(mqtt_terminal)


enum_str_begin(mqtt_msg_type)
enum_str_item_ex(mmt_unknown, "unknown")
enum_str_item_ex(mmt_cmd, "cmd")
enum_str_item_ex(mmt_result, "result")
enum_str_item_ex(mmt_notify, "notify")
enum_str_item_ex(mmt_status, "status")
enum_str_end(mqtt_msg_type)
