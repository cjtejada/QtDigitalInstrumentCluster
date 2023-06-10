#ifndef INSTRUMENTCLUSTER_H
#define INSTRUMENTCLUSTER_H

#include <QObject>
#include <QSerialPort>
#include "serialobd.h"
#include "serialgps.h"

class InstrumentCluster : public QObject
{
    Q_OBJECT
public:

    ///run OBD object in a different thread,
    ///that way there's no blocking operations
    InstrumentCluster(){
        OBD = new SerialOBD;
        GPS = new SerialGPS;

        ClusterThread = new QThread;
        GpsThread = new QThread;

        GPS->moveToThread(GpsThread);
        OBD->moveToThread(ClusterThread);

        connect(this,SIGNAL(start()),OBD,SLOT(ConnectToSerialPort()));
        connect(this,SIGNAL(start()),GPS,SLOT(ConnectToSerialPort()));

        //connect signals in OBD object to this object, to report to QML
        connect(OBD,SIGNAL(obdRPM(int)),this,SIGNAL(obdRPM(int)));
        connect(OBD,SIGNAL(obdMPH(int)),this,SIGNAL(obdMPH(int)));
        connect(OBD,SIGNAL(obdFuelStatus(int)),this,SIGNAL(obdFuelStatus(int)));
        connect(OBD,SIGNAL(obdCoolantTemp(int)),this,SIGNAL(obdCoolantTemp(int)));
        connect(OBD,SIGNAL(obdTroubleCode(QByteArray)),this,SIGNAL(obdTroubleCode(QByteArray)));

        ClusterThread->start();
        GpsThread->start();
    }

    ~InstrumentCluster(){
        ClusterThread->quit();
    }

signals:
    void start();
    void obdRPM(int rpm);
    void obdMPH(int speed);
    void obdFuelStatus(int fuel);
    void obdCoolantTemp(int coolantTemp);
    void obdThrottlePosition(int throttle);
    void obdTroubleCode(QByteArray troublecode);
    void gpsLat(float Lat);
    void gpsLong(float Long);
public slots:

private:
    SerialGPS* GPS;
    SerialOBD* OBD;
    QThread* ClusterThread;
    QThread* GpsThread;

};

#endif // INSTRUMENTCLUSTER_H
