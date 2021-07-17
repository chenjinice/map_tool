#include <QVBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include <QSplitter>
#include <QUdpSocket>
#include "obuwindow.h"
#include "webview.h"
#include "common_defines.h"
#include "logdock.h"

ObuWindow::ObuWindow(QString ip, QWidget *parent) : SubWindow(parent)
{
    QSplitter *spliter      = new QSplitter(Qt::Vertical);

    QWidget *state_widget   = this->addStateWidget();

    spliter->addWidget(state_widget);
    this->layout()->addWidget(spliter);

    QString err;
    m_obu = new ObuUdp(ip);
    if(m_obu->start(err)){
        this->setWindowTitle(ip);
    }else{
        this->setWindowTitle(ip+" : "+err);
    }
    connect(m_obu,&ObuUdp::toUi,this,&ObuWindow::gpsReceived);
}


ObuWindow::~ObuWindow()
{
    disconnect(m_obu);
    delete m_obu;
}

QWidget *ObuWindow::addStateWidget()
{
    QHBoxLayout *h        = new QHBoxLayout;
    QWidget *       w        = new QWidget;

    QLabel *     pos_l       = new QLabel("位置:");
    QLabel *     heading_l   = new QLabel("航向:");
    QLabel *     speed_l     = new QLabel("速度:");
    QLabel *     satellite_l = new QLabel("卫星:");
    QLabel *     hdop_l      = new QLabel("精度:");
    QLabel *     model_l     = new QLabel("定位模式:");

    m_pos                    = new QLineEdit;   m_pos->setFixedWidth(240);        m_pos->setReadOnly(true);
    m_heading                = new QLineEdit;   m_heading->setFixedWidth(50);     m_heading->setReadOnly(true);
    m_speed                  = new QLineEdit;   m_speed->setFixedWidth(50);       m_speed->setReadOnly(true);
    m_satellite              = new QLineEdit;   m_satellite->setFixedWidth(50);   m_satellite->setReadOnly(true);
    m_hdop                   = new QLineEdit;   m_hdop->setFixedWidth(50);        m_hdop->setReadOnly(true);
    m_model                  = new QLineEdit;   m_model->setFixedWidth(50);       m_model->setReadOnly(true);

    h->addWidget(pos_l);
    h->addWidget(m_pos);
    h->addWidget(heading_l);
    h->addWidget(m_heading);
    h->addWidget(speed_l);
    h->addWidget(m_speed);
    h->addWidget(satellite_l);
    h->addWidget(m_satellite);
    h->addWidget(hdop_l);
    h->addWidget(m_hdop);
    h->addWidget(model_l);
    h->addWidget(m_model);
    h->addStretch(1);

    h->setAlignment(Qt::AlignLeft);
    w->setMaximumHeight(40);
    w->setLayout(h);
    return w;
}

void ObuWindow::gpsReceived(UiGpsData data)
{
    double  d_lng     = data.lng * kObuPosRes;
    double  d_lat     = data.lat * kObuPosRes;
    double  d_heading = data.heading * kObuHeadingRes;

    QString lng       = QString::number(d_lng,10,7);
    QString lat       = QString::number(d_lat,10,7);
    QString elev      = QString::number(data.elev * kObuElevRes,10,2);
    QString heading   = QString::number(d_heading,10,2);
    QString speed     = QString::number(data.speed * kObuSpeedRes,10,2);
    QString sate      = QString::number(data.num_sat);
    QString hdop      = QString::number(data.hdop * kObuHdopRes,10,2);
    QString model     = QString::number(data.model);

    m_pos->setText(lng+","+lat+","+elev);
    m_heading->setText(heading);
    m_speed->setText(speed);
    m_satellite->setText(sate);
    m_hdop->setText(hdop);
    m_model->setText(model);

    emit m_webview->obj()->carUpdate(d_lng,d_lat,d_heading);
}

