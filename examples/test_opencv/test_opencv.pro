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



SOURCES += main.cpp

HEADERS  +=

FORMS    += mainwindow.ui
