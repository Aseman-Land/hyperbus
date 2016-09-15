TEMPLATE = lib
TARGET = HyperBusQml
QT += qml quick
CONFIG += qt plugin

isEmpty(ASEMAN_BUILD_DEST) {
    DESTDIR = ../build/qml/HyperBus
} else {
    DESTDIR = $$ASEMAN_BUILD_DEST/qml/HyperBus
}

TARGET = $$qtLibraryTarget($$TARGET)
uri = HyperBus

INCLUDEPATH += ../HyperBus


!isEmpty(PREFIX) LIBS += -L$$PREFIX
!isEmpty(DESTDIR) LIBS += -L$$DESTDIR/../../
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

files.source = plugins.qmltypes qmldir
files.target = $$DESTDIR
COPYFOLDERS += files

include(qmake/copyData.pri)
copyData()

qmlFile.files = $$OUT_PWD/$$DESTDIR/
qmlFile.path = $$PREFIX/qml/

INSTALLS += qmlFile
