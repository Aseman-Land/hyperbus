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

#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <QObject>
#include <QPair>
#include <QList>
#include <QGenericArgument>
#include <QVariant>
#include <QStringList>

#include "hyperbus_global.h"

typedef QList< QPair<QString,QString> > HighWayArgs;

/*!
 * \class
 * HighWay is a high way that allow plugins to talking to another plugins.
 * HighWay provide this feature using Qt MetaObject system. It is a DBus-like system.
 * Every Object register a key and members to HighWay, Then other plugins from another place
 * Using member of registered objects.
 */
class HighWayPrivate;
class HYPERBUS_EXPORT HighWay : public QObject
{
    Q_OBJECT

    friend class HighWayItemModerator;
public:
    HighWay(QObject *parent = 0);
    ~HighWay();

    static HighWay *staticHighWay(const QString &key, QObject *parent = 0);
    static void destroyStaticHighWay(const QString &key);

    enum Type{
        Invalid,
        Slot,
        Signal,
        Property
    };


    Q_INVOKABLE void registerAllMethods(const QString & parent_key , QObject *obj );
    Q_INVOKABLE bool registerFromName(const QString & key , QObject *obj , const QString &method , const QString & description = QString() );
    Q_INVOKABLE bool registerMethod(const QString & key , QObject *obj , const char * member , const QString & description = QString() );
    Q_INVOKABLE bool registerProperty( const QString & key , QObject *obj , const QString & mamber , const QString & description = QString() );

    Q_INVOKABLE bool remove( const QString & key , QObject *obj );

    Q_INVOKABLE QVariant call( const QString & key , QVariant val0 = QVariant(),
                                                     QVariant val1 = QVariant(),
                                                     QVariant val2 = QVariant(),
                                                     QVariant val3 = QVariant(),
                                                     QVariant val4 = QVariant(),
                                                     QVariant val5 = QVariant(),
                                                     QVariant val6 = QVariant(),
                                                     QVariant val7 = QVariant(),
                                                     QVariant val8 = QVariant(),
                                                     QVariant val9 = QVariant(), bool *ok = 0 );

    Q_INVOKABLE bool connectToSignal( const QString & key , QObject *reciver , const char * member );

    Q_INVOKABLE bool setProperty( const QString & key , const QVariant & value );
    Q_INVOKABLE QVariant property( const QString & key );

    Q_INVOKABLE static QString makeKeyAbsolute( const QString & key );

    Q_INVOKABLE bool contains( const QString & key );

    Q_INVOKABLE const QVariantList &lastCalledArguments();
    Q_INVOKABLE const QString &lastCalledKey();

    Q_INVOKABLE bool changeParamaters( const QString & key , QObject *obj , const HighWayArgs & new_params );

    Q_INVOKABLE HighWayArgs paramatersOf( const QString & key );
    Q_INVOKABLE Type typeOf( const QString & key );
    Q_INVOKABLE QString returnTypeOf( const QString & key );
    Q_INVOKABLE QString descriptionOf( const QString & key );
    Q_INVOKABLE QStringList keys( const QString & key );
    Q_INVOKABLE QStringList keys();
    Q_INVOKABLE int count();

    Q_INVOKABLE int callCount();

signals:
    void added( const QString & kay );
    void removed( const QString & kay );

    void methodAdded( const QString & key );
    void propertyAdded( const QString & key );

private slots:
    void objectRemoved( QObject *obj );

private:
    HighWayPrivate *p;
};

#endif // HIGHWAY_H
