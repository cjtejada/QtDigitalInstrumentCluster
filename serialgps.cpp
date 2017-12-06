#include "serialgps.h"

SerialGPS::SerialGPS(QObject *parent) : QObject(parent)
{

}

void SerialGPS::ConnectToSerialPort()
{
    QSerialPortInfo serialInfo;
    QList <QSerialPortInfo> availablePorts;

    QByteArray data;
    QRegExp regExOk(".*OK.*");

    availablePorts = serialInfo.availablePorts();

    //Make sure serial connects to the corrent device
    if(!availablePorts.empty()){
        qDebug() << "1st Port Detected: " << availablePorts.at(0).portName();
        while(availablePorts.at(0).portName() != "ttyACM0"){
            qDebug() << availablePorts.at(0).portName();
            QThread::msleep(500);
        }
        m_serial.setPortName("ttyUSB0");
    }
    else
        qDebug() << "EMPTY PORT LIST...";//make sure the ports list is not empty

    //Set all the port settings
    m_serial.setBaudRate(QSerialPort::Baud38400);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);
    if(m_serial.open(QIODevice::ReadWrite)){
        qDebug() << "Port Connected!";

        while(data.isEmpty()){

        }

    }

}
