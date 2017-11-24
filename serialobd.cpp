#include "serialobd.h"

void SerialOBD::ConnectToSerialPort()
{
    //ParseAndReportClusterData("OPPED\r\r>10C0D2F0511\rS");
    connect(this,SIGNAL(onEngineOff()),this,SLOT(EngineOff()));
    QSerialPortInfo serialInfo;
    QList <QSerialPortInfo> availablePorts;

    QByteArray data;
    QRegExp regExOk(".*OK.*");

    availablePorts = serialInfo.availablePorts();

    if(!availablePorts.empty()){
        qDebug() << "1st Port Detected: " << availablePorts.at(0).portName();
        while(availablePorts.at(0).portName() != "ttyUSB0"){
            qDebug() << availablePorts.at(0).portName();
            QThread::msleep(500);
        }
        m_serial.setPortName(availablePorts.at(0).portName());
    }
    else
        qDebug() << "EMPTY PORT LIST...";

    m_serial.setBaudRate(QSerialPort::Baud38400);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);
    if(m_serial.open(QIODevice::ReadWrite)){
        qDebug() << "Port Connected!";

        QThread::msleep(250);
        while(data.isEmpty()){

            //Verify OBD Connection
            if(data.isEmpty())
                m_serial.write("ATE1\r");
            m_serial.waitForBytesWritten();
            m_serial.waitForReadyRead();
            QThread::msleep(50);
            data = m_serial.readAll();
            qDebug() << data;
            if(regExOk.exactMatch(data)){
                while(!data.isEmpty())
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
void SerialOBD::RequestClusterData()
{
    QByteArray data;
    m_tCodeCounter++;

    if(m_tCodeCounter == 100){
        m_serial.write(PID.getMODE03TROUBLECODES() + "\r");
    }
    else{
        m_serial.write(PID.getMODE01CurrentData() +
                       PID.getRPM() +
                       PID.getSpeed() +
                       PID.getFuelTankLevel() +
                       PID.getEngineCoolantTemp() +
                       PID.getThrottlePosition() + "\r");
    }

    m_serial.waitForBytesWritten();
    m_serial.waitForReadyRead();
    QThread::msleep(100);
    data = m_serial.readAll();
    qDebug() << data;
    ParseAndReportClusterData(data);


}

void SerialOBD::ParseAndReportClusterData(QByteArray data)
{
    QByteArray tempData;
    QByteArray sRPM, sSpeed, sFuelStatus, sEngineCoolantTemp, sTroubleCode, sThrottlePosition;
    int k = 0;
    QRegExp TCodeRegEx(".*43.*");
    QRegExp dataTemp(".*\\d:\\s\\w\\w\\s\\w\\w\\s\\w\\w\\s\\w\\w\\s\\w\\w.*");

    if(!dataTemp.exactMatch(data) && !TCodeRegEx.exactMatch(data))
        data = "";

    if(dataTemp.exactMatch(data)){

        int i = 0;

        while(data[i] != ':')
            i++;

        data = data.mid(i + 1,data.length() - i);

        data = data.replace(" ", "").replace("\r1","").replace("\r2", "").replace("\r3", "").replace("\r", "").replace(":","")
                .replace(">", "").replace("r","");

        for(int i = 0; i < data.length(); i++){
            if(data[i] < '0' && data[i] > 'F')
                data.replace(data[i], "");
        }

        if(data.count() % 2 != 0){
            data == data.append("X");
        }
    }

    if(m_tCodeCounter == 20 || TCodeRegEx.exactMatch(data)){
        int i = 0;
        while(data[i] != '4' && data[i + 1] != '3')
            i++;

        data = data.mid(i,data.length() - i);

        data = data.replace(" ", "").replace("\r1","").replace("\r2", "").replace("\r3", "").replace("\r", "").replace(":","")
                .replace(">", "").replace("r","");

        m_tCodeCounter = 0;
    }

    for(int i = 0; i < data.length(); i++){

        if((data[i] >= '0' && data[i] <= '9') || (data[i] >= 'A' && data[i] <= 'F') || (data[i] != 'i')){
            tempData[k] = data[i];

            k++;

            if(tempData.length() == 2){

                if(tempData == PID.getRPM())
                {
                    for(int j = 0; j < 4; j++)
                        sRPM[j] = data[i + 1], i++;
                }
                if(tempData  == PID.getSpeed())
                {
                    for(int j = 0; j < 2; j++)
                        sSpeed[j] = data[i + 1], i++;
                }
                if(tempData == PID.getFuelTankLevel())
                {
                    for(int j = 0; j < 2; j++)
                        sFuelStatus[j] = data[i + 1], i++;
                }
                if(tempData == PID.getEngineCoolantTemp())
                {
                    for(int j = 0; j < 2; j++)
                        sEngineCoolantTemp[j] = data[i + 1], i++;
                }
                if(tempData == PID.getThrottlePosition())
                {
                    for(int j = 0; j < 2; j++)
                        sThrottlePosition[j] = data[i + 1], i++;
                }
                if(tempData == "43")
                {
                    for(int j = 0; j < 4; j++)
                        sTroubleCode[j] = data[i + 1], i++;
                }
                tempData = "";
                k = 0;
            }
        }
    }

    HexToDecimal(sRPM,sSpeed,sFuelStatus,sEngineCoolantTemp,sThrottlePosition, sTroubleCode);
}

void SerialOBD::EngineOff()
{
    qDebug() << "NIGNE OFF";
    emit obdRPM(0);
    emit obdCoolantTemp(-100);
    emit obdThrottlePosition(0);
}

void SerialOBD::HexToDecimal(QByteArray sRPM, QByteArray sSpeed, QByteArray sFuelStatus, QByteArray sECoolantTemp, QByteArray sThrottlePosition, QByteArray sTroubleCode)
{
    int RPM = 0;
    int Speed = 0;
    int FuelStatus = 0;
    int EngineCoolantTemp = 0;
    int ThrottlePosition = 0;
    QByteArray TroubleCode;

    RPM = QByteArray::fromHex(sRPM).toHex().toUInt(false,16) / 4;
    Speed = QByteArray::fromHex(sSpeed).toHex().toUInt(false,16) * 0.621371;
    FuelStatus = QByteArray::fromHex(sFuelStatus).toHex().toUInt(false,16) * 0.392156;
    EngineCoolantTemp = (QByteArray::fromHex(sECoolantTemp).toHex().toUInt(false,16));
    ThrottlePosition = QByteArray::fromHex(sThrottlePosition).toHex().toUInt(false,16) * 0.392156;

    if(RPM == 0)
        ArrayEngineOff[m_engineOffcount] = true;
    else
        ArrayEngineOff[m_engineOffcount] = false;
    m_engineOffcount++;

    if(sTroubleCode[0] >= '0' && sTroubleCode[0] <= '3')
        TroubleCode = "P" + sTroubleCode;
    if(sTroubleCode[0] >= '4' && sTroubleCode[0] <= '7')
        TroubleCode = "C" + sTroubleCode;
    if(sTroubleCode[0] == '8' || sTroubleCode[0] == '9' ||
            sTroubleCode[0] == 'A' || sTroubleCode[0] == 'B')
        TroubleCode = "B" + sTroubleCode;
    if(sTroubleCode[0] >= 'C' && sTroubleCode[0] <= 'F')
        TroubleCode = "U" + sTroubleCode;

    if(RPM > 100)
        emit obdRPM(RPM);
    if(Speed > 0)
        emit obdMPH(Speed);
    if(FuelStatus > 0 )
        emit obdFuelStatus(FuelStatus);
    if(EngineCoolantTemp > 0)
        emit obdCoolantTemp(EngineCoolantTemp);
    if(ThrottlePosition > 0)
        emit obdThrottlePosition(ThrottlePosition);
    if(TroubleCode != "")
        emit obdTroubleCode(TroubleCode);

    if(ArrayEngineOff[0] == true && ArrayEngineOff[1] == true && ArrayEngineOff[2] == true)
        EngineOff();
    if(m_engineOffcount == 3){
        m_engineOffcount = 0;
        ArrayEngineOff[0] = false;
        ArrayEngineOff[1] = false;
        ArrayEngineOff[2] = false;
    }
}
