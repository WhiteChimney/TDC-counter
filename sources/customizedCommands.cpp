#include "externalapplicationswidget.h"
#include "ui_externalapplicationswidget.h"

namespace Ui {
class ExternalApplicationsWidget;
}

//  自定义串口操作文件
//  注: nbrSCC（单道计数的指针），vNbrCoin（包含符合计数的指针的向量），serial（串口）
//  dealSingleCountTimeup 指当单道计数计时时间到后，进行什么操作

//  测试 按钮被按下
void ExternalApplicationsWidget::on_buttonTest_released()
{
    QString commandAskID = "*IDN?";                    // 查询仪器 ID
    sendData(commandAskID);                            // 发送指令
    QThread::msleep(100);                              // 加延时，不然读不到数据
    QString answer = readData();                       // 读取回复
    qDebug() << answer;                                // Qt 内输出结果
    ui->whiteBoard->setText(answer);                   // UI 上输出结果

    QString commandBeep = ":SYSTem:BEEPer:IMMediate";   // 蜂鸣器发声指令
    sendData(commandBeep);                              // 模拟 SOS 指令
    QThread::msleep(500);
    sendData(commandBeep);
    QThread::msleep(500);
    sendData(commandBeep);
    QThread::msleep(1000);
    sendData(commandBeep);
    QThread::msleep(1000);
    sendData(commandBeep);
    QThread::msleep(1000);
    sendData(commandBeep);
    QThread::msleep(1000);
    sendData(commandBeep);
    QThread::msleep(500);
    sendData(commandBeep);
    QThread::msleep(500);
    sendData(commandBeep);

    // Thorlabs 温湿度计操作
    temperature = tsp->getTemperature();    // 获取温度
    humidity = tsp->getHumidity();          // 获取湿度

    answer += tr("\n当前温度：") + QString::number(temperature) + tr("℃");
    answer += tr("\n当前湿度：") + QString::number(humidity) + tr("%");
    ui->whiteBoard->setText(answer);

}

//  开始 按钮被按下
void ExternalApplicationsWidget::customizedSPcommands_start()
{
    qDebug() << "开始自定义程序";
}

//  停止 按钮被按下
void ExternalApplicationsWidget::customizedSPcommands_stop()
{
    qDebug() << "停止自定义程序";
}

//  单道计数刷新时
void ExternalApplicationsWidget::dealSingleCountTimeup()
{
    qDebug() << "单道计数刷新";
    qDebug() << tr("单道计数：") << *nbrSCC;
    qDebug() << tr("第一个与单道计数同步的符合面板的计数：") << *vNbrCoin.at(0);
    qDebug() << tr("第一个与单道计数同步的符合面板的计数：") << *vNbrCoin.at(0);
    //
}
