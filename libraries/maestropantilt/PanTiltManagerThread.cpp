#include "PanTiltManagerThread.h"
#include <QTime>
#include <QDebug>
#include "ServoController.h"
#include <QCoreApplication>

PanTiltManagerThread::PanTiltManagerThread(unsigned char tiltChannelNumber,
                                           unsigned char panChannelNumber,
                                           unsigned short tiltHome,
                                           unsigned short panHome)
{
    qDebug() << "PanTiltManagerThread Constructor thread ID:" << QThread::currentThreadId();

    threadActive_ = false;   
    portOpened_ = false;
    closePort_ = false;
    myTimerTriggered_ = false;
    getNextPosition_ = false;
    givePositionFeedback_ = false;
    tiltChanNr_ = tiltChannelNumber;
    panChanNr_ = panChannelNumber;
    tiltHome_ = tiltHome;
    panHome_ = panHome;

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

void PanTiltManagerThread::activatePositionFeedback()
{
    givePositionFeedback_ = true;
}

void PanTiltManagerThread::deactivatePositionFeedback()
{
    givePositionFeedback_ = false;
}

void PanTiltManagerThread::getTiltPosition()
{
    QMutexLocker locker(&myMutex_);
    queueStruct request;
    request.op = op_tiltPos;
    request.val = 0;
    msgQueue_.enqueue(request);
}

void PanTiltManagerThread::getPanPosition()
{
    QMutexLocker locker(&myMutex_);
    queueStruct request;
    request.op = op_panPos;
    request.val = 0;
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
                        maestro.setSpeed(panChanNr_, 0);
                        maestro.setSpeed(tiltChanNr_, 0);
                        maestro.setAcceleration(panChanNr_, 0);
                        maestro.setAcceleration(tiltChanNr_, 0);
                        maestro.setTarget(panChanNr_, 6000);
                        maestro.setTarget(tiltChanNr_, 6000);
                        if (givePositionFeedback_) {
                            myTimer.start(500);
                        }
                        break;
                    }

                    case op_tilt:
                    {
                        maestro.setTarget(tiltChanNr_, firstInQ.val);
                        break;
                    }

                    case op_pan:
                    {
                        maestro.setTarget(panChanNr_, firstInQ.val);
                        break;
                    }

                    case op_tiltSpeed:
                    {
                        maestro.setSpeed(tiltChanNr_, firstInQ.val);
                        break;
                    }

                    case op_panSpeed:
                    {
                        maestro.setSpeed(panChanNr_, firstInQ.val);
                        break;
                    }

                    case op_tiltAcc:
                    {
                        maestro.setAcceleration(tiltChanNr_, firstInQ.val);
                        break;
                    }

                    case op_panAcc:
                    {
                        maestro.setAcceleration(panChanNr_, firstInQ.val);
                        break;
                    }

                    case op_tiltPos:
                    {
                        maestro.getPosition(tiltChanNr_);
                        break;
                    }

                    case op_panPos:
                    {
                        maestro.getPosition(panChanNr_);
                        break;
                    }

                    default:
                        break;
                }// switch

                msgQueue_.dequeue();
                myMutex_.unlock();
            }
        }

        if (givePositionFeedback_) {
            if (!myTimer.isActive()) {
                myTimer.start(500);
            }

            if (myTimerTriggered_) {
                myTimerTriggered_ = false;
                maestro.getPosition(panChanNr_);
            } else if (getNextPosition_) {
                getNextPosition_ = false;
                maestro.getPosition(tiltChanNr_);
                myTimer.start(500);
            }
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
    qDebug() << "PanTiltManagerThread slot_havePosition" << channel << result << posistion;
    if (result) {
        if (channel == panChanNr_) {
            getNextPosition_ = true;
            emit sig_newPanPosistion(posistion);
        } else if (channel == tiltChanNr_) {
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

