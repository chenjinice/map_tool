#include <QVBoxLayout>
#include <QDebug>
#include "ldmwindow.h"
#include "webview.h"
#include "common_defines.h"


LdmWindow::LdmWindow(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

    m_webview           = new WebView(HTML_INDEX);
    layout->setMargin(0);
    layout->addWidget(m_webview);
    this->setLayout(layout);
}


LdmWindow::~LdmWindow()
{
    delete m_webview;
}
