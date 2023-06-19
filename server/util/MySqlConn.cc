#include "MySqlConn.h"

MySqlConn* MySqlConn::getInstance()
{
    if(mysqlconn == nullptr){
        mysqlconn = new MySqlConn;
        mysqlconn->init();
    }
    return mysqlconn;
}

pair<int, string> MySqlConn::queryUser(const string& account, 
                                    const string& password)
{
    string querySql = 
        "select account, username from user where account = " 
                + account + " and password = " 
                + "\"" + password + "\"";
    pair<int, string> user;
    int res = mysql_query(conn, querySql.data());
    if(!res){   // 如果查询到了
        MYSQL_RES* result = mysql_store_result(conn);
        if(result && mysql_num_rows(result)){
            MYSQL_ROW row = mysql_fetch_row(result);
            user.first = atoi(row[0]);
            user.second = row[1];
        }
        mysql_free_result(result);
    }
    return user;
}

bool MySqlConn::queryUser(const string& account)
{
    string querySql = "select account from user where account = " + account;
    int res = mysql_query(conn, querySql.data());
    bool flag = false;
    if(!res){
        MYSQL_RES* result = mysql_store_result(conn);
        if(result && mysql_num_rows(result))
            flag = true;
        mysql_free_result(result);
    }
    return flag;
}

bool MySqlConn::insertUser(const string& account, const string& username, 
                    const string& password)
{
    string insertSql = "insert into `user` values(" +
                       account + ","
                                 "\"" + username + "\","
                       + "\"" + password + "\","
                       + "\"" + MyTime::getCurrentFormatTimeStr() + "\")";
    int res = mysql_query(conn, insertSql.data());
    return res == 0;                
}

MySqlConn::MySqlConn()
{}

MySqlConn::~MySqlConn()
{
    mysql_close(conn);
    delete mysqlconn;
}

void MySqlConn::init()
{
    conn = mysql_init(NULL);
    conn = mysql_real_connect(conn, HOST, USERNAME, PASSWORD,
                             DATABASE, PORT, NULL, 0);
    if(!conn){
        cout << "mysql_real_connect() failed" << endl;
        exit(-1);
    }
    mysql_query(conn, "set names utf8");    // 设置编码格式
}

MySqlConn *MySqlConn::mysqlconn = nullptr;