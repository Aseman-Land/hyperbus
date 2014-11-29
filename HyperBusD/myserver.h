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

#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <hyperbusserver.h>
#include <htcpserver.h>

class MyServer : public HyperBusServer
{
    Q_OBJECT
public:
    MyServer(const QString &address = "127.0.0.1", quint32 port = 2548, QObject *parent = 0);

    void void_call();
    void in_int_call( int a );
    int out_int_call();
    int inout_int_call( int a );
    void in_string_call( const QString & a );
    QString out_string_call();
    QString inout_string_call( const QString & a );

protected:
    bool reservedCall(QTcpSocket *socket, quint64 call_id, const QString &key, const QList<QByteArray> &args, QByteArray *res, bool *call_pause);

};

#endif // MYSERVER_H
