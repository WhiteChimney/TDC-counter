#include "dp832a_visa.h"

dp832a_visa::dp832a_visa(QString resourceName, QObject *parent) : QObject(parent)
{
    qDebug() << "11";
    rscName = new char[MAX_SCPI_LEN];
    qDebug() << "12";
    strcpy_s(rscName,resourceName.size()+1,resourceName.toStdString().c_str());
    qDebug() << "13";
}

dp832a_visa::~dp832a_visa()
{
}

ViStatus dp832a_visa::initializeDevice()
{
//    打开设备，获取指针
    nRetStatus = viOpenDefaultRM(&rmSession);
    nRetStatus = viOpen(rmSession,rscName,VI_NULL,VI_NULL,&pInstrHandle);

//    设置 VISA 格式
    nRetStatus = viSetAttribute(pInstrHandle,VI_ATTR_TMO_VALUE,DEFAULT_TMO);
    nRetStatus = viSetAttribute(pInstrHandle,VI_ATTR_SUPPRESS_END_EN,VI_FALSE);
    nRetStatus = viSetAttribute(pInstrHandle,VI_ATTR_SEND_END_EN,VI_FALSE);

    return nRetStatus;

}

ViStatus dp832a_visa::closeDevice()
{
    nRetStatus = viClose(pInstrHandle);
    nRetStatus = viClose(rmSession);
    delete rscName;
    return nRetStatus;
}

ViStatus dp832a_visa::sendCommand(QString* command, QString* result)
{
//    发送指令
    *command += "\n";
    char commandFormat[] = "%s";
    nRetStatus = viQueryf(pInstrHandle,command->toLocal8Bit().data(),commandFormat,rdBuff);

    qDebug() << rdBuff;
    qDebug() << QString(QLatin1String((char*)rdBuff));

    *result = QString(QLatin1String((char*)rdBuff));

    return nRetStatus;
}
