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

#include "HSmartTcpServer.h"
#include "HTcpServer.h"
#include "hyperbus_macros.h"

#include <QDebug>
#include <QHash>

class HSmartTcpServerPrivate
{
public:
    HTcpServer *server;

    QHash<QTcpSocket*,QString> send_buffer;
    QHash<QTcpSocket*,QString> read_buffer;
};

HSmartTcpServer::HSmartTcpServer(QObject *parent) :
    QObject(parent)
{
    p = new HSmartTcpServerPrivate;
    p->server = new HTcpServer(this);
    connect( p->server, SIGNAL(messageRecieved(QTcpSocket*,QString)), SLOT(messageRecieved_prev(QTcpSocket*,QString)) );
}

quint16 HSmartTcpServer::port() const
{
    return p->server->port();
}

bool HSmartTcpServer::openSession(const QString &address, quint32 port)
{
    return p->server->openSession(address,port);
}

void HSmartTcpServer::sendMessage(QTcpSocket *socket, const QString &m)
{
    QString msg = SMART_LAYER_SEND_MSG_PREFIX + m.mid(0,MAXIMUM_MSG_SIZE);
    p->send_buffer[socket] = m.mid(MAXIMUM_MSG_SIZE);
    if( p->send_buffer[socket].isEmpty() )
        msg += SMART_LAYER_SEND_MSG_END;
    else
        msg += SMART_LAYER_SEND_MSG_NOT_END;

    p->server->sendMessage(socket,msg);
}

void HSmartTcpServer::messageRecieved_prev(QTcpSocket *socket, const QString &m)
{
    if( m.left(SMART_LAYER_SEND_MSG_PREFIX.size()) == SMART_LAYER_SEND_MSG_PREFIX )
    {
        QString msg = m.mid(SMART_LAYER_SEND_MSG_PREFIX.size());
        if( msg.isEmpty() )
            return;

        if( msg.right(SMART_LAYER_SEND_MSG_NOT_END.size()) == SMART_LAYER_SEND_MSG_NOT_END )
        {
            p->read_buffer[socket] += msg.left(msg.size()-SMART_LAYER_SEND_MSG_NOT_END.size());
            p->server->sendMessage(socket,SMART_LAYER_NEXT_MSG_PREFIX);
        }
        else
        if( msg.right(SMART_LAYER_SEND_MSG_END.size()) == SMART_LAYER_SEND_MSG_END )
        {
            p->read_buffer[socket] += msg.left(msg.size()-SMART_LAYER_SEND_MSG_END.size());
            QString tmp = p->read_buffer[socket];
            p->read_buffer.remove(socket);
            emit messageRecieved(socket,tmp);
        }
    }
    else
    if( m.left(SMART_LAYER_NEXT_MSG_PREFIX.size()) == SMART_LAYER_NEXT_MSG_PREFIX )
    {
        QString msg = m.mid(SMART_LAYER_NEXT_MSG_PREFIX.size());
        Q_UNUSED(msg)
        sendMessage(socket,p->send_buffer[socket]);
    }
    else
        qDebug() << "HSmartTcpServer error: unknown message recieved.";
}

HSmartTcpServer::~HSmartTcpServer()
{
    delete p;
}
