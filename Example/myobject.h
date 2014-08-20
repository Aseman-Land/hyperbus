#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QObject>
#include <HyperBus.h>

class MyObjectPrivate;
class MyObject : public QObject
{
    Q_OBJECT
public:
    MyObject(HyperBus *hbus, QString appendTxt, QObject *parent = 0);
    ~MyObject();

public slots:
    void setText( const QString & txt );
    void setValue( int v );

private:
    MyObjectPrivate *p;
};

#endif // MYOBJECT_H
