#include "mainwindow.h"
#include "webview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    WebView *view = new WebView("https://www.baidu.com",this);
}

MainWindow::~MainWindow()
{
}

