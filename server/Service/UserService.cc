#include "UserService.h"

bool UserService::checkAccount(string &account)
{
    // 账号3-6位
    if(account.length() > 6 || account.length() < 3){
        cout << "账号位数长度请控制在(3-6)位" << endl;
        return false;
    }
    // 账号纯数字
    for(char& c : account){
        if(c < '0' || c > '9'){
            cout << "账号只能是数字形式" << endl;
            return false;
        }
    }
    // 账号控制在(100 ~ 65535)范围内
    if(stoi(account) > 65535){      // string -> int
        cout << "账号范围 (100, 65535) " << endl;
        return false;
    }
    return true;
}

bool UserService::checkPassword(string &password)
{
    // 密码长度6-16位
    if(password.length() < 6 || password.length() > 16){
        cout << "密码长度请控制在(6-16)位" << endl;
    }
    // 密码只包含数字和字母
    for(char& c : password){
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
            cout << "密码有误" << endl;
            return false;
        }
    }
    return true;;
}

UserService::UserService()
{
    mysqlconn = MySqlConn::getInstance();
}

pair<int, string> UserService::checkLogin(string account, string passwd)
{
    pair<int, string> user;
    if(checkAccount(account) && checkPassword(passwd))
        user = mysqlconn->queryUser(account, passwd);
    return user;
}

bool UserService::isRegistered(string account)
{
    // 密码不合格，肯定未注册
    if(!checkAccount(account))
        return true;
    return mysqlconn->queryUser(account);
}

bool UserService::registerUser(string account, string username, string password)
{
    return checkAccount(account) 
           && checkPassword(password) 
           && mysqlconn->insertUser(account, username, password);
}