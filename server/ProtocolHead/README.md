# 协议
## 编码协议
>* 协议号 1B 
>* 账号 2B 
>* 信息数据类型 1B 
>* 数据长度 4B

按照`8`位一组，，从`低位`开始，转为相应的ASCII码

---------------------------

## `protocolmsg`
定义了通信协议的一系列宏

---------------------------
## `HeadData` 协议头部类
主要作用：从消息中解析出头部信息，并存于相应的成员变量\
外部接口：
>* `HeadData(int fd); ---------------- 从套接字 fd 读取内容，并解析之
>* `parse`(const char* buffer) -------- 解析buffer中的数据
>* `getProtocolId`(); ------------------ 获取协议号
>* `getAccount`(); -------------------- 获取账户
>* `getDataType`(); ------------------- 获取数据类型
>* `getDataLength`(); ----------------- 获取数据大小

------------------------------
## `DataEncoder` 生成头部类
主要作用：根据传入的协议号、账户、数据类型、数据长度，编码成相应的头部\
外部接口：
>* `encode`(unsigned int protocolId, \
          unsigned int account, \
          unsigned int dataType, \
          unsigned int dataLength); ------ 将传入的头部信息进行编码