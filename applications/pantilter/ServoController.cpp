#include "ServoController.h"
#include <QDebug>
#include <QTime>

ServoController::ServoController()
{

}

bool ServoController::openCommPort(QString serialPortName)
{
    serialPort_.setPortName(serialPortName);

    /* Choose the baud rate (bits per second).
     * If the Maestro's serial mode is USB Dual Port, this number does not matter. */
    int serialPortBaudRate = 9600;
    serialPort_.setBaudRate(serialPortBaudRate);

    if (!serialPort_.open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open port" << serialPortName << serialPort_.errorString();
        return false;
    }

    qDebug() << serialPortName << "OPENED";

    return true;
}

void ServoController::closeCommsChannel()
{
    serialPort_.close();
    qDebug() << "ServoController::closeCommsChannel";
}

void ServoController::getPosition(unsigned char channel)
{
    int position;
    bool res;

    qDebug() << "ServoController::getPosition on channel" << channel;

    unsigned char command[2];
    command[0] = 0x90;
    command[1] = channel;

    res = transact(&serialPort_, (const char *)command, sizeof(command), true, &position);

    emit sig_havePosition(channel, res, position);
}

void ServoController::setTarget(unsigned char channel, unsigned short target)
{
    qDebug() << "ServoController::setTarget" << target
             << "on channel" << channel;

    unsigned char command[4];

    command[0] = 0x84;
    command[1] = channel;
    command[2] = target & 0x7F;
    command[3] = (target >> 7) & 0x7F;

    transact(&serialPort_, (const char *)command, sizeof(command), false, NULL);
}

void ServoController::setSpeed(unsigned char channel, unsigned short speed)
{
    qDebug() << "ServoController::setSpeed" << speed
             << "on channel" << channel;

    unsigned char command[4];

    command[0] = 0x87;
    command[1] = channel;
    command[2] = speed & 0x7F;
    command[3] = (speed >> 7) & 0x7F;

    transact(&serialPort_, (const char *)command, sizeof(command), false, NULL);
}

void ServoController::setAcceleration(unsigned char channel, unsigned short accel)
{
    qDebug() << "ServoController::setAcceleration" << accel
             << "on channel" << channel;

    unsigned char command[4];

    command[0] = 0x89;
    command[1] = channel;
    command[2] = accel & 0x7F;
    command[3] = (accel >> 7) & 0x7F;

    transact(&serialPort_, (const char *)command, sizeof(command), false, NULL);
}

bool ServoController::transact(QSerialPort *serialPortPtr,
                                  const char *sendDataPtr,
                                  int sendSize,
                                  bool getResponse,
                                  int *responsePtr)
{
    //qDebug() << command[0] << command[1];
    //qDebug() << "sizeof(command)" << sendSize;

    qint64 ret = serialPortPtr->write(sendDataPtr, sendSize);
    //qDebug() << "write ret = " << ret;
    if (serialPortPtr->waitForBytesWritten(1000)) {

        if (getResponse) {
            // read response
            if (serialPortPtr->waitForReadyRead(1000)) {
                QByteArray responseData = serialPortPtr->readAll();
                while (serialPortPtr->waitForReadyRead(10)) {
                    responseData += serialPortPtr->readAll();
                }
                //qDebug() << "Got" << responseData.length() << "bytes back" << responseData;
                const char *ptr = responseData.constData();
                /*for (int i=0; i < responseData.length(); i++) {
                    qDebug() << QString("byte %1").arg(ptr[i]);
                }*/

                int pos = 0;
                pos = ptr[0] + (ptr[1] << 8);
                *responsePtr = pos;
                //qDebug() << QString("pos %1").arg(pos);

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
