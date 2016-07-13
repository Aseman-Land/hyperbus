QT += core

LIBS += -L../build
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
contains(BUILD_MODE, opt):{
    BIN_PATH = $$PREFIX/
} else {
    BIN_PATH = $$PREFIX/bin
}

target = $$TARGET
target.path = $$BIN_PATH

INSTALLS += target
