TEMPLATE = app

QT += qml quick serialport
CONFIG += c++11

SOURCES += main.cpp \
    QmlCppGlue.cpp \
    ComPortScanner.cpp \
    GenTasksThread.cpp \
    ServoController.cpp \
    PanTiltManagerThread.cpp

RESOURCES += qml.qrc \
    art.qrc \
    fonts.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    QmlCppGlue.h \
    ComPortScanner.h \
    GenTasksThread.h \
    ServoController.h \
    PanTiltManagerThread.h
