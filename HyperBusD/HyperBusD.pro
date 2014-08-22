QT += core

LIBS += -L ../build
LIBS += -lHyperBus

TEMPLATE = app
DESTDIR = ../build
INCLUDEPATH += ../HyperBus
TARGET = HyperBusD

SOURCES += \
    main.cpp \
    myserver.cpp

HEADERS += \
    myserver.h

isEmpty(PREFIX) {
    PREFIX = /usr
}

target = $$TARGET
target.path = $$PREFIX/bin

INSTALLS += target
