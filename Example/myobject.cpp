#include "myobject.h"

class MyObjectPrivate
{
public:
    HyperBus *hbus;
    QString appendTxt;
};

MyObject::MyObject(HyperBus *hbus, QString appendTxt, QObject *parent) :
    QObject(parent)
{
    p = new MyObjectPrivate;
    p->hbus = hbus;
    p->appendTxt = appendTxt;
}

void MyObject::setText(const QString &txt)
{
    p->hbus->call("/setText" + p->appendTxt, txt);
}

void MyObject::setValue(int v)
{
    p->hbus->call("/setValue" + p->appendTxt, v);
}

MyObject::~MyObject()
{
    delete p;
}
