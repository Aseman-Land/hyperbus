#include "hyperbusqml.h"

#define HYPERBUS_OBJ HyperBus::staticHyperBus(p->source)

class HyperBusQmlPrivate
{
public:
    QString source;
    int session;
};

HyperBusQml::HyperBusQml(QObject *parent):
    QObject(parent)
{
    p = new HyperBusQmlPrivate;
    p->source = "127.0.0.1:25480";
    p->session = Global;
}

void HyperBusQml::setSource(const QString &source)
{
    if(p->source == source)
        return;

    p->source = source;
    emit sourceChanged();
}

QString HyperBusQml::source() const
{
    return p->source;
}

void HyperBusQml::setSession(int s)
{
    if(p->session == s)
        return;

    p->session = s;
    emit sessionChanged();
}

int HyperBusQml::session() const
{
    return p->session;
}

QVariant HyperBusQml::call(const QString &key, const QVariant &v0, const QVariant &v1, const QVariant &v2, const QVariant &v3, const QVariant &v4, const QVariant &v5, const QVariant &v6, const QVariant &v7, const QVariant &v8, const QVariant &v9)
{
    QString currentSession = HYPERBUS_OBJ->session();

    HYPERBUS_OBJ->setSession(static_cast<HyperBus::SessionType>(p->session));
    QVariant res = HYPERBUS_OBJ->call(key, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
    HYPERBUS_OBJ->setSession(currentSession);

    return res;
}

HyperBusQml::~HyperBusQml()
{
    delete p;
}
