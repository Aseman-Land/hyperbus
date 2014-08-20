#include "testobject.h"

#include <QDebug>
#include <QImage>

TestObject::TestObject(QObject *parent) :
    QObject(parent)
{
}

QString TestObject::ping()
{
    static int a = 0;
    QString res = QString::number(a);
    a++;
    return res;
}

QImage TestObject::getImage()
{
    return QImage("/home/bardia/Pictures/Cars/Civic/Pictures/1210369485-1-11563_zpsa81d9abe.jpg");
}

void TestObject::void_call()
{
    static int a = 0;
    a++;
    qDebug() << __FUNCTION__ << a;
}

void TestObject::in_int_call(int )
{
    static int a = 0;
    a++;
    qDebug() << __FUNCTION__ << a;
}

int TestObject::out_int_call()
{
    static int a = 0;
    a++;
    qDebug() << __FUNCTION__ << a;
    return 0;
}

int TestObject::inout_int_call(int )
{
    static int a = 0;
    a++;
    qDebug() << __FUNCTION__ << a;
    return 0;
}

void TestObject::in_string_call(const QString &)
{
    static int a = 0;
    a++;
    qDebug() << __FUNCTION__ << a;
}

QString TestObject::out_string_call()
{
    static int a = 0;
    a++;
    qDebug() << __FUNCTION__ << a;
    return QString();
}

QString TestObject::inout_string_call(const QString &)
{
    static int a = 0;
    a++;
    qDebug() << __FUNCTION__ << a;
    return QString();
}
