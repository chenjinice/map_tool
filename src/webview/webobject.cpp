#include <math.h>
#include <QFile>
#include <QDebug>
#include <QThread>
#include "webobject.h"

using namespace std;

#define M_PI       3.14159265358979323846


WebObject::WebObject(QObject *parent) : QObject(parent)
{
    std::thread t(&WebObject::threadFun,this);
    t.detach();
}

WebObject::~WebObject()
{
    m_ready = false;
}

void WebObject::threadFun()
{
    m_ready = true;
    while (m_ready) {
        qDebug() << "1111";
        QThread::sleep(1);
    }
}

void WebObject::listRemove(ImgInfo &m)
{
    m_mutex.lock();
    int len = m_list.length(),i;
    for(i=len-1;i>=0;i--){
        ImgInfo *p = &m_list[i];
        if( (p->x==m.x) &&(p->y==m.y)&&(p->z==m.z))m_list.removeAt(i);
    }
    m_img.status = 0;
    m_mutex.unlock();
}

bool WebObject::listFind(ImgInfo &m)
{
    bool ret = false;
    int len = m_list.length(),i;
    for(i=0;i<len;i++){
        ImgInfo *p = &m_list[i];
        if( (p->x==m.x) &&(p->y==m.y)&&(p->z==m.z)){
            ret = true;
            break;
        }
    }
    return ret;
}

void WebObject::listAdd(Range &x, Range &y, int z)
{
    int max = 100;
    if(!m_google)return;
    m_mutex.lock();
    for(int i=x.min;i<=x.max;i++){
        for(int j=y.min;j<=y.max;j++){
            QString file_name = QString("./html/map/%1/%2/%3.jpg").arg(z).arg(i).arg(j);
            QFile file(file_name);
            if(file.exists())continue;

            ImgInfo m;
            m.x = i;
            m.y = j;
            m.z = z;
            m.status = -1;
            if(this->listFind(m))continue;
            m_list.prepend(m);

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


void WebObject::getBounds(double lat1, double lat2, double lng1, double lng2, int zoom)
{
//    qDebug() << lat1 << "," << lat2 << "," << lng1 << "," << lng2 << "," << zoom << "---------";
    Range x_range,y_range;
    getX(lng1,lng2,zoom,x_range);
    getY(lat1,lat2,zoom,y_range);
    this->listAdd(x_range,y_range,zoom);
    qDebug("x(%d,%d) , y(%d,%d) , z(%d)",x_range.min,x_range.max,y_range.min,y_range.max,zoom);
}
