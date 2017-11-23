#ifndef SERIALOBD_H
#define SERIALOBD_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QProcess>
#include <QThread>
#include <QObject>
#include <QTimer>

#include "pids.h"

class SerialOBD : public QObject
{
    Q_OBJECT
public:
    void RequestClusterData();
    void ParseAndReportClusterData(QByteArray data);
    void HexToDecimal(QByteArray sRPM, QByteArray sSpeed, QByteArray sFuelStatus, QByteArray sECoolantTemp, QByteArray sThrottlePosition, QByteArray sTroubleCode);

signals:
    void obdRPM(int rpm);
    void obdMPH(int speed);
    void obdFuelStatus(int fuel);
    void obdCoolantTemp(int coolantTemp);
    void obdThrottlePosition(int throttle);
    void obdTroubleCode(QByteArray troublecode);

public slots:
    void ConnectToSerialPort();
    void EngineOff();

private:
    PIDs PID;
    QSerialPort m_serial;
    int m_tCodeCounter = 0;
};

#endif // SERIALOBD_H
