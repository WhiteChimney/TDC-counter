#ifndef SPD300QSKY_H
#define SPD300QSKY_H

#include <QWidget>
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QElapsedTimer>

class Spd300Qsky : public QWidget
{
    Q_OBJECT

public:
    explicit Spd300Qsky(QWidget *parent = nullptr);
    ~Spd300Qsky();

    void initializeDevice(QSerialPortInfo spInfo);

    void scan_port(int portNumber, QString* portName);

    void set_delay(double delay);

    void InstrWrite(int delay);

    void closeDevice();


private:
    QStringList port_string_line_;
    QSerialPort * my_port_;
    QByteArray byt_command_;
    QByteArray byt_ret_value_;
    QString portName;

};

#endif // SPD300QSKY_H
