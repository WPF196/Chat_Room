/**
 * 数据库格式：
 * 账户  用户名  密码  创建时间
*/
#include <iostream>
#include <string>
#include <mysql/mysql.h>
#include <unordered_map>
#include <json/json.h>
#include "./MyTime.h"
#include "../config/mysql_config.h"

using namespace std;

class MySqlConn
{
public:
    static MySqlConn* getInstance();
    // 具体信息查询，返回用户的 <用户账户，用户名>
    pair<int, string> queryUser(const string& account, const string& password);
    // 用户账户查询，返回用户受否存在
    bool queryUser(const string& account);
    // 用户注册
    bool insertUser(const string& account, const string& username, 
                    const string& password);

private:
    MySqlConn();
    ~MySqlConn();
    void init();
    MYSQL* conn;
    static MySqlConn* mysqlconn;
};