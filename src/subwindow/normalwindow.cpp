#include <QVBoxLayout>
#include <QDebug>
#include "normalwindow.h"
#include "webview.h"
#include "common/common_defines.h"


NormalWindow::NormalWindow(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

    m_webview           = new WebView(HTML_INDEX);
    layout->setMargin(0);
    layout->addWidget(m_webview);
    this->setLayout(layout);
}


NormalWindow::~NormalWindow()
{
    delete m_webview;
}
