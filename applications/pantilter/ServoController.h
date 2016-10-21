#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

class ServoController : public QObject
{
    Q_OBJECT

public:
    ServoController();

public slots:
    bool openCommPort(QString serialPortName);
    void closeCommsChannel();
    void getPosition();
    void setTarget(unsigned char channel, unsigned short target);
    //void maestroSetSpeed(unsigned short speed);

signals:
    void sig_openDone(bool);
    void sig_newPosition(int);
    void sig_terminated();

private:
    QSerialPort serialPort_;

    bool transact(QSerialPort *serialPortPtr, const char *sendDataPtr, int sendSize, bool getResponse, int *responsePtr);
};

#endif // SERVOCONTROLLER_H
