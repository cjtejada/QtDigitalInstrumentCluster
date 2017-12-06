#include "serialgps.h"

SerialGPS::SerialGPS(QObject *parent) : QObject(parent)
{

}

void SerialGPS::ConnectToSerialPort()
{
    QSerialPortInfo serialInfo;
    QList <QSerialPortInfo> availablePorts;

    QByteArray data;
    QRegExp geoRegEx(".*GPGLL.*");


    availablePorts = serialInfo.availablePorts();

    //Make sure serial connects to the current device
    if(!availablePorts.empty()){
        qDebug() << "1st Port Detected: " << availablePorts.at(0).portName();
        while(availablePorts.at(0).portName() != "ttyACM0"){
            qDebug() << availablePorts.at(0).portName();
            QThread::msleep(500);
        }
        m_serial.setPortName("ttyACM0");
    }
    else
        qDebug() << "EMPTY PORT LIST...";//make sure the ports list is not empty

    //Set all the port settings
    m_serial.setBaudRate(QSerialPort::Baud38400);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);
    if(m_serial.open(QIODevice::ReadOnly)){
        qDebug() << "Port Connected!";

        while(m_serial.isOpen()){
            m_serial.waitForReadyRead(5000);
            data = m_serial.readLine();
            if(geoRegEx.exactMatch(data)){
                GrabGeoLocation(data);
            }
        }
    }
    else
        qDebug() << "DID NOT CONNECT TO PORT!";

}
void SerialGPS::GrabGeoLocation(QByteArray data)
{
    bool isSetLat = false;
    bool isSetLong = true;
    bool isSouth = false;
    bool isWest = false;

    QString sLatitude;
    QString sLongitude;

    float Latitude = 0;
    float Longitude = 0;

    for(int i = 0; i < data.length(); i++){

        if(data[i] == 'N')
            isSetLat = true, isSetLong = false;
        if(data[i] == 'E')
            isSetLong = true;
        if(data[i] == 'S')
            isSetLat = true, isSetLong = false, isSouth = true;
        if(data[i] == 'W')
            isSetLong = true, isWest = true;

        if(((data[i] >= '0' && data[i] <= '9') || data[i] == '.') && isSetLat == false)
            sLatitude[i] = data[i];

        if(((data[i] >= '0' && data[i] <= '9') || data[i] == '.') && isSetLong == false)
            sLongitude[i] = data[i];
    }

    Latitude = LatDMMtoDD(sLatitude.replace(" ", ""),isSouth);
    Longitude = LongDMMtoDD(sLongitude.replace(" ", ""),isWest);

    emit GPSLat(Latitude);
    emit GPSLong(Longitude);

    qDebug() << Latitude << Longitude;
}

float SerialGPS::LatDMMtoDD(QString sLat, bool isSouth)
{
    QString degrees;
    QString minutes;

    float Latitude = 0;

    int i = 0;

    for(; sLat[i] != '.'; i++);

    minutes[0] = sLat[i - 2];
    minutes[1] = sLat[i - 1];

    for(int j = 3; i < sLat.length(); j++)
        minutes[j] = sLat[i], i++;

    minutes = minutes.replace(" ", "");

    degrees = sLat.replace(minutes, "");

    Latitude = degrees.toFloat() + (minutes.toFloat() / 60);

    if(isSouth == true)
        Latitude = Latitude * -1;

    return Latitude;
}

float SerialGPS::LongDMMtoDD(QString sLong, bool isWest)
{
    QString degrees;
    QString minutes;

    float Longitude = 0;

    int i = 0;

    for(; sLong[i] != '.'; i++);

    minutes[0] = sLong[i - 2];
    minutes[1] = sLong[i - 1];

    for(int j = 3; i < sLong.length(); j++)
        minutes[j] = sLong[i], i++;

    minutes = minutes.replace(" ", "");

    degrees = sLong.replace(minutes, "");

    Longitude = degrees.toFloat() + (minutes.toFloat() / 60);

    if(isWest == true)
        Longitude = Longitude * -1;

    return Longitude;
}
