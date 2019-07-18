#pragma once

#include <mysql.h>

//////////////////////////////////////////////////////////////////////////
class CDB
{
public:
    CDB();
    CDB(_lpcstr host, _lpcstr user, _lpcstr passowrd, _lpcstr db);
    ~CDB();

public:
    void SetHost(_lpcstr host);
    void SetUser(_lpcstr user);
    void SetPassword(_lpcstr password);
    void SetDB(_lpcstr db);

    bool Connect();
    bool DisConnect();
    bool IsConnected();
    bool ExCuteNoQuery(_lpcstr sql);
    MYSQL_RES* ExcuteQuery(_lpcstr sql);

protected:
    bool Init();
    bool Excute(_lpcstr sql);

protected:
    string m_strHost;
    string m_strUser;
    string m_strPassword;
    string m_strDB;

    bool m_bConnected;
    MYSQL* m_con;
};