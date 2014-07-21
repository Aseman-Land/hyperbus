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

#ifndef HSMARTTCPSERVER_H
#define HSMARTTCPSERVER_H

#include <QObject>

class QTcpSocket;
class HSmartTcpServerPrivate;
class HSmartTcpServer : public QObject
{
    Q_OBJECT
public:
    HSmartTcpServer(QObject *parent = 0);
    ~HSmartTcpServer();

    quint16 port() const;

public slots:
    bool openSession(const QString & address, quint32 port);
    void sendMessage(QTcpSocket *socket, const QString & msg);

signals:
    void messageRecieved(QTcpSocket *socket, const QString & msg);

private slots:
    void messageRecieved_prev(QTcpSocket *socket, const QString & msg);

private:
    HSmartTcpServerPrivate *p;
};

#endif // HSMARTTCPSERVER_H
