#include <math.h>
#include <QFile>
#include <QDebug>
#include <QThread>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QProcess>
#include <QDir>
#include <QTimer>
#include "webobject.h"


using namespace std;


#define MAX_ZOOM            22
#define M_PI                3.14159265358979323846
#define SATELLITE_MAP_DIR   "s/"
#define STREET_MAP_DIR      "m/"
#define SATELLITE_MAP_URL   "http://mt1.google.com/vt/lyrs=s"
#define STREET_MAP_URL      "http://mt1.google.com/vt/lyrs=m"


WebObject::WebObject(QString dir, QObject *parent) : QObject(parent)
{
    m_map_dir           = dir + "/map/";
    m_ready             = true;

    m_thread            = new QThread;
    this->moveToThread(m_thread);
    connect(m_thread,&QThread::started,this,&WebObject::threadFun);
    m_thread->start();
}

WebObject::~WebObject()
{
    m_ready = false;
    m_thread->quit();
    m_thread->wait();
    delete m_thread;
}

bool WebObject::googleValid()
{
    return m_google;
}

void WebObject::threadFun()
{
    QTimer *                timer   = new QTimer;
    QNetworkAccessManager * manager = new QNetworkAccessManager;
    QNetworkRequest *       request = new QNetworkRequest;
    timer->setInterval(3000);
    connect(timer,&QTimer::timeout,this,&WebObject::connectGoogleThread);
    connect(manager,&QNetworkAccessManager::finished,this,&WebObject::downloadFinish);
    timer->start();

    uint count  = 0;
    while(m_ready) {
        // while循环里边不用这个的话，不能接收QT的信号
        QApplication::processEvents();
        if(!m_google){
            QThread::sleep(1);
            continue;
        }
        m_mutex.lock();
        if(m_list.length() == 0){
            QThread::sleep(1);
            m_mutex.unlock();
            continue;
        }
        if(m_list.contains(m_url)){
            if(count++ > 200){
                m_list.removeAll(m_url);  // 下载超过一定时间，则发送新的下载请求
                count = 0;
            }
        }else{
            count = 0;
            m_url = m_list[0];
            request->setUrl(QUrl(m_url));
            manager->get(*request);
        }
        m_mutex.unlock();
        QThread::usleep(20*1000);
    }
    manager->clearAccessCache();
    timer->stop();
    delete timer;
    delete request;
    delete manager;
}

void WebObject::connectGoogleThread()
{
    QProcess process;
    process.start("ping -n 2 www.google.com");
    process.waitForStarted();
    if( !process.waitForFinished(2000) ){
        process.kill();
        process.waitForFinished(300);
    }
    QString str = process.readAll();
    QString log;
    if(str.indexOf("TTL") == -1){
        log = "connect to google failed !!";
        m_google = false;
    }else{
        log = "connect to google success";
        m_google = true;
    }
    qDebug() << log;
    emit logToUi(log);
}

QString WebObject::getLocalImgPath(int x, int y, int z, MapType type)
{
    QString str = "others/";
    QString path;
    switch (type) {
        case LOCAL_G_SATELLITE:
        case REMOTE_G_SATELLITE:
            path = SATELLITE_MAP_DIR;
            break;
        case LOCAL_G_STREET:
        case REMOTE_G_STREET:
            path = STREET_MAP_DIR;
            break;
        default:
            break;
    }
    str = m_map_dir + path + QString("%1/%2/%3.jpg").arg(z).arg(x).arg(y);
    return str;
}

QString WebObject::getRemoteImgPath(int x, int y, int z, MapType type)
{
    QString str;
    QString url;
    switch (type) {
        case LOCAL_G_SATELLITE:
        case REMOTE_G_SATELLITE:
            url = SATELLITE_MAP_URL;
            break;
        case LOCAL_G_STREET:
        case REMOTE_G_STREET:
            url = STREET_MAP_URL;
            break;
        default:
            break;
    }
    str = url + QString("&x=%1&y=%2&z=%3").arg(x).arg(y).arg(z);
    return str;
}

void WebObject::listAdd(Range &x, Range &y, int z, MapType type)
{
    int max = 1000;
    if(!m_google)return;
    m_mutex.lock();
    for(int i=x.min;i<=x.max;i++){
        for(int j=y.min;j<=y.max;j++){
            QString file_name = this->getLocalImgPath(i,j,z,type);
            QFile file(file_name);
            if(file.exists())continue;

            QString url       = this->getRemoteImgPath(i,j,z,type);
            if(m_list.contains(url))continue;
            m_list.prepend(url);
            if(m_list.length() > max)m_list.pop_back();
        }
    }
    m_mutex.unlock();
}

double WebObject::moduleOperation(double num, int min, int max)
{
    if( (num <= max) && (num >= min) )return num;
    // 将num转华为min到max范围内的数
    int m = (max-min);
    int n = floor( (num-min)/m );
    return num-n*m;
}

void WebObject::getX(double lng1, double lng2, int z, Range &r)
{
    lng1 = this->moduleOperation(lng1,-180,180);
    lng2 = this->moduleOperation(lng2,-180,180);
    int max = pow(2,z);
    int x1 = pow(2,z-1)*(lng1/180.0+1);
    int x2 = pow(2,z-1)*(lng2/180.0+1);
    x1 %= max;
    x2 %= max;
    if(x1>x2){
        r.max = x1;
        r.min = x2;
    }else{
        r.max = x2;
        r.min = x1;
    }
}

void WebObject::getY(double lat1, double lat2, int z, Range &r)
{
    int lat_max = 85;
    lat1 = this->moduleOperation(lat1,-90,90);
    lat2 = this->moduleOperation(lat2,-90,90);
    if(lat1 > lat_max)lat1 = lat_max;
    if(lat2 > lat_max)lat2 = lat_max;
    if(lat1 < -lat_max)lat1 = -lat_max;
    if(lat2 < -lat_max)lat2 = -lat_max;
    double temp1 = tan(M_PI*lat1/180.0)+1.0/cos(M_PI*lat1/180.0);
    double temp2 = tan(M_PI*lat2/180.0)+1.0/cos(M_PI*lat2/180.0);
    int y1 = pow(2,z-1)*(1-log(temp1)/M_PI);
    int y2 = pow(2,z-1)*(1-log(temp2)/M_PI);
    if(y1>y2){
        r.max = y1;
        r.min = y2;
    }else{
        r.max = y2;
        r.min = y1;
    }
}

void WebObject::downloadFinish(QNetworkReply *reply)
{
    QString url = reply->url().toString();
    QUrlQuery query(url);

    m_mutex.lock();
    m_list.removeAll(url);
    m_mutex.unlock();

    MapType type = OTHERS;
    int x = query.queryItemValue("x").toInt();
    int y = query.queryItemValue("y").toInt();
    int z = query.queryItemValue("z").toInt();
    if(url.indexOf("lyrs=s") != -1)type = REMOTE_G_SATELLITE;
    if(url.indexOf("lyrs=m") != -1)type = REMOTE_G_STREET;

    QString log;
    if (reply->error() == QNetworkReply::NoError){
        log = "download ok : " + url;
        QByteArray bytes = reply->readAll();
        QString file_name = getLocalImgPath(x,y,z,type);
        QFileInfo info(file_name);
        QString file_path = info.path();
        QDir dir;
        dir.mkpath(file_path);
        QFile file(file_name);
        file.open(QIODevice::WriteOnly);
        file.write(bytes,bytes.length());
        file.close();
        this->sendUpdateSignal();
    }else{
        log = "download fail : " + url + reply->error();
    }
    qDebug() << log;
    emit logToUi(log);
}

void WebObject::sendUpdateSignal()
{
    static QDateTime s_tv = QDateTime::currentDateTime();
    QDateTime now = QDateTime::currentDateTime();
    qint64 s = s_tv.secsTo(now);
    if( (s > 1) || (m_list.length() == 0) ){
        emit mapUpdate();
        s_tv = now;
    }
}

void WebObject::getBounds(double lat1, double lat2, double lng1, double lng2, int zoom, int type)
{
//    qDebug() << lat1 << "," << lat2 << "," << lng1 << "," << lng2 << "," << zoom << "---------";
    if(zoom > MAX_ZOOM)return;
    Range x_range,y_range;
    getX(lng1,lng2,zoom,x_range);
    getY(lat1,lat2,zoom,y_range);
    this->listAdd(x_range,y_range,zoom,(MapType)type);
//    qDebug("x(%d,%d) , y(%d,%d) , z(%d),type(%d)",x_range.min,x_range.max,y_range.min,y_range.max,zoom,type);
}

