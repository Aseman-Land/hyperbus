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

#include "HSmartTcpClient.h"
#include "HTcpClient.h"
#include "hyperbus_macros.h"

#include <QDebug>

class HSmartTcpClientPrivate
{
public:
    HTcpClient *client;

    QByteArray send_buffer;
    QByteArray read_buffer;
};

HSmartTcpClient::HSmartTcpClient(QObject *parent) :
    QObject(parent)
{
    p = new HSmartTcpClientPrivate;
    p->client = new HTcpClient(this);
    connect( p->client, SIGNAL(messageRecieved(QByteArray)), SLOT(messageRecieved_prev(QByteArray)) );
}

void HSmartTcpClient::setSession(const QString &ip, quint32 port)
{
    p->client->setSession(ip,port);
}

QByteArray HSmartTcpClient::lastRecievedMessage() const
{
    return p->client->lastRecievedMessage();
}

void HSmartTcpClient::openSession()
{
    p->client->openSession();
}

void HSmartTcpClient::sendMessage(const QByteArray &m)
{
    QByteArray msg = SMART_LAYER_SEND_MSG_PREFIX + m.mid(0,MAXIMUM_MSG_SIZE);
    p->send_buffer = m.mid(MAXIMUM_MSG_SIZE);
    if( p->send_buffer.isEmpty() )
        msg += SMART_LAYER_SEND_MSG_END;
    else
        msg += SMART_LAYER_SEND_MSG_NOT_END;

    p->client->sendMessage(msg);
}

void HSmartTcpClient::messageRecieved_prev(const QByteArray &m)
{
    if( m.left(SMART_LAYER_SEND_MSG_PREFIX.size()) == SMART_LAYER_SEND_MSG_PREFIX )
    {
        QByteArray msg = m.mid(SMART_LAYER_SEND_MSG_PREFIX.size());
        if( msg.isEmpty() )
            return;

        if( msg.right(SMART_LAYER_SEND_MSG_NOT_END.size()) == SMART_LAYER_SEND_MSG_NOT_END )
        {
            p->read_buffer += msg.left(msg.size()-SMART_LAYER_SEND_MSG_NOT_END.size());
            p->client->sendMessage(SMART_LAYER_NEXT_MSG_PREFIX);
        }
        else
        if( msg.right(SMART_LAYER_SEND_MSG_END.size()) == SMART_LAYER_SEND_MSG_END )
        {
            p->read_buffer += msg.left(msg.size()-SMART_LAYER_SEND_MSG_END.size());
            QByteArray tmp = p->read_buffer;
            p->read_buffer.clear();
            emit messageRecieved(tmp);
        }
    }
    else
    if( m.left(SMART_LAYER_NEXT_MSG_PREFIX.size()) == SMART_LAYER_NEXT_MSG_PREFIX )
    {
        QByteArray msg = m.mid(SMART_LAYER_NEXT_MSG_PREFIX.size());
        Q_UNUSED(msg)
        sendMessage(p->send_buffer);
    }
    else
        qDebug() << "SmartTcpClient error: unknown message recieved.";
}

HSmartTcpClient::~HSmartTcpClient()
{
    delete p;
}
