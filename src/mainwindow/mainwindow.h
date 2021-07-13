#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QStringList>
#include <QDebug>
#include <QPushButton>
#include <QMdiSubWindow>
#include <QApplication>
#include <QStatusBar>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void                setMenu();
    void                setToolBar();
    void                newNormalTrigged();
    void                logActionTrigged();

    QMdiArea *          m_mdi           = nullptr;
    QAction *           m_add           = nullptr;
    QAction *           m_addat         = nullptr;
    QAction *           m_addMapJson    = nullptr;
    QAction *           m_addPathJson   = nullptr;
    QAction *           m_clear         = nullptr;
};

#endif // MAINWINDOW_H


