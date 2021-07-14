#include <QVBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include <QSplitter>
#include "obuwindow.h"
#include "webview.h"
#include "common_defines.h"
#include "logdock.h"


ObuWindow::ObuWindow(QWidget *parent) : SubWindow(parent)
{
    QSplitter *spliter      = new QSplitter(Qt::Vertical);

    QWidget *state_widget   = this->addStateWidget();

    spliter->addWidget(state_widget);
    this->layout()->addWidget(spliter);

    ZmqCallBack fun         = ZmqBindClassFun(&ObuWindow::zmqDataReceived,this);
    m_zmq                   = new MyZmq();

    connect(m_zmq,&MyZmq::log,LogDock::ins(),&LogDock::log);


    m_zmq->startSubscriber("tcp://10.9.43.99:3411","location",fun);

}


ObuWindow::~ObuWindow()
{
    m_zmq->stop();
    delete m_zmq;
}

QWidget *ObuWindow::addStateWidget()
{
    QGridLayout *grid        = new QGridLayout;
    QWidget *       w        = new QWidget;

    QLabel *     pos_l       = new QLabel("位置:");
    QLabel *     heading_l   = new QLabel("航向:");
    QLabel *     speed_l     = new QLabel("速度:");
    QLabel *     satellite_l = new QLabel("卫星:");
    QLabel *     hdop_l      = new QLabel("精度:");
    QLabel *     model_l     = new QLabel("定位模式:");
    QLabel *     ip_l        = new QLabel("ip:");

    m_pos                    = new QLabel;
    m_heading                = new QLabel;
    m_speed                  = new QLabel;
    m_satellite              = new QLabel;
    m_hdop                   = new QLabel;
    m_model                  = new QLabel;
    m_ip                     = new QLineEdit;

    m_ip->setMaximumWidth(100);

    int  row = 0, index = 0;
    grid->addWidget(pos_l,row,index++);
    grid->addWidget(m_pos,row,index++);
    grid->addWidget(heading_l,row,index++);
    grid->addWidget(m_heading,row,index++);
    grid->addWidget(speed_l,row,index++);
    grid->addWidget(m_speed,row,index++);
    grid->addWidget(satellite_l,row,index++);
    grid->addWidget(m_satellite,row,index++);
    grid->addWidget(hdop_l,row,index++);
    grid->addWidget(m_hdop,row,index++);
    grid->addWidget(model_l,row,index++);
    grid->addWidget(m_model,row,index++);
    grid->addWidget(ip_l,row,index++);
    grid->addWidget(m_ip,row,index++);

    w->setMaximumHeight(40);
    w->setLayout(grid);
    return w;
}

void ObuWindow::zmqDataReceived(uint8_t *buffer, int len)
{
    qDebug() << len ;
}
