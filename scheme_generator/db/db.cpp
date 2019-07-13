#include "stdafx.h"
#include "db.h"

//////////////////////////////////////////////////////////////////////////
CDB::CDB() : m_driver(NULL), m_con(NULL)
{
    Init();
}

CDB::CDB(_lpcstr host, _lpcstr user, _lpcstr passowrd)
    : m_strHost(host),
      m_strUser(user),
      m_strPassword(passowrd),
      m_driver(NULL),
      m_con(NULL)
{
    Init();
}

CDB::~CDB()
{
    delete m_con;
}

void CDB::SetHost(_lpcstr host)
{
    m_strHost = host;
}

void CDB::SetUser(_lpcstr user)
{
    m_strUser = user;
}

void CDB::SetPassword(_lpcstr password)
{
    m_strPassword = password;
}

bool CDB::Init()
{
    if (m_driver != NULL) return true;
    try
    {
        m_driver = get_driver_instance();
    }
    catch (sql::SQLException& e)
    {
        ERROR_LOG(g_pLogger, "init db fail. exception: %d-%s", e.getErrorCode(), e.getSQLState());
        return false;
    }
    return true;
}

bool CDB::Connect()
{
    if (IsConnected()) return true;
    try
    {
        m_con = m_driver->connect(m_strHost.c_str(), m_strUser.c_str(), m_strPassword.c_str());
    }
    catch (sql::SQLException& e)
    {
        ERROR_LOG(g_pLogger, "connect db fail. exception: %d-%s", e.getErrorCode(), e.getSQLState());
        return false;
    }
    return true;
}

bool CDB::DisConnect()
{
    if (!IsConnected()) return true;
    try
    {
        m_con->close();
        delete m_con;
        m_con = NULL;
    }
    catch (sql::SQLException& e)
    {
        ERROR_LOG(g_pLogger, "disconnect db fail. exception: %d-%s", e.getErrorCode(), e.getSQLState());
        return false;
    }
    return true;
}

bool CDB::IsConnected()
{
    return m_con && !(m_con->isClosed());
}

bool CDB::UseDB(_lpcstr db)
{
    if (!IsConnected()) Connect();
    try
    {
        m_con->setSchema(db);
    }
    catch (sql::SQLException& e)
    {
        ERROR_LOG(g_pLogger, "use db[%s] fail. exception: %d-%s", db, e.getErrorCode(), e.getSQLState());
        return false;
    }
    return true;
}

bool CDB::Excute(_lpcstr sql)
{
    if (!IsConnected()) Connect();

    bool ret = false;
    sql::Statement* stmt = NULL;
    try
    {
        stmt = m_con->createStatement();
        stmt->execute(sql);
        ret = true;
    }
    catch (sql::SQLException& e)
    {
        ERROR_LOG(g_pLogger, "execute statement[%s] fail. exception: %d-%s", sql, e.getErrorCode(), e.getSQLState());
    }
    delete stmt;
    return ret;
}

sql::ResultSet* CDB::ExcuteQuery(_lpcstr sql)
{
    if (!IsConnected()) Connect();

    sql::Statement* stmt = NULL;
    sql::ResultSet* res = NULL;
    try
    {
        stmt = m_con->createStatement();
        res = stmt->executeQuery(sql);
    }
    catch (sql::SQLException& e)
    {
        ERROR_LOG(g_pLogger, "execute query statement[%s] fail. exception: %d-%s", sql, e.getErrorCode(), e.getSQLState());
    }
    delete stmt;
    return res;
}
