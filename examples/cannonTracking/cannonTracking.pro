TEMPLATE = app

QT += qml quick widgets

INCLUDEPATH += "../../libraries/opencv/mingw/install/include"

LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_core300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_highgui300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_imgproc300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_calib3d300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_imgcodecs300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_features2d300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_flann300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_imgproc300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_ml300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_objdetect300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_photo300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_shape300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_stitching300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_superres300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_video300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_videoio300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\libopencv_videostab300.dll"
LIBS += "c:\gtdev\turret_development\libraries\opencv\mingw\bin\opencv_ffmpeg300.dll"

SOURCES += main.cpp \
    cpp_qml_glue.cpp \
    trackingthread.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    cpp_qml_glue.h \
    trackingthread.h
