#include "mainwindow.h"
#include "webview.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    WebView *view = new WebView("c:/Users/63220/Desktop/web/index.html");
    view->show();
}


MainWindow::~MainWindow()
{
}

