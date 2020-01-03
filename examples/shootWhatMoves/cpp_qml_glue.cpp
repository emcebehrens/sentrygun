#include "cpp_qml_glue.h"
#include <QDebug>

CppQmlGlue::CppQmlGlue()
{
    trackingThreadPtr_ = new TrackingThread();

    /*connect(trackingThreadPtr_,
            SIGNAL(sig_changeAngle(int)),
            this,
            SLOT(slot_changeAngle(int)));*/
}

CppQmlGlue::~CppQmlGlue()
{
    delete trackingThreadPtr_;
}


void CppQmlGlue::slot_changeAngle(int angle)
{
    qDebug() << "CppQmlGlue: slot_changeAngle" << angle;

    //emit sig_updateAngle(angle);

}
