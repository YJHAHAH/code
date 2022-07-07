// encode UTF-8

// @Author        : yjh
// @Date          : 2022-06-23

#ifndef HTTP_CONNECTION_H
#define HTTP_CONNECTION_H

#include<arpa/inet.h> //sockaddr_in
#include<sys/uio.h> //readv/writev
#include<iostream>
#include<sys/types.h>
#include<assert.h>

#include "buffer.h"
#include "log.h"
#include "sqlconnRAII.h"
#include "HTTPrequest.h"
#include "HTTPresponse.h"
using namespace std;

class HTTPconnection{
public:
    HTTPconnection();
    ~HTTPconnection();

    void initHTTPConn(int socketFd,const sockaddr_in& addr);
    ssize_t readBuffer(int* saveErrno);
    ssize_t writeBuffer(int* saveErrno);
    void closeHTTPConn();
    bool handleHTTPConn();
    const char* getIP() const;
    int getPort() const;
    int getFd() const;
    sockaddr_in getAddr() const;

    int writeBytes()
    {
        return iov_[1].iov_len+iov_[0].iov_len;
    }

    bool isKeepAlive() const
    {
        return request_.isKeepAlive();
    }

    static bool isET;
    static const char* srcDir;
    static  atomic<int>userCount;

private:
    int fd_;                  //HTTP连接对应的描述符
    struct sockaddr_in addr_;
    bool isClose_;            //标记是否关闭连接

    int iovCnt_;
    struct iovec iov_[2];

    Buffer readBuffer_;       //读缓冲区
    Buffer writeBuffer_;      //写缓冲区

    HTTPrequest request_;    
    HTTPresponse response_;

};

#endif //HTTP_CONNECTION_H