#include "comportscanner.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

ComPortScanner::ComPortScanner()
{    
    availablePorts_.clear();
    handledPorts_.clear();
}

QStringList ComPortScanner::scanForComPorts()
{
    QStringList myList;

    myList.clear();
    descriptions_.clear();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        /*QString s = QObject::tr("Port: ") + info.portName() + "\n"
                    + QObject::tr("Description: ") + info.description() + "\n"
                    + QObject::tr("Manufacturer: ") + info.manufacturer() + "\n"
                    + QObject::tr("Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";
                    */

        //if (info.description() == "Pololu Micro Maestro 6-Servo Controller TTL Port") {
        if (info.description() == "Pololu Micro Maestro 6-Servo Controller Command Port") {
            //qDebug() << "Found a pololu on " << info.portName() << info.description() << info.manufacturer();
            myList.append(info.portName());
            descriptions_.append(info.description());
        }
    }

    return myList;
}

bool ComPortScanner::updateAvailableComPortLists(QStringList list,
                                                 QString *itemPtr,
                                                 QString *descrPtr)
{

    int i = 0;
    foreach (QString item, list) {
        if (!availablePorts_.contains(item)) {
            qDebug() << "ComPortScanner: Adding " << item << "on my list";
            availablePorts_.append(item);
            availablePortDescriptions_.append(descriptions_[i]);
        }
        i++;
    }

    i = 0;
    foreach (QString item, availablePorts_) {
        if ( !handledPorts_.contains(item)) {
            handledPorts_.append(item);
            *itemPtr = item;
            *descrPtr = availablePortDescriptions_[i];
            return true;
        }
        i++;
    }

    return false;
}

QStringList ComPortScanner::checkForDisapearedHandledPorts(QStringList list)
{
    QStringList disList;

    disList.clear();

    foreach (QString item, handledPorts_) {
        if (!list.contains(item)) {
            qDebug() << "ComPortScanner: " << item << "disapeared from ports";
            disList.append(item);
        }
    }

    foreach (QString item, disList) {
        int index = availablePorts_.indexOf(item);
        availablePortDescriptions_.removeAt(index);
        availablePorts_.removeOne(item);
        handledPorts_.removeOne(item);

        qDebug() << "ComPortScanner:  Removing" <<
                    item <<
                    "from my list (" <<
                    availablePorts_.size() <<
                    handledPorts_.size() << ")";
    }


    return disList;
}

/*void ComPortScanner::ce4PortNotHandled(QString port)
{
    foreach (QString item, handledCe4Ports_) {
        if (item == port) {
            qDebug() << "Removing " << item << "from my list";
            handledCe4Ports_.removeOne(item);
        }
    }
}*/
