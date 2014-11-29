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

#ifndef HYPERBUSABSTRACTSERVER_H
#define HYPERBUSABSTRACTSERVER_H

#include <QObject>
#include <QHash>
#include <QPair>
#include <QList>
#include <QSet>

#include "hyperbusglobals.h"

class HSocketData;
class HyperBusServiceItem;
class QTcpSocket;
class HyperBusAbstractServerPrivate;
class HyperBusAbstractServer : public QObject
{
    Q_OBJECT
public:
    HyperBusAbstractServer(const QString &address ,quint32 port ,QObject *parent = 0);
    ~HyperBusAbstractServer();

    quint16 port() const;

    virtual HSocketData *data(QTcpSocket *socket) = 0;

public slots:
    void resumeCall( quint64 call_id );

private slots:
    void messageRecieved(QTcpSocket *socket, const QByteArray &msg );
    void readResult(QTcpSocket *socket, const QByteArray &msg );
    void readCall(QTcpSocket *socket, const QByteArray &msg );

    void socketDestroyed(QObject *obj );

protected:
    virtual bool reservedCall(QTcpSocket *socket, quint64 call_id, const QString & key, const QList<QByteArray> & args, QByteArray *res, bool *call_pause ) = 0;
    virtual const QHash<QString,HyperBusServiceItem*> & services() const = 0;

private:
    void connectSocket( QTcpSocket *socket );
    void executeCall(quint64 call_id, bool reserved, QTcpSocket *socket, const QByteArray &reserved_res, const QString &key, QList<QByteArray> args, HSocketData *data);

private:
    HyperBusAbstractServerPrivate *p;
};

class HyperBusServiceItem
{
public:
    HyperBusServiceItem(){
        persmissions = HyperBusGlobals::CurrentUser;
    }

    QTcpSocket *socket;
    QString command_id;
    QString key;
    QString return_type;
    QList< QPair<QString,QString> > args;

    QSet<QString> firewall;
    HyperBusGlobals::FireWallMode firewall_mode;

    HyperBusGlobals::PermissionUserType persmissions;
};

#endif // HYPERBUSABSTRACTSERVER_H
