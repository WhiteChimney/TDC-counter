#ifndef DP832A_SERIAL_H
#define DP832A_SERIAL_H

#include <QObject>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

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
    QString readData();
    void sendCommand(QString dataText);

};

#endif // DP832A_SERIAL_H
