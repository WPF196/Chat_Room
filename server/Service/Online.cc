#include "Online.h"

string Online::getOnlineListStr()
{
    Json::Value onlineList;     // json表，存储所有用户信息
    for(auto& item : userMap){
        Json::Value userJson;
        userJson["account"] = item.second.account;
        userJson["username"] = item.second.username;
        userJson["loginTime"] = item.second.loginTime;
        onlineList.append(userJson);
    }
    return onlineList.toStyledString(); // 返回用户信息的Json列表
}

void Online::appendUser(int account, string username)
{
    user u = {account, move(username), MyTime::getCurrentFormatTimeStr()};
    userMap[account] = u;
}

void Online::appendUser(pair<int, string> &user)
{
    return appendUser(user.first, user.second);
}

void Online::removeUser(int account)
{
    userMap.erase(account);         // 从用户列表中删除用户
    writeFdToReadFd.erase(accountToFd[account]);    // 删除readfd
    accountToFd.erase(account);     // 删除writefd
}

int Online::getReadFd(int writeFd)
{
    return writeFdToReadFd[writeFd];
}

vector<int> Online::getAllReadFd()
{
    vector<int> v;
    for (auto &item : writeFdToReadFd) {
        v.push_back(item.second);
    }
    return v;
}  

void Online::appendWriteFd(int account, int fd)
{
    accountToFd[account] = fd;
}

void Online::appendReadFd(int account, int fd)
{
    writeFdToReadFd[accountToFd[account]] = fd;
}

string Online::getUserJsonStr(int account)
{
    Json::Value jsonUser;
    jsonUser["account"] = account;
    jsonUser["username"] = userMap[account].username;
    return jsonUser.toStyledString();
}

string Online::getUserName(int account)
{
    return userMap[account].username;
}

bool Online::isLogin(int account)
{
    return userMap.count(account) != 0;
}