#include "QmlCppGlue.h"
#include <QDebug>

QmlCppGlue::QmlCppGlue()
{
    genTasksThreadPtr_ = new GenTasksThread();
    maestroThreadPtr_ = NULL;

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

    if (maestroThreadPtr_) {
        maestroThreadPtr_->exitThread();
        maestroThreadPtr_->wait();
        delete maestroThreadPtr_;
    }
}

void QmlCppGlue::getPosition()
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
}

/*void QmlCppGlue::doDemoTilt()
{
    if (maestroThreadPtr_) {
        maestroThreadPtr_->doDemoTilt();
    }
}*/

void QmlCppGlue::slot_foundPort(QString port, QString desc)
{
    //allAvailableComPorts_.append(port);
    //allAvailableComPortDesc_.append(desc);
    qDebug() << "slot_foundPort" << port << desc;

    startMaestroThread();

    maestroThreadPtr_->openCommPort(port);

    //if (allAvailableComPorts_.size() == 1) {
        //QLOG_INFO() << "CppQmlGlue: Emitting sig_comPortsFound";
        //emit sig_firstPotDsPComPortFound();
    //}

    //emit sig_allAvailableComPortsChanged();
}

void QmlCppGlue::slot_portDisapeared(QString port)
{
    qDebug() << "CppQmlGlue: slot_portDisapeared" << port;

    if (maestroThreadPtr_) {
        maestroThreadPtr_->closeCommsChannel();
    }
}

void QmlCppGlue::startMaestroThread()
{
    if (!maestroThreadPtr_) {
        maestroThreadPtr_ = new MaestroCommsThread();
    }
}
