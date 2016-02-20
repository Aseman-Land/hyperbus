QT += core network

contains(QT,gui): DEFINES += GUI_SUPPORT

HEADERS += \
    $$PWD/hyperbus_macros.h \
    $$PWD/hyperbus_global.h \ 
    $$PWD/htcpclient.h \
    $$PWD/htcpserver.h \
    $$PWD/hmsgtransporter.h \
    $$PWD/hyperbusglobals.h \
    $$PWD/hyperbusrecord.h \
    $$PWD/hyperbusreciever.h \
    $$PWD/hyperbusabstractserver.h \
    $$PWD/hyperbusserver.h \
    $$PWD/highway.h \
    $$PWD/hvariantconverterunit.h \
    $$PWD/hvariantconverter.h \
    $$PWD/hvariantconvertergeneraltypes.h \
    $$PWD/hsocketdata.h \
    $$PWD/hsmarttcpclient.h \
    $$PWD/hsmarttcpserver.h \
    $$PWD/hpidtools.h \
    $$PWD/hyperbus.h \
    $$PWD/hyperbustools.h

SOURCES += \
    $$PWD/htcpserver.cpp \
    $$PWD/htcpclient.cpp \
    $$PWD/hmsgtransporter.cpp \
    $$PWD/hyperbusglobals.cpp \
    $$PWD/hyperbusrecord.cpp \
    $$PWD/hyperbusreciever.cpp \
    $$PWD/hyperbusabstractserver.cpp \
    $$PWD/hyperbusserver.cpp \
    $$PWD/highway.cpp \
    $$PWD/hvariantconverterunit.cpp \
    $$PWD/hvariantconverter.cpp \
    $$PWD/hvariantconvertergeneraltypes.cpp \
    $$PWD/hsocketdata.cpp \
    $$PWD/hsmarttcpclient.cpp \
    $$PWD/hsmarttcpserver.cpp \
    $$PWD/hpidtools.cpp \
    $$PWD/hyperbustools.cpp

FRAMEWORKS_HEADERS += \
    $$PWD/HighWay \
    $$PWD/HMshTransporter \
    $$PWD/HPidTools \
    $$PWD/HSmartTcpClient \
    $$PWD/HSmartTcpServer \
    $$PWD/HSocketData \
    $$PWD/HTcpClient \
    $$PWD/HTcpServer \
    $$PWD/HVariantConverter \
    $$PWD/HVariantConverterGeneralTypes \
    $$PWD/HVariantConverterUnit \
    $$PWD/HyperBus \
    $$PWD/HyperBus_global \
    $$PWD/HyperBus_macros \
    $$PWD/HyperBusAbstractServer \
    $$PWD/HyperBusGlobals \
    $$PWD/HyperBusReciever \
    $$PWD/HyperBusRecord \
    $$PWD/HyperBusServer \
    $$PWD/HyperBusTools
