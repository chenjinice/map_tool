#ifndef LOGDOCK_H
#define LOGDOCK_H

class QTextEdit;


#include <QDockWidget>

class LogDock : public QDockWidget
{
    Q_OBJECT
public:
    static LogDock *ins();
    void setLogSize(int s);

private:
    LogDock();
    ~LogDock();
    void init();

    static LogDock *    m_instance;
    int                 m_size;
    QTextEdit *         m_edit;

public slots:
    void log(QString str);

signals:
    void showLog(QString str);
};

#endif // LOGDOCK_H
