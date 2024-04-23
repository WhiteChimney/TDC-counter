#include "spd300qsky.h"

Spd300Qsky::Spd300Qsky(QWidget *parent) :
    QWidget(parent)
{
    my_port_ = new QSerialPort(this);
}

Spd300Qsky::~Spd300Qsky()
{
    closeDevice();
}

void Spd300Qsky::initializeDevice(QSerialPortInfo spInfo)
{
    if (!my_port_->isOpen())
    {
        // scan_port(portNumber,&portName);

        // my_port_->setPortName(portName);
        my_port_->setPort(spInfo);
        my_port_->setBaudRate(QSerialPort::Baud9600); //设置波特率
        my_port_->setParity(QSerialPort::NoParity);    //设置校验位
        my_port_->setDataBits(QSerialPort::Data8);     //设置数据位
        my_port_->setStopBits(QSerialPort::OneStop);   //设置停止位
        my_port_->setFlowControl(QSerialPort::NoFlowControl);   //设置流控制
        my_port_->open(QIODevice::ReadWrite);
    }
}

void Spd300Qsky::scan_port(int portNumber, QString *portName)                  //扫描串口
{
    port_string_line_.clear();
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) //查找可用的串口
    port_string_line_ +=info.portName();

    *portName = port_string_line_[portNumber];

    my_port_->setPortName(*portName);

    if(!my_port_->open(QIODevice::ReadWrite))
    {
        qDebug() << "串口打开失败";
        return;
    }
    else
    {
        qDebug() << "串口打开成功";
    }
}

void Spd300Qsky::InstrWrite(int delay)
{
    QString str_command_0 = "01 02 E1 86 ";
    QString str_command_1;
    QString str_command_2;
    QString str_command_3 = " 00 00 03";

    int high = (delay >> 8) & 0xff;	 	//高8位
    int low = delay & 0xff; 		//低8位

    str_command_1 = QString("%2").arg(low, 2, 16, QLatin1Char('0'));
    str_command_2 = QString("%2").arg(high, 2, 16, QLatin1Char('0'));
    QString str_command = str_command_0 + str_command_1 + " " + str_command_2 + str_command_3;
    byt_command_ = QByteArray::fromHex(str_command.toLatin1());

    my_port_ -> write(byt_command_);
}

void Spd300Qsky::set_delay(double delay)
{
    InstrWrite(int(100*delay)%2000);

    QElapsedTimer time;
    time.start();
    while(time.elapsed() < 150)
        QCoreApplication::processEvents();
}

void Spd300Qsky::closeDevice()
{
    if (my_port_->isOpen())
        my_port_->close();
}

