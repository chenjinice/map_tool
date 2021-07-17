#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

    MainWindow w;
    w.show();

    return a.exec();
}



