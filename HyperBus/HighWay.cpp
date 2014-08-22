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
#define HIGHWAY_INS_EXIST_ER      "Error: HighWay::register : \"%1\" key is existed on the HighWay map. Please choose another key or remove this key first."
#define HIGHWAY_RM_DONT_EXIST_ER  "Error: HighWay::remove : \"%1\" key is not exist on the HighWay map."
#define HIGHWAY_WRONG_OBJ_ER      "Error: HighWay::remove : \"%1\" object is wrong. Object useing in signature role to remove items."
#define HIGHWAY_CALL_KEY_EXIST_ER "Error: HighWay::call : \"%1\" key is not exists on the HighWay map."
#define HIGHWAY_INCOMBATIBLE_ER   "Error: HighWay::call : \"%1\" Incompatible using member."
#define HIGHWAY_CALL_ARGS_CNT_FTL "Warning: HighWay::call : Number of the input arguments is less."
#define HIGHWAY_CALL_TYPE_FTL( key ) "Warning: HighWay::call \"" + key.toLatin1() + "\" : Incompatible arguments types."
#define HIGHWAY_INST_PRIVATE_ER   "Error: HighWay::registerMethod: Can't register \"%1\", Member is a private method."

#define HIGHWAY_INST_CHECK_EXIST( key ) \
    if( p->hw_items.contains(key) ) \
    { \
        /*qDebug() << QString(HIGHWAY_INS_EXIST_ER).arg(key);*/ \
        return false; \
    }

#define HIGHWAY_INS_CHECK_PRIVATE( meta_method , key ) \
    if( meta_method.access() == QMetaMethod::Private ) \
    { \
        qDebug() << QString(HIGHWAY_INST_PRIVATE_ER).arg(key); \
        return false; \
    }

#define HIGHWAY_RM_CHECK_EXIST( key ) \
    if( !p->hw_items.contains( key ) ) \
    { \
        qDebug() << QString(HIGHWAY_RM_DONT_EXIST_ER).arg(key); \
        return false; \
    }

#define HIGHWAY_CHECK_SIGNATURE( key , object ) \
    if( p->hw_items.value(key)->obj != object ) \
    { \
        qDebug() << QString(HIGHWAY_WRONG_OBJ_ER).arg(QString::number((quintptr)(void*)object)); \
        return false; \
    }

#define HIGHWAY_CALL_CHECK_KEY( key ) \
    if( !p->hw_items.contains( key ) ) \
    { \
        qDebug() << QString(HIGHWAY_CALL_KEY_EXIST_ER).arg(key); \
        return false; \
    }

#define HIGHWAY_CALL_CHECK_TYPE( key , tp ) \
    if( p->hw_items.value(key)->type != tp ) \
    { \
        qDebug() << QString( HIGHWAY_INCOMBATIBLE_ER ).arg(key); \
        return false; \
    }

#define STRING_TO_SIGNAL( member ) \
    QString( SIGNAL(test()) ).replace( "test" , member ).toLatin1()

#define STRING_TO_SLOT( member ) \
    QString( SLOT(test()) ).replace( "test" , member ).toLatin1()

#define HIGHWAY_CONNECT_OBJ( OBJ ) \
    if( !p->obj_connected.contains(OBJ) ) \
    { \
        connect( OBJ , SIGNAL(destroyed(QObject*)) , this , SLOT(objectRemoved(QObject*)) ); \
        p->obj_connected.insert(OBJ); \
    }

#define HIGHWAY_MAKE_KEY_ABSOLUTE( key ) \
    for( int i=0 ; i<key.count()-1 ; i++ ) \
        if( key[i] == '/' && key[i+1] == '/' ) \
        { \
            key.remove(i,1); \
            i--; \
        } \
    key.remove( key.size()-1 , 1 ); \

#ifdef QT5_BUILD
    #define METHOD_SIGNATURE methodSignature
#else
    #define METHOD_SIGNATURE signature
#endif



#include "HighWay.h"

#include <QGenericArgument>
#include <QHash>
#include <QMetaMethod>
#include <QStringList>
#include <QtDebug>

class HighWayItem
{
public:
    QObject *obj;
    QString key;
    QString member;
    QString description;
    QString ret_type;
    HighWayArgs args;
    HighWay::Type type;
};





class HighWayPrivate
{
public:
    QHash<QString,HighWayItem*> hw_items;
    QSet<QObject*> obj_connected;
    QString hw_last_called_key;
    QVariantList hw_last_called_arguments;
    int hw_call_counter;
};

HighWay::HighWay(QObject *parent) :
    QObject(parent)
{
    p = new HighWayPrivate;
    p->hw_call_counter = 0;
}

void HighWay::registerAllMethods(const QString &parent_key, QObject *obj)
{
    const QMetaObject *meta_obj = obj->metaObject();
    for( int i=0 ; i<meta_obj->methodCount() ; i++ )
    {
        QMetaMethod meta_method = meta_obj->method(i);
        if( meta_method.access() == QMetaMethod::Private )
            continue;

        QString key = parent_key + "/" + QString(meta_method.METHOD_SIGNATURE()).split("(").first() + "/";
        HIGHWAY_MAKE_KEY_ABSOLUTE( key )

        if( p->hw_items.contains(key) && p->hw_items.value(key)->args.count() != meta_method.parameterNames().count() )
            key += QString::number( meta_method.parameterNames().count() );

        QString member = meta_method.METHOD_SIGNATURE();
        switch( static_cast<int>(meta_method.methodType()) )
        {
        case QMetaMethod::Method:
            member.prepend( QString::number(QMETHOD_CODE) );
            break;
        case QMetaMethod::Slot:
            member.prepend( QString::number(QSLOT_CODE) );
            break;
        case QMetaMethod::Signal:
            member.prepend( QString::number(QSIGNAL_CODE) );
            break;
        }
        HighWay::registerMethod( key , obj , member.toLatin1() );
    }

    for( int i=0 ; i<meta_obj->propertyCount() ; i++ )
    {
        QMetaProperty meta_property = meta_obj->property(i);
        if( !meta_property.isValid() )
            continue;

        QString key = parent_key + "/" + QString(meta_property.name()) + "/";
        HIGHWAY_MAKE_KEY_ABSOLUTE( key );

        registerProperty( key , obj , meta_property.name() );
    }
}

bool HighWay::registerFromName(const QString & k, QObject *obj, const QString & method, const QString &description)
{
    QString key = "/" + k + "/";
    HIGHWAY_MAKE_KEY_ABSOLUTE( key )

    const QMetaObject *meta_obj = obj->metaObject();
    for( int i=0 ; i<meta_obj->methodCount() ; i++ )
    {
        QMetaMethod meta_method = meta_obj->method(i);
        if( QString(meta_method.METHOD_SIGNATURE()).left( method.size()+1 ) != method + "(" )
            continue;

        if( meta_method.access() == QMetaMethod::Private )
            return false;

        QString member = meta_method.METHOD_SIGNATURE();
        switch( static_cast<int>(meta_method.methodType()) )
        {
        case QMetaMethod::Method:
            member.prepend( QString::number(QMETHOD_CODE) );
            break;
        case QMetaMethod::Slot:
            member.prepend( QString::number(QSLOT_CODE) );
            break;
        case QMetaMethod::Signal:
            member.prepend( QString::number(QSIGNAL_CODE) );
            break;
        }

        HighWay::registerMethod( key , obj , member.toLatin1() , description );
        return true;
    }

    for( int i=0 ; i<meta_obj->propertyCount() ; i++ )
    {
        QMetaProperty meta_property = meta_obj->property(i);
        if( meta_property.name() != method )
            continue;

        if( !meta_property.isValid() )
            return false;

        registerProperty( key , obj , meta_property.name() );
        return true;
    }

    return false;
}

bool HighWay::registerMethod( const QString & k , QObject *obj , const char *member , const QString & description )
{
    QString key = "/" + k + "/";

    HIGHWAY_MAKE_KEY_ABSOLUTE( key )
    HIGHWAY_INST_CHECK_EXIST( key );

    const QMetaObject *meta_obj = obj->metaObject();
    QMetaMethod meta_method = meta_obj->method( meta_obj->indexOfMethod(QString(member).mid(1).toLatin1()) );
    HIGHWAY_INS_CHECK_PRIVATE( meta_method , key )

    QList<QByteArray> param_types = meta_method.parameterTypes();
    QList<QByteArray> param_names = meta_method.parameterNames();

    HighWayArgs args;
    for( int i=0 ; i<param_types.count() ; i++ )
        args << QPair<QString,QString>( param_types.at(i) , param_names.at(i) );

    HighWayItem *item = new HighWayItem;
        item->obj         = obj;
        item->key         = key;
        item->args        = args;
        item->member      = QString(member).mid(1).split("(").first();
        item->ret_type    = meta_method.typeName();
        item->description = description;

    switch( static_cast<int>(meta_method.methodType()) )
    {
    case QMetaMethod::Method:
    case QMetaMethod::Slot:
        item->type = HighWay::Slot;
        break;

    case QMetaMethod::Signal:
        item->type = HighWay::Signal;
        break;
    }

    p->hw_items.insert( key , item );
    HIGHWAY_CONNECT_OBJ( obj )

    emit added( key );
    emit methodAdded( key );

    return true;
}

bool HighWay::registerProperty(const QString &k, QObject *obj, const QString &member , const QString & description)
{
    QString key = "/" + k + "/";

    HIGHWAY_MAKE_KEY_ABSOLUTE( key )
    HIGHWAY_INST_CHECK_EXIST( key )

    HighWayItem *item = new HighWayItem;
        item->type        = HighWay::Property;
        item->obj         = obj;
        item->key         = key;
        item->member      = member;
        item->ret_type    = "QVariant";
        item->description = description;
        item->args    << QPair<QString,QString>( "QVariant" , "value" ) ;

     p->hw_items.insert( key , item );
     HIGHWAY_CONNECT_OBJ( obj )

     emit added( key );
     emit propertyAdded( key );
     return true;
}

bool HighWay::remove(const QString &k, QObject *obj)
{
    QString key = "/" + k + "/";

    HIGHWAY_MAKE_KEY_ABSOLUTE( key )
    HIGHWAY_RM_CHECK_EXIST( key )
    HIGHWAY_CHECK_SIGNATURE( key , obj )

    HighWayItem *item = p->hw_items.value( key );

    const QMetaObject *meta_obj = item->obj->metaObject();
    QMetaMethod meta_method;
    for( int i=0 ; i<meta_obj->methodCount() ; i++ )
    {
        QMetaMethod mm = meta_obj->method(i);
        if( QString(mm.METHOD_SIGNATURE()).left( item->member.size()+1 ) == item->member+"(" )
        {
            meta_method = mm;
            break;
        }
    }

    switch( static_cast<int>(item->type) )
    {
    case HighWay::Slot:
        break;

    case HighWay::Signal:
        QObject::disconnect( item->obj , QString(QString::number(QSIGNAL_CODE) + meta_method.METHOD_SIGNATURE()).toLatin1() , 0 , 0 );
        break;

    case HighWay::Property:
        break;
    }

    p->hw_items.remove( key );
    delete item;

    emit removed( key );
    return true;
}

QVariant HighWay::call(const QString &k, QVariant val0, QVariant val1, QVariant val2, QVariant val3, QVariant val4, QVariant val5, QVariant val6, QVariant val7, QVariant val8, QVariant val9, bool *ok)
{
    p->hw_call_counter++;
    QString key = "/" + k + "/";

    HIGHWAY_MAKE_KEY_ABSOLUTE( key );
    HIGHWAY_CALL_CHECK_KEY( key );
    if( p->hw_items.value(key)->type == HighWay::Property )
    {
        if( val0.isValid() )
            return setProperty( key , val0 );
        else
            return property( key );
    }

    HIGHWAY_CALL_CHECK_TYPE( key , HighWay::Slot );

    HighWayItem *item = p->hw_items.value( key );
    QVariantList vals;
        vals << val0 << val1 << val2 << val3 << val4 << val5 << val6 << val7 << val8 << val9;

    p->hw_last_called_arguments.clear();

    QList< QPair<QString,const void*> > args;
    for( int i=0 ; i<vals.count() ; i++ )
    {
        if( i<item->args.count() )
        {
            QString type = item->args.at(i).first;

//            if( i<item->args.count() && !vals.at(i).isValid() )
//                qWarning() << HIGHWAY_CALL_ARGS_CNT_FTL;
            if( type != vals.at(i).typeName() )
            {
                if( !vals[i].canConvert( QVariant::nameToType(type.toLatin1()) ) )
//                    qWarning() << HIGHWAY_CALL_TYPE_FTL(key);
                    ;
                else
                    vals[i].convert( QVariant::nameToType(type.toLatin1()) );
            }

            args << QPair<QString,const void*>( type.toLatin1() , vals.at(i).data() );
            p->hw_last_called_arguments << vals.at(i);
        }
        else
        {
            args << QPair<QString,const void*>( vals.at(i).typeName() , vals.at(i).data() );
        }
    }

    int type = QMetaType::type(item->ret_type.toLatin1());
#ifdef QT5_BUILD
    void *res = QMetaType::create( type );
#else
    void *res = QMetaType::construct( type );
#endif
    bool is_pointer = item->ret_type.contains('*');

    p->hw_last_called_key = key;

    bool done = QMetaObject::invokeMethod( item->obj , item->member.toLatin1() , QGenericReturnArgument( item->ret_type.toLatin1() , (is_pointer)? &res : res ) ,
                                      QGenericArgument( args.at(0).first.toLatin1() , args.at(0).second ) ,
                                      QGenericArgument( args.at(1).first.toLatin1() , args.at(1).second ) ,
                                      QGenericArgument( args.at(2).first.toLatin1() , args.at(2).second ) ,
                                      QGenericArgument( args.at(3).first.toLatin1() , args.at(3).second ) ,
                                      QGenericArgument( args.at(4).first.toLatin1() , args.at(4).second ) ,
                                      QGenericArgument( args.at(5).first.toLatin1() , args.at(5).second ) ,
                                      QGenericArgument( args.at(6).first.toLatin1() , args.at(6).second ) ,
                                      QGenericArgument( args.at(7).first.toLatin1() , args.at(7).second ) ,
                                      QGenericArgument( args.at(8).first.toLatin1() , args.at(8).second ) ,
                                      QGenericArgument( args.at(9).first.toLatin1() , args.at(9).second ) );

    if( ok )
        *ok = done;

    p->hw_last_called_key.clear();
    p->hw_last_called_arguments.clear();

    QVariant result;
    if( type == QMetaType::Void )
        result = QVariant();
    else
    if( is_pointer )
        result = QVariant( type , &res );
    else
        result = QVariant( type , res );

    if( type == QMetaType::type("QVariant") )
        return result.value<QVariant>();
    else
        return result;
}

bool HighWay::connectToSignal(const QString &k, QObject *reciver, const char *member)
{
    QString key = "/" + k + "/";

    HIGHWAY_MAKE_KEY_ABSOLUTE( key )
    HIGHWAY_CALL_CHECK_KEY( key )
    HIGHWAY_CALL_CHECK_TYPE( key , HighWay::Signal );

    HighWayItem *item = p->hw_items.value( key );

    const QMetaObject *meta_obj = item->obj->metaObject();
    QMetaMethod meta_method;
    for( int i=0 ; i<meta_obj->methodCount() ; i++ )
    {
        QMetaMethod mm = meta_obj->method(i);
        if( QString(mm.METHOD_SIGNATURE()).left( item->member.size()+1 ) == item->member+"(" )
        {
            meta_method = mm;
            break;
        }
    }

    QObject::connect( item->obj , QString(QString(QString::number(QSIGNAL_CODE)) + meta_method.METHOD_SIGNATURE()).toLatin1() , reciver , member );

    return true;
}

bool HighWay::setProperty(const QString &k, const QVariant &value)
{
    QString key = "/" + k + "/";

    HIGHWAY_MAKE_KEY_ABSOLUTE( key )
    HIGHWAY_CALL_CHECK_KEY( key );
    HIGHWAY_CALL_CHECK_TYPE( key , HighWay::Property )

    HighWayItem *item = p->hw_items.value( key );
    item->obj->setProperty( item->member.toLatin1() , value );

    return true;
}

QVariant HighWay::property(const QString &k)
{
    QString key = "/" + k + "/";

    HIGHWAY_MAKE_KEY_ABSOLUTE( key )
    HIGHWAY_CALL_CHECK_KEY( key );
    HIGHWAY_CALL_CHECK_TYPE( key , HighWay::Property )

    HighWayItem *item = p->hw_items.value( key );
    return item->obj->property( item->member.toLatin1() );
}

QString HighWay::makeKeyAbsolute(const QString &k)
{
    QString key = "/" + k + "/";
    HIGHWAY_MAKE_KEY_ABSOLUTE( key )
    return key;
}

bool HighWay::contains(const QString &k )
{
    QString key = "/" + k + "/";

    HIGHWAY_MAKE_KEY_ABSOLUTE( key );

    const QStringList & mk = key.split("/",QString::SkipEmptyParts);

    QHashIterator<QString,HighWayItem*> i(p->hw_items);
    while(i.hasNext())
    {
        i.next();
        const QStringList & ik = i.key().split("/",QString::SkipEmptyParts);
        if( ik.mid(0,mk.size()) == mk )
            return true;
    }

    return false;
}

const QVariantList &HighWay::lastCalledArguments()
{
    return p->hw_last_called_arguments;
}

const QString & HighWay::lastCalledKey()
{
    return p->hw_last_called_key;
}

bool HighWay::changeParamaters(const QString &k, QObject *obj, const HighWayArgs &new_params)
{
    QString key = "/" + k + "/";
    HIGHWAY_MAKE_KEY_ABSOLUTE( key )

    HighWayItem *item = p->hw_items.value(key);
    if( !item || item->obj != obj )
        return false;

    item->args = new_params;

    emit removed( key );
    emit added( key );
    return true;
}

HighWayArgs HighWay::paramatersOf(const QString &k)
{
    QString key = "/" + k + "/";

    HIGHWAY_MAKE_KEY_ABSOLUTE( key )
    if( !p->hw_items.contains(key) )
        return HighWayArgs();

    return p->hw_items.value( key )->args;
}

HighWay::Type HighWay::typeOf(const QString &k)
{
    QString key = "/" + k + "/";

    HIGHWAY_MAKE_KEY_ABSOLUTE( key )
    if( !p->hw_items.contains(key) )
        return HighWay::Invalid;

    return p->hw_items.value( key )->type;
}

QString HighWay::returnTypeOf(const QString &k)
{
    QString key = "/" + k + "/";

    HIGHWAY_MAKE_KEY_ABSOLUTE( key )
    if( !p->hw_items.contains(key) )
        return QString();

    return p->hw_items.value( key )->ret_type;
}

QString HighWay::descriptionOf(const QString &k)
{
    QString key = "/" + k + "/";

    HIGHWAY_MAKE_KEY_ABSOLUTE( key )
    if( !p->hw_items.contains(key) )
        return QString();

    return p->hw_items.value( key )->description;
}

QStringList HighWay::keys(const QString &key)
{
    const QList<QString> & mk = key.split("/",QString::SkipEmptyParts);
    QStringList result;

    const QStringList & keys = HighWay::keys();
    for( int i=0 ; i<keys.count() ; i++ )
    {
        const QList<QString> & k = keys.at(i).split("/",QString::SkipEmptyParts);
        if( k.mid(0,mk.size()) != mk || k.size()-1 != mk.size()  )
            continue;

        result << k.last();
    }

    return result;
}

QStringList HighWay::keys()
{
    return p->hw_items.keys();
}

int HighWay::count()
{
    return p->hw_items.count();
}

int HighWay::callCount()
{
    return p->hw_call_counter;
}
void HighWay::objectRemoved(QObject *obj)
{
    QHashIterator<QString,HighWayItem*> i( p->hw_items );
    while( i.hasNext() )
    {
        i.next();
        if( i.value()->obj != obj )
            continue;

        p->hw_items.remove( i.key() );
        delete i.value();

        emit removed( i.key() );
    }
}

HighWay::~HighWay()
{
    delete p;
}
