/*
    Copyright (C) 2014 Aseman
    http://aseman.co

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

#include "myserver.h"

#include <QDebug>

MyServer::MyServer(const QString &address, quint32 port, QObject *parent) :
    HyperBusServer(address,port,parent)
{
}

void MyServer::void_call()
{
    static int a = 0;
    a++;
    qDebug() << __FUNCTION__ << a;
}

void MyServer::in_int_call(int )
{
    static int a = 0;
    a++;
    qDebug() << __FUNCTION__ << a;
}

int MyServer::out_int_call()
{
    static int a = 0;
    a++;
    qDebug() << __FUNCTION__ << a;
    return 0;
}

int MyServer::inout_int_call(int )
{
    static int a = 0;
    a++;
    qDebug() << __FUNCTION__ << a;
    return 0;
}

void MyServer::in_string_call(const QString &)
{
    static int a = 0;
    a++;
    qDebug() << __FUNCTION__ << a;
}

QString MyServer::out_string_call()
{
    static int a = 0;
    a++;
    qDebug() << __FUNCTION__ << a;
    return QString();
}

QString MyServer::inout_string_call(const QString &)
{
    static int a = 0;
    a++;
    qDebug() << __FUNCTION__ << a;
    return QString();
}

bool MyServer::reservedCall(QTcpSocket *socket, quint64 call_id, const QString &key, const QList<QByteArray> &args, QByteArray *res, bool *call_pause)
{
    return HyperBusServer::reservedCall(socket,call_id,key,args,res,call_pause);
}
