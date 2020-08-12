#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebChannel>
#include "webview.h"
#include "webobject.h"


WebView::WebView(QString path, QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout         = new QVBoxLayout;

    m_path                      = path;
    m_web                       = new QWebEngineView;
    m_obj                       = new WebObject(this);
    QWebChannel *web_channel    = new QWebChannel(this);
    web_channel->registerObject("webobject",m_obj);

    m_web->page()->setWebChannel(web_channel);
    m_web->load(QUrl(m_path));
    layout->addWidget(m_web);
    this->setLayout(layout);

    connect(m_web,&QWebEngineView::loadFinished,this,&WebView::loadFinish);
}


void WebView::loadFinish()
{

}
