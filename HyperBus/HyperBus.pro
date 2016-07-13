QT += gui

DEFINES += HYPERBUS_LIBRARY
TEMPLATE = lib
DESTDIR= \
    ../build

TARGET = hyperbus

include(hyperbus.pri)

headers.source = $$HEADERS
headers.target = $$DESTDIR/include/hyperbus
fwheaders.source = $$FRAMEWORKS_HEADERS
fwheaders.target = $$DESTDIR/include/hyperbus
COPYFOLDERS += headers fwheaders
win32-msvc* {
        include (qmake/copyData.pri)
        copyData ()
    }

isEmpty(PREFIX) {
    PREFIX = /usr
}

contains(BUILD_MODE, opt):{
} else {
    contains(QMAKE_HOST.arch, x86_64):{
        LIBS_PATH = lib/x86_64-linux-gnu/
    } else {
        LIBS_PATH = lib/i386-linux-gnu/
    }
}

target = $$TARGET
target.path = $$PREFIX/$$LIBS_PATH
headers.files = $$HEADERS
headers.path = $$PREFIX/include/hyperbus
fwheaders.files = $$FRAMEWORKS_HEADERS
fwheaders.path = $$PREFIX/include/hyperbus

INSTALLS += target headers fwheaders
