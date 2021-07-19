#ifndef WEBOBJECT_H
#define WEBOBJECT_H

#include <QObject>
#include <QMutex>
#include <thread>
#include <QThread>
#include <QStringList>


class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;
class QTimer;


// 瓦片类型
enum MapType{
    LOCAL_G_SATELLITE    = 0,
    REMOTE_G_SATELLITE   = 1,
    LOCAL_G_STREET       = 2,
    REMOTE_G_STREET      = 3,
    OTHERS
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
    bool                    googleValid();


private:
    void                    threadFun();
    void                    connectGoogleThread();
    QString                 getLocalImgPath(int x,int y,int z,MapType type);
    QString                 getRemoteImgPath(int x,int y, int z, MapType type);
    void                    listAdd(Range &x, Range &y, int z,MapType type);
    double                  moduleOperation(double num, int min, int max);
    void                    getX(double lng1, double lng2, int z, Range &r);
    void                    getY(double lat1, double lat2, int z, Range &r);
    void                    downloadFinish(QNetworkReply *reply);
    void                    sendUpdateSignal();

    bool                    m_google        = false;
    bool                    m_ready         = false;
    QString                 m_url;
    QString                 m_map_dir;
    QStringList             m_list;
    QThread *               m_thread        = nullptr;
    QMutex                  m_mutex;
    QNetworkRequest *       request       = nullptr;
    QNetworkAccessManager * manager       = nullptr;
    QTimer *                timer         = nullptr;

signals:
    void                    logToUi(QString str);
    void                    mapUpdate();
    void                    carUpdate(double lng,double lat,float heading);
    void                    addMarker();
    void                    addMarkerAt(double lng,double lat);
    void                    openMapJson(QString file);
    void                    openPathJson(QString file);
    void                    addAsnJson(const QJsonObject &json);
    void                    clearAll();

public slots:
    void                    getBounds(double lat1, double lat2, double lng1, double lng2,int zoom,int type);
};

#endif // WEBOBJECT_H
