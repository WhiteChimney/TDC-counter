#include "dp832a_usb.h"

DP832A_USB::DP832A_USB(QString resourceName, QObject *parent) : QObject(parent)
{
    rscName = new char[MAX_SCPI_LEN];
    strcpy_s(rscName,resourceName.size()+1,resourceName.toStdString().c_str());
}

DP832A_USB::~DP832A_USB()
{
    delete rscName;
}

bool DP832A_USB::initializeDevice()
{
//    打开设备，获取指针
    nRetStatus = viOpenDefaultRM(&rmSession);
    nRetStatus = viOpen(rmSession,rscName,VI_NULL,VI_NULL,&pInstrHandle);

//    设置 VISA 格式
    nRetStatus = viSetAttribute(pInstrHandle,VI_ATTR_TMO_VALUE,DEFAULT_TMO);
    nRetStatus = viSetAttribute(pInstrHandle,VI_ATTR_SUPPRESS_END_EN,VI_FALSE);
    nRetStatus = viSetAttribute(pInstrHandle,VI_ATTR_SEND_END_EN,VI_FALSE);

    if (nRetStatus == VI_SUCCESS)
        return true;
    else
        return false;
}

bool DP832A_USB::closeDevice()
{
    nRetStatus = viClose(pInstrHandle);
    nRetStatus = viClose(rmSession);

    if (nRetStatus == VI_SUCCESS)
        return true;
    else
        return false;
}

bool DP832A_USB::sendCommand(QString* command, QString* result)
{
//    发送指令
    *command += "\n";
    char commandFormat[] = "%s";
    nRetStatus = viQueryf(pInstrHandle,command->toLocal8Bit().data(),commandFormat,rdBuff);

    *result = QString(QLatin1String((char*)rdBuff));

    if (nRetStatus == VI_SUCCESS)
        return true;
    else
        return false;
}

bool DP832A_USB::sendCommand2(QString command)
{
    //    发送指令
        command += "\n";
//        char commandFormat[] = "%s";
//        nRetStatus = viQueryf(pInstrHandle,command.toLocal8Bit().data(),commandFormat);



        char ch_command[MAX_SCPI_LEN];
        strcpy(ch_command,":APPL CH1,1.32\n");

        unsigned long retcnt = DEFAULT_TMO;
        nRetStatus = viWrite(pInstrHandle,(ViBuf)ch_command,(ViUInt32)strlen(ch_command),&retcnt);

        if (nRetStatus == VI_SUCCESS)
            return true;
        else
            return false;

}

void DP832A_USB::setVoltage(double voltage)
{
    QString command = ":APPL CH1,";
    command += QString::number(voltage);
    qDebug() << sendCommand2(command);
}
