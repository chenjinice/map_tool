#ifndef NORMALWINDOW_H
#define NORMALWINDOW_H

#include <QWidget>

class WebView;

class NormalWindow : public QWidget
{
    Q_OBJECT
public:
    NormalWindow(QWidget *parent = nullptr);
    ~NormalWindow();

private:
    WebView *        m_webview      = nullptr;

signals:

public slots:
};

#endif // NORMALWINDOW_H
