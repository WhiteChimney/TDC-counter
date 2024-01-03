#ifndef QASKYDELAYBOARD_H
#define QASKYDELAYBOARD_H

#include <QObject>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#define WAIT_FOR_READ_TIME 1000

#define MIN_VOLTAGE 0
#define MAX_VOLTAGE 2047
#define MIN_DELAY 0
#define MAX_DELAY 4095

class QaskyDelayBoard : public QObject
{
    Q_OBJECT
public:
    explicit QaskyDelayBoard(QObject *parent = nullptr);

signals:

private:
    QSerialPort *serial;
    int voltage[8] = {0};
    int delay[8] = {0};

public:
    bool initializeDevice(QSerialPortInfo spInfo,
                          QSerialPort::BaudRate baudRate = QSerialPort::Baud115200,
                          QSerialPort::DataBits dataBits = QSerialPort::Data8,
                          QSerialPort::StopBits stopBits = QSerialPort::OneStop,
                          QSerialPort::Parity parity = QSerialPort::NoParity);
    bool closeDevice();
    bool readReply(QString* reply);
    QString readReply();
    bool sendCommand(QString command);
    bool sendCommand();

    // 延时数据格式说明
    // 使用 16 进制字符串进行通信，共 70 位
    // 第 1-4 位为固定 0x6868
    // 第 5-68 位为各通道的甄别电平与延时设定
    // 每个通道的前 4 位为电平，单位 mV
    // 每个通道的后 4 位为延时量，范围为 0-20 ns，
    // 可调离散点数为 4096 个，因此精度约为 5 ps
    // 以上为一个通道的数据格式，8 个通道依次往后排
    // 最后 2 位为固定 0x16
    QString generateCommand();
    bool setVoltage(int channel, int m_voltage);
    int getVoltage(int channel);
    bool setDelay(int channel, double m_delay);
    bool setRelativeDelay(int channel, double m_delay);
    double getDelay(int channel);


};

#endif // QASKYDELAYBOARD_H
