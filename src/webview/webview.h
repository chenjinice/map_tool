#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWidget>


class QWebEngineView;
class WebObject;


class WebView : public QWidget
{
    Q_OBJECT
public:
    WebView(QString path,QWidget *parent = nullptr);
    ~WebView();

private:
    void                  loadFinish();

    QWebEngineView *      m_web         = nullptr;
    WebObject *           m_obj         = nullptr;
    QString               m_path;


signals:

public slots:
};

#endif // WEBVIEW_H
