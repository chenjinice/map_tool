#include <QThread>
#include <QUdpSocket>
#include <QTimer>
#include <QDebug>
#include "udpthread.h"
#include "common/setting.h"
#include <WinSock2.h>


UdpThread::UdpThread()
{
    qRegisterMetaType<UiData>("UiData");
    m_thread    = new QThread;
    this->moveToThread(m_thread);

    connect(m_thread,&QThread::started,this,&UdpThread::threadRun);
    connect(m_thread,&QThread::finished,this,&UdpThread::threadFinish);
}

void UdpThread::startThread(QString &ip)
{
    if(!m_thread->isRunning()){
        m_ip = ip;
        m_thread->start();
    }
}

UdpThread::~UdpThread()
{
    m_thread->quit();
    m_thread->wait();
    delete m_thread;
}

void UdpThread::setIp(QString ip)
{
    if(m_udp == nullptr)return;
    Setting::ins()->setCard(ip);
    m_ip = ip;
    this->closeUdp();
    this->bindUdp();
}

void UdpThread::threadRun()
{
    m_udp   = new QUdpSocket;
    m_timer = new QTimer;
    m_timer->setInterval(C_UDPTIME);

    connect(m_udp  ,&QUdpSocket::readyRead, this, &UdpThread::readData);
    connect(m_timer,&QTimer::timeout      , this, &UdpThread::bindUdp);
    m_timer->start();
    this->bindUdp();
}

void UdpThread::threadFinish()
{
    if(m_timer){
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
    if(m_udp){
        this->closeUdp();
        delete m_udp;
        m_udp = nullptr;
    }
}

void UdpThread::bindUdp()
{
    if(!m_ready){
        m_ready = m_udp->bind(QHostAddress(m_ip), C_UDPPORT);
        emit bindState(m_ready);
    }
}

void UdpThread::closeUdp()
{
    m_ready = false;
    m_udp->close();
}

void UdpThread::readData()
{
    QByteArray array;
    QHostAddress address;
    int len = static_cast<int>(m_udp->pendingDatagramSize());
    array.resize(len);
    m_udp->readDatagram(array.data(), array.size(),&address);

    int need_len = sizeof (PadCarData);
    if(len < need_len)return;

    PadCarData state = {0};
    memcpy(&state,array.data(),need_len);
    uint8_t *p = (uint8_t *)array.data();

    if(state.head != 0x01)return;
    if(ntohl(state.id) != 1952999795)return;

    UiData ui_data;
    ui_data.lng = ntohl(state.lng)*RES_POS;
    ui_data.lat = ntohl(state.lat)*RES_POS;
    ui_data.heading = ntohs(state.heading)*RES_HEADING;
    ui_data.speed = ntohs(state.speed);
    ui_data.satellite = state.num_st;
    ui_data.accuracy = ntohs(state.accuracy_hz)*RES_ACCURACY;
    emit toUi(ui_data);
}


