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

#ifndef HVARIANTCONVERTER_H
#define HVARIANTCONVERTER_H

#include "hvariantconverterunit.h"
#include "hyperbus_global.h"

class HVariantConverterUnit;
class HYPERBUS_EXPORT HVariantConverter
{
public:
    static QByteArray encode( const QVariant & var );
    static QVariant decode(const QByteArray & str );
    static void registerConverterUnit(HVariantConverterUnit *unit );
};

#endif // HVARIANTCONVERTER_H
