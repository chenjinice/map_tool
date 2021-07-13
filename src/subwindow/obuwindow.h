#ifndef OBUWINDOW_H
#define OBUWINDOW_H


#include <QLabel>
#include "subwindow.h"


class ObuWindow : public SubWindow
{
    Q_OBJECT
public:
    ObuWindow(QWidget *parent = nullptr);
    ~ObuWindow();

private:
    QWidget *       addStateWidget();

    QLabel *        m_pos       = nullptr;
    QLabel *        m_heading   = nullptr;
    QLabel *        m_speed     = nullptr;
    QLabel *        m_satellite = nullptr;
    QLabel *        m_hdop      = nullptr;
    QLabel *        m_model     = nullptr;

};

#endif // LDMWINDOW_H
