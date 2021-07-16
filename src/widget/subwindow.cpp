#include "subwindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLineEdit>
#include <QDesktopWidget>
#include <QLabel>
#include <QPushButton>
#include <QApplication>


SubWindow::SubWindow(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

    m_webview           = new WebView(HTML_INDEX);
    layout->setMargin(0);
    layout->addWidget(m_webview);
    this->setLayout(layout);
}

void SubWindow::addClicked()
{
    emit m_webview->obj()->addMarker();
}

void SubWindow::addAtClicked()
{
    static QDialog *   dialog  = nullptr;
    static QLineEdit *lat_edit = new QLineEdit;
    static QLineEdit *lng_edit = new QLineEdit;
    static QDesktopWidget *deskdop = QApplication::desktop();

    if(dialog == nullptr){
        dialog = new QDialog;
        QGridLayout *layout = new QGridLayout;
        QLabel *lat_label = new QLabel("纬度");
        QLabel *lng_label = new QLabel("经度");
        QLabel *ex_label  = new QLabel("例如 : 112.1234567 , 22.7654321");
        QPushButton *ok_button = new QPushButton("确定");
        QPushButton *cancel_button = new QPushButton("取消");
        layout->addWidget(ex_label,0,0,1,2);
        layout->addWidget(lng_label,1,0,1,1);
        layout->addWidget(lng_edit,1,1,1,1);
        layout->addWidget(lat_label,2,0,1,1);
        layout->addWidget(lat_edit,2,1,1,1);
        layout->addWidget(ok_button,3,0,1,1);
        layout->addWidget(cancel_button,3,1,1,1);
        dialog->setLayout(layout);
        dialog->setWindowTitle("经纬度点");
        dialog->move((deskdop->width()-dialog->width())/2,(deskdop->height()-dialog->height())/2);

        connect(ok_button,&QPushButton::clicked,dialog,&QDialog::accept);
        connect(cancel_button,&QPushButton::clicked,dialog,&QDialog::reject);
    }
    int ret = dialog->exec();
    if(ret == QDialog::Accepted){
        double lng = lng_edit->text().toDouble();
        double lat = lat_edit->text().toDouble();
        emit m_webview->obj()->addMarkerAt(lng,lat);
    }
}

void SubWindow::openMapClicked()
{
    QStringList l = QFileDialog::getOpenFileNames(this,"打开地图文件");
    for(int i=0; i<l.length();i++){
        emit m_webview->obj()->openMapJson(l[i]);
    }
}

void SubWindow::openPathClicked()
{
    QStringList l = QFileDialog::getOpenFileNames(this,"打开采点路径文件");
    for(int i=0; i<l.length();i++){
        emit m_webview->obj()->openPathJson(l[i]);
    }
}

void SubWindow::clearClicked()
{
    emit m_webview->obj()->clearAll();
}
