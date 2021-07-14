#ifndef OBUWINDOW_H
#define OBUWINDOW_H


#include <QLabel>
#include <QLineEdit>
#include "subwindow.h"
#include "my_zmq.h"


class ObuWindow : public SubWindow
{
    Q_OBJECT
public:
    ObuWindow(QWidget *parent = nullptr);
    ~ObuWindow();

private:
    QWidget *       addStateWidget();

    void            zmqDataReceived(uint8_t *buffer,int len);

    QLabel *        m_pos       = nullptr;
    QLabel *        m_heading   = nullptr;
    QLabel *        m_speed     = nullptr;
    QLabel *        m_satellite = nullptr;
    QLabel *        m_hdop      = nullptr;
    QLabel *        m_model     = nullptr;
    QLineEdit *     m_ip        = nullptr;

    MyZmq  *        m_zmq       = nullptr;

};

#endif // LDMWINDOW_H
