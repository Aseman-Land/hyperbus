QT += core
QT += widgets network
QT += testlib

LIBS += -L ../build
LIBS += -lHyperBus

TARGET = Example
TEMPLATE = app
DESTDIR = ../build
INCLUDEPATH += ../Diby ../HyperBus

SOURCES += \
    main.cpp \
    myobject.cpp

HEADERS += \
    myobject.h
