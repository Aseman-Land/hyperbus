QT += core
QT += gui
QT += network

DEFINES += HYPERBUS_LIBRARY
TEMPLATE = lib
DESTDIR = ../build
TARGET = HyperBus

DEFINES += GUI_SUPPORT
greaterThan(QT_MAJOR_VERSION, 4): DEFINES += QT5_BUILD

HEADERS += \
    hyperbus_macros.h \
    hyperbus_global.h \ 
    HTcpClient.h \
    HTcpServer.h \
    HMsgTransporter.h \
    HyperBusGlobals.h \
    HyperBusRecord.h \
    HyperBusReciever.h \
    HyperBusAbstractServer.h \
    HyperBusServer.h \
    HighWay.h \
    HVariantConverterUnit.h \
    HVariantConverter.h \
    HVariantConverterGeneralTypes.h \
    HSocketData.h \
    HSmartTcpClient.h \
    HSmartTcpServer.h \
    HPidTools.h \
    HyperBus.h \
    HyperBusTools.h

SOURCES += \
    HTcpServer.cpp \
    HTcpClient.cpp \
    HMsgTransporter.cpp \
    HyperBusGlobals.cpp \
    HyperBusRecord.cpp \
    HyperBusReciever.cpp \
    HyperBusAbstractServer.cpp \
    HyperBusServer.cpp \
    HighWay.cpp \
    HVariantConverterUnit.cpp \
    HVariantConverter.cpp \
    HVariantConverterGeneralTypes.cpp \
    HSocketData.cpp \
    HSmartTcpClient.cpp \
    HSmartTcpServer.cpp \
    HPidTools.cpp \
    HyperBusTools.cpp

isEmpty(PREFIX) {
    PREFIX = /usr
}

contains(QMAKE_HOST.arch, x86):{
    LIBS_PATH = lib/i386-linux-gnu/
}
contains(QMAKE_HOST.arch, x86_64):{
    LIBS_PATH = lib/x86_64-linux-gnu/
}

target = $$TARGET
target.path = $$PREFIX/$$LIBS_PATH
headers.files = $$HEADERS
headers.path = $$PREFIX/include/hyperbus

INSTALLS += target headers
