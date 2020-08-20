#include "setting.h"


Setting *Setting::m_instance = nullptr;

Setting *Setting::ins()
{
    if(m_instance == nullptr){
        m_instance = new Setting;
    }
    return m_instance;
}

Setting::Setting() : QSettings("CIDI","qtPoints")
{

}

void Setting::setCard(QString str)
{
    this->setValue(CS_CARD,str);
}

QString Setting::getCard()
{
    return this->value(CS_CARD).toString();
}





