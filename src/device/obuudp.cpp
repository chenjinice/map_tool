#include "obuudp.h"

#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>

#include <iostream>
#include <thread>
#include <QDebug>

using namespace std;

static const int kPort = 30000;


ObuUdp::ObuUdp(QString ip)
{
    m_ip = ip;
    qDebug() << m_ip;
    m_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_fd <= 0){
        emit err(QString("ObuUdp : create udp failed,%s").arg(strerror(errno)));
        return;
    }
    // 绑定地址,用于接收消息
    struct sockaddr_in this_addr;
    bzero(&this_addr, sizeof(struct sockaddr_in));
    this_addr.sin_family = AF_INET;
    this_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 自动分配空闲端口
    this_addr.sin_port = htons(11110);
    if(bind(m_fd,(struct sockaddr *)&(this_addr), sizeof(struct sockaddr_in)) == -1){
        emit err("ObuUdp : bind error");
        return;
    }

    m_ready = true;
    std::thread t1(&ObuUdp::readThread,this);
    std::thread t2(&ObuUdp::heartThread,this);
    t1.detach();
    t2.detach();
}

ObuUdp::~ObuUdp()
{
    m_ready = false;
    if(m_fd != -1){
        close(m_fd);
        m_fd = -1;
    }
}

void ObuUdp::readThread()
{
    while (m_ready) {
        uint8_t buffer[30] = {0};
        struct sockaddr_in from;
        int len = sizeof(struct sockaddr_in);

        while (true) {
            int ret = recvfrom(m_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&from,(socklen_t*)&len);
            if(ret > 0){
                std::cout << "ret ======== "<< ret << std::endl;
            }
        }
    }
}

void ObuUdp::heartThread()
{
    char *str = (char *)"hi";

//    char *ip;
//    QByteArray ba = m_ip.toLatin1(); // must
//    ip = ba.data();

//    qDebug() << m_ip ;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(kPort);

    while (m_ready) {
        std::cout << "11111111111111111111111111" << std::endl;
        printf("22222222222222\n");
        sendto(m_fd, str, strlen(str), 0, (struct sockaddr *)&addr,sizeof(addr));
        usleep(1000000);
    }
}

