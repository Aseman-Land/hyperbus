/*
    Copyright (C) 2014 Sialan Labs
    http://labs.sialan.org

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

#define GLOBAL_ENCODE( VTYPE, INPUT ) \
    QByteArray result; \
    QBuffer buffer(&result); \
    buffer.open(QBuffer::WriteOnly); \
    QDataStream stream(&buffer); \
    stream << INPUT.value<VTYPE>(); \
    buffer.close(); \
    return result;

#define GLOBAL_DECODE( VTYPE, INPUT ) \
    VTYPE result; \
    QByteArray data = INPUT; \
    QBuffer buffer(&data); \
    buffer.open(QBuffer::ReadOnly); \
    QDataStream stream(&buffer); \
    stream >> result; \
    buffer.close(); \
    return QVariant::fromValue<VTYPE>(result);

#include "HVariantConverterGeneralTypes.h"
#include "HVariantConverter.h"
#include "HyperBusRecord.h"

#include <QImage>
#include <QBitmap>
#include <QFont>
#include <QPixmap>
#include <QColor>
#include <QBrush>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QBuffer>
#include <QDataStream>

bool h_variant_register_default_units();
bool h_variant_default_units_registered = h_variant_register_default_units();

bool h_variant_register_default_units()
{
    if( h_variant_default_units_registered )
        return false;

    HVariantConverter::registerConverterUnit( new HVariantConverterUnitBrush );
    HVariantConverter::registerConverterUnit( new HVariantConverterUnitColor );
    HVariantConverter::registerConverterUnit( new HVariantConverterUnitDate );
    HVariantConverter::registerConverterUnit( new HVariantConverterUnitDateTime );
    HVariantConverter::registerConverterUnit( new HVariantConverterUnitFont );
#ifdef GUI_SUPPORT
    HVariantConverter::registerConverterUnit( new HVariantConverterUnitImage );
    HVariantConverter::registerConverterUnit( new HVariantConverterUnitPixmap );
    HVariantConverter::registerConverterUnit( new HVariantConverterUnitBitmap );
#endif
    HVariantConverter::registerConverterUnit( new HVariantConverterUnitNumber );
    HVariantConverter::registerConverterUnit( new HVariantConverterUnitPoint );
    HVariantConverter::registerConverterUnit( new HVariantConverterUnitSize );
    HVariantConverter::registerConverterUnit( new HVariantConverterUnitString );
    HVariantConverter::registerConverterUnit( new HVariantConverterUnitStringList );
    HVariantConverter::registerConverterUnit( new HVariantConverterUnitTime );
    HVariantConverter::registerConverterUnit( new HVariantConverterUnitPairedList );
    return true;
}

QStringList HVariantConverterUnitString::supportedTypes() const
{
    return QStringList() << "QString";
}

QByteArray HVariantConverterUnitString::encode(const QVariant &var)
{
    return var.toString().toUtf8();
}

QVariant HVariantConverterUnitString::decode(const QByteArray &str)
{
    return QVariant(QString::fromUtf8(str));
}


QStringList HVariantConverterUnitStringList::supportedTypes() const
{
    return QStringList() << "QStringList";
}

QByteArray HVariantConverterUnitStringList::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QStringList,var)
}

QVariant HVariantConverterUnitStringList::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QStringList,str)
}

#ifdef GUI_SUPPORT
QStringList HVariantConverterUnitImage::supportedTypes() const
{
    return QStringList() << "QImage" << "QPixmap" << "QBitmap";
}

QByteArray HVariantConverterUnitImage::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QImage,var)
}

QVariant HVariantConverterUnitImage::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QImage,str)
}

QStringList HVariantConverterUnitPixmap::supportedTypes() const
{
    return QStringList() << "QPixmap";
}

QByteArray HVariantConverterUnitPixmap::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QPixmap,var)
}

QVariant HVariantConverterUnitPixmap::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QPixmap,str)
}


QStringList HVariantConverterUnitBitmap::supportedTypes() const
{
    return QStringList() << "QBitmap";
}

QByteArray HVariantConverterUnitBitmap::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QBitmap,var)
}

QVariant HVariantConverterUnitBitmap::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QBitmap,str)
}
#endif


QStringList HVariantConverterUnitFont::supportedTypes() const
{
    return QStringList() << "QFont";
}

QByteArray HVariantConverterUnitFont::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QFont,var)
}

QVariant HVariantConverterUnitFont::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QFont,str)
}


QStringList HVariantConverterUnitColor::supportedTypes() const
{
    return QStringList() << "QColor";
}

QByteArray HVariantConverterUnitColor::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QColor,var)
}

QVariant HVariantConverterUnitColor::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QColor,str)
}


QStringList HVariantConverterUnitBrush::supportedTypes() const
{
    return QStringList() << "QBrush";
}

QByteArray HVariantConverterUnitBrush::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QBrush,var)
}

QVariant HVariantConverterUnitBrush::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QBrush,str)
}


QStringList HVariantConverterUnitDate::supportedTypes() const
{
    return QStringList() << "QDate";
}

QByteArray HVariantConverterUnitDate::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QDate,var)
}

QVariant HVariantConverterUnitDate::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QDate,str)
}


QStringList HVariantConverterUnitTime::supportedTypes() const
{
    return QStringList() << "QTime";
}

QByteArray HVariantConverterUnitTime::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QTime,var)
}

QVariant HVariantConverterUnitTime::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QTime,str)
}


QStringList HVariantConverterUnitDateTime::supportedTypes() const
{
    return QStringList() << "QDateTime";
}

QByteArray HVariantConverterUnitDateTime::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QDateTime,var)
}

QVariant HVariantConverterUnitDateTime::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QDateTime,str)
}


QStringList HVariantConverterUnitSize::supportedTypes() const
{
    return QStringList() << "QSize";
}

QByteArray HVariantConverterUnitSize::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QSize,var)
}

QVariant HVariantConverterUnitSize::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QSize,str)
}


QStringList HVariantConverterUnitSizeF::supportedTypes() const
{
    return QStringList() << "QSizeF";
}

QByteArray HVariantConverterUnitSizeF::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QSizeF,var)
}

QVariant HVariantConverterUnitSizeF::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QSizeF,str)
}


QStringList HVariantConverterUnitPoint::supportedTypes() const
{
    return QStringList() << "QPoint";
}

QByteArray HVariantConverterUnitPoint::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QPoint,var)
}

QVariant HVariantConverterUnitPoint::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QPoint,str)
}


QStringList HVariantConverterUnitPointF::supportedTypes() const
{
    return QStringList() << "QPointF";
}

QByteArray HVariantConverterUnitPointF::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QPointF,var)
}

QVariant HVariantConverterUnitPointF::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QPointF,str)
}


QStringList HVariantConverterUnitRect::supportedTypes() const
{
    return QStringList() << "QRect";
}

QByteArray HVariantConverterUnitRect::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QRect,var)
}

QVariant HVariantConverterUnitRect::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QRect,str)
}


QStringList HVariantConverterUnitRectF::supportedTypes() const
{
    return QStringList() << "QRectF";
}

QByteArray HVariantConverterUnitRectF::encode(const QVariant &var)
{
    GLOBAL_ENCODE(QRectF,var)
}

QVariant HVariantConverterUnitRectF::decode(const QByteArray &str)
{
    GLOBAL_DECODE(QRectF,str)
}


QStringList HVariantConverterUnitNumber::supportedTypes() const
{
    return QStringList() << "qulonglong" << "int" << "qlonglong" << "uint" << "double" << "float";
}

QByteArray HVariantConverterUnitNumber::encode(const QVariant &var)
{
    return QByteArray::number(var.toDouble());
}

QVariant HVariantConverterUnitNumber::decode(const QByteArray &str)
{
    return str.toDouble();
}


HVariantConverterUnitPairedList::HVariantConverterUnitPairedList()
{
    qRegisterMetaType<PairedList>("PairedList");
}

QStringList HVariantConverterUnitPairedList::supportedTypes() const
{
    return QStringList() << "PairedList" << "QList<QPair<QString,QString> >" << "QList<QPair<QString,QString>>";
}

QByteArray HVariantConverterUnitPairedList::encode(const QVariant &var)
{
    const PairedList & list = var.value<PairedList>();
    HyperBusRecord res;

    QListIterator< QPair<QString,QString> > i(list);
    while( i.hasNext() )
    {
        const QPair<QString,QString> & pair = i.next();
        HyperBusRecord pair_record;
            pair_record << pair.first.toUtf8();
            pair_record << pair.second.toUtf8();

        res << pair_record.toQByteArray();
    }

    return res.toQByteArray();
}

QVariant HVariantConverterUnitPairedList::decode(const QByteArray &str)
{
    PairedList res;

    HyperBusRecord list(str);
    for( int i=0 ; i<list.count() ; i++ )
    {
        HyperBusRecord pair_record(list.at(i));
        if( pair_record.count() != 2 )
            continue;

        QPair<QString,QString> pair( pair_record.at(0), pair_record.at(1) );
        res << pair;
    }

    return QVariant::fromValue<PairedList>(res);
}
