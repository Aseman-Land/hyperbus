QT += core
QT += widgets
QT += testlib

LIBS += -L ../build
LIBS += -lHyperBus

TARGET = Example2
TEMPLATE = app
DESTDIR = ../build
INCLUDEPATH += ../Diby ../HyperBus

SOURCES += \
    main.cpp \
    testobject.cpp

HEADERS += \
    testobject.h
