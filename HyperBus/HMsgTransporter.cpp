/*
    Copyright (C) 2014 Sialan Labs
    http://labs.sialan.org

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

#define NAME_OF( FUNCTION ) #FUNCTION

#include "HMsgTransporter.h"
#include "HSmartTcpClient.h"
#include "HyperBusGlobals.h"
#include "hyperbus_macros.h"

#include <QMutex>
#include <QThread>
#include <QDebug>
#include <QWaitCondition>
#include <QEventLoop>
#include <QQueue>

class HMsgTransporterPrivate
{
public:
    HSmartTcpClient *client;
    QThread *thread;

    QWaitCondition transfare_waitc;
    QMutex transfare_mutex;

    QMutex mutex;
    QByteArray last_recieved_msg;

    QQueue<QByteArray> queued_call;

    HMsgMiddleTransfare_prev *prev_obj;
    int wait;

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
    p->wait   = 0;

    connect(p->thread, SIGNAL(started()), p->client, SLOT(openSession()));
    connect(p->thread, SIGNAL(finished()), p->client, SLOT(deleteLater()));
    connect(p->client, SIGNAL(messageRecieved(QByteArray)), p->prev_obj, SLOT(messageRecieved(QByteArray)) );

    p->client->moveToThread(p->thread);
    p->prev_obj->moveToThread(p->thread);
    p->thread->start();
}

QByteArray HMsgTransporter::transfare(const QByteArray &msg)
{
    p->mutex.lock();
    p->wait++;
    int wait_id = p->wait;
    p->mutex.unlock();

    QByteArray msg_prv = CLIENT_MSG_PREFIX + msg;
    QMetaObject::invokeMethod( p->client, NAME_OF(sendMessage), Q_ARG(QByteArray,msg_prv) );

    quint64 wait_time = 1;
    while( p->wait == wait_id )
    {
        p->mutex.lock();
        if( !p->queued_call.isEmpty() )
        {
            QByteArray msg = p->queued_call.takeFirst();
            p->mutex.unlock();
            messageEvent_prev(msg);
        }
        else
            p->mutex.unlock();

        QThread::usleep( 1 );
        if( wait_time < 10000 )
            wait_time += 100;
    }

    QByteArray res;
    p->mutex.lock();
    res = p->last_recieved_msg;
    p->mutex.unlock();

    return res;
}

void HMsgTransporter::messageEvent_prev(const QByteArray &msg)
{
    const QByteArray & res = SERVER_MSG_PREFIX + messageEvent(msg);
    QMetaObject::invokeMethod( p->client, NAME_OF(sendMessage), Q_ARG(QByteArray,res) );
}

QByteArray HMsgTransporter::messageEvent(const QByteArray &msg)
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

void HMsgMiddleTransfare_prev::messageRecieved(const QByteArray &msg)
{
    if( msg.left(CLIENT_MSG_PREFIX.size()) == CLIENT_MSG_PREFIX )
    {
        p->p->mutex.lock();
        p->p->last_recieved_msg = msg.mid(CLIENT_MSG_PREFIX.size());
        p->p->wait--;
        if( p->p->wait < 0 )
            p->p->wait = 0;

        p->p->mutex.unlock();
    }
    else
    if( msg.left(SERVER_MSG_PREFIX.size()) == SERVER_MSG_PREFIX )
    {
        QByteArray msg_prv = msg.mid(SERVER_MSG_PREFIX.size());
        if( p->p->wait != 0 )
        {
            p->p->mutex.lock();
            p->p->queued_call.append(msg_prv);
            p->p->mutex.unlock();
        }
        else
        {
            QMetaObject::invokeMethod( p, NAME_OF(messageEvent_prev), Q_ARG(QByteArray,msg_prv) );
        }
    }
    else
        qDebug() << "Unknown message recieved.";
}
