TEMPLATE = app

QT += qml quick
CONFIG += c++11

INCLUDEPATH += "../../libraries/opencv/mingw/install/include"

SOURCES += main.cpp \
    cpp_qml_glue.cpp \
    trackingthread.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

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

HEADERS += \
    cpp_qml_glue.h \
    trackingthread.h
