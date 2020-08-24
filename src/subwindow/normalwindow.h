#ifndef NORMALWINDOW_H
#define NORMALWINDOW_H

#include <QWidget>
#include "udpthread.h"

class QGridLayout;
class WebView;
class QLabel;
class QComboBox;
class UdpThread;
class QTimer;
class QLineEdit;

class NormalWindow : public QWidget
{
    Q_OBJECT
public:
    NormalWindow(QWidget *parent = nullptr);
    ~NormalWindow();


private:
    void            addCarInfoWidget();
    void            addButtons();
    void            getIpList();
    void            udpState(bool flag);
    void            dataSlot(UiData data);
    void            logEnd();
    void            saveClicked();
    void            addClicked();
    void            addAtClicked();
    void            openMapClicked();
    void            openPathClicked();
    void            clearClicked();

    int              m_index        = 0;
    QWidget *        m_car_widget   = nullptr;
    QGridLayout *    m_grid         = nullptr;
    QLabel *         m_lng          = nullptr;
    QLabel *         m_lat          = nullptr;
    QLabel *         m_heading      = nullptr;
    QLabel *         m_speed        = nullptr;
    QLabel *         m_satellite    = nullptr;
    QLabel *         m_gpod         = nullptr;
    QLabel *         m_state        = nullptr;
    QLineEdit *      m_line_edit    = nullptr;
    WebView *        m_webview      = nullptr;
    QComboBox *      m_combo        = nullptr;
    UdpThread *      m_udp_thread   = nullptr;
    QTimer *         m_timer        = nullptr;
    QString          m_log_file     = nullptr;
    UiData           m_data;

signals:

public slots:

};

#endif // NORMALWINDOW_H
