#ifndef PANTILTMANAGERTHREAD_H
#define PANTILTMANAGERTHREAD_H

#include <QThread>
#include <QStringList>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QQueue>
#include <QMutex>

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
    void tiltSpeed(unsigned short speed);
    void panSpeed(unsigned short speed);
    void tiltAcceleration(unsigned short acc);
    void panAcceleration(unsigned short acc);

signals:
    void sig_openDone(bool);
    void sig_newTiltPosistion(int);
    void sig_newPanPosistion(int);
    void sig_terminated();
    void sig_cancelTimer();
    void sig_startTimer();

private slots:
    void slot_myTimerTimeout();
    void slot_havePosition(unsigned char channel, bool result, int posistion);
private:
    enum operation_
    {        
        op_open,       
        op_init,
        op_tilt,
        op_pan,
        op_tiltSpeed,
        op_panSpeed,
        op_tiltAcc,
        op_panAcc,        
    };

    bool threadActive_;
    QString serialPortName_;
    unsigned short targetVal_;    
    bool myTimerTriggered_;
    bool getNextPosition_;

    bool portOpened_;
    bool closePort_;

    struct queueStruct {
        operation_ op;
        unsigned short val;
    };

    QQueue<queueStruct> msgQueue_;
    QMutex myMutex_;    
};

#endif // PANTILTMANAGERTHREAD_H
