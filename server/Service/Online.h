#pragma once

#include <string>
#include <json/json.h>
#include <unordered_map>
#include <utility>

#include "../util/MyTime.h"

using namespace std;

// 在线列表
class Online
{
public:
    string getOnlineListStr();  // 获取在线用户列表字符串
    void appendUser(int account, string username);  // 添加用户
    void appendUser(pair<int, string> &user);       // 添加用户（重载）
    void removeUser(int account);               // 删除用户
    int getReadFd(int writeFd);                 // 获取读文件的描述符
    vector<int> getAllReadFd();                 // 获取所有读文件的描述符
    void appendWriteFd(int account, int fd);    // 添加写文件描述符
    void appendReadFd(int account, int fd);     // 添加读文件描述符
    string getUserJsonStr(int account);         // 获取用户的json字符串
    string getUserName(int account);            // 根据账号获取用户名
    bool isLogin(int account);                  // 根据账号获取用户登录状态

private:
    struct user{
        int account;
        string username;
        string loginTime;
    };
    unordered_map<int, int> writeFdToReadFd; // write -> read
    unordered_map<int, int> accountToFd;     // account -> write
    unordered_map<int, user> userMap;        // account -> user 
};