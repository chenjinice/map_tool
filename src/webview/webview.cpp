#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebChannel>
#include <QFileInfo>
#include <QDebug>
#include "webview.h"


WebView::WebView(QString path, QWidget *parent) : QWidget(parent)
{
    QFileInfo info(path);
    QVBoxLayout *layout         = new QVBoxLayout;

    m_path                      = path;
    m_web                       = new QWebEngineView;
    m_obj                       = new WebObject(info.path());
    QWebChannel *web_channel    = new QWebChannel(this);
    web_channel->registerObject("webobject",m_obj);

    m_web->page()->setWebChannel(web_channel);
    m_web->load(QUrl("file://"+m_path));
    layout->addWidget(m_web);
    layout->setMargin(0);
    this->setLayout(layout);

    connect(m_web,&QWebEngineView::loadFinished,this,&WebView::loadFinish);
}

WebView::~WebView()
{
    delete m_obj;
    delete m_web;
}

WebObject *WebView::obj()
{
    return m_obj;
}

void WebView::loadFinish()
{

}

