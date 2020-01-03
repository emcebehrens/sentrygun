#ifndef MAESTROCOMMSYHREAD_H
#define MAESTROCOMMSYHREAD_H

#include <QThread>
#include <QStringList>
#include <QtSerialPort/QSerialPort>

class MaestroCommsThread : public QThread
{
    Q_OBJECT

protected:
    void run();

public:   

    MaestroCommsThread();
    ~MaestroCommsThread();

    void exitThread();

    void openCommPort(QString serialPortName);
    void closeCommsChannel();

    void doDemoPan();
    //void doDemoTilt();

    void maestroGetPosition();
    void maestroGoHome();
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

    void task_closeCommsChannel(QSerialPort *serialPortPtr);
    bool task_openCommPort(QSerialPort *serialPortPtr, QString serialPortName);
    int task_maestroGetPosition(QSerialPort *serialPortPtr);
    bool transact(QSerialPort *serialPortPtr,
                  const char *sendDataPtr,
                  int sendSize,
                  bool getResponse,
                  int *responsePtr);
    void task_maestroGoHome(QSerialPort *serialPortPtr);
    void task_maestroSetTarget(QSerialPort *serialPortPtr, unsigned short target);
    void task_maestroSetSpeed(QSerialPort *serialPortPtr, unsigned short speed);
    void waitForPosition(QSerialPort *serialPortPtr, int finalPos);
};

#endif // MAESTROCOMMSYHREAD_H
