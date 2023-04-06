#include "dp832a_serial.h"

DP832A_Serial::DP832A_Serial(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
}


bool DP832A_Serial::initializeDevice(QSerialPortInfo spInfo,
                                     QSerialPort::BaudRate baudRate,
                                     QSerialPort::DataBits dataBits,
                                     QSerialPort::StopBits stopBits,
                                     QSerialPort::Parity parity)
{
    if (!serial->isOpen())
    {
        serial->setPort(spInfo);
        serial->setBaudRate(baudRate);
        serial->setDataBits(dataBits);
        serial->setStopBits(stopBits);
        serial->setParity(parity);
        return serial->open(QIODevice::ReadWrite);
    }
    else
        return false;
}

bool DP832A_Serial::closeDevice()
{
    if (serial->isOpen())
        serial->close();
    return true;
}

bool DP832A_Serial::sendCommand(QString dataText)
{
    if (serial->isOpen())
    {
        serial->write(dataText.toLocal8Bit().append("\n"));
        return serial->waitForBytesWritten();
    }
    else
        return false;
}

bool DP832A_Serial::readReply(QString *reply)
{
    if (serial->isOpen())
    {
        serial->waitForReadyRead(WAIT_FOR_READ_TIME);
        QByteArray buffer = serial->readAll();
        if (buffer.size() > 0)
        {
            *reply = buffer.remove(buffer.size()-1,1);
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

QString DP832A_Serial::readReply()
{
    if (serial->isOpen())
    {
        serial->waitForReadyRead(WAIT_FOR_READ_TIME);
        QByteArray buffer = serial->readAll();
        if (buffer.size() > 0)
            buffer = buffer.remove(buffer.size()-1,1);
        return buffer;
    }
    else
        return QString("");
}

bool DP832A_Serial::getVoltage(int channel, double *voltage)
{
    QString command = ":APPL? CH";
    command += QString::number(channel);
    command += ",VOLT";
    bool status = sendCommand(command);
    if (status)
    {
        QString reply;
        status = readReply(&reply);
        if (status)
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

double DP832A_Serial::getVoltage(int channel)
{
    QString command = ":APPL? CH";
    command += QString::number(channel);
    command += ",VOLT";
    sendCommand(command);
    return readReply().toDouble();
}

bool DP832A_Serial::setVoltage(int channel, double voltage)
{
    QString command = ":APPL CH";
    command += QString::number(channel);
    command += ",";
    command += QString::number(voltage);
    return sendCommand(command);
}
