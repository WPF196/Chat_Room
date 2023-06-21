#pragma once

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#include "protocolmsg.h"

using namespace std;

// 消息头类 = 协议号 + 账号 + 信息数据类型 + 数据长度
class HeadData
{
private:
    // 将指定长度的字符转换（解析）为整数
    unsigned int parseInt(int len);
    // 从缓冲区解析消息头。
    void baseParse();

public:
    HeadData();
    HeadData(int fd);
    ~HeadData();

    // 从给定的缓冲区解析消息头
    void parse(const char* buffer);

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