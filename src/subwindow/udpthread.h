#ifndef UDPTHREAD_H
#define UDPTHREAD_H

#include <QObject>
#include <QMutex>

class QThread;
class QUdpSocket;
class MainWindow;
class QTimer;

#define C_UDPPORT       8888
#define C_UDPTIME       3000
#define C_LOG_PATH      "log"
#define C_LINE_END      "\r\n"
#define RES_POS         1e-7
#define RES_HEADING     0.01
#define RES_ACCURACY    0.1


#pragma pack(1)
// 发送本车消息结构体
typedef struct{
    uint8_t     head;           // 协议类型
    uint32_t    id;             // 车辆Id
    int32_t     lat;            // 纬度
    int32_t     lng;            // 经度
    uint16_t    heading;        // 真实航向角×100
    uint16_t    speed;          // 车速 , km/h
    uint8_t     num_st;         // 使用卫星数量，从00到12
    uint16_t    accuracy_hz;    // 水平精确度，5到999.
}PadCarData;
#pragma pack()


class UiData
{
public:
    UiData(){
      lng       = 0;
      lat       = 0;
      heading   = 0;
      speed     = 0;
      satellite = 0;
      accuracy  = 0;
    }
    double     lng;
    double     lat;
    float      heading;
    float      speed;
    uint8_t    satellite;
    float      accuracy;
};


class UdpThread : public QObject
{
    Q_OBJECT
public:
    UdpThread();
    ~UdpThread();
    void startThread(QString &ip);

private:

    void threadRun();
    void threadFinish();
    void bindUdp();
    void closeUdp();
    void readData();

    bool               m_ready      = false;
    QString            m_ip         = "127.0.0.1";
    QThread          * m_thread     = nullptr;
    QUdpSocket       * m_udp        = nullptr;
    QTimer           * m_timer      = nullptr;
    QList<QString>     m_list;
    QMutex             m_mutex;

public slots:
    void setIp(QString ip);

signals:
    void bindState(bool flag);
    void toUi(UiData data);
};

#endif // UDPTHREAD_H
