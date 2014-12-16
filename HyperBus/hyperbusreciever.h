/*
    Copyright (C) 2014 Aseman
    http://aseman.co

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

#ifndef HYPERBUSRECIEVER_H
#define HYPERBUSRECIEVER_H

#include "hmsgtransporter.h"
#include "hyperbusglobals.h"
#include "hyperbus_global.h"
#include <QVariant>

class HyperBusRecieverPrivate;
class HYPERBUS_EXPORT HyperBusReciever : public HMsgTransporter
{
    Q_OBJECT
public:
    HyperBusReciever(const QString &address = "127.0.0.1", quint32 port = 25480, QObject *parent = 0);
    ~HyperBusReciever();

    enum SessionType{
        Normal,
        Global,
        Share,
        Local
    };

    /*! Key like: "127.0.0.1:25480" */
    static HyperBusReciever *staticHyperBus(const QString &key, QObject *parent = 0);
    static void destroyStaticHyperBus(const QString &key);

    void setSession( const QString & session );
    void setSession( SessionType type );
    QString session() const;

    void setServicePermission( const QString & service, HyperBusGlobals::PermissionUserType user );
    HyperBusGlobals::PermissionUserType servicePermission( const QString & service );

    void addPidToFirewall( const QString & service, const QString & exe_path );
    void removePidFromFirewall( const QString & service, const QString & exe_path );
    bool firewallContaintsPid( const QString & service, const QString & exe_path );

    void setFireWallMode( const QString & service, HyperBusGlobals::FireWallMode mode );
    HyperBusGlobals::FireWallMode fireWallMode( const QString & service );

    QVariant call( const QString & key , const QVariant & val0 = QVariant(),
                                         const QVariant & val1 = QVariant(),
                                         const QVariant & val2 = QVariant(),
                                         const QVariant & val3 = QVariant(),
                                         const QVariant & val4 = QVariant(),
                                         const QVariant & val5 = QVariant(),
                                         const QVariant & val6 = QVariant(),
                                         const QVariant & val7 = QVariant(),
                                         const QVariant & val8 = QVariant(),
                                         const QVariant & val9 = QVariant() );

    bool registerService( const QString & key, QObject *obj, const char *member, const QString & description, SessionType type = Normal );

    quint64 lastPID() const;

protected:
    QByteArray sendCommand(const QString &commandName, const QList<QByteArray> &args );

    virtual QByteArray callMessage(const QByteArray &msg );
    QByteArray messageEvent(const QByteArray &msg );

protected:
    HyperBusRecieverPrivate *p;
};

#endif // HYPERBUSRECIEVER_H
