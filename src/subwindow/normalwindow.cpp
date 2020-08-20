#include <QVBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QPushButton>
#include <QNetworkInterface>
#include <QComboBox>
#include <QDateTime>
#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QDir>
#include <QDialog>
#include <QApplication>
#include <QLineEdit>
#include <QDesktopWidget>
#include "normalwindow.h"
#include "webview.h"
#include "common_defines.h"
#include "setting.h"


NormalWindow::NormalWindow(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout      = new QVBoxLayout;
    QSplitter   *splitter    = new QSplitter(Qt::Horizontal);
    m_car_widget             = new QWidget;
    m_grid                   = new QGridLayout;

    this->addCarInfoWidget();
    this->addButtons();
    splitter->addWidget(m_car_widget);
    // 右边栏webview
    m_webview                = new WebView(HTML_INDEX);
    splitter->addWidget(m_webview);
    layout->setMargin(0);
    layout->addWidget(splitter);
    this->setLayout(layout);
    // udp , 定时器
    m_udp_thread             = new UdpThread;
    m_timer                  = new QTimer;
    connect(m_timer,&QTimer::timeout,this,&NormalWindow::getIpList);
    connect(m_combo,&QComboBox::currentTextChanged,m_udp_thread,&UdpThread::setIp);
    connect(m_udp_thread,&UdpThread::bindState,this,&NormalWindow::udpState);
    connect(m_udp_thread,&UdpThread::toUi,this,&NormalWindow::dataSlot);
    m_timer->start(3000);
}

NormalWindow::~NormalWindow()
{
    m_timer->stop();
    delete m_timer;
    delete m_udp_thread;
    delete m_webview;
    this->logEnd();
    qDebug() << "~~~~~~~~~~~~ normal window";
}

void NormalWindow::addCarInfoWidget()
{
    // 左边栏的车数据相关控件
    QLabel *     udp_l       = new QLabel("udp : ");
    m_state                  = new QLabel;
    QLabel *     lng_l       = new QLabel("经度 : ");
    QLabel *     lat_l       = new QLabel("纬度 : ");
    QLabel *     heading_l   = new QLabel("航向 : ");
    QLabel *     speed_l     = new QLabel("速度 : ");
    QLabel *     satellite_l = new QLabel("卫星 : ");
    QLabel *     gpod_l      = new QLabel("精度因子 : ");
    m_lng                    = new QLabel;
    m_lat                    = new QLabel;
    m_heading                = new QLabel;
    m_speed                  = new QLabel;
    m_satellite              = new QLabel;
    m_gpod                   = new QLabel;
    m_combo                  = new QComboBox;
    m_combo->setMinimumWidth(120);
    m_state->setFixedSize(15,15);
    m_state->setScaledContents(true);

    m_grid->addWidget(udp_l,m_index,0);
    m_grid->addWidget(m_combo,m_index,1);
    m_grid->addWidget(m_state,m_index++,2);
    m_grid->addWidget(lng_l,m_index,0);
    m_grid->addWidget(m_lng,m_index++,1);
    m_grid->addWidget(lat_l,m_index,0);
    m_grid->addWidget(m_lat,m_index++,1);
    m_grid->addWidget(heading_l,m_index,0);
    m_grid->addWidget(m_heading,m_index++,1);
    m_grid->addWidget(speed_l,m_index,0);
    m_grid->addWidget(m_speed,m_index++,1);
    m_grid->addWidget(satellite_l,m_index,0);
    m_grid->addWidget(m_satellite,m_index++,1);
    m_grid->addWidget(gpod_l,m_index,0);
    m_grid->addWidget(m_gpod,m_index++,1);
    m_car_widget->setLayout(m_grid);
}

void NormalWindow::addButtons()
{
    QPushButton *add_bt      = new QPushButton("中心加点");
    QPushButton *add_at_bt   = new QPushButton("加点");
    QPushButton *map_bt      = new QPushButton("国标地图json");
    QPushButton *path_bt     = new QPushButton("采点路径json");

    m_grid->addWidget(add_bt,m_index++,0,1,2);
    m_grid->addWidget(add_at_bt,m_index++,0,1,2);
    m_grid->addWidget(map_bt,m_index++,0,1,2);
    m_grid->addWidget(path_bt,m_index++,0,1,2);

    connect(add_bt,&QPushButton::clicked,this,&NormalWindow::addClicked);
    connect(add_at_bt,&QPushButton::clicked,this,&NormalWindow::addAtClicked);
    connect(map_bt,&QPushButton::clicked,this,&NormalWindow::openMapClicked);
    connect(path_bt,&QPushButton::clicked,this,&NormalWindow::openPathClicked);
}

void NormalWindow::getIpList()
{
    QStringList value;
    QList<QHostAddress> l = QNetworkInterface::allAddresses();
    for(int i = 0; i < l.size(); i++){
        if (l.at(i) != QHostAddress::LocalHost && l.at(i).toIPv4Address()) {
            value <<  l.at(i).toString();
        }
    }
    value.append("127.0.0.1");
    int combo_num = m_combo->count();
    for(int m = combo_num-1 ; m >= 0; m--){
       QString text = m_combo->itemText(m);
       if(value.indexOf(text) == -1){
           m_combo->removeItem(m);
       }
    }
    for(int i = 0; i < value.size(); i++){
        QString ip = value.at(i);
        if(m_combo->findText(ip) == -1)m_combo->addItem(ip);
        if( ip == Setting::ins()->getCard() ){
            m_combo->setCurrentText(ip);
        }
    }
    QString ip = m_combo->currentText();
    if(!ip.isEmpty()){
        m_udp_thread->startThread(ip);
    }
}

void NormalWindow::udpState(bool flag)
{
    static QPixmap yes(":/images/images/connect_yes.png");
    static QPixmap no(":/images/images/connect_no.png");
    if(flag)m_state->setPixmap(yes);
    else m_state->setPixmap(no);
}

void NormalWindow::dataSlot(UiData data)
{
    QString lng       = QString::number(data.lng,10,7);
    QString lat       = QString::number(data.lat,10,7);
    QString heading   = QString::number(data.heading,10,2);
    QString speed     = QString::number(data.speed);
    QString sate      = QString::number(data.satellite);
    QString accuracy  = QString::number(data.accuracy,10,2);
    m_lng->setText(lng);
    m_lat->setText(lat);
    m_heading->setText(heading);
    m_speed->setText(speed + " km/h");
    m_satellite->setText(sate);
    m_gpod->setText(accuracy);

    QString dirct = C_LOG_PATH;
    QDir dir;
    if(!dir.exists(dirct))dir.mkpath(dirct);
    if(m_log_file.isEmpty()){
        m_log_file = dirct + "/path_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")+".json";
    }
    QFile file(m_log_file);
    bool exist = file.exists();
    bool ret = file.open(QIODevice::Append);
    if(!ret)return;

    if(!exist){
        QString head_str = "{" + QString(C_LINE_END);
        head_str += "\"points\":[" + QString(C_LINE_END);
        file.write(head_str.toUtf8());
    }else{
        QString tmp = "," + QString(C_LINE_END);
        file.write(tmp.toUtf8());
    }
    QString log = "[" ;
    log += lng + ","  ;
    log += lat + ","  ;
    log += heading + ",";
    log += speed + ",";
    log += sate + ",";
    log += accuracy + ",";
    log += QDateTime::currentDateTime().toString("\"yyyy.MM.dd_hh:mm:ss\"]");
    file.write(log.toUtf8());
    file.close();
}

void NormalWindow::logEnd()
{
    if(m_log_file.isEmpty())return;
    QFile file(m_log_file);
    if(!file.exists())return;
    if(file.size() <= 0 )return;
    bool ret = file.open(QIODevice::Append);
    if(!ret)return;
    QString tmp = QString(C_LINE_END) + "]}" + QString(C_LINE_END);
    file.write(tmp.toUtf8());
}

void NormalWindow::addClicked()
{
    emit m_webview->obj()->addMarker();
}

void NormalWindow::addAtClicked()
{
    static QDialog *dialog = nullptr;
    static QLineEdit *lat_edit = new QLineEdit;
    static QLineEdit *lng_edit = new QLineEdit;
    static QDesktopWidget *deskdop = QApplication::desktop();

    double lat ,lng;
    if(dialog == nullptr){
        dialog = new QDialog;
        QGridLayout *layout = new QGridLayout;
        QLabel *lat_label = new QLabel("纬度");
        QLabel *lng_label = new QLabel("经度");
        QPushButton *ok_button = new QPushButton("确定");
        QPushButton *cancel_button = new QPushButton("取消");
        layout->addWidget(lat_label,0,0,1,1);
        layout->addWidget(lat_edit,0,1,1,1);
        layout->addWidget(lng_label,1,0,1,1);
        layout->addWidget(lng_edit,1,1,1,1);
        layout->addWidget(ok_button,2,0,1,1);
        layout->addWidget(cancel_button,2,1,1,1);
        dialog->setLayout(layout);
        dialog->setWindowTitle("经纬度点");
        dialog->move((deskdop->width()-dialog->width())/2,(deskdop->height()-dialog->height())/2);

        connect(ok_button,&QPushButton::clicked,dialog,&QDialog::accept);
        connect(cancel_button,&QPushButton::clicked,dialog,&QDialog::reject);
    }
    int ret = dialog->exec();
    if(ret == QDialog::Accepted){
        lat = lat_edit->text().toDouble();
        lng = lng_edit->text().toDouble();
        emit m_webview->obj()->addMarkerAt(lng,lat);
    }
}

void NormalWindow::openMapClicked()
{

}

void NormalWindow::openPathClicked()
{

}

