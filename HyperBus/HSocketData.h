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

#ifndef HSOCKETDATA_H
#define HSOCKETDATA_H

#include <QHash>

class QTcpSocket;
class HSocketData
{
public:
    HSocketData( QTcpSocket *socket );
    ~HSocketData();

    enum Protocol{
        IPv4,
        IPv6
    };

    enum Type{
        Established,
        Listener
    };

    void refresh();

    QString command() const;
    QString commandPath() const;
    quint64 pid() const;
    QString userName() const;
    QString fd() const;
    Protocol protocol() const;
    quint64 device() const;
    QString size() const;
    QString node() const;
    QString address() const;
    Type type() const;


private:
    QTcpSocket *socket;
    QHash<QString,QString> data;
};

#endif // HSOCKETDATA_H
