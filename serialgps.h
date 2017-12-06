#ifndef SERIALGPS_H
#define SERIALGPS_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QThread>

class SerialGPS : public QObject
{
    Q_OBJECT
public:
    explicit SerialGPS(QObject *parent = nullptr);
    void GrabGeoLocation(QByteArray data);
    float LatDMMtoDD(QString sLat, bool isSouth);
    float LongDMMtoDD(QString sLong, bool isWest);

signals:
    void GPSLat(float Lat);
    void GPSLong(float Long);

public slots:
    void ConnectToSerialPort();
public:
    QSerialPort m_serial;
};

#endif // SERIALGPS_H
