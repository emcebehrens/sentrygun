#ifndef PANTILTMANAGERTHREAD_H
#define PANTILTMANAGERTHREAD_H

#include <QThread>
#include <QStringList>
#include <QtSerialPort/QSerialPort>

class PanTiltManagerThread : public QThread
{
    Q_OBJECT

protected:
    void run();

public:   

    PanTiltManagerThread();
    ~PanTiltManagerThread();

    void exitThread();

    void openCommPort(QString serialPortName);
    void closeCommsChannel();
    void init();
    void tilt(unsigned short target);
    void pan(unsigned short target);

    void doDemoPan();
    //void doDemoTilt();

    void maestroGetPosition();
    void maestroSetTarget(unsigned short target);
    void maestroSetSpeed(unsigned short speed);
signals:
    void sig_openDone(bool);
    void sig_newPosition(int);
    void sig_terminated();

private:
    enum operation_
    {
        op_none,
        op_open,
        op_close,
        op_init,
        op_tilt,
        op_pan,
        op_maestroGetPosition,
        op_maestroGoHome,
        op_maestroSetTarget,
        op_maestroSetSpeed,
        op_demoPan
    };

    operation_   currentOperation_;

    bool threadActive_;
    QString serialPortName_;
    unsigned short targetVal_;

    //void task_closeCommsChannel(QSerialPort *serialPortPtr);
    //bool task_openCommPort(QSerialPort *serialPortPtr, QString serialPortName);
    //int task_maestroGetPosition(QSerialPort *serialPortPtr);
   /* bool transact(QSerialPort *serialPortPtr,
                  const char *sendDataPtr,
                  int sendSize,
                  bool getResponse,
                  int *responsePtr);*/
    //void task_maestroGoHome(QSerialPort *serialPortPtr);
    //void task_maestroSetTarget(QSerialPort *serialPortPtr, unsigned short target);
    //void task_maestroSetSpeed(QSerialPort *serialPortPtr, unsigned short speed);
    void waitForPosition(QSerialPort *serialPortPtr, int finalPos);
};

#endif // PANTILTMANAGERTHREAD_H
