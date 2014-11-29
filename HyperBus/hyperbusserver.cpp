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

#define SERVICE_METHOD_FIRST_CHECK( ITEM, SOCKET, SERVICE_KEY, RETURN_VALUE ) \
    if( !p->services.contains(SERVICE_KEY) ) \
        return RETURN_VALUE; \
    ITEM = p->services[SERVICE_KEY]; \
    if( !ITEM || ITEM->socket != SOCKET ) \
        return RETURN_VALUE;
#define VOID_RETURN
#define SERVICE_METHOD_FIRST_CHECK_VOID( ITEM, SOCKET, SERVICE_KEY ) \
    SERVICE_METHOD_FIRST_CHECK( ITEM, SOCKET, SERVICE_KEY, VOID_RETURN )

#include "hyperbusserver.h"
#include "hvariantconverter.h"
#include "hvariantconvertergeneraltypes.h"
#include "hsocketdata.h"

#include <QStringList>
#include <QDebug>
#include <QFileInfo>
#include <QTcpSocket>

class HyperBusServerPrivate
{
public:
    QHash<QString,HyperBusServiceItem*> services;
    QSet<QTcpSocket*> connects;

    QHash<QTcpSocket*,HSocketData*> accepted;
    QHash<QTcpSocket*,HSocketData*> rejected;
    QHash<QTcpSocket*,HSocketData*> datas;
};

HyperBusServer::HyperBusServer(const QString &address, quint32 port, QObject *parent) :
    HyperBusAbstractServer(address, port, parent)
{
    p = new HyperBusServerPrivate;
}

bool HyperBusServer::getAccess(QTcpSocket *socket)
{
    if( p->accepted.contains(socket) )
        return true;
    if( p->rejected.contains(socket) )
        return false;

    HSocketData *data = this->data(socket);
//    if( !data->pid() )
//    {
//        p->rejected.insert( socket, data );
//        return false;
//    }

    p->accepted.insert( socket, data );
    return true;
}

HSocketData *HyperBusServer::data(QTcpSocket *socket)
{
    if( p->datas.contains(socket) )
        return p->datas[socket];

    p->datas[socket] = new HSocketData(socket);
    return p->datas[socket];
}

bool HyperBusServer::registerService(QTcpSocket *socket, const QString &key, const QString & command_id, const QString & ret_type, const QList< QPair<QString,QString> > & args , HyperBusGlobals::PermissionUserType perm)
{
    if( p->services.contains(key) )
        return false;

    HyperBusServiceItem *item = new HyperBusServiceItem;
        item->socket = socket;
        item->command_id = command_id;
        item->key = key;
        item->return_type = ret_type;
        item->args = args;
        item->persmissions = perm;
        item->firewall_mode = HyperBusGlobals::Disable;

    p->services.insert( key, item );
    return true;
}

void HyperBusServer::setServicePermission(QTcpSocket *socket, const QString &service, HyperBusGlobals::PermissionUserType user)
{
    HyperBusServiceItem *item = 0;
    SERVICE_METHOD_FIRST_CHECK_VOID(item,socket,service)

    p->services[service]->persmissions = user;
}

HyperBusGlobals::PermissionUserType HyperBusServer::servicePermission(const QString &service)
{
    if( !p->services.contains(service) )
        return HyperBusGlobals::CurrentUser;
    HyperBusServiceItem *item = p->services[service];
    if( !item )
        return HyperBusGlobals::CurrentUser;

    return p->services[service]->persmissions;
}

void HyperBusServer::addPidToFirewall(QTcpSocket *socket, const QString & service, const QString & exe_path)
{
    HyperBusServiceItem *item = 0;
    SERVICE_METHOD_FIRST_CHECK_VOID(item,socket,service)

    item->firewall.insert( QFileInfo(exe_path).canonicalFilePath() );
}

void HyperBusServer::removePidFromFirewall(QTcpSocket *socket, const QString & service, const QString & exe_path)
{
    HyperBusServiceItem *item = 0;
    SERVICE_METHOD_FIRST_CHECK_VOID(item,socket,service)

    item->firewall.remove( QFileInfo(exe_path).canonicalFilePath() );
}

bool HyperBusServer::firewallContaintsPid(QTcpSocket *socket, const QString & service, const QString & exe_path)
{
    HyperBusServiceItem *item = 0;
    SERVICE_METHOD_FIRST_CHECK(item,socket,service,false)

    return item->firewall.contains( QFileInfo(exe_path).canonicalFilePath() );
}

void HyperBusServer::setFireWallMode(QTcpSocket *socket, const QString &service, HyperBusGlobals::FireWallMode mode)
{
    HyperBusServiceItem *item = 0;
    SERVICE_METHOD_FIRST_CHECK_VOID(item,socket,service)
    item->firewall_mode = mode;
}

HyperBusGlobals::FireWallMode HyperBusServer::fireWallMode(QTcpSocket *socket, const QString &service)
{
    HyperBusServiceItem *item = 0;
    SERVICE_METHOD_FIRST_CHECK(item,socket,service,HyperBusGlobals::Disable)
    return item->firewall_mode;
}

bool HyperBusServer::reservedCall(QTcpSocket *socket, quint64 call_id, const QString &key, const QList<QByteArray> &args, QByteArray *res, bool *call_pause)
{
    Q_UNUSED(call_pause)
    Q_UNUSED(call_id)

    connect_socket(socket);
    if( !getAccess(socket) )
        return true;

    HSocketData *data = this->data(socket);

    QByteArray res_str;
    if( key == "/register" && args.count() == 4 )
    {
        const QString & key = QString(data->userName().isEmpty()?"/%1%2":"/%1@%2").arg(data->userName(),data->address()) + args.at(0);
        const QByteArray & cid = args.at(1);
        const QByteArray & ret = args.at(2);
        const PairedList & vals = HVariantConverter::decode(args.at(3)).value<PairedList>();

        res_str = QByteArray::number(registerService( socket, key, cid, ret, vals, HyperBusGlobals::LocalUsers ));
    }
    else
    if( key == "/registerGlobal" && args.count() == 4 )
    {
        if( !data->pid() )
            return true;

        const QString & key = QString("/global") + args.at(0);
        const QByteArray & cid = args.at(1);
        const QByteArray & ret = args.at(2);
        const PairedList & vals = HVariantConverter::decode(args.at(3)).value<PairedList>();

        res_str = QByteArray::number(registerService( socket, key, cid, ret, vals, HyperBusGlobals::PublicUsers ));
    }
    else
    if( key == "/registerLocal" && args.count() == 4 )
    {
        if( !data->pid() )
            return true;

        const QString & key = QString("/local") + args.at(0);
        const QByteArray & cid = args.at(1);
        const QByteArray & ret = args.at(2);
        const PairedList & vals = HVariantConverter::decode(args.at(3)).value<PairedList>();

        res_str = QByteArray::number(registerService( socket, key, cid, ret, vals, HyperBusGlobals::LocalUsers ));
    }
    else
    if( key == "/registerShare" && args.count() == 4 )
    {
        const QString & key = QString("/share") + args.at(0);
        const QByteArray & cid = args.at(1);
        const QByteArray & ret = args.at(2);
        const PairedList & vals = HVariantConverter::decode(args.at(3)).value<PairedList>();

        res_str = QByteArray::number(registerService( socket, key, cid, ret, vals, HyperBusGlobals::PublicUsers ));
    }
    else
    if( key == "/setServicePermission" && args.count() == 2 )
    {
        const QByteArray & service = args.at(0);
        HyperBusGlobals::PermissionUserType user = static_cast<HyperBusGlobals::PermissionUserType>(args.at(1).toInt());
        setServicePermission( socket, service, user );
    }
    else
    if( key == "/servicePermission" && args.count() == 1 )
    {
        const QByteArray & service = args.at(0);
        res_str = QByteArray::number( static_cast<int>(servicePermission(service)) );
    }
    else
    if( key == "/addPidToFirewall" && args.count() == 2 )
    {
        const QByteArray & service = args.at(0);
        const QByteArray & exe_path = args.at(1);
        addPidToFirewall( socket, service, exe_path );
    }
    else
    if( key == "/removePidFromFirewall" && args.count() == 2 )
    {
        const QByteArray & service = args.at(0);
        const QByteArray & exe_path = args.at(1);
        removePidFromFirewall( socket, service, exe_path );
    }
    else
    if( key == "/firewallContaintsPid" && args.count() == 2 )
    {
        const QByteArray & service = args.at(0);
        const QByteArray & exe_path = args.at(1);
        res_str = QByteArray::number( static_cast<int>(firewallContaintsPid(socket, service, exe_path)) );
    }
    else
    if( key == "/setFireWallMode" && args.count() == 2 )
    {
        const QByteArray & service = args.at(0);
        HyperBusGlobals::FireWallMode mode = static_cast<HyperBusGlobals::FireWallMode>(args.at(1).toInt());
        setFireWallMode( socket, service, mode );
    }
    else
    if( key == "/fireWallMode" && args.count() == 1 )
    {
        const QByteArray & service = args.at(0);
        res_str = QByteArray::number( static_cast<int>(fireWallMode(socket, service)) );
    }
    else
        return false;

    *res = res_str;
    return true;
}

const QHash<QString, HyperBusServiceItem *> &HyperBusServer::services() const
{
    return p->services;
}

void HyperBusServer::socket_destroyed(QObject *obj)
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(obj);
    QHashIterator<QString,HyperBusServiceItem*> i(p->services);
    while( i.hasNext() )
    {
        i.next();
        if( i.value()->socket == socket )
            p->services.remove(i.key());
    }

    if( p->datas.contains(socket) )
        delete p->datas[socket];

    p->connects.remove(socket);
    p->accepted.remove(socket);
    p->rejected.remove(socket);
    p->datas.remove(socket);
}

void HyperBusServer::connect_socket(QTcpSocket *socket)
{
    if( p->connects.contains(socket) )
        return;

    connect( socket, SIGNAL(destroyed(QObject*)), SLOT(socket_destroyed(QObject*)) );
    p->connects.insert(socket);
}

HyperBusServer::~HyperBusServer()
{
    delete p;
}
