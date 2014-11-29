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

#include "hvariantconverter.h"
#include "hvariantconverterunit.h"

#include <QHash>
#include <QDebug>

QHash<QString,HVariantConverterUnit*> h_variant_converter_units;

QByteArray HVariantConverter::encode(const QVariant &var)
{
    if( !h_variant_converter_units.contains(var.typeName()) )
        return QByteArray();

    return QByteArray(var.typeName()) + ":" + h_variant_converter_units[var.typeName()]->encode(var);
}

QVariant HVariantConverter::decode(const QByteArray &str)
{
    int index = str.indexOf(":");
    if( index == -1 )
        return QVariant();

    const QByteArray & type = str.mid(0,index);
    const QByteArray & data = str.mid(index+1);

    if( !h_variant_converter_units.contains(type) )
        return QVariant();

    return h_variant_converter_units[type]->decode(data);
}

void HVariantConverter::registerConverterUnit( HVariantConverterUnit *unit)
{
    const QStringList & types = unit->supportedTypes();
    foreach( const QString & type, types )
        h_variant_converter_units.insert( type, unit );
}
