TEMPLATE = lib
TARGET = HyperBusQml
QT += qml quick
CONFIG += qt plugin
DESTDIR = ../build/qml/HyperBus

TARGET = $$qtLibraryTarget($$TARGET)
uri = HyperBus

INCLUDEPATH += $$OUT_PWD/../build/include/hyperbus
INCLUDEPATH += ../HyperBus

LIBS += -L../build
LIBS += -lhyperbus

SOURCES += \
    hyperbusqml_plugin.cpp \
    hyperbusqml.cpp \
    highwayqml.cpp

HEADERS += \
    hyperbusqml_plugin.h \
    hyperbusqml.h \
    highwayqml.h

isEmpty(PREFIX) {
    PREFIX = /usr
}

include(qmake/qmlplugindump.pri)

qmlFile.files = $$OUT_PWD/$$DESTDIR/
qmlFile.path = $$PREFIX/qml/

INSTALLS += qmlFile
