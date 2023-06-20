#pragma once

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#define HEAD_BUFFER_SIZE 8      // 头部
#define PROTOCOL_ID_SIZE 1      // 协议
#define ACCOUNT_SIZE 2          // 账户
#define DATA_TYPE_SIZE 1        // 数据类型
#define DATA_SIZE 4             // 数据大小

using namespace std;

// 消息头类，包含协议号、账号信息、数据类型和数据长度等信息。
class HeadData
{
private:
    // 将指定长度的字符转换（解析）为整数
    unsigned int parseInt(int len);
    // 从缓冲区解析消息头。
    bool baseParse();

public:
    HeadData();
    HeadData(int fd);
    ~HeadData();

    // 从给定的缓冲区解析消息头
    bool parse(const char* buffer);

    unsigned int getProtocolId();
    unsigned int getAccount();
    unsigned int getDataType();
    unsigned int getDataLength();

private:
    unsigned int protocolId;        // 协议号 1B
    unsigned int account;           // 账号 2B
    unsigned int dataType;          // 数据类型 1B
    unsigned int dataLength;        // 数据长度 4B
    char buffer[HEAD_BUFFER_SIZE];  // 存储消息头 8B = 1 + 2 + 1 + 4（B）
    const char* bp;                 // 指向buffer（用于遍历）
};