#ifndef COMPORTSCANNER_H
#define COMPORTSCANNER_H

#include <QObject>

class ComPortScanner : public QObject
{
    Q_OBJECT

public:
    ComPortScanner();

    QStringList scanForComPorts();
    bool updateAvailableComPortLists(QStringList list,
                                     QString *itemPtr,
                                     QString *descrPtr);
    QStringList checkForDisapearedHandledPorts(QStringList list);

    //void ce4PortNotHandled(QString port);
private:

    QStringList availablePorts_;
    QStringList handledPorts_;
    QStringList availablePortDescriptions_;
    QStringList descriptions_;

};

#endif // COMPORTSCANNER_H
