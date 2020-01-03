#include "general_tasks_thread.h"
#include <QDebug>

GenTasksThread::GenTasksThread()
{    
    qDebug() << "GenTasksThread Constructor thread ID:" << QThread::currentThreadId();

    threadActive_ = false;    

    start(NormalPriority);
}

GenTasksThread::~GenTasksThread()
{
}

void GenTasksThread::exitThread()
{
    threadActive_ = false;
}

void GenTasksThread::run()
{
    QStringList portNames;
    QStringList somePortList;
    QString item, description;

    qDebug() << "GenTasksThread started thread ID:" << QThread::currentThreadId();

    threadActive_ = true;

    while(threadActive_) {

        portNames.clear();
        portNames = scanner.scanForComPorts();
        if (scanner.updateAvailableComPortLists(portNames, &item, &description)) {
            emit sig_foundValidPort(item, description);
        }
        somePortList.clear();
        somePortList = scanner.checkForDisapearedHandledPorts(portNames);
        if (!somePortList.empty()) {
            foreach (QString item, somePortList) {
                emit sig_handledPortDisapeared(item);
            }
        }

        msleep(1000);
    }
}

/*void GenTasksThread::slot_portNotHandled(QString port)
{
    scanner.ce4PortNotHandled(port);
}*/

