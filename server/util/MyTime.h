#pragma once

#include <iostream>
#include <string>
#include <sys/time.h>

using namespace std;

class MyTime
{
public:
    // 按照指定格式输出时间
    static string getCurrentFormatTimeStr();
    // 获取时间戳（精确到毫秒）
    static string getTimeStampStr();
};