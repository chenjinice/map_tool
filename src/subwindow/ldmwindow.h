#ifndef LDMWINDOW_H
#define LDMWINDOW_H


#include <QWidget>

class WebView;

class LdmWindow : public QWidget
{
    Q_OBJECT
public:
    LdmWindow(QWidget *parent = nullptr);
    ~LdmWindow();

private:
    WebView *        m_webview      = nullptr;

signals:

public slots:
};

#endif // LDMWINDOW_H
