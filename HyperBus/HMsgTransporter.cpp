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

#include "HMsgTransporter.h"
#include "HSmartTcpClient.h"
#include "HyperBusGlobals.h"
#include "hyperbus_macros.h"

#include <QMutex>
#include <QThread>
#include <QDebug>
#include <QWaitCondition>

class HMsgTransporterPrivate
{
public:
    HSmartTcpClient *client;
    QThread *thread;

    QWaitCondition transfare_waitc;
    QMutex transfare_mutex;

    QMutex mutex;
    QString last_recieved_msg;

    HMsgMiddleTransfare_prev *prev_obj;
    bool wait;

    QString address;
    quint32 port;
};

HMsgTransporter::HMsgTransporter(const QString &address, quint32 port, QObject *parent) :
    QObject(parent)
{
    p = new HMsgTransporterPrivate;
    p->address = address;
    p->port = port;

    p->thread = new QThread( this );
    p->client = new HSmartTcpClient();
        p->client->setSession(p->address,p->port);

    p->prev_obj = new HMsgMiddleTransfare_prev(this);
    p->wait   = false;

    connect(p->thread, SIGNAL(started()), p->client, SLOT(openSession()));
    connect(p->thread, SIGNAL(finished()), p->client, SLOT(deleteLater()));
    connect(p->client, SIGNAL(messageRecieved(QString)), p->prev_obj, SLOT(messageRecieved(QString)) );

    p->client->moveToThread(p->thread);
    p->prev_obj->moveToThread(p->thread);
    p->thread->start();
}

QString HMsgTransporter::transfare(const QString &msg)
{
    p->wait = true;
    QString msg_prv = CLIENT_MSG_PREFIX + msg;
    QMetaObject::invokeMethod( p->client, "sendMessage", Q_ARG(QString,msg_prv) );

    quint64 wait_time = 1;
    while( p->wait )
    {
        QThread::usleep( 1 );
        if( wait_time < 10000 )
            wait_time+=10;
    }

    QString res;
    p->mutex.lock();
    res = p->last_recieved_msg;
    p->mutex.unlock();

    return res;
}

void HMsgTransporter::messageEvent_prev(const QString &msg)
{
    const QString & res = SERVER_MSG_PREFIX + messageEvent(msg);
    QMetaObject::invokeMethod( p->client, "sendMessage", Q_ARG(QString,res) );
}

QString HMsgTransporter::messageEvent(const QString &msg)
{
    return msg;
}

void HMsgTransporter::openSession()
{
    p->client->openSession();
}

HMsgTransporter::~HMsgTransporter()
{
    delete p;
}

void HMsgMiddleTransfare_prev::messageRecieved(const QString &msg)
{
    if( msg.left(CLIENT_MSG_PREFIX.size()) == CLIENT_MSG_PREFIX )
    {
        p->p->mutex.lock();
        p->p->last_recieved_msg = msg.mid(CLIENT_MSG_PREFIX.size());
        p->p->mutex.unlock();
        p->p->wait = false;
    }
    else
    if( msg.left(SERVER_MSG_PREFIX.size()) == SERVER_MSG_PREFIX )
    {
        QString msg_prv = msg.mid(SERVER_MSG_PREFIX.size());
        QMetaObject::invokeMethod( p, "messageEvent_prev", Q_ARG(QString,msg_prv) );
    }
    else
        qDebug() << "Unknown message recieved.";
}
