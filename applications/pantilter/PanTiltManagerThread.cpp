#include "PanTiltManagerThread.h"
#include <QTime>
#include <QDebug>
#include "ServoController.h"

PanTiltManagerThread::PanTiltManagerThread()
{    
    qDebug() << "PanTiltManagerThread Constructor thread ID:" << QThread::currentThreadId();

    threadActive_ = false;
    currentOperation_ = op_none;

    start(NormalPriority);
}

PanTiltManagerThread::~PanTiltManagerThread()
{
    //task_closeCommsChannel();
}

void PanTiltManagerThread::exitThread()
{
    threadActive_ = false;
}

void PanTiltManagerThread::openCommPort(QString serialPortName)
{
    if (currentOperation_ == op_none) {
        serialPortName_ = serialPortName;
        currentOperation_ = op_open;
    }
}

void PanTiltManagerThread::closeCommsChannel()
{
    if (currentOperation_ == op_none) {
        currentOperation_ = op_close;
    }
}

void PanTiltManagerThread::init()
{
    if (currentOperation_ == op_none) {
        currentOperation_ = op_init;
    }
}

void PanTiltManagerThread::tilt(unsigned short target)
{
    if (currentOperation_ == op_none) {
        currentOperation_ = op_tilt;
        targetVal_ = target;
    }
}

void PanTiltManagerThread::pan(unsigned short target)
{
    if (currentOperation_ == op_none) {
        currentOperation_ = op_pan;
        targetVal_ = target;
    }
}

void PanTiltManagerThread::doDemoPan()
{
    if (currentOperation_ == op_none) {
        currentOperation_ = op_demoPan;
    }
}

void PanTiltManagerThread::maestroGetPosition()
{
    if (currentOperation_ == op_none) {
        currentOperation_ = op_maestroGetPosition;
    }
}

void PanTiltManagerThread::waitForPosition(QSerialPort *serialPortPtr, int finalPos)
{
    /*int currentPos;

    currentPos = task_maestroGetPosition(serialPortPtr);

    while(currentPos != finalPos) {
        qDebug() << "*";
        usleep(100);
        currentPos = task_maestroGetPosition(serialPortPtr);
    }*/
}

void PanTiltManagerThread::maestroSetTarget(unsigned short target)
{
    if (currentOperation_ == op_none) {
        currentOperation_ = op_maestroSetTarget;
        targetVal_ = target;
    }
}

void PanTiltManagerThread::maestroSetSpeed(unsigned short speed)
{
    if (currentOperation_ == op_none) {
        currentOperation_ = op_maestroSetSpeed;
        targetVal_ = speed;
    }
}

void PanTiltManagerThread::run()
{
    qDebug() << "PanTiltManagerThread started thread ID:" << QThread::currentThreadId();

    threadActive_ = true;

    ServoController maestro;

    while(threadActive_) {

        switch (currentOperation_) {
            case op_open:
            {
                bool ret = maestro.openCommPort(serialPortName_);
                emit sig_openDone(ret);
                currentOperation_ = op_none;
                break;
            }

            case op_close:
            {
                maestro.closeCommsChannel();
                currentOperation_ = op_none;
                break;
            }

            case op_init:
            {
                maestro.setTarget(0, 6000);
                maestro.setTarget(1, 6000);
                currentOperation_ = op_none;
                break;
            }

            case op_tilt:
            {
                maestro.setTarget(1, targetVal_);
                currentOperation_ = op_none;
                break;
            }

            case op_pan:
            {
                maestro.setTarget(0, targetVal_);
                currentOperation_ = op_none;
                break;
            }

            /*case op_maestroGetPosition:
            {
                task_maestroGetPosition(&serialPort);
                //emit sig_newPosition(ret);
                currentOperation_ = op_none;
                break;
            }

            case op_maestroGoHome:
            {
                task_maestroGoHome(&serialPort);

                currentOperation_ = op_none;
                break;
            }

            case op_maestroSetTarget:
            {
                task_maestroSetTarget(&serialPort, targetVal_);

                currentOperation_ = op_none;
                break;
            }

            case op_maestroSetSpeed:
            {
                task_maestroSetSpeed(&serialPort, targetVal_);

                currentOperation_ = op_none;
                break;
            }

            case op_demoPan:
            {
                task_maestroSetTarget(&serialPort, 4000);
                waitForPosition(&serialPort, 3744);

                task_maestroSetTarget(&serialPort, 8000);
                waitForPosition(&serialPort, 8000);

                task_maestroSetTarget(&serialPort, 4000);
                waitForPosition(&serialPort, 3744);

                task_maestroSetTarget(&serialPort, 8000);
                waitForPosition(&serialPort, 8000);

                currentOperation_ = op_none;
                break;
            }*/

            case op_none:
            default:
                break;
        }// switch

        usleep(100);
    }

    maestro.closeCommsChannel();
    emit sig_terminated();
    qDebug() << "PanTiltManagerThread terminated";
}

