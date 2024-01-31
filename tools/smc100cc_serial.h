#ifndef SMC100CC_SERIAL_H
#define SMC100CC_SERIAL_H

#include <QObject>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#define WAIT_FOR_READ_TIME 1000

class SMC100CC_Serial : public QObject
{
    Q_OBJECT
public:
    explicit SMC100CC_Serial(QObject *parent = nullptr);

signals:

private:
    QSerialPort *serial;

public:
    bool initializeDevice(QSerialPortInfo spInfo,
                          QSerialPort::BaudRate baudRate = QSerialPort::Baud57600,
                          QSerialPort::DataBits dataBits = QSerialPort::Data8,
                          QSerialPort::StopBits stopBits = QSerialPort::OneStop,
                          QSerialPort::Parity parity = QSerialPort::NoParity);
    bool homingDevice(int address);
    bool closeDevice();
    bool readReply(QString* reply);
    QString readReply();
    bool sendCommand(QString command);

    bool getAbsoluteAngle(int address, double* angle);
    double getAbsoluteAngle(int address);
    bool setAbsoluteAngle(int address, double angle);
    bool setRelativeAngle(int address, double angle);

};

#endif // SMC100CC_SERIAL_H
