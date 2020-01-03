TEMPLATE = app

QT += qml quick serialport
CONFIG += c++11

SOURCES += main.cpp \
    comportscanner.cpp \
    general_tasks_thread.cpp \
    QmlCppGlue.cpp \
    MaestroCommsThread.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    comportscanner.h \
    general_tasks_thread.h \
    QmlCppGlue.h \
    MaestroCommsThread.h
