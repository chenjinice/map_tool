#ifndef OBUUDP_H
#define OBUUDP_H


#include <QObject>
#include <QString>


const double kObuPosRes         = 1e-7;
const double kObuHeadingRes     = 0.01;
const double kObuAccRes         = 0.01;
const double kObuSpeedRes       = 1e-2;
const double kObuElevRes        = 0.1;
const double kObuHdopRes        = 0.1;


#pragma pack(1)
typedef struct{
    uint8_t head[4]             = {'g','p','s','0'};
    int32_t lng                 = 0;
    int32_t lat                 = 0;
    int32_t elev                = 0;
    int32_t speed               = 0;
    int32_t heading             = 0;
    int32_t num_sat             = 0;
    int32_t hdop                = 0;
    int32_t model               = 0;
}UiGpsData;
#pragma pack()


class ObuUdp : public QObject
{
    Q_OBJECT
public:
    ObuUdp(QString ip);
    ~ObuUdp();

    bool start(QString &err);

private:
    void readThread();
    void heartThread();
    void gpsParse(uint8_t *buffer,int len);
    void asnParse(uint8_t *buffer,int len);

    bool    m_ready         = false;
    int     m_fd            = -1;
    QString m_ip;

signals:
    void log(QString str);
    void toUi(const UiGpsData   &data);
    void toWeb(const QJsonObject &json);
};

#endif // OBUUDP_H

