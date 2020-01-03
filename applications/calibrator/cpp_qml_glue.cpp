#include "cpp_qml_glue.h"
#include <QDebug>
#include <QCameraInfo>
#include <QApplication>

CppQmlGlue::CppQmlGlue()
{
    panTiltManThreadPtr_ = Q_NULLPTR;
    trackingThreadPtr_ = Q_NULLPTR;
    settListPtr_ = Q_NULLPTR;

    qDebug() << "CppQmlGlue construct";

    QString settingsFile = QApplication::applicationDirPath() + "/calibrator_settings.ini";
    settings_.loadSettings(settingsFile);
    settListPtr_ = settings_.getSettingPtr();
    for (int i = 0; i < settListPtr_->size(); ++i) {
        comboBoxSettingsNamesListModel_.append(settListPtr_->at(i).Name);
    }

    getCameraList();

    if (settListPtr_->size()) {

        geom_ = new Geometry(settListPtr_->at(0).calservominy,//tiltmin
                             settListPtr_->at(0).calservomaxy,//tiltmax
                             settListPtr_->at(0).calservominx,//panmin
                             settListPtr_->at(0).calservomaxx,//panmax
                             640,//cameraX
                             480,//cameraY
                             settListPtr_->at(0).caldistancez,//calDepth
                             settListPtr_->at(0).caldistancex,//calXDistance
                             settListPtr_->at(0).caldistancey//calYDistance
                             );
        geom_->setNewDepth(settListPtr_->at(0).depth);
    }

    genTasksThreadPtr_ = new GenTasksThread();
    connect(genTasksThreadPtr_,
            SIGNAL(sig_foundValidPort(QString, QString)),
            this,
            SLOT(slot_foundPort(QString, QString)));
    connect(genTasksThreadPtr_,
            SIGNAL(sig_handledPortDisapeared(QString)),
            this,
            SLOT(slot_portDisapeared(QString)));

}

CppQmlGlue::~CppQmlGlue()
{
    if (trackingThreadPtr_ != Q_NULLPTR) {
        delete trackingThreadPtr_;
    }
}

void CppQmlGlue::shutdown()
{
    qDebug() << "CppQmlGlue::shutdown";

    if (trackingThreadPtr_ != Q_NULLPTR) {
        trackingThreadPtr_->exitThread();
        trackingThreadPtr_->wait();
    }
}

void CppQmlGlue::trackingEnabled(bool activate)
{
    qDebug() << "CppQmlGlue::trackingEnabled" << activate;
    if (trackingThreadPtr_ != Q_NULLPTR) {

        if (activate) {
            trackingThreadPtr_->StartTracking();
        } else {
            trackingThreadPtr_->StopTracking();
        }
    }
}

void CppQmlGlue::setTiltTarget(int target)
{
    qDebug() << "CppQmlGlue::setTiltTarget" << target;
    if (panTiltManThreadPtr_) {
        panTiltManThreadPtr_->tilt(target);
    }
}

void CppQmlGlue::setPanTarget(int target)
{
    qDebug() << "CppQmlGlue::setPanTarget" << target;
    if (panTiltManThreadPtr_) {
        panTiltManThreadPtr_->pan(target);
    }
}

void CppQmlGlue::getPanTiltPositions()
{
    if (panTiltManThreadPtr_) {
        panTiltManThreadPtr_->getPanPosition();
        panTiltManThreadPtr_->getTiltPosition();
    }
}

int CppQmlGlue::calculateStepsizeY(int tiltmax,
                                   int tiltmin,
                                   qreal calYDistance,
                                   qreal calDepth)
{
    int stpsize;
    qDebug() << tiltmax
             << tiltmin
             << calYDistance
             << calDepth;
    stpsize = geom_->calcServoStepSize(tiltmax,tiltmin,calYDistance,calDepth);
    qDebug() << stpsize;

    return stpsize;
}

int CppQmlGlue::calculateStepsizeX(int panmax,
                                   int panmin,
                                   qreal calXDistance,
                                   qreal calDepth)
{
    int stpsize;
    qDebug() << panmax
             << panmin
             << calXDistance
             << calDepth;

    stpsize = geom_->calcServoStepSize(panmax,panmin,calXDistance,calDepth);
    qDebug() << stpsize;

    return stpsize;
}

qreal CppQmlGlue::getCaldistanceX()
{
    return (qreal)settListPtr_->at(0).caldistancex;
}

qreal CppQmlGlue::getCaldistanceY()
{
    return (qreal)settListPtr_->at(0).caldistancey;
}

qreal CppQmlGlue::getCaldistanceZ()
{
    return (qreal)settListPtr_->at(0).caldistancez;
}

qreal CppQmlGlue::getDepthDistance()
{
    return (qreal)settListPtr_->at(0).depth;
}

void CppQmlGlue::setDepth(qreal value)
{
    if (settListPtr_ != Q_NULLPTR) {
        (*settListPtr_)[0].depth = (quint32) value;
        emit sig_minServoPanChanged();
        geom_->setNewDepth(value);
    }
}

void CppQmlGlue::cameraEnable(bool yesno, int indx)
{
    qDebug() << "CppQmlGlue::cameraEnable" << yesno << indx;
    if (yesno) {
        if (trackingThreadPtr_ != Q_NULLPTR) {
            qDebug() << "Previous trackingThreadPtr_ not deleted";
            return;
        }

        trackingThreadPtr_ = new TrackingThread(indx);
        connect(trackingThreadPtr_,
                SIGNAL(sig_newBlobPos(int,int)),
                this,
                SLOT(slot_newBlobPos(int,int)));
    } else {
        trackingThreadPtr_->exitThread();
        trackingThreadPtr_ = Q_NULLPTR;
    }
}

void CppQmlGlue::getCameraList()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {
        qDebug() << cameraInfo.description();
        comboBoxCameraListModel_.append(cameraInfo.description());
    }
    emit sig_comboBoxCameraListModelChanged();
}

QStringList CppQmlGlue::getComboBoxCameraListModel()
{
    return comboBoxCameraListModel_;
}

qreal CppQmlGlue::getTiltSliderValue()
{
    return tiltSliderValue_;
}

qreal CppQmlGlue::getPanSliderValue()
{
    return panSliderValue_;
}

QStringList CppQmlGlue::getComboBoxSettingsNamesListModel()
{
    return comboBoxSettingsNamesListModel_;
}

qreal CppQmlGlue::getMaxServoTilt()
{
    //qDebug() << "CppQmlGlue::getMaxServoTilt" << (qreal)settListPtr_->at(0).calservomaxy;
    return (qreal)settListPtr_->at(0).calservomaxy;//maxServoTilt_
}

qreal CppQmlGlue::getMinServoTilt()
{
    return (qreal)settListPtr_->at(0).calservominy;
}

qreal CppQmlGlue::getMaxServoPan()
{
    return (qreal)settListPtr_->at(0).calservomaxx;
}

qreal CppQmlGlue::getMinServoPan()
{
    return (qreal)settListPtr_->at(0).calservominx;
}

void CppQmlGlue::setTiltSliderValue(qreal value)
{
    if (value != tiltSliderValue_) {
        tiltSliderValue_ = value;
        emit sig_tiltSliderValueChanged();
    }
}

void CppQmlGlue::setPanSliderValue(qreal value)
{
    if (value != panSliderValue_) {
        panSliderValue_ = value;
        emit sig_panSliderValueChanged();
    }
}

void CppQmlGlue::setMaxServoTilt(qreal value)
{
    if (settListPtr_ != Q_NULLPTR) {
        //settListPtr_->at(0).calservomaxy = (quint32) value;
        (*settListPtr_)[0].calservomaxy = (quint32) value;
        emit sig_maxServoTiltChanged();
    }
}

void CppQmlGlue::setMinServoTilt(qreal value)
{
    if (settListPtr_ != Q_NULLPTR) {
        (*settListPtr_)[0].calservominy = (quint32) value;
        emit sig_minServoTiltChanged();
    }
}

void CppQmlGlue::setMaxServoPan(qreal value)
{
    if (settListPtr_ != Q_NULLPTR) {
        (*settListPtr_)[0].calservomaxx = (quint32) value;
        emit sig_maxServoPanChanged();
    }
}

void CppQmlGlue::setMinServoPan(qreal value)
{
    if (settListPtr_ != Q_NULLPTR) {
        (*settListPtr_)[0].calservominx = (quint32) value;
        emit sig_minServoPanChanged();
    }
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
    //if (!haveAPort_) {
        panTiltManThreadPtr_->openCommPort(port);
    //}
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

void CppQmlGlue::slot_newPanPosistion(int pos)
{
    emit sig_havePanPosition(pos);
}

void CppQmlGlue::slot_newTiltPosistion(int pos)
{
    emit sig_haveTiltPosition(pos);
}

void CppQmlGlue::startPanTiltManagerThread()
{
    if (!panTiltManThreadPtr_) {
        panTiltManThreadPtr_ = new PanTiltManagerThread(1, 0, 6000, 6000);

        connect(panTiltManThreadPtr_,
                SIGNAL(sig_openDone(bool)),
                this,
                SLOT(slot_panTiltManOpenDone(bool)));
        connect(panTiltManThreadPtr_,
                SIGNAL(sig_terminated()),
                this,
                SLOT(slot_panTiltManTerminated()));
        connect(panTiltManThreadPtr_,
                SIGNAL(sig_newPanPosistion(int)),
                this,
                SLOT(slot_newPanPosistion(int)));
        connect(panTiltManThreadPtr_,
                SIGNAL(sig_newTiltPosistion(int)),
                this,
                SLOT(slot_newTiltPosistion(int)));
    }
}
