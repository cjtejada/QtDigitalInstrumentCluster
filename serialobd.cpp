#include "serialobd.h"

void SerialOBD::ConnectToSerialPort()
{

    ////TO USE WITH CAR DELETE THIS LINE AND THE
    /// LOOP AT THE BOTTOM OF THE PAGE
    ParseAndReportClusterData("");

    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    QSerialPortInfo serialInfo;
    QList <QSerialPortInfo> availablePorts;

    QByteArray data;
    QRegExp regExOk(".*OK.*");

    availablePorts = serialInfo.availablePorts();

    //qDebug() << availablePorts.at(0).portName();

    if(!availablePorts.empty())
        m_serial.setPortName(availablePorts.at(0).portName());
    m_serial.setBaudRate(QSerialPort::Baud38400);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);
    if(m_serial.open(QIODevice::ReadWrite)){
        qDebug() << "Port Connected!";

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

    //data = "//10C0D2F0503\r00D \r0: 41 0C 09 BB 0D 00 \r1: 2F BE 05 90 03 02 00 \r\r>0";

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
                {//What if the temp is -40????
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

    //QProcess::execute("clear");
    qDebug() << "_____________________________";

    RPM = QByteArray::fromHex(sRPM).toHex().toUInt(false,16) / 4;
    if(RPM > 0)
        qDebug() << "RPM: " << RPM;

    Speed = QByteArray::fromHex(sSpeed).toHex().toUInt(false,16) * 0.621371;
    if(Speed >= 0)
        qDebug() << "MPH: " << Speed;

    FuelStatus = QByteArray::fromHex(sFuelStatus).toHex().toUInt(false,16) * 0.392156;
    if(FuelStatus > 0)
        qDebug() << "FUEL :" << FuelStatus << "%" ;

    EngineCoolantTemp = (QByteArray::fromHex(sECoolantTemp).toHex().toUInt(false,16) - 40);
    if(EngineCoolantTemp > 0)
        qDebug() << "Coolant Temp: " << EngineCoolantTemp << "C";

    ThrottlePosition = QByteArray::fromHex(sThrottlePosition).toHex().toUInt(false,16) * 0.392156;
    if(ThrottlePosition > 0 && ThrottlePosition <= 100)
        qDebug() << "Throttle:" << ThrottlePosition << "%" ;

    qDebug() << "_____________________________";

    if(sTroubleCode[0] <= '3')
        TroubleCode = "Powertrain Code: P" + sTroubleCode;
    if(sTroubleCode[0] >= '4' && sTroubleCode[0] <= '7')
        TroubleCode = "Chassis Code: C" + sTroubleCode;
    if(sTroubleCode[0] == '8' || sTroubleCode[0] == '9' || sTroubleCode[0] == 'A' || sTroubleCode[0] == 'B')
        TroubleCode = "Body Code: B" + sTroubleCode;
    if(sTroubleCode[0] >= 'C' && sTroubleCode[0] <= 'F')
        TroubleCode = "Network Code: U" + sTroubleCode;

    //qDebug() << TroubleCode;

    ////DELETE THIS WHILE LOOP IF USING WITH CAR THROUGH OBD AND
    while(true){
        ///DELETE THIS AS WELL
        RPM = (RPM + 1) * 4;

        ArrayRPM[GaugeCount] = RPM;
        ArrayMPH[GaugeCount] = Speed;

        GaugeCount = 1;

        if(ArrayRPM[0] != 0 && ArrayRPM[1] != 0)
            DifRPM = -1 * (ArrayRPM[0] - ArrayRPM[1]);

        if(RPM != 0){
            int newrpm = 0;
            float i = DifRPM / 20;
            newrpm = ArrayRPM[0] + DifRPM;
            while(i != DifRPM){
                emit obdRPM(ArrayRPM[0] + i);
                i = i + DifRPM / 20;
                QThread::msleep(100 / 20);
            }
            ArrayRPM[0] = newrpm;
        }

        if(Speed > 0)
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

}
