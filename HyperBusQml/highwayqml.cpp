#include "highwayqml.h"

#include <HighWay>
#include <QHash>

#define HIGHWAY_OBJ HighWay::staticHighWay(p->source)

class HighWayQmlPrivate
{
public:
    QString source;
};

HighWayQml::HighWayQml(QObject *parent) :
    QObject(parent)
{
    p = new HighWayQmlPrivate;
    setSource("default");

    connect(this, SIGNAL(added(QString))  , SIGNAL(changed()) );
    connect(this, SIGNAL(removed(QString)), SIGNAL(changed()) );
}

void HighWayQml::setSource(const QString &source)
{
    if(p->source == source)
        return;
    if(!p->source.isEmpty())
    {
        disconnect(HIGHWAY_OBJ, SIGNAL(added(QString))  , this, SIGNAL(added(QString))   );
        disconnect(HIGHWAY_OBJ, SIGNAL(removed(QString)), this, SIGNAL(removed(QString)) );
    }

    p->source = source;
    if(!p->source.isEmpty())
    {
        connect(HIGHWAY_OBJ, SIGNAL(added(QString))  , this, SIGNAL(added(QString))   );
        connect(HIGHWAY_OBJ, SIGNAL(removed(QString)), this, SIGNAL(removed(QString)) );
    }

    emit keyChanged();
}

QString HighWayQml::source() const
{
    return p->source;
}

QStringList HighWayQml::keys() const
{
    return HIGHWAY_OBJ->keys();
}

int HighWayQml::count() const
{
    return HIGHWAY_OBJ->count();
}

bool HighWayQml::contains(const QString &key)
{
    return HIGHWAY_OBJ->contains(key);
}

bool HighWayQml::connectToSignal(const QString &key, QObject *reciver, const QString &member)
{
    return HIGHWAY_OBJ->connectToSignal(key, reciver, member.toLatin1());
}

QVariant HighWayQml::property(const QString &key)
{
    return HIGHWAY_OBJ->property(key);
}

QVariant HighWayQml::call(const QString &key, QVariant v0, QVariant v1, QVariant v2, QVariant v3, QVariant v4, QVariant v5, QVariant v6, QVariant v7, QVariant v8, QVariant v9)
{
    return HIGHWAY_OBJ->call(key, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}

HighWayQml::~HighWayQml()
{
    delete p;
}

