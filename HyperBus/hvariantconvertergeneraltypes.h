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

#ifndef HVARIANTCONVERTERGENERALTYPES_H
#define HVARIANTCONVERTERGENERALTYPES_H

#include "hvariantconverterunit.h"
#include "hyperbus_global.h"

class HYPERBUS_EXPORT HVariantConverterUnitString: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitStringList: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

#ifdef GUI_SUPPORT
class HYPERBUS_EXPORT HVariantConverterUnitImage: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitPixmap: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitBitmap: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};
#endif

class HYPERBUS_EXPORT HVariantConverterUnitFont: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitColor: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitBrush: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitDate: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitTime: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitDateTime: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitSize: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitSizeF: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitPoint: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitPointF: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitRect: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitRectF: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitNumber: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

class HYPERBUS_EXPORT HVariantConverterUnitPairedList: public HVariantConverterUnit
{
public:
    HVariantConverterUnitPairedList();
    QStringList supportedTypes() const;
    QByteArray encode(const QVariant &var);
    QVariant decode(const QByteArray &str);
};

typedef QList< QPair<QString,QString> > PairedList;
Q_DECLARE_METATYPE(PairedList)

#endif // HVARIANTCONVERTERGENERALTYPES_H
