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

#ifndef HYPERBUSRECORD_H
#define HYPERBUSRECORD_H

#include <QByteArray>
#include <QList>
#include <cstdio>

#include "hyperbus_global.h"

class HYPERBUS_EXPORT HyperBusRecord
{
public:
    HyperBusRecord();
    HyperBusRecord( const QByteArray & record );

    void operator<<( HyperBusRecord record );
    void operator<<( const QByteArray & str );
    void operator<<( const QList<QByteArray> & list );

    QByteArray operator[]( int index );
    QByteArray at( int index );
    QByteArray last();
    QByteArray first();
    QByteArray takeLast();
    QByteArray takeFirst();
    QList<QByteArray> mid( int i , int len );
    QList<QByteArray> toQByteArrayList();

    void removeAt( int index );
    QByteArray takeAt( int index );

    void FromQByteArray( const QByteArray & str );
    QByteArray toQByteArray();

    int count();
    int size();
    int lenght();

    void clear();

private:
    QList<QByteArray> list;
    QList<int> offsets;

    QByteArray strToRecord( const QByteArray & str );
};

#endif // HYPERBUSRECORD_H
