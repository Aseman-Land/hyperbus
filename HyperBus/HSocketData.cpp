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

#define LSOF_COMMAND "lsof"

#define DATA_COMMAND  data["command"]
#define DATA_PATH     data["path"]
#define DATA_PID      data["pid"]
#define DATA_USER     data["user"]
#define DATA_FD       data["fd"]
#define DATA_PROTOCOL data["protocol"]
#define DATA_DEVICE   data["device"]
#define DATA_SIZE     data["size"]
#define DATA_NODE     data["node"]
#define DATA_ADDRESS  data["address"]
#define DATA_PORT     data["port"]
#define DATA_TYPE     data["type"]

#include "HSocketData.h"

#include <QProcess>
#include <QStringList>
#include <QTcpSocket>
#include <QHostAddress>
#include <QHostInfo>
#include <QHash>
#include <QDebug>
#include <QFileInfo>
#include <QRegExp>

HSocketData::HSocketData(QTcpSocket *s):
    socket(s)
{
    refresh();
}

void HSocketData::refresh()
{
    QProcess prc;

    const QString & cmd = LSOF_COMMAND;
    QStringList args;
        args << "-i";
        args << QString("@%1:%2").arg(socket->peerAddress().toString()).arg(socket->peerPort());

    prc.start( cmd, args );
    if( !prc.waitForStarted() )
        return;
    if( !prc.waitForReadyRead() )
        return;
    if( !prc.waitForFinished() )
        return;

    QStringList res = QString(prc.readAll()).split("\n",QString::SkipEmptyParts).mid(1);
    foreach( const QString & line, res )
    {
        if( !line.contains(QString(":%1->").arg(socket->peerPort())) )
            continue;

        const QStringList & parts = line.split(" ",QString::SkipEmptyParts);
        if( parts.count() < 10 )
            continue;

        DATA_COMMAND  = parts.at(0);
        DATA_PID      = parts.at(1);
        DATA_USER     = parts.at(2);
        DATA_FD       = parts.at(3);
        DATA_PROTOCOL = parts.at(4);
        DATA_DEVICE   = parts.at(5);
        DATA_SIZE     = parts.at(6);
        DATA_NODE     = parts.at(7);
        DATA_TYPE     = parts.at(9);
        DATA_PATH     = QFileInfo( "/proc/" + QString::number(pid()) + "/exe" ).canonicalFilePath();

        const QString & adrs = parts.at(8);
        QStringList adrs_parts = adrs.split("->",QString::SkipEmptyParts);
        int index_adrs = (adrs_parts.isEmpty()? -1 : adrs_parts.first().lastIndexOf(":"));
        if( index_adrs != -1 )
        {
            const QHostInfo & host_info = QHostInfo::fromName(adrs.left(index_adrs));
            DATA_ADDRESS  = (host_info.addresses().isEmpty()? adrs : host_info.addresses().first().toString());
        }
        else
        {
            DATA_ADDRESS = adrs;
            DATA_PORT    = "0";
        }


        break;
    }
}

QString HSocketData::command() const
{
    return DATA_COMMAND;
}

QString HSocketData::commandPath() const
{
    return DATA_PATH;
}

quint64 HSocketData::pid() const
{
    return DATA_PID.toULongLong();
}

QString HSocketData::userName() const
{
    return DATA_USER;
}

QString HSocketData::fd() const
{
    return DATA_FD;
}

HSocketData::Protocol HSocketData::protocol() const
{
    return static_cast<Protocol>(DATA_PROTOCOL.toInt());
}

quint64 HSocketData::device() const
{
    return DATA_DEVICE.toULongLong();
}

QString HSocketData::size() const
{
    return DATA_SIZE;
}

QString HSocketData::node() const
{
    return DATA_NODE;
}

QString HSocketData::address() const
{
    return DATA_ADDRESS;
}

HSocketData::Type HSocketData::type() const
{
    return static_cast<Type>(DATA_TYPE.toInt());
}

HSocketData::~HSocketData()
{
}
