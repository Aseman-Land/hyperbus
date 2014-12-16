#ifndef HYPERBUSQML_PLUGIN_H
#define HYPERBUSQML_PLUGIN_H

#include <QQmlExtensionPlugin>

class HyperBusQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // HYPERBUSQML_PLUGIN_H

