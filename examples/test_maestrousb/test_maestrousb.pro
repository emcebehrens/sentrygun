QT += core
QT -= gui

CONFIG += c++11

TARGET = test_maestrousb
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../libraries/libusc/libusc.c

INCLUDEPATH += "../../libraries/libusb/include/libusb-1.0"
INCLUDEPATH += "../../libraries/libusc"

LIBUSB = "c:\gtdev\software\turret_development\libraries\libusb\MinGW32\dll"
LIBS += $$LIBUSB/libusb-1.0.dll

HEADERS += \
    ../../libraries/libusc/libusc.h
