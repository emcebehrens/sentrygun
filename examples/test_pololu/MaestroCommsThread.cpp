#include "MaestroCommsThread.h"
#include <QTime>
#include <QDebug>

MaestroCommsThread::MaestroCommsThread()
{    
    qDebug() << "MaestroCommsThread Constructor thread ID:" << QThread::currentThreadId();

    threadActive_ = false;
    currentOperation_ = op_none;

    start(NormalPriority);
}

MaestroCommsThread::~MaestroCommsThread()
{
    //task_closeCommsChannel();
}

void MaestroCommsThread::exitThread()
{
    threadActive_ = false;
}

void MaestroCommsThread::openCommPort(QString serialPortName)
{
    if (currentOperation_ == op_none) {
        serialPortName_ = serialPortName;
        currentOperation_ = op_open;
    }
}

bool MaestroCommsThread::task_openCommPort(QSerialPort *serialPortPtr, QString serialPortName)
{
    serialPortPtr->setPortName(serialPortName);

    /* Choose the baud rate (bits per second).
     * If the Maestro's serial mode is USB Dual Port, this number does not matter. */
    int serialPortBaudRate = 9600;
    serialPortPtr->setBaudRate(serialPortBaudRate);

    if (!serialPortPtr->open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open port" << serialPortName << serialPortPtr->errorString();
        return false;
    }

    qDebug() << serialPortName << "OPENED";

    return true;
}

void MaestroCommsThread::closeCommsChannel()
{
    if (currentOperation_ == op_none) {
        currentOperation_ = op_close;
    }
}

void MaestroCommsThread::doDemoPan()
{
    if (currentOperation_ == op_none) {
        currentOperation_ = op_demoPan;
    }
}

void MaestroCommsThread::task_closeCommsChannel(QSerialPort *serialPortPtr)
{
    serialPortPtr->close();
    qDebug() << serialPortName_ << "CLOSED";
}

void MaestroCommsThread::maestroGetPosition()
{
    if (currentOperation_ == op_none) {
        currentOperation_ = op_maestroGetPosition;
    }
}

int MaestroCommsThread::task_maestroGetPosition(QSerialPort *serialPortPtr)
{
    int position;
    qDebug() << "task_maestroGetPosition";

    unsigned char command[2];
    command[0] = 0x90;
    command[1] = 0x00;

    transact(serialPortPtr, (const char *)command, sizeof(command), true, &position);

    return position;
}

void MaestroCommsThread::maestroGoHome()
{
    if (currentOperation_ == op_none) {
        currentOperation_ = op_maestroGoHome;
    }
}

void MaestroCommsThread::task_maestroGoHome(QSerialPort *serialPortPtr)
{
    qDebug() << "task_maestroGoHome";

    unsigned char command[1];
    command[0] = 0xA2;    

    transact(serialPortPtr, (const char *)command, sizeof(command), false, NULL);
}

bool MaestroCommsThread::transact(QSerialPort *serialPortPtr,
                                  const char *sendDataPtr,
                                  int sendSize,
                                  bool getResponse,
                                  int *responsePtr)
{
    //qDebug() << command[0] << command[1];
    qDebug() << "sizeof(command)" << sendSize;

    qint64 ret = serialPortPtr->write(sendDataPtr, sendSize);
    qDebug() << "write ret = " << ret;
    if (serialPortPtr->waitForBytesWritten(1000)) {

        if (getResponse) {
            // read response
            if (serialPortPtr->waitForReadyRead(1000)) {
                QByteArray responseData = serialPortPtr->readAll();
                while (serialPortPtr->waitForReadyRead(10)) {
                    responseData += serialPortPtr->readAll();
                }
                qDebug() << "Got" << responseData.length() << "bytes back" << responseData;
                const char *ptr = responseData.constData();
                for (int i=0; i < responseData.length(); i++) {
                    qDebug() << QString("byte %1").arg(ptr[i]);
                }

                int pos = 0;
                pos = ptr[0] + (ptr[1] << 8);
                *responsePtr = pos;
                qDebug() << QString("pos %1").arg(pos);

                return true;

            } else {
                qDebug() << "Wait read response timeout " << QTime::currentTime().toString();
                return false;
            }
        }
    } else {
        qDebug() << "Wait write request timeout " << QTime::currentTime().toString();
        return false;
    }

    return true;
}

void MaestroCommsThread::waitForPosition(QSerialPort *serialPortPtr, int finalPos)
{
    int currentPos;

    currentPos = task_maestroGetPosition(serialPortPtr);

    while(currentPos != finalPos) {
        qDebug() << "*";
        usleep(100);
        currentPos = task_maestroGetPosition(serialPortPtr);
    }
}

void MaestroCommsThread::maestroSetTarget(unsigned short target)
{
    if (currentOperation_ == op_none) {
        currentOperation_ = op_maestroSetTarget;
        targetVal_ = target;
    }
}

void MaestroCommsThread::maestroSetSpeed(unsigned short speed)
{
    if (currentOperation_ == op_none) {
        currentOperation_ = op_maestroSetSpeed;
        targetVal_ = speed;
    }
}

void MaestroCommsThread::task_maestroSetTarget(QSerialPort *serialPortPtr, unsigned short target)
{
    qDebug() << "task_maestroSetTarget" << target;

    unsigned char command[4];

    command[0] = 0x84;
    command[1] = 0;
    command[2] = target & 0x7F;
    command[3] = (target >> 7) & 0x7F;

    transact(serialPortPtr, (const char *)command, sizeof(command), false, NULL);
}

void MaestroCommsThread::task_maestroSetSpeed(QSerialPort *serialPortPtr, unsigned short speed)
{
    qDebug() << "task_maestroSetSpeed" << speed;

    unsigned char command[4];

    command[0] = 0x87;
    command[1] = 0;
    command[2] = speed & 0x7F;
    command[3] = (speed >> 7) & 0x7F;

    transact(serialPortPtr, (const char *)command, sizeof(command), false, NULL);
}

void MaestroCommsThread::run()
{
    qDebug() << "MaestroCommsThread started thread ID:" << QThread::currentThreadId();

    threadActive_ = true;

    QSerialPort serialPort;

    while(threadActive_) {

        switch (currentOperation_) {
            case op_open:
            {
                bool ret = task_openCommPort(&serialPort, serialPortName_);
                emit sig_openDone(ret);
                currentOperation_ = op_none;
                break;
            }

            case op_close:
            {
                task_closeCommsChannel(&serialPort);
                currentOperation_ = op_none;
                break;
            }

            case op_maestroGetPosition:
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
            }

            case op_none:
            default:
                break;
        }// switch

        usleep(100);
    }

    task_closeCommsChannel(&serialPort);
    emit sig_terminated();
    qDebug() << "MaestroCommsThread terminated";
}

