#ifndef QMLCPPGLUE_H
#define QMLCPPGLUE_H

#include <QObject>
#include "general_tasks_thread.h"
#include "MaestroCommsThread.h"

class QmlCppGlue : public QObject
{
    Q_OBJECT

    /*Q_PROPERTY(QStringList ipAddressesList
               READ getIpAddressesList
               NOTIFY sig_IpAddressesListChanged)*/

public:
    QmlCppGlue();
    ~QmlCppGlue();

    // Q Properties' READ functions
    //QStringList getIpAddressesList();

    // Q Properties' WRITE functions
    //void setMode(QString mode);

    // Accessible from QML
    Q_INVOKABLE void getPosition();
    Q_INVOKABLE void goHome();
    Q_INVOKABLE void setTarget(int val);
    Q_INVOKABLE void setSpeed(int val);
    Q_INVOKABLE void doDemoPan();

//signals:
    //void sig_IpAddressesListChanged();


private:
    void startMaestroThread();

    GenTasksThread  *genTasksThreadPtr_;
    MaestroCommsThread *maestroThreadPtr_;

private slots:
    void slot_foundPort(QString port, QString desc);
    void slot_portDisapeared(QString port);
};


#endif // QMLCPPGLUE_H
