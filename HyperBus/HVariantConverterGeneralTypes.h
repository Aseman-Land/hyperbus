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

#ifndef HVARIANTCONVERTERGENERALTYPES_H
#define HVARIANTCONVERTERGENERALTYPES_H

#include "HVariantConverterUnit.h"

class HVariantConverterUnitString: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QString encode(const QVariant &var);
    QVariant decode(const QString &str);
};

class HVariantConverterUnitStringList: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QString encode(const QVariant &var);
    QVariant decode(const QString &str);
};

#ifdef GUI_SUPPORT
class HVariantConverterUnitImage: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QString encode(const QVariant &var);
    QVariant decode(const QString &str);
};
#endif

class HVariantConverterUnitFont: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QString encode(const QVariant &var);
    QVariant decode(const QString &str);
};

class HVariantConverterUnitColor: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QString encode(const QVariant &var);
    QVariant decode(const QString &str);
};

class HVariantConverterUnitBrush: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QString encode(const QVariant &var);
    QVariant decode(const QString &str);
};

class HVariantConverterUnitDate: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QString encode(const QVariant &var);
    QVariant decode(const QString &str);
};

class HVariantConverterUnitTime: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QString encode(const QVariant &var);
    QVariant decode(const QString &str);
};

class HVariantConverterUnitDateTime: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QString encode(const QVariant &var);
    QVariant decode(const QString &str);
};

class HVariantConverterUnitSize: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QString encode(const QVariant &var);
    QVariant decode(const QString &str);
};

class HVariantConverterUnitPoint: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QString encode(const QVariant &var);
    QVariant decode(const QString &str);
};

class HVariantConverterUnitNumber: public HVariantConverterUnit
{
public:
    QStringList supportedTypes() const;
    QString encode(const QVariant &var);
    QVariant decode(const QString &str);
};

class HVariantConverterUnitPairedList: public HVariantConverterUnit
{
public:
    HVariantConverterUnitPairedList();
    QStringList supportedTypes() const;
    QString encode(const QVariant &var);
    QVariant decode(const QString &str);
};

typedef QList< QPair<QString,QString> > PairedList;
Q_DECLARE_METATYPE(PairedList)

#endif // HVARIANTCONVERTERGENERALTYPES_H
