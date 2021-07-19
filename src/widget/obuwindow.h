#ifndef OBUWINDOW_H
#define OBUWINDOW_H


#include <QLabel>
#include <QLineEdit>
#include "subwindow.h"
#include "obuudp.h"


class ObuWindow : public SubWindow
{
    Q_OBJECT
public:
    ObuWindow(QString ip,QWidget *parent = nullptr);
    ~ObuWindow();

private:
    QWidget *       addStateWidget();

    void            gpsReceived(const UiGpsData &data);

    QLineEdit *     m_pos       = nullptr;
    QLineEdit *     m_heading   = nullptr;
    QLineEdit *     m_speed     = nullptr;
    QLineEdit *     m_satellite = nullptr;
    QLineEdit *     m_hdop      = nullptr;
    QLineEdit *     m_model     = nullptr;

    ObuUdp *        m_obu       = nullptr;
};

#endif // LDMWINDOW_H
