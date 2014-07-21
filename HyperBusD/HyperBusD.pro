QT += core

LIBS += -L ../build
LIBS += -lHyperBus

TARGET = HyperBusD
TEMPLATE = app
DESTDIR = ../build
INCLUDEPATH += ../HyperBus

SOURCES += \
    main.cpp \
    myserver.cpp

HEADERS += \
    myserver.h
