TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS +=  \
    HyperBus \
    HyperBusD \
    HyperBusQml

contains(EXNTESIONS,examples) {
    SUBDIRS +=  \
        Example \
        Example2
}
