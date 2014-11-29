QT += core

LIBS += -L ../build
LIBS += -lhyperbus

TEMPLATE = app
DESTDIR = ../build
INCLUDEPATH += ../HyperBus
TARGET = hyperbusd

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
