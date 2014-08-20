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

#include "HyperBusRecord.h"
#include <iostream>

#include <QChar>

HyperBusRecord::HyperBusRecord()
{
    offsets.clear();
    offsets << 0;
}

HyperBusRecord::HyperBusRecord( const QByteArray & record )
{
    offsets.clear();
    offsets << 0;
    this->FromQByteArray( record );
}

void HyperBusRecord::operator<<( HyperBusRecord record )
{
    for( int i=0 ; ; i++ )
        this->operator <<( record[i] );
}

void HyperBusRecord::operator<<( const QByteArray & str )
{
    list << this->strToRecord( str );
    offsets << offsets.last() + list.last().size();
}

void HyperBusRecord::operator<<( const QList<QByteArray> & list )
{
    for( int i=0 ; i<list.count() ; i++ )
        this->operator <<( list[i] );
}

QByteArray HyperBusRecord::operator[]( int index )
{
    QByteArray result = list.at(index);

    int cnt = result.size();
    for( int i=0 ; i<cnt ; i++ )
    {
        if( result.at(0)==',' )
        {
            result.remove(0,1);
            break;
        }
        if( !QChar(result.at(0)).isNumber() )
        {
            break;
        }
        result.remove(0,1);
    }
    return result;
}

QByteArray HyperBusRecord::at( int i )
{
    return this->operator [](i);
}

QByteArray HyperBusRecord::last()
{
    return this->at( this->count()-1 );
}

QByteArray HyperBusRecord::first()
{
    return this->at(0);
}

QByteArray HyperBusRecord::takeLast()
{
    return this->takeAt( this->count()-1 );
}

QByteArray HyperBusRecord::takeFirst()
{
    return this->takeAt(0);
}

QList<QByteArray> HyperBusRecord::mid( int index , int len )
{
    QList<QByteArray> res;
    for( int i=index ; i<index+len ; i++ )
        res << this->operator [](i);

    return res;
}

QList<QByteArray> HyperBusRecord::toQByteArrayList()
{
    return this->mid( 0 , this->count() );
}

void HyperBusRecord::removeAt( int index )
{
    int shift_size = list.at(index).size();
    list.removeAt( index );

    for( int i=index+1 ; i<offsets.count() ; i++ )
        offsets[i] -= shift_size;
}

QByteArray HyperBusRecord::takeAt( int index )
{
    QByteArray tmp( this->at(index) );
    this->removeAt( index );
    return tmp;
}

void HyperBusRecord::FromQByteArray( const QByteArray & str )
{
    int data_size = str.size();
    bool ok;

    int ext = offsets.last();
    for( int i=0,j=0 ; i<data_size ; i++ )
    {
        if( str[i] == ',' )
        {
            QByteArray tmp = str.mid( j , i-j );
            j += tmp.toInt( &ok );
            offsets << j+ext;

            if( j > i )
                i=j;
        }
    }

    for( int i=0 ; i<offsets.count()-1 ; i++ )
        list << str.mid( offsets[i] , offsets[i+1]-offsets[i] );
}

QByteArray HyperBusRecord::toQByteArray()
{
    QByteArray result;
    foreach( const QByteArray & ba, list )
        result += ba;

    return result;
}

int HyperBusRecord::count()
{
    return this->list.count();
}

int HyperBusRecord::size()
{
    return this->count();
}

int HyperBusRecord::lenght()
{
    return this->count();
}

void HyperBusRecord::clear()
{
    list.clear();
    offsets.clear();
    offsets << 0;
}

QByteArray HyperBusRecord::strToRecord(const QByteArray &str )
{
    char str_size[13];
    QByteArray record("0," + str);
    int size;

    do
    {
        size = record.count();
        sprintf( str_size , "%d" , size  );
        record = "," + str;
        record.prepend( str_size );
    }while( size != record.count() );

    return record;
}
