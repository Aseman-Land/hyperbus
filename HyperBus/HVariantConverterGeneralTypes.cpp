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
    HVariantConverter::registerConverterUnit( new HVariantConverterUnitImage );
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

QString HVariantConverterUnitString::encode(const QVariant &var)
{
    return var.toString();
}

QVariant HVariantConverterUnitString::decode(const QString &str)
{
    return QVariant(str);
}


QStringList HVariantConverterUnitStringList::supportedTypes() const
{
    return QStringList() << "QStringList";
}

QString HVariantConverterUnitStringList::encode(const QVariant &var)
{
    HyperBusRecord record;
    const QStringList & list = var.toStringList();
    foreach( const QString & str, list )
        record << str;

    return record.toQSting();
}

QVariant HVariantConverterUnitStringList::decode(const QString &str)
{
    HyperBusRecord record( str );
    return record.toQStringList();
}

#ifdef GUI_SUPPORT
QStringList HVariantConverterUnitImage::supportedTypes() const
{
    return QStringList() << "QImage" << "QPixmap" << "QBitmap";
}

QString HVariantConverterUnitImage::encode(const QVariant &var)
{
    QString pre = QString(var.typeName());
    QImage img;
    switch( static_cast<int>(var.type()) )
    {
    case QVariant::Image:
        img = var.value<QImage>();
        break;

    case QVariant::Bitmap:
        img = var.value<QBitmap>().toImage();
        break;

    case QVariant::Pixmap:
        img = var.value<QPixmap>().toImage();
        break;

    default:
        return QString();
    }

    QString res = QString("%1:%2x%3:").arg(pre).arg(img.width()).arg(img.height());

    int cnt = 0;
    QString last;
    for( int i=0 ; i<img.height() ; i++ )
        for( int j=0 ; j<img.width() ; j++ )
        {
            const QString & clr = QByteArray::fromHex(QColor(img.pixel(j,i)).name().mid(1).toLatin1()).toBase64();
            if( last.isEmpty() )
            {
                last = clr;
                cnt++;
                continue;
            }
            else
            {
                if( last == clr )
                {
                    cnt++;
                    continue;
                }
                else
                {
                    if( cnt != 1 )
                        res += QString("%1%2:").arg(cnt).arg(last);
                    else
                        res += QString("%1:").arg(last);

                    cnt = 0;

                    last = clr;
                    cnt++;
                    continue;
                }
            }
        }

    if( !last.isEmpty() )
        res += QString("%1%2:").arg(cnt).arg(last);

    return res;
}

QVariant HVariantConverterUnitImage::decode(const QString &str)
{
    int index = str.indexOf(":");
    if( index == -1 )
        return QVariant();

    QString type = str.mid(0,index);
    QString txt = str.mid(index+1);
    QImage img;

    int cnt = -1;
    while( !txt.isEmpty() )
    {
        int index = txt.indexOf(':');
        QString section = txt.mid(0,index);

        if( cnt == -1 )
        {
            img = QImage( section.split("x").first().toInt(), section.split("x").last().toInt() , QImage::Format_ARGB32 );
            cnt = 0;
            txt.remove(0,index+1);
            continue;
        }

        int rpt = section.left( section.size()-4 ).toInt();
        if( rpt == 0 )
            rpt = 1;

        QString c_name = QString("#%1").arg( QString(QByteArray::fromBase64(section.right(4).toLatin1()).toHex()).rightJustified(6,'0') );
        for( int i=0 ; i<rpt ; i++ )
        {
            img.setPixel( cnt%img.width(), (int)cnt/img.width(), QColor(c_name).rgb() );
            cnt++;
        }

        txt.remove(0,index+1);
    }

    int t = QVariant::nameToType(type.toLatin1());
    switch( t )
    {
    case QVariant::Image:
        return img;
        break;

    case QVariant::Bitmap:
        return QPixmap::fromImage(img);
        break;

    case QVariant::Pixmap:
        return QPixmap::fromImage(img);
        break;

    default:
        return QVariant();
    }
}
#endif


QStringList HVariantConverterUnitFont::supportedTypes() const
{
    return QStringList() << "QFont";
}

QString HVariantConverterUnitFont::encode(const QVariant &var)
{
    QFont fnt = var.value<QFont>();
    return QString("%1,%2,%3,%4,%5,%6,%7").arg(fnt.family())
                                          .arg(fnt.pointSize())
                                          .arg(fnt.weight())
                                          .arg((int)fnt.bold())
                                          .arg((int)fnt.italic())
                                          .arg((int)fnt.strikeOut())
                                          .arg((int)fnt.underline());
}

QVariant HVariantConverterUnitFont::decode(const QString &str)
{
    const QStringList & list = str.split(",");
    if( list.count() != 7 )
        return QFont();

    QFont res;
        res.setFamily( list.at(0) );
        res.setPointSize( list.at(1).toInt() );
        res.setWeight( list.at(2).toInt() );
        res.setBold( list.at(3).toInt() );
        res.setItalic( list.at(4).toInt() );
        res.setStrikeOut( list.at(5).toInt() );
        res.setUnderline( list.at(6).toInt() );

    return QVariant::fromValue<QFont>(res);
}


QStringList HVariantConverterUnitColor::supportedTypes() const
{
    return QStringList() << "QColor";
}

QString HVariantConverterUnitColor::encode(const QVariant &var)
{
    return var.value<QColor>().name();
}

QVariant HVariantConverterUnitColor::decode(const QString &str)
{
    return QVariant::fromValue<QColor>(QColor( str ));
}


QStringList HVariantConverterUnitBrush::supportedTypes() const
{
    return QStringList() << "QBrush";
}

QString HVariantConverterUnitBrush::encode(const QVariant &var)
{
    QBrush brsh = var.value<QBrush>();
    return QString("%1,%2").arg( brsh.color().name(), QString::number(brsh.style()) );
}

QVariant HVariantConverterUnitBrush::decode(const QString &str)
{
    const QStringList & list = str.split(",",QString::SkipEmptyParts);
    if( list.count() != 2 )
        return QBrush();

    QBrush res = QBrush( QColor(list.at(0)), static_cast<Qt::BrushStyle>(list.at(1).toInt()) );
    return QVariant::fromValue<QBrush>(res);
}


QStringList HVariantConverterUnitDate::supportedTypes() const
{
    return QStringList() << "QDate";
}

QString HVariantConverterUnitDate::encode(const QVariant &var)
{
    return var.value<QDate>().toString();
}

QVariant HVariantConverterUnitDate::decode(const QString &str)
{
    return QVariant::fromValue<QDate>(QDate::fromString(str));
}


QStringList HVariantConverterUnitTime::supportedTypes() const
{
    return QStringList() << "QTime";
}

QString HVariantConverterUnitTime::encode(const QVariant &var)
{
    return var.value<QTime>().toString();
}

QVariant HVariantConverterUnitTime::decode(const QString &str)
{
    return QVariant::fromValue<QTime>(QTime::fromString(str));
}


QStringList HVariantConverterUnitDateTime::supportedTypes() const
{
    return QStringList() << "QDateTime";
}

QString HVariantConverterUnitDateTime::encode(const QVariant &var)
{
    return var.value<QDateTime>().toString();
}

QVariant HVariantConverterUnitDateTime::decode(const QString &str)
{
    return QVariant::fromValue<QDateTime>( QDateTime::fromString(str) );
}


QStringList HVariantConverterUnitSize::supportedTypes() const
{
    return QStringList() << "QSize" << "QSizeF";
}

QString HVariantConverterUnitSize::encode(const QVariant &var)
{
    QString pre = QString(var.typeName());
    QSizeF size;
    switch( static_cast<int>(var.type()) )
    {
    case QVariant::Size:
        size = var.toSize();
        break;

    case QVariant::SizeF:
        size = var.toSizeF();
        break;

    default:
        return QString();
    }

    return QString("%1:%2x%3").arg(pre).arg(size.width()).arg(size.height());
}

QVariant HVariantConverterUnitSize::decode(const QString &str)
{
    int index = str.indexOf(":");
    if( index == -1 )
        return QVariant();

    QString type = str.mid(0,index);
    QString txt = str.mid(index+1);

    const QStringList & list = txt.split("x");
    if( list.count() != 2 )
        return QVariant();

    QSizeF res = QSizeF( list.first().toDouble(), list.last().toDouble() );

    int t = QVariant::nameToType(type.toLatin1());
    switch( t )
    {
    case QVariant::Size:
        return QVariant::fromValue<QSize>(res.toSize());
        break;

    case QVariant::SizeF:
        return QVariant::fromValue<QSizeF>(res);
        break;

    default:
        return QVariant();
    }
}


QStringList HVariantConverterUnitPoint::supportedTypes() const
{
    return QStringList() << "QPoint" << "QPointF";
}

QString HVariantConverterUnitPoint::encode(const QVariant &var)
{
    QString pre = QString(var.typeName());
    QPointF point;
    switch( static_cast<int>(var.type()) )
    {
    case QVariant::Point:
        point = var.toPoint();
        break;

    case QVariant::PointF:
        point = var.toPointF();
        break;

    default:
        return QString();
    }

    return QString("%1:%2,%3").arg(pre).arg(point.x()).arg(point.y());
}

QVariant HVariantConverterUnitPoint::decode(const QString &str)
{
    int index = str.indexOf(":");
    if( index == -1 )
        return QVariant();

    QString type = str.mid(0,index);
    QString txt = str.mid(index+1);

    const QStringList & list = txt.split(",");
    if( list.count() != 2 )
        return QVariant();

    QPointF res = QPointF( list.first().toDouble(), list.last().toDouble() );

    int t = QVariant::nameToType(type.toLatin1());
    switch( t )
    {
    case QVariant::Point:
        return QVariant::fromValue<QPoint>(res.toPoint());
        break;

    case QVariant::PointF:
        return QVariant::fromValue<QPointF>(res);
        break;

    default:
        return QVariant();
    }
}


QStringList HVariantConverterUnitNumber::supportedTypes() const
{
    return QStringList() << "qulonglong" << "int" << "qlonglong" << "uint" << "double" << "float";
}

QString HVariantConverterUnitNumber::encode(const QVariant &var)
{
    return QString::number(var.toReal());
}

QVariant HVariantConverterUnitNumber::decode(const QString &str)
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

QString HVariantConverterUnitPairedList::encode(const QVariant &var)
{
    const PairedList & list = var.value<PairedList>();
    HyperBusRecord res;

    QListIterator< QPair<QString,QString> > i(list);
    while( i.hasNext() )
    {
        const QPair<QString,QString> & pair = i.next();
        HyperBusRecord pair_record;
            pair_record << pair.first;
            pair_record << pair.second;

        res << pair_record.toQSting();
    }

    return res.toQSting();
}

QVariant HVariantConverterUnitPairedList::decode(const QString &str)
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
