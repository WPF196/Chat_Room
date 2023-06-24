#pragma once

#include "../util/MySqlConn.h"

using namespace std;

class UserService
{
private:
    // 合格检验
    bool checkAccount(string &account);
    bool checkPassword(string &password);

public:
    UserService();      // 初始化mysqlconn，即获取实例
    // 通过账户和密码查询用户信息，返回 <账户、用户名>
    pair<int, string> checkLogin(string account, string passwd);   
    bool isRegistered(string account);      // 是否已注册过
    bool registerUser(string account, string username, string password); // 注册用户

private:
    MySqlConn* mysqlconn;   // 数据库实例
};
