#include "serialobd.h"

void SerialOBD::ConnectToSerialPort()
{
    //ParseAndReportClusterData("");
    QSerialPortInfo serialInfo;
    QList <QSerialPortInfo> availablePorts;

    QByteArray data;
    QRegExp regExOk(".*OK.*");

    availablePorts = serialInfo.availablePorts();

    qDebug() << availablePorts.at(0).portName();

    if(!availablePorts.empty())
        m_serial.setPortName(availablePorts.at(0).portName());
    m_serial.setBaudRate(QSerialPort::Baud38400);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);
    if(m_serial.open(QIODevice::ReadWrite)){
        qDebug() << "Port Connected!";

        QThread::msleep(300);
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

    if(sTroubleCode[0] <= '3')
        TroubleCode = "Powertrain Code: P" + sTroubleCode;
    if(sTroubleCode[0] >= '4' && sTroubleCode[0] <= '7')
        TroubleCode = "Chassis Code: C" + sTroubleCode;
    if(sTroubleCode[0] == '8' || sTroubleCode[0] == '9' || sTroubleCode[0] == 'A' || sTroubleCode[0] == 'B')
        TroubleCode = "Body Code: B" + sTroubleCode;
    if(sTroubleCode[0] >= 'C' && sTroubleCode[0] <= 'F')
        TroubleCode = "Network Code: U" + sTroubleCode;

    //qDebug() << TroubleCode;
    ArrayRPM[GaugeCount] = RPM;
    ArrayMPH[GaugeCount] = Speed;

    if(GaugeCount == 1)
        DifRPM = -1 * (ArrayRPM[0] - ArrayRPM[1]);

    if(GaugeCount == 1)
        DifMPH = -1 * (ArrayMPH[0] - ArrayMPH[1]);

    if(ArrayRPM[0] != 0)
        GaugeCount = 1;

    int newdifrpm;

    if(DifRPM < 0)
        newdifrpm = -DifRPM;
    else
        newdifrpm = DifRPM;

    if(RPM > 200){
        int r = 0;
        int newrpm = ArrayRPM[0] + DifRPM;
        for(int i = 0; i != (newdifrpm - (newdifrpm % 9)) / 9; i++){
            emit obdRPM(ArrayRPM[0] + r + (DifRPM % 9));
            if(DifRPM < 0)
                r = r - 9;
            else
                r = r + 9;
            qDebug() << ArrayRPM[0] + r + (DifRPM % 9);
            //QThread::msleep(100 / ((newdifrpm - (newdifrpm % 9)) / 9));
        }
        ArrayRPM[0] = newrpm;
    }
    if(DifRPM < 36 && RPM > 100)
        emit obdRPM(RPM);

    if(Speed != 0)
        emit obdMPH(Speed);

    if(FuelStatus > 0 )
        emit obdFuelStatus(FuelStatus);

    if(EngineCoolantTemp > 0)
        emit obdCoolantTemp(EngineCoolantTemp);

    if(ThrottlePosition > 0)
        emit obdThrottlePosition(ThrottlePosition);
    if(TroubleCode != "0")
        emit obdTroubleCode(TroubleCode);
}
