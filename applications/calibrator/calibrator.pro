TEMPLATE = app

QT += qml quick serialport multimedia widgets
CONFIG += c++11

SOURCES += main.cpp \
    cpp_qml_glue.cpp \
    GenTasksThread.cpp \
    geometry.cpp \
    trackingthread.cpp \
    ../../libraries/maestropantilt/PanTiltManagerThread.cpp \
    ../../libraries/maestroservocntlr/ServoController.cpp \
    ../../libraries/comportscanner/ComPortScanner.cpp \
    ../../libraries/objecttracking/Blob.cpp \
    settings_manager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    cpp_qml_glue.h \
    GenTasksThread.h \
    geometry.h \
    trackingthread.h \
    ../../libraries/maestropantilt/PanTiltManagerThread.h \
    ../../libraries/maestroservocntlr/ServoController.h \
    ../../libraries/comportscanner/ComPortScanner.h \
    ../../libraries/objecttracking/Blob.h \
    settings_manager.h

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

INCLUDEPATH += "../../libraries/opencv/mingw/install/include"
INCLUDEPATH += "../../libraries/comportscanner"
INCLUDEPATH += "../../libraries/maestropantilt"
INCLUDEPATH += "../../libraries/maestroservocntlr"
INCLUDEPATH += "../../libraries/objecttracking"
