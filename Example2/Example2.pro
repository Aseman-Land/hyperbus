QT += core
QT += widgets
QT += testlib

LIBS += -L ../build
LIBS += -lhyperbus

TARGET = Example2
TEMPLATE = app

isEmpty(ASEMAN_BUILD_DEST) {
    DESTDIR = ../build
} else {
    DESTDIR = $$ASEMAN_BUILD_DEST
}

INCLUDEPATH += ../Diby ../HyperBus

SOURCES += \
    main.cpp \
    testobject.cpp

HEADERS += \
    testobject.h
