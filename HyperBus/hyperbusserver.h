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

#ifndef HYPERBUSSERVER_H
#define HYPERBUSSERVER_H

#include "hyperbusabstractserver.h"
#include "hyperbusglobals.h"
#include "hyperbus_global.h"

class HyperBusServerPrivate;
class HYPERBUS_EXPORT HyperBusServer : public HyperBusAbstractServer
{
    Q_OBJECT
public:
    HyperBusServer(const QString &address = "127.0.0.1", quint32 port = 2548, QObject *parent = 0);
    ~HyperBusServer();

    bool getAccess( QTcpSocket *socket );
    virtual HSocketData *data(QTcpSocket *socket);

    void setServicePermission(QTcpSocket *socket, const QString & service, HyperBusGlobals::PermissionUserType user);
    HyperBusGlobals::PermissionUserType servicePermission(const QString & service);

    void addPidToFirewall( QTcpSocket *socket, const QString & service, const QString & exe_path );
    void removePidFromFirewall( QTcpSocket *socket, const QString & service, const QString & exe_path );
    bool firewallContaintsPid( QTcpSocket *socket, const QString & service, const QString & exe_path );

    void setFireWallMode( QTcpSocket *socket, const QString & service, HyperBusGlobals::FireWallMode mode );
    HyperBusGlobals::FireWallMode fireWallMode( QTcpSocket *socket, const QString & service );

    bool registerService(QTcpSocket *socket, const QString &key, const QString &command_id, const QString &ret_type , const QList< QPair<QString,QString> > &args, HyperBusGlobals::PermissionUserType perm);

protected:
    virtual bool reservedCall( QTcpSocket *socket, quint64 call_id, const QString & key, const QList<QByteArray> & args, QByteArray *res, bool *call_pause );
    virtual const QHash<QString,HyperBusServiceItem*> & services() const;

    void connect_socket( QTcpSocket *socket );

private slots:
    void socket_destroyed( QObject *obj );

private:
    HyperBusServerPrivate *p;
};

#endif // HYPERBUSSERVER_H
