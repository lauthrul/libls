// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "db.h"

void test1()
{
    try
    {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;
        sql::PreparedStatement* pstmt;

        driver = get_driver_instance();
        con = driver->connect("tcp://192.168.169.215:3306", "suser", "cyberuser");
        con->setSchema("autoservice");

        printf("service_merchant:\n--------------------------------------\n");
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM service_merchant");
        while (res->next())
        {
            printf("%d | %s | %s | %d | %s | %s\n",
                res->getInt("id"),
                res->getString("code").c_str(),
                crypto::encoding_convert(res->getString("name").c_str(), "utf-8", "gb2312").c_str(),
                res->getBoolean("state"),
                res->getString("add_time").c_str(),
                res->getString("update_time").c_str());
        }

        printf("service_line:\n--------------------------------------\n");
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM service_line");
        while (res->next())
        {
            printf("%d | %d | %s | %s | %d | %s | %s | %d\n",
                res->getInt("id"),
                res->getInt("type"),
                res->getString("url").c_str(),
                res->getString("ip").c_str(),
                res->getBoolean("state"),
                res->getString("add_time").c_str(),
                res->getString("update_time").c_str(),
                res->getInt("merchant_id"));
        }

        delete res;
        delete stmt;
        delete con;
    }
    catch (sql::SQLException& e)
    {
        printf("exception[%s]: %d - %s\n", e.what(), e.getErrorCode(), e.getSQLState());
    }
}

void test2()
{
    CDB db("tcp://192.168.169.215:3306", "suser", "cyberuser");
    if (db.Connect())
    {
        db.UseDB("autoservice");

        printf("service_merchant:\n--------------------------------------\n");
        sql::ResultSet* res = db.ExcuteQuery("SELECT * FROM service_merchant");
        while (res->next())
        {
            printf("%d | %s | %s | %d | %s | %s\n",
                res->getInt("id"),
                res->getString("code").c_str(),
                crypto::encoding_convert(res->getString("name").c_str(), "utf-8", "gb2312").c_str(),
                res->getBoolean("state"),
                res->getString("add_time").c_str(),
                res->getString("update_time").c_str());
        }
        delete res;

        printf("service_line:\n--------------------------------------\n");
        res = db.ExcuteQuery("SELECT * FROM service_line");
        while (res->next())
        {
            printf("%d | %d | %s | %s | %d | %s | %s | %d\n",
                res->getInt("id"),
                res->getInt("type"),
                res->getString("url").c_str(),
                res->getString("ip").c_str(),
                res->getBoolean("state"),
                res->getString("add_time").c_str(),
                res->getString("update_time").c_str(),
                res->getInt("merchant_id"));
        }
        delete res;
    }
}

// int main(int argc, char* argv[])
// {
//     test2();
//     return EXIT_SUCCESS;
// }
