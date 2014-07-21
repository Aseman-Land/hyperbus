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

#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QBuffer>
#include <QDebug>
#include <QTime>

#include "HTcpClient.h"

class HTcpClientPrivate
{
public:
    QTcpSocket *tcpSocket;
    QString currentFortune;

    QString ip;
    quint32 port;

    QString last_recieved_msg;

    QHash<QTcpSocket*,QString> buffer;
};

HTcpClient::HTcpClient(QObject *parent) :
    QObject(parent)
{
    p = new HTcpClientPrivate;

    p->tcpSocket = new QTcpSocket(this);

    connect(p->tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    setSession("127.0.0.1",2548);
}

void HTcpClient::setSession(const QString &ip, quint32 port)
{
    p->ip = ip;
    p->port = port;
}

QString HTcpClient::lastRecievedMessage() const
{
    return p->last_recieved_msg;
}

void HTcpClient::openSession()
{
    p->tcpSocket->abort();
    p->tcpSocket->connectToHost(QHostAddress(p->ip),p->port);
    p->tcpSocket->waitForConnected();
}

void HTcpClient::sendMessage(const QString &msg)
{
    p->tcpSocket->write(msg.toUtf8());
    p->tcpSocket->flush();
}

void HTcpClient::readMessage()
{
    QString msg = QString::fromUtf8(p->tcpSocket->readAll());

    p->last_recieved_msg = msg;
    emit messageRecieved( msg );
}

HTcpClient::~HTcpClient()
{
    delete p;
}

