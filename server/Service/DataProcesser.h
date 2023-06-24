#pragma once

#include<iostream>
#include<string>
#include<unistd.h>
#include<vector>
#include<fstream>
#include<arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include"../ProtocolHead/protocolmsg.h"
#include"../util/MyTime.h"
#include"../ProtocolHead/HeadData.h"
#include "../ProtocolHead/DataEncoder.h"

#define IMAGE_PATH "./image/"
#define TCP_BUFSIZE 8192

using namespace std;

// 数据处理类
class DataProcesser
{
private:
    // 查看是否已连接
    int checkSocketConnected(int sock); 

public:
    DataProcesser();
    // 读取内容并返回
    string readTextContent(int fd, unsigned int dataLength);
    // 读取图片并返回图片保存路径
    string readImageContent(int fd, unsigned int dataLength);
    // 向fd写一个文本
    void writeText(int fd, unsigned int account, string text, 
                unsigned int protocolId = SEND);
    // 向fd写一张图片
    void writeImage(int fd, unsigned int account, const string &imagePath);
    // 向fd写一个短信
    void writeMsg(int fd, unsigned int account, string text, 
                unsigned int protocolId = SEND);
    // 向所有用于写文本
    void writeTextToAllUser(const vector<int> &fds, int account, 
                            const string &text, unsigned int protocolId = SEND);
    // 向所有用户写图片
    void writeImageToAllUser(const vector<int> &fds, int account, 
                            const string &imagePath);
    // 获取文件大小
    int getFileLength(const string &fileName);

private:
    char buffer[TCP_BUFSIZE];
};