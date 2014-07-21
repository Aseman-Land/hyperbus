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

#ifndef HYPERBUSRECORD_H
#define HYPERBUSRECORD_H

#include <QString>
#include <QStringList>
#include <QList>
#include <cstdio>

class HyperBusRecord
{
public:
    HyperBusRecord();
    HyperBusRecord( const QString & record );

    void operator<<( HyperBusRecord record );
    void operator<<( const QString & str );
    void operator<<( const QStringList & list );

    QString operator[]( int index );
    QString at( int index );
    QString last();
    QString first();
    QString takeLast();
    QString takeFirst();
    QStringList mid( int i , int len );
    QStringList toQStringList();

    void removeAt( int index );
    QString takeAt( int index );

    void FromQString( const QString & str );
    QString toQSting();

    int count();
    int size();
    int lenght();

    void clear();

private:
    QStringList list;
    QList<int> offsets;

    QString strToRecord( const QString & str );
};

#endif // HYPERBUSRECORD_H
