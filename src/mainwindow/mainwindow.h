#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMdiArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void                setMenu();
    void                newNormalTrigged();
    void                newLdmTrigged();
    void                logActionTrigged();

    QMdiArea *          m_mdi       = nullptr;
};

#endif // MAINWINDOW_H


