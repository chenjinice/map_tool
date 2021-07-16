#ifndef OBUUDP_H
#define OBUUDP_H

#include <QObject>

class ObuUdp : public QObject
{
    Q_OBJECT
public:
    ObuUdp(QString ip);


private:
    int m_fd            = -1;


signals:
    void err(QString str);
    void log(QString str);
};

#endif // OBUUDP_H
