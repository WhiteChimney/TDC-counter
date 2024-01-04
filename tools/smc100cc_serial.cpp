#include "smc100cc_serial.h"

SMC100CC_Serial::SMC100CC_Serial(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
}


bool SMC100CC_Serial::initializeDevice(QSerialPortInfo spInfo,
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

bool SMC100CC_Serial::homingDevice(int address)
{
    return sendCommand(QString::number(address) + "OR");
}

bool SMC100CC_Serial::closeDevice()
{
    if (serial->isOpen())
        serial->close();
    return true;
}

bool SMC100CC_Serial::sendCommand(QString dataText)
{
    qDebug() << dataText;
    if (serial->isOpen())
    {
        serial->write(dataText.toLocal8Bit().append("\r\n"));
        return serial->waitForBytesWritten();
    }
    else
        return false;
}

bool SMC100CC_Serial::readReply(QString *reply)
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

QString SMC100CC_Serial::readReply()
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

bool SMC100CC_Serial::getAbsoluteAngle(int address, double* angle)
{
    QString command = QString::number(address) + "PA?";
    bool status = sendCommand(command);
    if (status)
    {
        QString reply;
        status = readReply(&reply);
        if (status)
        {
            qDebug() << reply;
            *angle = reply.toDouble();
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

double SMC100CC_Serial::getAbsoluteAngle(int address)
{
    QString command = QString::number(address) + "PA?";
    sendCommand(command);
    return readReply().toDouble();
}

bool SMC100CC_Serial::setAbsoluteAngle(int address, double angle)
{
    QString command = QString::number(address) + "PA" + QString::number(angle);
    return sendCommand(command);
}

bool SMC100CC_Serial::setRelativeAngle(int address, double angle)
{
    QString command = QString::number(address) + "PR" + QString::number(angle);
    return sendCommand(command);
}
