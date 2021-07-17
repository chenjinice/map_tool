#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <QWidget>
#include "webview.h"
#include "setting.h"
#include "common_defines.h"


class SubWindow : public QWidget
{
    Q_OBJECT
public:
    SubWindow(QWidget *parent = nullptr);

signals:
    void log(QString str);

public slots:
    void            addClicked();
    void            addAtClicked();
    void            openMapClicked();
    void            openPathClicked();
    void            clearClicked();

protected:
    WebView *       m_webview      = nullptr;

};

#endif // SUBWINDOW_H
