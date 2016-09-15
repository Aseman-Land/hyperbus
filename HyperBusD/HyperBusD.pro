QT += core

TEMPLATE = app

isEmpty(ASEMAN_BUILD_DEST) {
    DESTDIR = ../build
} else {
    DESTDIR = $$ASEMAN_BUILD_DEST
}

!isEmpty(PREFIX) LIBS += -L$$PREFIX
!isEmpty(DESTDIR) LIBS += -L$$DESTDIR
LIBS += -L../build
LIBS += -lhyperbus

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
