#include "obuudp.h"

#include <unistd.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <QDebug>
#include <QJsonObject>

#include "msg.h"


using namespace std;

static const int kPort = 30000;


ObuUdp::ObuUdp(QString ip)
{
    qRegisterMetaType<UiGpsData>("UiGpsData");
    m_ip = ip;
}

ObuUdp::~ObuUdp()
{
    m_ready = false;
    if(m_fd != -1){
        close(m_fd);
        m_fd = -1;
    }
}

bool ObuUdp::start(QString &err)
{
    m_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_fd <= 0){
        err = QString("create udp failed,%1").arg(strerror(errno));
        return m_ready;
    }
    // 绑定地址,用于接收消息
    struct sockaddr_in this_addr;
    bzero(&this_addr, sizeof(struct sockaddr_in));
    this_addr.sin_family = AF_INET;
    this_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 自动分配空闲端口
    this_addr.sin_port = 0;
    if(bind(m_fd,(struct sockaddr *)&(this_addr), sizeof(struct sockaddr_in)) == -1){
        err =QString("bind error,%1").arg(strerror(errno));
        return m_ready;
    }
    m_ready = true;

    std::thread *t1 = new std::thread(&ObuUdp::readThread,this);
    std::thread *t2 = new std::thread(&ObuUdp::heartThread,this);
    t1->detach();
    t2->detach();
    return m_ready;
}

void ObuUdp::readThread()
{
    uint8_t gps_pre[]       = {'g','p','s','0'};
    uint8_t buffer[10240]   = {0};
    struct sockaddr_in from;
    int struct_len = sizeof(struct sockaddr_in);
    int gps_pre_len         = sizeof (gps_pre);

    while (m_ready) {
        int recv_len = recvfrom(m_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&from,(socklen_t*)&struct_len);
        if(recv_len < 0) break;
        if(recv_len == 0)continue;
        if( (recv_len > gps_pre_len) && (memcmp(buffer,gps_pre,gps_pre_len) == 0) ){
            this->gpsParse(buffer,recv_len);
        }else{
            this->asnParse(buffer,recv_len);
        }
    }
}

void ObuUdp::heartThread()
{
    const char *str = static_cast<const char *>("hi");
    QByteArray ba = m_ip.toLatin1();
    char *ip = ba.data();

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(kPort);

    while (m_ready) {
        sendto(m_fd, str, strlen(str), 0, (struct sockaddr *)&addr,sizeof(addr));
        usleep(2000000);
    }
}

void ObuUdp::gpsParse(uint8_t *buffer, int len)
{
    if(len >= sizeof(UiGpsData)){
        emit toUi(*((UiGpsData*)buffer));
    }
}

void ObuUdp::asnParse(uint8_t *buffer, int len)
{
    qDebug() << "asn -------" << len;
    // 写入文件
    timeval tv;
    QJsonObject json;

    gettimeofday(&tv,nullptr);
    msgDecode(buffer,len,m_ip,tv,json,true);

//    timeval tv;
//    gettimeofday(&tv,nullptr);
//    string path = to_string(tv.tv_sec) + "_" + to_string(tv.tv_usec) + ".log";
//    ofstream outFile(path, ios::out | ios::binary);
//    outFile.write((char*)buffer,len);
//    outFile.close();
    if(!json.isEmpty()){
        emit toWeb(json);
    }
}

