#include "mainwindow.h"
#include "webview.h"
#include "logdock.h"
#include "obuwindow.h"
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    m_mdi = new QMdiArea;
    m_mdi->setViewMode(QMdiArea::SubWindowView);

    LogDock *log_dock = LogDock::ins();
    this->addDockWidget(Qt::BottomDockWidgetArea,log_dock);
    log_dock->close();


    this->setMenu();
    this->setToolBar();
    this->setCentralWidget(m_mdi);
//    this->newNormalTrigged();
}

void MainWindow::setMenu()
{
    QMenuBar *  menu        = new QMenuBar;
    QMenu *    w_menu       = new QMenu("窗口");
    QAction *  new_normal   = new QAction("新建",this);
    QAction *  log_a        = new QAction("log窗口",this);
    w_menu->addAction(new_normal);
    w_menu->addSeparator();
    w_menu->addAction(log_a);
    menu->addMenu(w_menu);

    QMenu *  about_menu     = new QMenu("关于");
    QAction * aboutme_a     = new QAction("Me",this);
    QAction *qt_a = new QAction("Qt",this);
    about_menu->addAction(qt_a);
    about_menu->addAction(aboutme_a);
    menu->addMenu(about_menu);

    this->setMenuBar(menu);
    connect(new_normal,&QAction::triggered,this,&MainWindow::newNormalTrigged);
    connect(log_a,&QAction::triggered,this,&MainWindow::logActionTrigged);
    connect(qt_a,&QAction::triggered,qApp,&QApplication::aboutQt);
}

void MainWindow::setToolBar()
{
    QToolBar *toolbar       = this->addToolBar("toolbar");
    m_add                   = new QAction("中心加点",this);
    m_addat                 = new QAction("加点",this);
    m_addMapJson            = new QAction("地图Json",this);
    m_addPathJson           = new QAction("路径Json",this);
    m_clear                 = new QAction("清空",this);
    toolbar->addAction(m_add);
    toolbar->addAction(m_addat);
    toolbar->addAction(m_addMapJson);
    toolbar->addAction(m_addPathJson);
    toolbar->addAction(m_clear);
    toolbar->setMovable(false);
}

void MainWindow::newNormalTrigged()
{
    static QDialog *   dialog       = nullptr;
    static QLineEdit *ip_edit       = new QLineEdit;
    static QDesktopWidget *deskdop  = QApplication::desktop();

    if(dialog == nullptr){
        dialog = new QDialog;
        QGridLayout *layout = new QGridLayout;
        QLabel *ip_label = new QLabel("ip:");
        QPushButton *ok_button = new QPushButton("确定");
        QPushButton *cancel_button = new QPushButton("取消");
        layout->addWidget(ip_label,2,0,1,1);
        layout->addWidget(ip_edit,2,1,1,1);
        layout->addWidget(ok_button,3,0,1,1);
        layout->addWidget(cancel_button,3,1,1,1);
        dialog->setLayout(layout);
        dialog->setWindowTitle("obu ip");
        dialog->move((deskdop->width()-dialog->width())/2,(deskdop->height()-dialog->height())/2);

        connect(ok_button,&QPushButton::clicked,dialog,&QDialog::accept);
        connect(cancel_button,&QPushButton::clicked,dialog,&QDialog::reject);
    }
    if(dialog->exec() == QDialog::Rejected)return;

    QString ip = ip_edit->text();
    ObuWindow *w = new ObuWindow(ip);
    m_mdi->addSubWindow(w);
    w->show();

    connect(m_add,&QAction::triggered,w,&ObuWindow::addClicked);
    connect(m_addat,&QAction::triggered,w,&::ObuWindow::addAtClicked);
    connect(m_addMapJson,&QAction::triggered,w,&ObuWindow::openMapClicked);
    connect(m_addPathJson,&QAction::triggered,w,&ObuWindow::openPathClicked);
    connect(m_clear,&QAction::triggered,w,&ObuWindow::clearClicked);
    connect(w,&ObuWindow::log,LogDock::ins(),&LogDock::log);
}

void MainWindow::logActionTrigged()
{
    LogDock *log_dock = LogDock::ins();
    if(log_dock->isHidden())log_dock->show();
    else log_dock->hide();
}

MainWindow::~MainWindow()
{
//    qDebug() << "~~~~~~~~~~~~~mainwindow";
//    m_mdi->closeAllSubWindows();
//    delete m_mdi;
}


