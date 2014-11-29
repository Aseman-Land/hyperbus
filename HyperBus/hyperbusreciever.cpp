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

#include "hyperbusreciever.h"
#include "hyperbusrecord.h"
#include "highway.h"
#include "hvariantconverter.h"
#include "hpidtools.h"

#include <QDebug>
#include <QCoreApplication>
#include <QHash>

class HyperBusRecieverPrivate
{
public:
    HighWay *highway;
    QHash<quint64,QString> key_maps;
    quint64 last_map_id;
    quint64 last_pid;

    QString session;
};

HyperBusReciever::HyperBusReciever(const QString &address, quint32 port, QObject *parent) :
    HMsgTransporter(address,port,parent)
{
    p = new HyperBusRecieverPrivate;
    p->highway     = new HighWay(this);
    p->last_map_id = 0;
    p->last_pid    = 0;

    setSession(HyperBusReciever::Normal);
}

void HyperBusReciever::setSession(const QString &session)
{
    p->session = session;
}

void HyperBusReciever::setSession(SessionType type)
{
    switch( static_cast<int>(type) )
    {
    case HyperBusReciever::Normal:
        setSession(HPidTools::userOf(QCoreApplication::applicationPid()) + "@127.0.0.1");
        break;

    case HyperBusReciever::Global:
        setSession("global");
        break;

    case HyperBusReciever::Local:
        setSession("local");
        break;

    case HyperBusReciever::Share:
        setSession("share");
        break;
    }
}

QString HyperBusReciever::session() const
{
    return p->session;
}

void HyperBusReciever::setServicePermission(const QString &service, HyperBusGlobals::PermissionUserType user)
{
    const QString & key = QString("/") + p->session + HighWay::makeKeyAbsolute(service);
    sendCommand( "setServicePermission", QList<QByteArray>()
                                   << key.toUtf8()
                                   << QString::number(static_cast<int>(user)).toUtf8() );
}

HyperBusGlobals::PermissionUserType HyperBusReciever::servicePermission(const QString &service)
{
    const QString & key = QString("/") + p->session + HighWay::makeKeyAbsolute(service);
    QString res_str = sendCommand( "servicePermission", QList<QByteArray>()<< key.toUtf8() );

    return static_cast<HyperBusGlobals::PermissionUserType>(res_str.toInt());
}

void HyperBusReciever::addPidToFirewall(const QString &service, const QString &exe_path)
{
    const QString & key = QString("/") + p->session + HighWay::makeKeyAbsolute(service);
    sendCommand( "addPidToFirewall", QList<QByteArray>()
                                   << key.toUtf8()
                                   << exe_path.toUtf8() );
}

void HyperBusReciever::removePidFromFirewall(const QString &service, const QString &exe_path)
{
    const QString & key = QString("/") + p->session + HighWay::makeKeyAbsolute(service);
    sendCommand( "removePidFromFirewall", QList<QByteArray>()
                                   << key.toUtf8()
                                   << exe_path.toUtf8() );
}

bool HyperBusReciever::firewallContaintsPid(const QString &service, const QString &exe_path)
{
    const QString & key = QString("/") + p->session + HighWay::makeKeyAbsolute(service);
    QString res_str = sendCommand( "firewallContaintsPid", QList<QByteArray>()
                                   << key.toUtf8()
                                   << exe_path.toUtf8() );

    return res_str.toInt();
}

void HyperBusReciever::setFireWallMode(const QString &service, HyperBusGlobals::FireWallMode mode)
{
    const QString & key = QString("/") + p->session + HighWay::makeKeyAbsolute(service);
    sendCommand( "setFireWallMode", QList<QByteArray>()
                                   << key.toUtf8()
                                   << QString::number(static_cast<int>(mode)).toUtf8() );
}

HyperBusGlobals::FireWallMode HyperBusReciever::fireWallMode(const QString &service)
{
    const QString & key = QString("/") + p->session + HighWay::makeKeyAbsolute(service);
    QString res_str = sendCommand( "fireWallMode", QList<QByteArray>()
                                    << key.toUtf8() );

    return static_cast<HyperBusGlobals::FireWallMode>(res_str.toInt());
}

QByteArray HyperBusReciever::sendCommand(const QString &commandName, const QList<QByteArray> &args)
{
    HyperBusRecord record;
    record << commandName.toUtf8();
    record << args;
    return HMsgTransporter::transfare( record.toQByteArray() );
}

QVariant HyperBusReciever::call(const QString &k, const QVariant &val0, const QVariant &val1, const QVariant &val2, const QVariant &val3, const QVariant &val4, const QVariant &val5, const QVariant &val6, const QVariant &val7, const QVariant &val8, const QVariant &val9)
{
    QString key = "/" + p->session + k;
    QVariantList vars;
        vars << val0 << val1 << val2 << val3 << val4 << val5 << val6 << val7 << val8 << val9;

    QList<QByteArray> args;
    foreach( const QVariant & var, vars )
    {
        if( var.isNull() )
            break;

        args << HVariantConverter::encode(var);
    }

    QByteArray res = sendCommand( key, args );
    return HVariantConverter::decode(res);
}

bool HyperBusReciever::registerService(const QString &k, QObject *obj, const char *member, const QString &description, SessionType type)
{
    const QString & key = HighWay::makeKeyAbsolute(k);
    if( !p->highway->registerMethod( key, obj, member, description ) )
        return false;

    p->key_maps[p->last_map_id] = key;

    QString cmd;
    switch( static_cast<int>(type) )
    {
    case HyperBusReciever::Normal:
        cmd = "register";
        break;

    case HyperBusReciever::Global:
        cmd = "registerGlobal";
        break;

    case HyperBusReciever::Local:
        cmd = "registerLocal";

    case HyperBusReciever::Share:
        cmd = "registerShare";
        break;
        break;
    }

    QByteArray res_str = sendCommand( cmd, QList<QByteArray>()
                                   << key.toUtf8()
                                   << QByteArray::number(p->last_map_id)
                                   << p->highway->returnTypeOf(key).toUtf8()
                                   << HVariantConverter::encode( QVariant::fromValue<HighWayArgs>(p->highway->paramatersOf(key))) );

    p->last_map_id++;
    return res_str.toInt();
}

quint64 HyperBusReciever::lastPID() const
{
    return p->last_pid;
}

QByteArray HyperBusReciever::callMessage(const QByteArray &msg)
{
    HyperBusRecord msg_record( msg );
    if( msg_record.count() == 0 )
        return QByteArray();

    quint64 map_id = msg_record.takeFirst().toULongLong();
    if( !p->key_maps.contains(map_id) )
        return QByteArray();

    QVariantList vals;
    for( int i=0; i<10; i++ )
    {
        if( i < msg_record.count() )
            vals << HVariantConverter::decode(msg_record.at(i));
        else
            vals << QVariant();
    }

    const QString & key = p->key_maps.value(map_id);
    const QVariant & res_var = p->highway->call( key, vals[0], vals[1], vals[2], vals[3], vals[4], vals[5], vals[6], vals[7], vals[8], vals[9] );

    return HVariantConverter::encode(res_var);
}

QByteArray HyperBusReciever::messageEvent(const QByteArray &msg)
{
    int index = msg.indexOf(":");
    if( index == -1 )
        return QByteArray();

    QByteArray msg_prv = msg.mid(index+1);
    HyperBusRecord msg_record( msg_prv );
    if( msg_record.count() == 0 )
        return QByteArray();

    p->last_pid = msg_record.takeFirst().toULongLong();
    QByteArray res = callMessage(msg_record.toQByteArray());

    return msg.mid(0,index+1)+res;
}

HyperBusReciever::~HyperBusReciever()
{
    delete p;
}
