#include "DataEncoder.h"

DataEncoder::DataEncoder()
{
    hp = head;
}

DataEncoder::~DataEncoder()
{}

void DataEncoder::encodeElement(unsigned int data, unsigned int len)
{
    // c从后往前写入hp
    char* c = hp + len - 1;
    for(int i = len; i > 0; --i){   // 遍历每一个字节
        *c = (char)(data & 0xff);   // 取data的低8位写入c指向的位置
        c--;
        data >>= 8;     // data右移8位
    }
    hp = hp + len;
}

string DataEncoder::encode(unsigned int protocolId, unsigned int account, 
                unsigned int dataType, unsigned int dataLength)
{
    encodeElement(protocolId, PROTOCOL_ID_SIZE);
    encodeElement(account, ACCOUNT_SIZE);
    encodeElement(dataType, DATA_TYPE_SIZE);
    encodeElement(dataLength, DATA_SIZE);
    return string(head, sizeof(head));
}

/*
#include <iostream>
using namespace std;

unsigned int protocolId = 65;      // A
unsigned int account = 65;         // A
unsigned int dataType = 65;        // A
unsigned int dataLength = 65;      // A

int main(){
    cout << "aaa\n";
    DataEncoder a;
    cout << a.encode(protocolId, account, dataType, dataLength) << endl;
    return 0;
}
*/