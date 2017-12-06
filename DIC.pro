TEMPLATE = app

QT += qml quick
QT += serialport
CONFIG += c++11

SOURCES += main.cpp \
    serialobd.cpp \
    serialgps.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    pids.h \
    instrumentcluster.h \
    serialobd.h \
    serialgps.h
