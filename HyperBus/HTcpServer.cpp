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

#include "HTcpServer.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QBuffer>

class HTcpServerPrivate
{
public:
    QTcpServer *tcpServer;
    quint16 port;
};

HTcpServer::HTcpServer(QObject *parent):
    QObject(parent)
{
    p = new HTcpServerPrivate;
    p->port = 0;
}

quint16 HTcpServer::port() const
{
    return p->port;
}

bool HTcpServer::openSession(const QString &address, quint32 port)
{
    p->tcpServer = new QTcpServer(this);
    if( !p->tcpServer->listen(QHostAddress(address),port) )
        return false;

    p->port = p->tcpServer->serverPort();

    connect(p->tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
    return true;
}

void HTcpServer::newConnection()
{
    QTcpSocket *socket = p->tcpServer->nextPendingConnection();

    connect(socket, SIGNAL(readyRead()), SLOT(readMessage()));
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
}

void HTcpServer::readMessage()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    const QByteArray & msg = socket->readAll();

    emit messageRecieved( socket, msg );
}

void HTcpServer::sendMessage(QTcpSocket *socket , const QByteArray &msg)
{
    socket->write(msg);
    socket->flush();
}

HTcpServer::~HTcpServer()
{
    delete p;
}
