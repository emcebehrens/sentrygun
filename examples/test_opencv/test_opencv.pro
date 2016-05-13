#-------------------------------------------------
#
# Project created by QtCreator 2015-07-18T20:15:35
#
#-------------------------------------------------

QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_opencv
TEMPLATE = app

INCLUDEPATH += "../../libraries/opencv/mingw/install/include"

#LIBOPENCV = $$PWD\libraries\opencv\mingw\bin
LIBOPENCV = "c:\gtdev\software\turret_development\libraries\opencv\mingw\bin"
LIBS += $$LIBOPENCV/libopencv_core310.dll
LIBS += $$LIBOPENCV/libopencv_highgui310.dll
LIBS += $$LIBOPENCV/libopencv_imgproc310.dll
LIBS += $$LIBOPENCV/libopencv_calib3d310.dll
LIBS += $$LIBOPENCV/libopencv_imgcodecs310.dll
LIBS += $$LIBOPENCV/libopencv_features2d310.dll
LIBS += $$LIBOPENCV/libopencv_flann310.dll
LIBS += $$LIBOPENCV/libopencv_ml310.dll
LIBS += $$LIBOPENCV/libopencv_objdetect310.dll
LIBS += $$LIBOPENCV/libopencv_photo310.dll
LIBS += $$LIBOPENCV/libopencv_shape310.dll
LIBS += $$LIBOPENCV/libopencv_stitching310.dll
LIBS += $$LIBOPENCV/libopencv_superres310.dll
LIBS += $$LIBOPENCV/libopencv_video310.dll
LIBS += $$LIBOPENCV/libopencv_videoio310.dll
LIBS += $$LIBOPENCV/libopencv_videostab310.dll
LIBS += $$LIBOPENCV/opencv_ffmpeg310.dll

#QMAKE_CXXFLAGS += `pkg-config opencv --cflags --libs`

SOURCES += main.cpp

HEADERS  +=

FORMS    += mainwindow.ui
