#pragma once

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

//////////////////////////////////////////////////////////////////////////
class CDB
{
public:
    CDB();
    CDB(_lpcstr host, _lpcstr user, _lpcstr passowrd);
    ~CDB();

public:
    void SetHost(_lpcstr host);
    void SetUser(_lpcstr user);
    void SetPassword(_lpcstr password);

    bool Init();
    bool Connect();
    bool DisConnect();
    bool IsConnected();
    bool UseDB(_lpcstr db);
    bool Excute(_lpcstr sql);
    sql::ResultSet* ExcuteQuery(_lpcstr sql);

protected:
    lstring m_strHost;
    lstring m_strUser;
    lstring m_strPassword;

    sql::Driver* m_driver;
    sql::Connection* m_con;
};