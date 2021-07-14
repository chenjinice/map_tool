/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */


#include <QDebug>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include "zmq.h"
#include "my_zmq.h"


//#pragma comment(lib, "libzmq.lib")

#define CLASS               "MyZmq : "


static const int kSubBufSize = 8192;


MyZmq::MyZmq()
{
    m_context = zmq_ctx_new();
    m_fd      = zmq_socket(m_context,ZMQ_SUB);
}

MyZmq::~MyZmq()
{
    m_ready             = false;
    zmq_close(m_fd);
    m_fd = nullptr;
    zmq_ctx_destroy(m_context);
    m_context       = nullptr;
}

// 订阅模式 , ipc:///tmp/example.ipc ,  tcp://192.168.1.10:12345
void MyZmq::startSubscriber(string addr, string filter, ZmqCallBack callback)
{
    if(m_ready){emit log(CLASS"already start");return;}
    int ret;

    m_filter           = filter;
    m_callback         = callback;
    emit log(QString(CLASS"sub = %1,filter = %2").arg(addr.c_str()).arg(m_filter.c_str()) );
    ret = zmq_connect(m_fd,addr.c_str());
    if(ret != 0){
        emit log(CLASS"connect error,%s");
        exit(1);
    }
    zmq_setsockopt(m_fd,ZMQ_SUBSCRIBE,m_filter.c_str(),m_filter.length());
    std::thread tmp_thread(&MyZmq::run,this);
    tmp_thread.detach();
}

void MyZmq::stop()
{
    zmq_close(m_fd);
}

// 订阅模式的线程里边，阻塞接收数据
void MyZmq::run()
{
    if(!m_callback)return;

    m_ready = true;
    int len;
    uint8_t filter[100];
    uint8_t buffer[kSubBufSize];
    while(true){
        if(m_filter.length() > 0)zmq_recv(m_fd,filter,sizeof(filter),0);
        len = zmq_recv(m_fd,buffer,kSubBufSize,0);
        if(len == -1){
            emit log(QString(CLASS"recv failed(%1)").arg(len));
            break;
        }
        if(len > kSubBufSize){
            emit log(QString(CLASS"recv len(%1) > buffer len(%2)").arg(len).arg(kSubBufSize)) ;
            continue;
        }
        m_callback(buffer,len);
    }
    m_ready = false;
}

