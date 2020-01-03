#ifndef CPP_QML_GLUE_H
#define CPP_QML_GLUE_H

#include <QObject>
#include "trackingthread.h"
#include "GenTasksThread.h"
#include "PanTiltManagerThread.h"
#include "geometry.h"

class CppQmlGlue : public QObject
{
    Q_OBJECT

public:
    CppQmlGlue();
    ~CppQmlGlue();

public slots:
    void slot_changeAngle(int angle);

signals:
    void sig_updateAngle(int);
    void sig_servoControllerState(int);

private slots:
    void slot_foundPort(QString port, QString desc);
    void slot_portDisapeared(QString port);
    void slot_panTiltManOpenDone(bool success);
    void slot_panTiltManTerminated();
    void slot_newBlobPos(int x, int y);

private:
    bool haveAPort_;
    TrackingThread  *trackingThreadPtr_;
    GenTasksThread  *genTasksThreadPtr_;
    PanTiltManagerThread *panTiltManThreadPtr_;
    Geometry *geom_;

    void startPanTiltManagerThread();
};

#endif // CPP_QML_GLUE_H
