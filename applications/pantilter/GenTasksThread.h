#ifndef GENERAL_TASKS_THREAD_H
#define GENERAL_TASKS_THREAD_H

#include <QThread>
#include <QStringList>
#include "ComPortScanner.h"

class GenTasksThread : public QThread
{
    Q_OBJECT

protected:
    void run();

public:   

    GenTasksThread();
    ~GenTasksThread();
    void exitThread();


signals:
    void sig_foundValidPort(QString, QString);
    void sig_handledPortDisapeared(QString);

public slots:
    //void slot_portNotHandled(QString port);

private:
    enum gen_operation
    {
        genop_none,        
    };        

    ComPortScanner scanner;
    bool threadActive_;
};

#endif // GENERAL_TASKS_THREAD_H
