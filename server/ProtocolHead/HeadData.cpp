#include "HeadData.h"

unsigned int HeadData::parseInt(int len)
{
    unsigned int sum = 0;
    unsigned int i = 0;
    // 解析
    for(const char* end = bp + len - 1; bp <= end; --end){
        sum = sum + (((unsigned int) ((unsigned char) (*end))) << i);
        i += 8; 
    }
    bp = bp + len;
    return sum;
}

bool HeadData::baseParse()
{
    // 顺序解析头部所有信息
    this->protocolId = parseInt(PROTOCOL_ID_SIZE);
    this->account = parseInt(ACCOUNT_SIZE);
    this->dataType = parseInt(DATA_TYPE_SIZE);
    this->dataLength = parseInt(DATA_SIZE);
}

HeadData::HeadData()
{}

HeadData::HeadData(int fd)
{
    read(fd, buffer, HEAD_BUFFER_SIZE);
    bp = buffer;
    baseParse();
}
HeadData::~HeadData()
{}

bool HeadData::parse(const char* buffer)
{
    bp = buffer;
    baseParse();
}

unsigned int HeadData::getProtocolId()
{
    return this->protocolId;
}
unsigned int HeadData::getAccount()
{
    return this->account;
}
unsigned int HeadData::getDataType()
{
    return this->dataType;
}
unsigned int HeadData::getDataLength()
{
    return this->dataLength;
}