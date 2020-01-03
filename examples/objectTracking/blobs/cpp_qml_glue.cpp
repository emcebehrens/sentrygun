#include "cpp_qml_glue.h"
#include <QDebug>

CppQmlGlue::CppQmlGlue()
{
    panTiltManThreadPtr_ = Q_NULLPTR;

    qDebug() << "CppQmlGlue construct";

    geom_ = new Geometry(4000,//tiltmin
                         8000,//tiltmax
                         4000,//panmin
                         8000,//panmax
                         640,//cameraX
                         480,//cameraY
                         600.0,//calDepth
                         600.0,//calXDistance
                         600.0//calYDistance
                         );

    genTasksThreadPtr_ = new GenTasksThread();
    connect(genTasksThreadPtr_,
            SIGNAL(sig_foundValidPort(QString, QString)),
            this,
            SLOT(slot_foundPort(QString, QString)));
    connect(genTasksThreadPtr_,
            SIGNAL(sig_handledPortDisapeared(QString)),
            this,
            SLOT(slot_portDisapeared(QString)));

    trackingThreadPtr_ = new TrackingThread();

    connect(trackingThreadPtr_,
            SIGNAL(sig_newBlobPos(int,int)),
            this,
            SLOT(slot_newBlobPos(int,int)));
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

void CppQmlGlue::slot_foundPort(QString port, QString desc)
{
    qDebug() << "CppQmlGlue::slot_foundPort" << port << desc;

    startPanTiltManagerThread();
    if (!haveAPort_) {
        panTiltManThreadPtr_->openCommPort(port);
    }
}

void CppQmlGlue::slot_portDisapeared(QString port)
{
    qDebug() << "CppQmlGlue: slot_portDisapeared" << port;

    if (panTiltManThreadPtr_) {
        panTiltManThreadPtr_->closeCommsChannel();
        emit sig_servoControllerState(0);
    }
}

void CppQmlGlue::slot_panTiltManOpenDone(bool success)
{
    qDebug() << "CppQmlGlue: slot_panTiltManOpenDone" << success;

    if (success) {
        emit sig_servoControllerState(1);
        panTiltManThreadPtr_->init();
    } else {
        emit sig_servoControllerState(0);
    }
}

void CppQmlGlue::slot_panTiltManTerminated()
{
    if (panTiltManThreadPtr_) {
        delete panTiltManThreadPtr_;
        panTiltManThreadPtr_ = Q_NULLPTR;
    }
}

void CppQmlGlue::slot_newBlobPos(int x, int y)
{
    qDebug() << "CppQmlGlue: slot_newBlobPos" << x << "," << y;

    if (panTiltManThreadPtr_) {
        //panTiltManThreadPtr_->tilt(4000 + (y * 6.25));
        //panTiltManThreadPtr_->pan(4000 + (x * 6.25));

        panTiltManThreadPtr_->tilt(geom_->getServoTiltAngle(y));
        panTiltManThreadPtr_->pan(geom_->getServoPanAngle(x));

    }
}

void CppQmlGlue::startPanTiltManagerThread()
{
    if (!panTiltManThreadPtr_) {
        panTiltManThreadPtr_ = new PanTiltManagerThread();

        connect(panTiltManThreadPtr_,
                SIGNAL(sig_openDone(bool)),
                this,
                SLOT(slot_panTiltManOpenDone(bool)));
        connect(panTiltManThreadPtr_,
                SIGNAL(sig_terminated()),
                this,
                SLOT(slot_panTiltManTerminated()));
        /*connect(panTiltManThreadPtr_,
                SIGNAL(sig_newPanPosistion(int)),
                this,
                SLOT(slot_newPanPosistion(int)));
        connect(panTiltManThreadPtr_,
                SIGNAL(sig_newTiltPosistion(int)),
                this,
                SLOT(slot_newTiltPosistion(int)));*/
    }
}
