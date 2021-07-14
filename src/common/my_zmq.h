/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#ifndef __MY_ZMQ_H__
#define __MY_ZMQ_H__


#include <QObject>
#include <iostream>
#include <mutex>
#include <functional>


// 回调函数
typedef                          std::function<void(uint8_t *buffer,int len)> ZmqCallBack;
// 绑定普通函数(两个参数)
#define ZmqBindFun(FUN)          {std::bind(FUN,  std::placeholders::_1,std::placeholders::_2)}
// 绑定类成员函数(两个参数)
#define ZmqBindClassFun(FUN,P)   {std::bind(FUN,P,std::placeholders::_1,std::placeholders::_2)}


using namespace std;


class MyZmq : public QObject
{
    Q_OBJECT
public:
    MyZmq();
    ~MyZmq();

    void               startSubscriber(string addr, string filter, ZmqCallBack callback);
    void               stop();

private:
    void               run();

    string             m_filter         = "";
    void *             m_context        = nullptr;
    void *             m_fd             = nullptr;
    bool               m_ready          = false;
    std::mutex         m_mutex;
    ZmqCallBack        m_callback       = nullptr;

signals:
    void               log(QString str);

};


#endif
