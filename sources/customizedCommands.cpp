#include "externalapplicationswidget.h"
#include "ui_externalapplicationswidget.h"
#include <QtMath>
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

/*   //稳相位
    static int i=0;      //用于数秒
    static int k=0;      //用于数每次调整到最大干涉需要调几次电压
    static int coin10=0,coin01=0,coin11=0,coin00=0,tag=0;     //各对通道的符合计数
    static double thsum=0,thaver1=0,thaver2=0,error1=0,error2=0;           //用于温度求和
    int coin10r=0,coin01r=0,coin11r=0,coin00r=0;
    double th1r, phi1;
    i++;
    coin10r=3;                 //读取一次符合计数
    coin01r=0;
    coin11r=50;
    coin00r=50;
    th1r=22.5;               //读取温度
    if(i<11){
   coin10=coin10+coin10r;                //累计10s内的数据
   coin01=coin01+coin01r;
   coin11=coin11+coin11r;
   coin00=coin00+coin00r;
   thsum=thsum+th1r;
    }
    else{
     error1 = (double)(coin10+coin01)/(coin00+coin11+coin01+coin10);
     thaver1=thsum/10;
     phi1 = 1-(acos(2*error1-1))/3.1415926535897;
     if(error1<0.01){
         qDebug() << "k=" << k;
         k=0;
     }
     else{
         k++;
         if(k==1){
             if(thaver1-thaver2){
                 tag = 0;
                 qDebug()<<"输出负电压:"<<phi1/0.06;
                 qDebug()<<"phi1" << phi1;

             }
             else{
                 tag = 1;
                 qDebug()<<"输出正电压:"<<phi1/0.06;
             }
         }
         else{
             if(error2>error1){
                 if(tag==0){
                     qDebug()<< "输出负电压："<< "1";
                     tag=0;
                 }
                 else{
                     qDebug()<< "输出正电压："<< "1";
                     tag=1;
                 }
             }
             else{
                 if(tag==0){
                   qDebug()<< "输出正电压:" << "1";
                   tag = 1;
                  }
                 else{
                   qDebug()<< "输出负电压:"<< "1";
                   tag = 0;
                  }
             }
         }
     }

     i=1;
     thaver2 = thaver1;
     error2 = error1;
     coin10=coin10r;
     coin01=coin01r;
     coin11=coin11r;
     coin00=coin00r;
    }


*/



}

//  开始 按钮被按下
void ExternalApplicationsWidget::customizedSPcommands_start()
{
    qDebug() << "开始自定义程序";
    QString setcurrport = ":CURR:PROT 5.3";
    sendData(setcurrport);
    QThread::msleep(100);
    QString setcurrporton = ":CURR:PROT:STAT ON";
    sendData(setcurrporton);
    QThread::msleep(100);
    QString setinitialvol = ":APPL CH1,5,2";              //设置初始电压，需要实验前先测试出消光的电压
    sendData(setinitialvol);
    QThread::msleep(100);
    QString setoutputon = ":CURR:PROT:STAT ON";            //打开输出
    sendData(setoutputon);
    QThread::msleep(100);

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

    QString setvol = ":APPL CH1"  ;
    static int i=0;      //用于数秒
    static int k=0;      //用于数每次调整到最大干涉需要调几次电压
    static int coin10=0,coin01=0,coin11=0,coin00=0,tag=0;     //各对通道的符合计数
    static double thsum=0,thaver1=0,thaver2=0,error1=0,error2=0;           //用于温度求和
    int coin10r=0,coin01r=0,coin11r=0,coin00r=0;
    double th1r, phi1;
    i++;
    coin10r=*vNbrCoin.at(0);                 //读取一次符合计数  符合面板打开顺序要与其一致。
    coin01r=*vNbrCoin.at(1);
    coin11r=*vNbrCoin.at(2);
    coin00r=*vNbrCoin.at(3);
    th1r = tsp->getTemperature();;               //读取温度
    if(i<11){
   coin10=coin10+coin10r;                //累计10s内的数据
   coin01=coin01+coin01r;
   coin11=coin11+coin11r;
   coin00=coin00+coin00r;
   thsum=thsum+th1r;
    }
    else{
     error1 = (double)(coin10+coin01)/(coin00+coin11+coin01+coin10);
     thaver1=thsum/10;
     phi1 = 1-(acos(2*error1-1))/3.1415926535897;
     if(error1<0.015){
         qDebug() << "k=" << k;                               //后续要补充写入文档
         k=0;
     }
     else{
         k++;

         if(k==1){
             if((thaver1-thaver2)<0){
                 tag = 0;
                 qDebug()<<"输出负电压:"<<phi1/0.06;

               //  sendData(commandAskID);
             }
             else{
                 tag = 1;
                 qDebug()<<"输出正电压:"<<phi1/0.06;
             }
         }
         else{
             if(error2>error1){
                 if(tag==0){
                     qDebug()<< "输出负电压："<< "1";
                     tag=0;
                 }
                 else{
                     qDebug()<< "输出正电压："<< "1";
                     tag=1;
                 }
             }
             else{
                 if(tag==0){
                   qDebug()<< "输出正电压:" << "1";
                   tag = 1;
                  }
                 else{
                   qDebug()<< "输出负电压:"<< "1";
                   tag = 0;
                  }
             }
         }
     }

     i=1;
     thaver2 = thaver1;
     error2 = error1;
     coin10=coin10r;
     coin01=coin01r;
     coin11=coin11r;
     coin00=coin00r;
    }



}
