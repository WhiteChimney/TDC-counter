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
    serial->setPort(spInfo);
    serial->setBaudRate(baudRate);
    serial->setDataBits(dataBits);
    serial->setStopBits(stopBits);
    serial->setParity(parity);
    return serial->open(QIODevice::ReadWrite);
}

bool DP832A_Serial::closeDevice()
{
    if (serial->isOpen())
        serial->close();
    return true;
}

QString DP832A_Serial::readData()
{
    qDebug() << serial->waitForReadyRead(1000);
    QByteArray buffer = serial->readAll();
    if (buffer.size() > 0)
        buffer = buffer.remove(buffer.size()-1,1);
    return buffer;
}

void DP832A_Serial::sendCommand(QString dataText)
{
    serial->write(dataText.toLocal8Bit().append("\n"));
    serial->waitForBytesWritten();
}
