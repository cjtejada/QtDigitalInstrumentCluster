#include "obdconnection.h"

OBDConnection::OBDConnection()
{
    ConnectToSerialPort();
}

void OBDConnection::ConnectToSerialPort()
{
    QSerialPortInfo serialInfo;
    QList <QSerialPortInfo> availablePorts;

    QByteArray data;
    QRegExp regExOk(".*OK.*");

    availablePorts = serialInfo.availablePorts();
    if(!availablePorts.empty())
        m_serial.setPortName(availablePorts.at(0).portName());
    m_serial.setBaudRate(QSerialPort::Baud38400);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);
    if(m_serial.open(QIODevice::ReadWrite)){
        qDebug() << "Port Connected!";

        //Verify OBD Connection
        m_serial.write("ATE0\r");
        QThread::msleep(40);

        while(data.isEmpty()){
            data = m_serial.readAll();
            if(regExOk.exactMatch(data)){                
                RequestClusterData();
            }
            else
                data = "";
        }
        qDebug() << "ERROR: UNEXPECTED EXIT...";
    }
    else
        qDebug() << "ERROR: UNABLE TO OPEN PORT.";
}

void OBDConnection::RequestClusterData()
{
    QByteArray data;

    m_serial.write(PID.getMODE01CurrentData() +
                   PID.getRPM() +
                   PID.getSpeed() +
                   PID.getFuelStatus() +
                   PID.getEngineCoolantTemp() +
                   PID.getMODE03TROUBLECODES());

    QThread::msleep(75);
    data = m_serial.readAll();

    ParseAndReportClusterData(data);

}

void OBDConnection::ParseAndReportClusterData(QByteArray data)
{

    //parse data


    //report data

}

