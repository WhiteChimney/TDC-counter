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
