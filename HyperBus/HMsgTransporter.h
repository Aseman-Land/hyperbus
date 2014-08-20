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

#ifndef HMSGTRANSPORTER_H
#define HMSGTRANSPORTER_H

#include <QObject>

class HMsgTransporterPrivate;
class HMsgTransporter : public QObject
{
    friend class HMsgMiddleTransfare_prev;
    Q_OBJECT
public:
    HMsgTransporter(const QString &address, quint32 port, QObject *parent = 0);
    ~HMsgTransporter();

public slots:
    QByteArray transfare(const QByteArray &msg );

private slots:
    void openSession();
    void messageEvent_prev(const QByteArray &msg );

protected:
    virtual QByteArray messageEvent(const QByteArray &msg );

private:
    HMsgTransporterPrivate *p;
};

class HMsgMiddleTransfare_prev: public QObject
{
    friend class HMsgTransporter;
    Q_OBJECT
private:
    HMsgMiddleTransfare_prev( HMsgTransporter *parent ){
        p = parent;
    }

public slots:
    void messageRecieved(const QByteArray &msg);

private:
    HMsgTransporter *p;
};

#endif // HMSGTRANSPORTER_H
