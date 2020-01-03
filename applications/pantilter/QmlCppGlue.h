#ifndef QMLCPPGLUE_H
#define QMLCPPGLUE_H

#include <QObject>
#include "GenTasksThread.h"
#include "PanTiltManagerThread.h"

class QmlCppGlue : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qreal tiltSliderValue
               READ getTiltSliderValue
               WRITE setTiltSliderValue
               NOTIFY sig_tiltSliderValueChanged)

    Q_PROPERTY(qreal panSliderValue
               READ getPanSliderValue
               WRITE setPanSliderValue
               NOTIFY sig_panSliderValueChanged)

public:
    QmlCppGlue();
    ~QmlCppGlue();   

    // Q Properties' READ functions
    qreal getTiltSliderValue();
    qreal getPanSliderValue();

    // Q Properties' WRITE functions
    void setTiltSliderValue(qreal value);
    void setPanSliderValue(qreal value);

    // Accessible from QML
    Q_INVOKABLE void setTiltTarget(int target);
    Q_INVOKABLE void setPanTarget(int target);
    Q_INVOKABLE void setPanSpeed(int speed);
    Q_INVOKABLE void setTiltSpeed(int speed);
    Q_INVOKABLE void setPanAcceleration(int acc);
    Q_INVOKABLE void setTiltAcceleration(int acc);

    /*Q_INVOKABLE void getPosition();
    Q_INVOKABLE void goHome();
    Q_INVOKABLE void setTarget(int val);
    Q_INVOKABLE void setSpeed(int val);
    Q_INVOKABLE void doDemoPan();*/
signals:
    qreal sig_tiltSliderValueChanged();
    qreal sig_panSliderValueChanged();

    void sig_servoControllerState(int);
    void sig_havePanPosition(int);
    void sig_haveTiltPosition(int);
//signals:
    //void sig_IpAddressesListChanged();


private:
    bool haveAPort_;
    qreal tiltSliderValue_;
    qreal panSliderValue_;
    void startPanTiltManagerThread();

    GenTasksThread  *genTasksThreadPtr_;
    PanTiltManagerThread *panTiltManThreadPtr_;

private slots:
    void slot_foundPort(QString port, QString desc);
    void slot_portDisapeared(QString port);
    void slot_panTiltManOpenDone(bool success);
    void slot_panTiltManTerminated();
    void slot_newPanPosistion(int pos);
    void slot_newTiltPosistion(int pos);
};


#endif // QMLCPPGLUE_H
