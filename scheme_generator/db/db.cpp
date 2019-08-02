#include "stdafx.h"
#include "db.h"

//////////////////////////////////////////////////////////////////////////
CDB::CDB() : m_con(NULL)
{
    Init();
}

CDB::CDB(_lpcstr host, _lpcstr user, _lpcstr passowrd, _lpcstr db)
    : m_strHost(host),
      m_nPort(3306),
      m_strUser(user),
      m_strPassword(passowrd),
      m_strDB(db),
      m_bConnected(false),
      m_con(NULL)
{
    Init();
}

CDB::~CDB()
{
    mysql_close(m_con);
}

void CDB::SetHost(_lpcstr host)
{
    m_strHost = host;
}

void CDB::SetPort(int port)
{
    m_nPort = port;
}

void CDB::SetUser(_lpcstr user)
{
    m_strUser = user;
}

void CDB::SetPassword(_lpcstr password)
{
    m_strPassword = password;
}

void CDB::SetDB(_lpcstr db)
{
    m_strDB = db;
}

bool CDB::Init()
{
    m_con = mysql_init(NULL);
    return (m_con != NULL);
}

bool CDB::Connect()
{
    if (IsConnected()) return true;

    mysql_options(m_con, MYSQL_SET_CHARSET_NAME, "utf8");
    mysql_options(m_con, MYSQL_INIT_COMMAND, "SET NAMES utf8");
    MYSQL* ret = mysql_real_connect(m_con, m_strHost.c_str(), m_strUser.c_str(), m_strPassword.c_str(), m_strDB.c_str(), m_nPort, NULL, CLIENT_MULTI_STATEMENTS);
    if (ret == NULL)
    {
        ERROR_LOG(g_pLogger, "connect db fail. exception: %d-%s", mysql_errno(m_con), mysql_error(m_con));
        mysql_close(m_con);
        return false;
    }
    m_bConnected = true;
    return true;
}

bool CDB::DisConnect()
{
    if (!IsConnected()) return true;

    m_bConnected = false;
    mysql_close(m_con);
    return true;
}

bool CDB::IsConnected()
{
    return m_bConnected;
}

bool CDB::Excute(_lpcstr sql)
{
    if (!IsConnected()) Connect();
    if (!IsConnected()) return false;

    if (mysql_query(m_con, sql) != 0)
    {
        ERROR_LOG(g_pLogger, "execute statement[%s] fail. exception: %d-%s", sql, mysql_errno(m_con), mysql_error(m_con));
        return false;
    }
    return true;
}

bool CDB::ExCuteNoQuery(_lpcstr sql)
{
    if (!Excute(sql)) return false;

    do
    {
        MYSQL_RES* res = mysql_store_result(m_con);
        mysql_free_result(res);
    }
    while (!mysql_next_result(m_con));
    return true;
}

MYSQL_RES* CDB::ExcuteQuery(_lpcstr sql)
{
    if (!Excute(sql)) return NULL;
    return mysql_store_result(m_con);
}
