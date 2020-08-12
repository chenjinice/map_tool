#ifndef WEBOBJECT_H
#define WEBOBJECT_H


#include <QObject>


class WebObject : public QObject
{
    Q_OBJECT
public:
    WebObject(QObject *parent = nullptr);

signals:

public slots:
};

#endif // WEBOBJECT_H
