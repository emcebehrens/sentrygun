#include "PanTiltManagerThread.h"
#include <QTime>
#include <QDebug>
#include "ServoController.h"
#include <QCoreApplication>

PanTiltManagerThread::PanTiltManagerThread()
{
    qDebug() << "PanTiltManagerThread Constructor thread ID:" << QThread::currentThreadId();

    threadActive_ = false;   
    portOpened_ = false;
    closePort_ = false;
    myTimerTriggered_ = false;
    getNextPosition_ = false;

    start(NormalPriority);
}

PanTiltManagerThread::~PanTiltManagerThread()
{    
}

void PanTiltManagerThread::exitThread()
{
    threadActive_ = false;
}

void PanTiltManagerThread::openCommPort(QString serialPortName)
{    
    if (!portOpened_) {
        QMutexLocker locker(&myMutex_);

        portOpened_ = true;
        serialPortName_ = serialPortName;
        queueStruct request;
        request.op = op_open;
        request.val = 0;
        msgQueue_.enqueue(request);        
    }
}

void PanTiltManagerThread::closeCommsChannel()
{    
    closePort_ = true;
}

void PanTiltManagerThread::init()
{
    qDebug() << "PanTiltManagerThread::init";
    QMutexLocker locker(&myMutex_);
    queueStruct request;
    request.op = op_init;
    request.val = 0;
    msgQueue_.enqueue(request);
}

void PanTiltManagerThread::tilt(unsigned short target)
{
    QMutexLocker locker(&myMutex_);
    queueStruct request;
    request.op = op_tilt;
    request.val = target;
    msgQueue_.enqueue(request);    
}

void PanTiltManagerThread::pan(unsigned short target)
{
    QMutexLocker locker(&myMutex_);
    queueStruct request;
    request.op = op_pan;
    request.val = target;
    msgQueue_.enqueue(request);
}

void PanTiltManagerThread::tiltSpeed(unsigned short speed)
{
    QMutexLocker locker(&myMutex_);
    queueStruct request;
    request.op = op_tiltSpeed;
    request.val = speed;
    msgQueue_.enqueue(request);    
}

void PanTiltManagerThread::panSpeed(unsigned short speed)
{
    QMutexLocker locker(&myMutex_);
    queueStruct request;
    request.op = op_panSpeed;
    request.val = speed;
    msgQueue_.enqueue(request);    
}

void PanTiltManagerThread::tiltAcceleration(unsigned short acc)
{
    QMutexLocker locker(&myMutex_);
    queueStruct request;
    request.op = op_tiltAcc;
    request.val = acc;
    msgQueue_.enqueue(request);    
}

void PanTiltManagerThread::panAcceleration(unsigned short acc)
{
    QMutexLocker locker(&myMutex_);
    queueStruct request;
    request.op = op_tiltAcc;
    request.val = acc;
    msgQueue_.enqueue(request);    
}

void PanTiltManagerThread::run()
{
    qDebug() << "PanTiltManagerThread started thread ID:" << QThread::currentThreadId();

    threadActive_ = true;

    ServoController maestro;
    QObject::connect(&maestro,
                     SIGNAL(sig_havePosition(unsigned char, bool, int)),
                     this,
                     SLOT(slot_havePosition(unsigned char, bool, int)));


    QTimer  myTimer;
    QObject::connect(&myTimer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(slot_myTimerTimeout()));
    myTimer.setSingleShot(true);

    while(threadActive_) {

        if (closePort_) {
            maestro.closeCommsChannel();
            portOpened_ = false;
            threadActive_ = false;
            break;
        }


        if(!msgQueue_.isEmpty()) {
            if (myMutex_.tryLock()) {
                queueStruct firstInQ = msgQueue_.head();
                switch(firstInQ.op) {
                    case op_open:
                    {
                        bool ret = maestro.openCommPort(serialPortName_);
                        if (!ret) {
                            portOpened_ = false;
                        }
                        emit sig_openDone(ret);
                        break;
                    }

                    case op_init:
                    {
                        maestro.setSpeed(0, 0);
                        maestro.setSpeed(1, 0);
                        maestro.setAcceleration(0, 0);
                        maestro.setAcceleration(1, 0);
                        maestro.setTarget(0, 6000);
                        maestro.setTarget(1, 6000);
                        myTimer.start(500);
                        break;
                    }

                    case op_tilt:
                    {
                        maestro.setTarget(1, firstInQ.val);
                        break;
                    }

                    case op_pan:
                    {
                        maestro.setTarget(0, firstInQ.val);
                        break;
                    }

                    case op_tiltSpeed:
                    {
                        maestro.setSpeed(1, firstInQ.val);
                        break;
                    }

                    case op_panSpeed:
                    {
                        maestro.setSpeed(0, firstInQ.val);
                        break;
                    }

                    case op_tiltAcc:
                    {
                        maestro.setAcceleration(1, firstInQ.val);
                        break;
                    }

                    case op_panAcc:
                    {
                        maestro.setAcceleration(0, firstInQ.val);
                        break;
                    }

                    default:
                        break;
                }// switch

                msgQueue_.dequeue();
                myMutex_.unlock();
            }
        }

        if (myTimerTriggered_) {
            myTimerTriggered_ = false;
            maestro.getPosition(0);
        } else if (getNextPosition_) {
            getNextPosition_ = false;
            maestro.getPosition(1);
            myTimer.start(500);
        }

        QCoreApplication::processEvents();
        usleep(100);
    }

    maestro.closeCommsChannel();
    emit sig_terminated();
    qDebug() << "PanTiltManagerThread terminated";
}

void PanTiltManagerThread::slot_havePosition(unsigned char channel, bool result, int posistion)
{
    //qDebug() << "PanTiltManagerThread slot_havePosition" << channel << result << posistion;
    if (result) {
        if (channel == 0) {
            getNextPosition_ = true;
            emit sig_newPanPosistion(posistion);
        } else if (channel == 1) {
            emit sig_newTiltPosistion(posistion);
        }
    } else {
        qDebug() << "Getting Position of channel" << channel << "FAILED!!";
    }
}

void PanTiltManagerThread::slot_myTimerTimeout()
{
    //qDebug() << "PanTiltManagerThread slot_myTimerTimeout";
    myTimerTriggered_ = true;
}

