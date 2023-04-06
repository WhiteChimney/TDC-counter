#ifndef DP832A_SERIAL_H
#define DP832A_SERIAL_H

#include <QObject>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#define WAIT_FOR_READ_TIME 1000

class DP832A_Serial : public QObject
{
    Q_OBJECT
public:
    explicit DP832A_Serial(QObject *parent = nullptr);

signals:

private:
    QSerialPort *serial;

public:
    bool initializeDevice(QSerialPortInfo spInfo,
                          QSerialPort::BaudRate baudRate,
                          QSerialPort::DataBits dataBits,
                          QSerialPort::StopBits stopBits,
                          QSerialPort::Parity parity);
    bool closeDevice();
    bool readReply(QString* reply);
    QString readReply();
    bool sendCommand(QString command);

    bool setVoltage(int channel, double voltage);

};

#endif // DP832A_SERIAL_H
