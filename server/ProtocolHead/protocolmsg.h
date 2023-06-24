#pragma once

// 协议宏 单位B
// 消息头部
#define HEAD_BUFFER_SIZE 8      // 头部
#define PROTOCOL_ID_SIZE 1      // 协议
#define ACCOUNT_SIZE 2          // 账户
#define DATA_TYPE_SIZE 1        // 数据类型
#define DATA_SIZE 4             // 数据大小

// TCP
#define LOGIN 1
#define SEND 2
#define READ 3
#define NOTICE 4
#define ACK 5
#define LOGOUT 6
#define REGISTER 7
#define ONLINELIST 8
#define CLOSE 9
#define TEXT 1                  // 文本
#define IMAGE 2                 // 图片
#define LOGIN_SUCCESS 0
#define LOGIN_FAIL 1
#define LOGIN_EXIST 2
#define REGISTER_SUCCESS 0
#define REGISTER_FAIL 1