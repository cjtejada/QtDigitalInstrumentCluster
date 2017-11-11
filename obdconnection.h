#ifndef OBDCONNECTION_H
#define OBDCONNECTION_H

#include "instrumentcluster.h"
#include "pids.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
//#include <QList>
//#include <QThread>
//#include <QByteArray>
//#include <QRegExp>

class OBDConnection
{

public:
    explicit OBDConnection();

    void ConnectToSerialPort();
    void RequestClusterData();
    void ParseAndReportClusterData(QByteArray data);

private:
    //InstrumentCluster m_Dash;
    PIDs PID;
    QSerialPort m_serial;

};

#endif // OBDCONNECTION_H
