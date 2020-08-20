#ifndef SETTING_H
#define SETTING_H

#include <QSettings>



#define CS_CARD     "card"



class Setting : public QSettings
{
public:
    static Setting *ins();
    void            setCard(QString str);
    QString         getCard();

private:
    static Setting * m_instance;

    Setting();
};



#endif // SETTING_H
