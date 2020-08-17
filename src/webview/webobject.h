#ifndef WEBOBJECT_H
#define WEBOBJECT_H


#include <QObject>
#include <QMutex>
#include <thread>


// 瓦片信息
class ImgInfo
{
public:
    int x;
    int y;
    int z;
    int status;
};


// 地图的瓦片范围
class Range
{
public:
    int max     = 0;
    int min     = 0;
};


class WebObject : public QObject
{
    Q_OBJECT
public:
    WebObject(QObject *parent = nullptr);
    ~WebObject();


private:
    void            threadFun();
    void            listRemove(ImgInfo &m);
    bool            listFind(ImgInfo &m);
    void            listAdd(Range &x, Range &y, int z);
    double          moduleOperation(double num, int min, int max);
    void            getX(double lng1, double lng2, int z, Range &r);
    void            getY(double lat1, double lat2, int z, Range &r);


    bool            m_google;
    bool            m_ready;
    ImgInfo         m_img;
    QList<ImgInfo>  m_list;
    QMutex          m_mutex;

signals:



public slots:
    void            getBounds(double lat1, double lat2, double lng1, double lng2,int zoom);
};

#endif // WEBOBJECT_H
