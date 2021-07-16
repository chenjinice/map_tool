#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QDebug>
#include "logdock.h"

LogDock *LogDock::m_instance = nullptr;

LogDock::LogDock()
{
    this->init();
}

LogDock::~LogDock()
{
    m_edit->clear();
}

LogDock *LogDock::ins()
{
    if(m_instance == nullptr){
        m_instance = new LogDock();
    }
    return m_instance;
}

void LogDock::setLogSize(int s)
{
    m_size = s;
}

void LogDock::init()
{
    m_size = 1024*1024*10;
    this->setWindowTitle("log");
    this->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetFloatable);

    QWidget *widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    m_edit = new QTextEdit;
    m_edit->setReadOnly(true);
    QPushButton *clear_button = new QPushButton("clear");

    layout->addWidget(m_edit);
    layout->addWidget(clear_button);
    layout->setMargin(0);
    widget->setLayout(layout);
    this->setWidget(widget);
    connect(clear_button,&QPushButton::clicked,m_edit,&QTextEdit::clear);
}

void LogDock::log(QString str)
{
    if(!this->isVisible())return;
    const QString &text = m_edit->toPlainText();
    int len = text.count();
    if(len > m_size){
        QString new_text = text;
        new_text.remove(0,len/2);
        m_edit->setText(new_text);
    }
    m_edit->append(str);
}
