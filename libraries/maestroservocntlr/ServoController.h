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
    void getPosition(unsigned char channel);
    void setTarget(unsigned char channel, unsigned short target);

    /*
     * This command limits the speed at which a servo channel’s output value changes
     * A speed of 0 makes the speed unlimited, so that setting the target will
     * immediately affect the position
     * At the minimum speed setting of 1, the servo output takes 40 seconds to move
     * from 1 to 2 ms.
    */
    void setSpeed(unsigned char channel, unsigned short speed);

    /*
     * This command limits the acceleration of a servo channel’s output.
     * The acceleration limit is a value from 0 to 255 in units of (0.25 μs)/(10 ms)/(80 ms)
     * A value of 0 corresponds to no acceleration limit
     * An acceleration limit causes the speed of a servo to slowly ramp up until it reaches
     * the maximum speed, then to ramp down again as position approaches target
     * At the minimum acceleration setting of 1, the servo output takes about 3 seconds to
     * move smoothly from a target of 1 ms to a target of 2 ms
    */
    void setAcceleration(unsigned char channel, unsigned short accel);

    //void getErrors();

signals:
    void sig_openDone(bool);    
    void sig_terminated();
    void sig_havePosition(unsigned char, bool, int);

private:
    QSerialPort serialPort_;

    bool transact(QSerialPort *serialPortPtr, const char *sendDataPtr, int sendSize, bool getResponse, int *responsePtr);
};

#endif // SERVOCONTROLLER_H
