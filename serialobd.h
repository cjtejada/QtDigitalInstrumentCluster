#ifndef SERIALOBD_H
#define SERIALOBD_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QProcess>
#include <QThread>
#include <QObject>

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


private:
    PIDs PID;
    QSerialPort m_serial;
    int m_tCodeCounter = 0;

    int GaugeCount = 0;

    float DifRPM = 0;
    int DifMPH = 0;

    int ArrayRPM[2] = {0};
    int ArrayMPH[2] = {0};

};

#endif // SERIALOBD_H
