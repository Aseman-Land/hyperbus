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

#ifndef HVARIANTCONVERTERUNIT_H
#define HVARIANTCONVERTERUNIT_H

#include <QVariant>
#include <QString>
#include <QStringList>

#include "hyperbus_global.h"

class HYPERBUS_EXPORT HVariantConverterUnit
{
public:
    virtual QStringList supportedTypes() const = 0;
    virtual QByteArray encode( const QVariant & var ) = 0;
    virtual QVariant decode( const QByteArray & str ) = 0;
};

#endif // HVARIANTCONVERTERUNIT_H
