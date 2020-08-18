#ifndef WEBOBJECT_H
#define WEBOBJECT_H


#include <QObject>
#include <QMutex>
#include <thread>
#include <QThread>

class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

// 瓦片类型
enum MapType{
    LOCAL_G_SATELLITE    = 0,
    REMOTE_G_SATELLITE   = 1,
    LOCAL_G_STREET       = 2,
    REMOTE_G_STREET      = 3
};

// 瓦片信息
class ImgInfo
{
public:
    int x                = 0;
    int y                = 0;
    int z                = 0;
    int status           = 0;
    QString url;
};

// 地图的瓦片范围
class Range
{
public:
    int max              = 0;
    int min              = 0;
};


class WebObject : public QObject
{
    Q_OBJECT
public:
    WebObject(QString dir, QObject *parent = nullptr);
    ~WebObject();


private:
    void                    threadFun();
    void                    connectGoogleThread();
    QString                 getLocalImgPath(int x,int y,int z,MapType type);
    QString                 getRemoteImgPath(int x,int y, int z, MapType type);
    void                    listRemove(ImgInfo &m);
    bool                    listFind(ImgInfo &m);
    void                    listAdd(Range &x, Range &y, int z,MapType type);
    double                  moduleOperation(double num, int min, int max);
    void                    getX(double lng1, double lng2, int z, Range &r);
    void                    getY(double lat1, double lat2, int z, Range &r);
    void                    downloadFinish(QNetworkReply *reply);
    void                    sendUpdateSignal();

    bool                    m_google        = false;
    bool                    m_ready         = false;
    QString                 m_map_dir;
    ImgInfo                 m_img;
    QList<ImgInfo>          m_list;
    QThread *               m_thread        = nullptr;
    QMutex                  m_mutex;
    QNetworkRequest *       m_request       = nullptr;
    QNetworkAccessManager * m_manager       = nullptr;

signals:
    void                    mapUpdate();

public slots:
    void                    getBounds(double lat1, double lat2, double lng1, double lng2,int zoom,int type);
};

#endif // WEBOBJECT_H
