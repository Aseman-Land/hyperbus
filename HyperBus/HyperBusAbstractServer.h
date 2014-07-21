/*
    Copyright (C) 2014 Sialan Labs
    http://labs.sialan.org

    HyperBus is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    HyperBus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HYPERBUSABSTRACTSERVER_H
#define HYPERBUSABSTRACTSERVER_H

#include <QObject>
#include <QHash>
#include <QPair>
#include <QList>
#include <QSet>

#include "HyperBusGlobals.h"

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

private slots:
    void messageRecieved( QTcpSocket *socket, const QString & msg );
    void readResult( QTcpSocket *socket, const QString & msg );
    void readCall( QTcpSocket *socket, const QString & msg );

    void socketDestroyed(QObject *obj );

protected:
    virtual bool reservedCall( QTcpSocket *socket, const QString & key, const QStringList & args, QString *res = 0 ) = 0;
    virtual const QHash<QString,HyperBusServiceItem*> & services() const = 0;

private:
    void connectSocket( QTcpSocket *socket );

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
