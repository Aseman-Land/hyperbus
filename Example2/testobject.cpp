/*
    Copyright (C) 2014 Sialan Labs
    http://labs.sialan.org

    HyperBus is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    HyperBus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

void TestObject::setProgress(int value)
{
    if( !pbar )
        return;

    pbar->setValue(value);
    if( hbus )
        hbus->call("/setValue2"+append_txt,100-value);
}
