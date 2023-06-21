#pragma once

#include <string>

#include "protocolmsg.h"

using namespace std;

// 头部生成类，根据传入参数生成头部
class DataEncoder
{
public:
    DataEncoder();
    ~DataEncoder();

private:
    // 将无符号整数data编码为len字节，存于head
    void encodeElement(unsigned int data, unsigned int len);

public:
    // 将传入的用户信息转为string
    string encode(unsigned int protocolId, unsigned int account, 
                unsigned int dataType, unsigned int dataLength);

private:
    char head[HEAD_BUFFER_SIZE];
    char* hp;       // 指向head
};