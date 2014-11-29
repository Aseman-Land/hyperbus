TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS +=  \
    HyperBus \
    HyperBusD

contains(EXNTESIONS,examples) {
    SUBDIRS +=  \
        Example \
        Example2
}
