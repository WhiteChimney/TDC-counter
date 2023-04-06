#include "dp832a_usb.h"

DP832A_USB::DP832A_USB(QString resourceName0, QObject *parent) : QObject(parent)
{
    resourceName = resourceName0;
}

DP832A_USB::~DP832A_USB()
{
}

bool DP832A_USB::initializeDevice()
{
//    仪器地址
    SendAddr = resourceName.toLocal8Bit().data();

//    打开设备，获取指针
    status = viOpenDefaultRM(&defaultRM);
    if (status < VI_SUCCESS)
        return false;
    status = viOpen(defaultRM,SendAddr,VI_NULL,VI_NULL,&instr);

    if (status == VI_SUCCESS)
        return true;
    else
        return false;
}

bool DP832A_USB::closeDevice()
{
    if (status == VI_SUCCESS)
    {
        status = viClose(instr);
        status = viClose(defaultRM);

        if (status == VI_SUCCESS)
            return true;
        else
            return false;
    }
    else
        return false;
}

bool DP832A_USB::sendCommand(QString command)
{
//    发送指令
    if (status == VI_SUCCESS)
    {
        command += "\n";
        SendBuf = command.toLocal8Bit().data();
        status = viWrite(instr,(unsigned char*)SendBuf,strlen(SendBuf),&retCount);

        if (status == VI_SUCCESS)
            return true;
        else
            return false;
    }
    else
        return false;
}

bool DP832A_USB::readReply(QString* reply)
{
    if (status == VI_SUCCESS)
    {
        memset(RecBuf,0,MAX_REC_SIZE);
        status = viRead(instr,RecBuf,MAX_REC_SIZE,&retCount);
        if (status == VI_SUCCESS)
        {
            *reply = QString(QLatin1String((char*)RecBuf));
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

QString DP832A_USB::readReply()
{
    if (status == VI_SUCCESS)
    {
        memset(RecBuf,0,MAX_REC_SIZE);
        status = viRead(instr,RecBuf,MAX_REC_SIZE,&retCount);
        if (status == VI_SUCCESS)
        {
            return QString(QLatin1String((char*)RecBuf));
        }
        else
            return QString("");
    }
    else
        return QString("");
}

bool DP832A_USB::getVoltage(int channel, double *voltage)
{
    QString command = ":APPL? CH";
    command += QString::number(channel);
    command += ",VOLT";
    status = sendCommand(command);
    if (status == VI_SUCCESS)
    {
        QString reply;
        status = readReply(&reply);
        if (status == VI_SUCCESS)
        {
            *voltage = reply.toDouble();
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

double DP832A_USB::getVoltage(int channel)
{
    QString command = ":APPL? CH";
    command += QString::number(channel);
    command += ",VOLT";
    sendCommand(command);
    return readReply().toDouble();
}

bool DP832A_USB::setVoltage(int channel, double voltage)
{
    QString command = ":APPL CH";
    command += QString::number(channel);
    command += ",";
    command += QString::number(voltage);
    return sendCommand(command);
}
