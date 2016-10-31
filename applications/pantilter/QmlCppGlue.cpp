#include "QmlCppGlue.h"
#include <QDebug>

QmlCppGlue::QmlCppGlue()
{
    haveAPort_ = false;
    genTasksThreadPtr_ = new GenTasksThread();
    panTiltManThreadPtr_ = Q_NULLPTR;

    connect(genTasksThreadPtr_,
            SIGNAL(sig_foundValidPort(QString, QString)),
            this,
            SLOT(slot_foundPort(QString, QString)));
    connect(genTasksThreadPtr_,
            SIGNAL(sig_handledPortDisapeared(QString)),
            this,
            SLOT(slot_portDisapeared(QString)));
}

QmlCppGlue::~QmlCppGlue()
{
    genTasksThreadPtr_->exitThread();
    genTasksThreadPtr_->wait();
    delete genTasksThreadPtr_;

    if (panTiltManThreadPtr_) {
        panTiltManThreadPtr_->exitThread();
        panTiltManThreadPtr_->wait();
        delete panTiltManThreadPtr_;
    }
}

qreal QmlCppGlue::getTiltSliderValue()
{
    return tiltSliderValue_;
}

qreal QmlCppGlue::getPanSliderValue()
{
    return panSliderValue_;
}

void QmlCppGlue::setTiltSliderValue(qreal value)
{
    if (value != tiltSliderValue_) {
        tiltSliderValue_ = value;
        emit sig_tiltSliderValueChanged();
    }
}

void QmlCppGlue::setPanSliderValue(qreal value)
{
    if (value != panSliderValue_) {
        panSliderValue_ = value;
        emit sig_panSliderValueChanged();
    }
}

void QmlCppGlue::setTiltTarget(int target)
{
    qDebug() << "QmlCppGlue::setTiltTarget" << target;
    if (panTiltManThreadPtr_) {
        panTiltManThreadPtr_->tilt(target);
    }
}

void QmlCppGlue::setPanTarget(int target)
{
    qDebug() << "QmlCppGlue::setPanTarget" << target;
    if (panTiltManThreadPtr_) {
        panTiltManThreadPtr_->pan(target);
    }
}

void QmlCppGlue::setPanSpeed(int speed)
{
    if (panTiltManThreadPtr_) {
        panTiltManThreadPtr_->panSpeed(speed);
    }
}

void QmlCppGlue::setTiltSpeed(int speed)
{
    if (panTiltManThreadPtr_) {
        panTiltManThreadPtr_->tiltSpeed(speed);
    }
}

void QmlCppGlue::setPanAcceleration(int acc)
{
    if (panTiltManThreadPtr_) {
        panTiltManThreadPtr_->panAcceleration(acc);
    }
}

void QmlCppGlue::setTiltAcceleration(int acc)
{
    if (panTiltManThreadPtr_) {
        panTiltManThreadPtr_->tiltAcceleration(acc);
    }
}

/*void QmlCppGlue::getPosition()
{
    if (maestroThreadPtr_) {
        maestroThreadPtr_->maestroGetPosition();
    }
}

void QmlCppGlue::goHome()
{
    if (maestroThreadPtr_) {
        maestroThreadPtr_->maestroGoHome();
    }
}

void QmlCppGlue::setTarget(int val)
{
    if (maestroThreadPtr_) {
        maestroThreadPtr_->maestroSetTarget((unsigned short) val);
    }
}

void QmlCppGlue::setSpeed(int val)
{
    if (maestroThreadPtr_) {
        maestroThreadPtr_->maestroSetSpeed((unsigned short) val);
    }
}

void QmlCppGlue::doDemoPan()
{
    if (maestroThreadPtr_) {
        maestroThreadPtr_->doDemoPan();
    }
}*/

/*void QmlCppGlue::doDemoTilt()
{
    if (maestroThreadPtr_) {
        maestroThreadPtr_->doDemoTilt();
    }
}*/

void QmlCppGlue::slot_foundPort(QString port, QString desc)
{    
    qDebug() << "QmlCppGlue::slot_foundPort" << port << desc;

    startPanTiltManagerThread();
    if (!haveAPort_) {
        panTiltManThreadPtr_->openCommPort(port);
    }
}

void QmlCppGlue::slot_portDisapeared(QString port)
{
    qDebug() << "CppQmlGlue: slot_portDisapeared" << port;

    if (panTiltManThreadPtr_) {
        panTiltManThreadPtr_->closeCommsChannel();
        emit sig_servoControllerState(0);
    }
}

void QmlCppGlue::slot_panTiltManOpenDone(bool success)
{
    qDebug() << "CppQmlGlue: slot_panTiltManOpenDone" << success;

    if (success) {
        emit sig_servoControllerState(1);
        panTiltManThreadPtr_->init();
    } else {
        emit sig_servoControllerState(0);
    }
}

void QmlCppGlue::slot_panTiltManTerminated()
{
    if (panTiltManThreadPtr_) {
        delete panTiltManThreadPtr_;
        panTiltManThreadPtr_ = Q_NULLPTR;
    }
}

void QmlCppGlue::slot_newPanPosistion(int pos)
{
    emit sig_havePanPosition(pos);
}

void QmlCppGlue::slot_newTiltPosistion(int pos)
{
    emit sig_haveTiltPosition(pos);
}

void QmlCppGlue::startPanTiltManagerThread()
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
