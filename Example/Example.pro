QT += core
QT += widgets network
QT += testlib

LIBS += -L ../build
LIBS += -lhyperbus

TARGET = Example
TEMPLATE = app

isEmpty(ASEMAN_BUILD_DEST) {
    DESTDIR = ../build
} else {
    DESTDIR = $$ASEMAN_BUILD_DEST
}

INCLUDEPATH += ../Diby ../HyperBus

SOURCES += \
    main.cpp \
    myobject.cpp

HEADERS += \
    myobject.h
