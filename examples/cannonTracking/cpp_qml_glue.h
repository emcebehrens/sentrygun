#ifndef CPP_QML_GLUE_H
#define CPP_QML_GLUE_H

#include <QObject>
#include "trackingthread.h"

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

private:
    TrackingThread  *trackingThreadPtr_;


};

#endif // CPP_QML_GLUE_H
