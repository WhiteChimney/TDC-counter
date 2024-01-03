#include "qaskydelayboard.h"

QaskyDelayBoard::QaskyDelayBoard(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
}


bool QaskyDelayBoard::initializeDevice(QSerialPortInfo spInfo,
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

bool QaskyDelayBoard::closeDevice()
{
    if (serial->isOpen())
        serial->close();
    return true;
}

bool QaskyDelayBoard::sendCommand(QString dataText)
{
    if (serial->isOpen())
    {
        serial->write(dataText.toLocal8Bit().append("\n"));
        return serial->waitForBytesWritten();
    }
    else
        return false;
}

bool QaskyDelayBoard::sendCommand()
{
    if (serial->isOpen())
    {
        serial->write(generateCommand().toLocal8Bit().append("\n"));
        return serial->waitForBytesWritten();
    }
    else
        return false;
}

bool QaskyDelayBoard::readReply(QString *reply)
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

QString QaskyDelayBoard::readReply()
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

QString QaskyDelayBoard::generateCommand()
{
    QString command = "6868";
    for(int i = 0; i < 8; i++)
    {
        command += QString("%1").arg(voltage[i],4,16,QLatin1Char('0'));
        command += QString("%1").arg(delay[i],4,16,QLatin1Char('0'));
    }
    command += "16";
    qDebug() << command;
    return command;
}

bool QaskyDelayBoard::setVoltage(int channel, double m_voltage)
{
    if (channel < 0 or channel > 7) return false;
    if (m_voltage < MIN_VOLTAGE) voltage[channel] = MIN_VOLTAGE;
    if (m_voltage > MAX_VOLTAGE) voltage[channel] = MAX_VOLTAGE;
    voltage[channel] = int(m_voltage);
    return true;
}

bool QaskyDelayBoard::setDelay(int channel, double m_delay)
{
    if (channel < 0 or channel > 7) return false;
    if (m_delay < MIN_DELAY) delay[channel] = MIN_DELAY;
    if (m_delay > MAX_DELAY) delay[channel] = 4095;
    delay[channel] = int(m_delay/20)*4095;
    return true;
}
