#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QStringList>
#include <QDebug>
#include <QPushButton>
#include <QMdiArea>
#include <QTextEdit>
#include <QMdiSubWindow>
#include <QApplication>
#include "mainwindow.h"
#include "webview.h"
#include "logdock.h"
#include "ldmwindow.h"
#include "normalwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    m_mdi = new QMdiArea;
    m_mdi->setViewMode(QMdiArea::TabbedView);

    LogDock *log_dock = LogDock::ins();
    this->addDockWidget(Qt::BottomDockWidgetArea,log_dock);
    log_dock->close();

    this->setMenu();
    this->setCentralWidget(m_mdi);
    this->newNormalTrigged();
}

void MainWindow::setMenu()
{
    QMenuBar *  menu        = new QMenuBar;
    QMenu *    w_menu       = new QMenu("窗口");
    QAction *  new_normal   = new QAction("新建",this);
    QAction *  new_ldm      = new QAction("新LDM窗口",this);
    QAction *  log_a        = new QAction("log窗口",this);
    w_menu->addAction(new_normal);
    w_menu->addAction(new_ldm);
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
    connect(new_ldm,&QAction::triggered,this,&MainWindow::newLdmTrigged);
    connect(log_a,&QAction::triggered,this,&MainWindow::logActionTrigged);
    connect(qt_a,&QAction::triggered,qApp,&QApplication::aboutQt);
//    connect(aboutme_a,&QAction::triggered,this,&MainWindow::aboutMe_triggered);
}

void MainWindow::newNormalTrigged()
{
    NormalWindow * w = new NormalWindow;
    w->setWindowTitle("地图");
    m_mdi->addSubWindow(w);
    w->show();
}

void MainWindow::newLdmTrigged()
{
    LdmWindow * w = new LdmWindow;
    w->setWindowTitle("Ldm");
    m_mdi->addSubWindow(w);
    w->show();
}

void MainWindow::logActionTrigged()
{
    LogDock *log_dock = LogDock::ins();
    if(log_dock->isHidden())log_dock->show();
    else log_dock->hide();
}

MainWindow::~MainWindow()
{
    qDebug() << "~~~~~~~~~~~~~mainwindow";
    m_mdi->closeAllSubWindows();
    delete m_mdi;
}


