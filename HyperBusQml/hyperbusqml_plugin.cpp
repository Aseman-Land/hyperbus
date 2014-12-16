#include "hyperbusqml_plugin.h"
#include "hyperbusqml.h"
#include "highwayqml.h"

#include <qqml.h>

void HyperBusQmlPlugin::registerTypes(const char *uri)
{
    // @uri HyperBus
    qmlRegisterType<HyperBusQml>(uri, 1, 0, "HyperBus");
    qmlRegisterType<HighWayQml>(uri, 1, 0, "HighWay");
}


