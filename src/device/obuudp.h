#ifndef OBUUDP_H
#define OBUUDP_H


#include <QObject>
#include <QString>

class ObuUdp : public QObject
{
    Q_OBJECT
public:
    ObuUdp(QString ip);
    ~ObuUdp();


private:
    void readThread();
    void heartThread();

    bool    m_ready         = false;
    int     m_fd            = -1;
    QString m_ip;


signals:
    void err(QString str);
    void log(QString str);
};

#endif // OBUUDP_H

