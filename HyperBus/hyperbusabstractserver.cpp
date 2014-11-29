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

#define RETURN_READ_CALL_CMD \
    { \
        p->server->sendMessage( socket, CLIENT_MSG_PREFIX ); \
        return; \
    }

#include "hyperbusabstractserver.h"
#include "hsmarttcpserver.h"
#include "hyperbusrecord.h"
#include "hsocketdata.h"
#include "highway.h"
#include "hyperbus_macros.h"

#include <QTcpSocket>
#include <QHash>
#include <QPair>
#include <QQueue>

typedef QPair<QTcpSocket*,QTcpSocket*> PairedSocket;

class HyperBusPauseItem
{
public:
    quint64 call_id;
    bool reserved;
    QTcpSocket *socket;
    QString key;
    QList<QByteArray> args;
    HSocketData *data;
    QByteArray reserved_res;
};

class HyperBusSendMessageItem
{
public:
    QTcpSocket *socket;
    QTcpSocket *caller;
    QByteArray message;
};

class HyperBusAbstractServerPrivate
{
public:
    HSmartTcpServer *server;
    QHash<quint64, QPair<QTcpSocket*,QTcpSocket*> > pending_calls;

    quint64 last_id;

    QHash<QTcpSocket*,QQueue<HyperBusSendMessageItem> > send_queues;
    QSet<QTcpSocket*> connects;

    QHash<quint64,HyperBusPauseItem> paused_items;
};

HyperBusAbstractServer::HyperBusAbstractServer(const QString &address, quint32 port, QObject *parent) :
    QObject(parent)
{
    p = new HyperBusAbstractServerPrivate;
    p->last_id = 0;

    p->server = new HSmartTcpServer( this );
    p->server->openSession(address,port);

    connect( p->server, SIGNAL(messageRecieved(QTcpSocket*,QByteArray)), SLOT(messageRecieved(QTcpSocket*,QByteArray)) );
}

quint16 HyperBusAbstractServer::port() const
{
    return p->server->port();
}

void HyperBusAbstractServer::messageRecieved(QTcpSocket *socket, const QByteArray &msg)
{
    if( msg.left(CLIENT_MSG_PREFIX.size()) == CLIENT_MSG_PREFIX )
    {
        QByteArray msg_prv = msg.mid(CLIENT_MSG_PREFIX.size());
        readCall( socket, msg_prv );
    }
    else
    if( msg.left(SERVER_MSG_PREFIX.size()) == SERVER_MSG_PREFIX )
    {
        QByteArray msg_prv = msg.mid(SERVER_MSG_PREFIX.size());
        readResult( socket, msg_prv );
    }
    else
        qDebug() << "Unknown message recieved.";
}

void HyperBusAbstractServer::readResult(QTcpSocket *socket, const QByteArray &msg)
{
    connectSocket(socket);
    int index = msg.indexOf(":");
    if( index == -1 )
        return;

    QByteArray msg_prv = msg.mid(index+1);
    quint64 msg_id  = msg.mid(0,index).toULongLong();
    if( !p->pending_calls.contains(msg_id) )
        return;

    const QPair<QTcpSocket*,QTcpSocket*> pair = p->pending_calls.value(msg_id);
    if( pair.second != socket )
        return;

    p->server->sendMessage( pair.first, CLIENT_MSG_PREFIX + msg_prv );
    p->pending_calls.remove(msg_id);

    p->send_queues[socket].takeFirst();
    if( !p->send_queues[socket].isEmpty() )
        p->server->sendMessage( p->send_queues[socket].first().socket, p->send_queues[socket].first().message );
    if( p->send_queues[socket].isEmpty() )
        p->send_queues.remove(socket);
}

void HyperBusAbstractServer::readCall(QTcpSocket *socket, const QByteArray &msg)
{
    p->last_id++;

    connectSocket(socket);
    QList<QByteArray> args = HyperBusRecord(msg).toQByteArrayList();
    if( args.isEmpty() )
        RETURN_READ_CALL_CMD

    HSocketData *data = this->data(socket);
    const QString & key = HighWay::makeKeyAbsolute(args.takeFirst());
    if( services().contains(key) )
    {
        HyperBusServiceItem *item = services()[key];
        switch( static_cast<int>(item->persmissions) )
        {
        case HyperBusGlobals::Owner:
            if( item->socket != socket )
                RETURN_READ_CALL_CMD
            break;

        case HyperBusGlobals::CurrentUser:
        {
            HSocketData *s_data = this->data(socket);
            if( !data || !s_data || ( data->userName()!=s_data->userName() && s_data->userName()!="root" ) )
                RETURN_READ_CALL_CMD

            switch( static_cast<int>(item->firewall_mode) )
            {
            case HyperBusGlobals::Disable:
                break;

            case HyperBusGlobals::BlackList:
                if( item->firewall.contains(s_data->commandPath()) )
                    RETURN_READ_CALL_CMD;
                break;

            case HyperBusGlobals::WhiteList:
                if( !item->firewall.contains(s_data->commandPath()) )
                    RETURN_READ_CALL_CMD
                break;
            }
        }
            break;

        case HyperBusGlobals::LocalUsers:
        {
            HSocketData *s_data = this->data(socket);
            if( !data || !s_data || !data->pid() )
                RETURN_READ_CALL_CMD

            switch( static_cast<int>(item->firewall_mode) )
            {
            case HyperBusGlobals::Disable:
                break;

            case HyperBusGlobals::BlackList:
                if( item->firewall.contains(s_data->commandPath()) )
                    RETURN_READ_CALL_CMD;
                break;

            case HyperBusGlobals::WhiteList:
                if( !item->firewall.contains(s_data->commandPath()) )
                    RETURN_READ_CALL_CMD
                break;
            }
         }
            break;

        case HyperBusGlobals::PublicUsers:
            break;
        }
    }

    QByteArray reserved_res;
    bool call_pause = false;
    const bool reserved = reservedCall(socket,p->last_id,key,args,&reserved_res,&call_pause);
    if( call_pause )
    {
        HyperBusPauseItem pause;
        pause.reserved = false;
        pause.socket = socket;
        pause.key = key;
        pause.args = args;
        pause.data = data;
        pause.call_id = p->last_id;
        pause.reserved_res = reserved_res;

        p->paused_items.insert( pause.call_id, pause );
    }
    else
        executeCall( p->last_id, reserved, socket, reserved_res, key, args, data );
}

void HyperBusAbstractServer::resumeCall(quint64 call_id)
{
    if( !p->paused_items.contains(call_id) )
        return;

    const HyperBusPauseItem & pause = p->paused_items.take(call_id);
    executeCall(pause.call_id, pause.reserved, pause.socket, pause.reserved_res, pause.key, pause.args, pause.data);
}

void HyperBusAbstractServer::executeCall( quint64 call_id, bool reserved, QTcpSocket *socket, const QByteArray & reserved_res, const QString & key, QList<QByteArray> args, HSocketData *data )
{
    if( reserved )
    {
        p->server->sendMessage( socket, CLIENT_MSG_PREFIX + reserved_res );
        return;
    }
    else
    if( !services().contains(key) )
        RETURN_READ_CALL_CMD;

    HyperBusServiceItem *service = services().value(key);
    if( service->args.count() != args.count() )
        RETURN_READ_CALL_CMD;

    args.prepend( service->command_id.toUtf8() );
    args.prepend( QByteArray::number(data->pid()) );

    HyperBusRecord record;
    foreach( const QByteArray & arg, args )
        record << arg;

    HyperBusSendMessageItem send_item;
    send_item.socket  = service->socket;
    send_item.caller  = socket;
    send_item.message = SERVER_MSG_PREFIX + QByteArray::number(call_id) + ":" + record.toQByteArray();

    p->send_queues[send_item.socket].append( send_item );
    if( p->send_queues[send_item.socket].count() == 1 )
        p->server->sendMessage( send_item.socket, send_item.message );

    QPair<QTcpSocket*,QTcpSocket*> pair;
    pair.first = socket;
    pair.second = service->socket;

    p->pending_calls.insert( call_id, pair );
}

void HyperBusAbstractServer::socketDestroyed(QObject *obj)
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(obj);
    QHashIterator< quint64, PairedSocket> i(p->pending_calls);
    while( i.hasNext() )
    {
        i.next();
        const PairedSocket & val = i.value();
        if( val.first == socket || val.second == socket )
        {
            if( val.first != socket )
                p->server->sendMessage( val.first, CLIENT_MSG_PREFIX );
            p->pending_calls.remove(i.key());
        }
    }

    QHashIterator<QTcpSocket*,QQueue<HyperBusSendMessageItem> > j(p->send_queues);
    while( j.hasNext() )
    {
        j.next();
        QQueue<HyperBusSendMessageItem> queue = j.value();
        for( int i=0 ; i<queue.count() ; i++ )
        {
            const HyperBusSendMessageItem & item = queue.at(i);
            if( j.key() == socket )
            {
                p->server->sendMessage( item.caller, CLIENT_MSG_PREFIX );
                queue.removeAt(i);
                i--;
                continue;
            }
            if( item.caller == socket )
            {
                queue.removeAt(i);
                i--;
                continue;
            }
        }
        p->send_queues[j.key()] = queue;
    }
}

void HyperBusAbstractServer::connectSocket(QTcpSocket *socket)
{
    if( p->connects.contains(socket) )
        return;

    connect( socket, SIGNAL(destroyed(QObject*)), SLOT(socketDestroyed(QObject*)) );
    p->connects.insert(socket);
}

bool HyperBusAbstractServer::reservedCall(QTcpSocket *socket, quint64 call_id, const QString &key, const QList<QByteArray> &args, QByteArray *res, bool *call_pause)
{
    Q_UNUSED(socket)
    Q_UNUSED(call_id)
    Q_UNUSED(key)
    Q_UNUSED(args)
    Q_UNUSED(res)
    Q_UNUSED(call_pause)
    return false;
}

HyperBusAbstractServer::~HyperBusAbstractServer()
{
    delete p;
}
