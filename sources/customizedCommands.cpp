#include "externalapplicationswidget.h"
#include "ui_externalapplicationswidget.h"

#include <QtMath>
#include <QTextStream>

#include "customizedfunctions.hpp"

namespace Ui {
class ExternalApplicationsWidget;
}


//  自定义操作文件
//  注: nbrSCC（单道计数的指针），vNbrCoin（包含符合计数的指针的向量）
//  dealSingleCountTimeup 指当单道计数计时时间到后，进行什么操作

//  各仪器操作：
//  DP832A 电压源（串口）：dp832serial
//      获取电压：dp832serial->getVoltage(channel)
//      设定电压：dp832serial->setVoltage(channel,voltage)
//      发送指令：dp832serial->sendCommand("abc");
//      接收回复：QString reply = dp832serial->readReply();
//  DP832A 电压源（USB）：dp832usb
//      用法同上
//  Thorlabs 温湿度计：tsp
//      获取湿度：tsp->getHumidity()
//      获取温度：tsp->getTemperature()
//      获取湿度偏移量：tsp->getHumidityOffset()
//      获取温度偏移量：tsp->getTemperatureOffset()

//  剩余自定义函数写在 "customizedfunctions.hpp" 中




double volinitial=0,volch3=0,volch1=0;
QFile  myfile("C:/Users/EntangleQKD/Desktop/timebin纠缠/0km_noise=0_m=2X基AwBb.txt");
DP832A_USB device1("USB0::0x1AB1::0x0E11::DP8B240700265::INSTR",this);


//  测试 按钮被按下
void ExternalApplicationsWidget::on_buttonTest_released()
{

//  DP832A_USB device1("USB0::0x1AB1::0x0E11::DP8B240700265::INSTR",this);
//    DP832A_USB device2("USB0::0x1AB1::0x0E11::DP8B240700265::INSTR",this);
    device1.initializeDevice();

    device1.setVoltage(1,0.1);

    qDebug() << device1.getVoltage(1);

    device1.setVoltage(1,0.5);

    device1.closeDevice();



    qDebug() << sum(1,3);

//    qDebug() << "temp is " << tsp->getTemperature();
//    qDebug() << "error code: " << tsp->getErrorCode();

//    dp832usb->setVoltage(1,0.18);
//    dp832serial->setVoltage(1,4.3);

//    computerA(dp832serial,tsp);
//    computerB(dp832usb,tsp);

   /* QString commandAskID = "*IDN?";                    // 查询仪器 ID
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
    ui->whiteBoard->setText(answer);   */

//    qDebug() << 11;

//    QString command = "*IDN?";
//    QString result = "";

//    dp832usb->sendCommand(&command,&result);

//    qDebug() << result;

//    qDebug() << sumAB(1,3);

//    sendComplicatedCommands(dp832usb);

//    setVoltage(0.5,dp832usb);

//   //稳相位
//    QString setvol;
//    static int i=0;      //用于数秒
//    static int k=0;      //用于数每次调整到最大干涉需要调几次电压
//    static int coin10=0,coin01=0,coin11=0,coin00=0,tag=0;     //各对通道的符合计数
//    static double thsum=0,thaver1=0,thaver2=0,error1=0,error2=0;           //用于温度求和
//    double volinitial;
//    int coin10r=0,coin01r=0,coin11r=0,coin00r=0;
//    double th1r, phi1;
//    i++;
////    qDebug() << "tsfdk" << vNbrCoin.size();
////    qDebug()<<*(vNbrCoin.at(0));                 //读取一次符合计数  符合面板打开顺序要与其一致。
////    //coin01r=*vNbrCoin.at(1);
////    //coin11r=*vNbrCoin.at(2);
////    //coin00r=*vNbrCoin.at(3);
//    QString commandAskVOL = ":APPL? CH1,VOLT";
//    sendData(commandAskVOL);
//    QThread::msleep(100);                              // 加延时，不然读不到数据
//    QString answervol = readData();                       // 读取回复
//    volinitial  = answervol.toDouble();                   // Qt 内输出结果
//    volinitial =volinitial +1;
//    qDebug()<< "输出电压"<<volinitial;
//    th1r = tsp->getTemperature();
//    qDebug()<<"符合计数10"<< coin10r;
    //读取温度
  /*  if(i<11){
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
                 volinitial=volinitial-phi1/0.06;
                 qDebug()<<"输出负电压:"<<volinitial;
                 qDebug()<<"phi1" << phi1;
                 setvol =":VOLT " + QString::number(volinitial);
                 sendData(setvol);
             }
             else{
                 tag = 1;
                 volinitial=volinitial+phi1/0.06;
                 qDebug()<<"输出正电压:"<<volinitial;
                 setvol =":VOLT " + QString::number(volinitial);
                 sendData(setvol);
             }
         }
         else{
             if(error2>error1){
                 if(tag==0){
                     volinitial=volinitial-1;
                     qDebug()<< "输出负电压："<< volinitial;
                     setvol =":VOLT " + QString::number(volinitial);
                     sendData(setvol);
                     tag=0;
                 }
                 else{
                     volinitial=volinitial+1;
                     qDebug()<< "输出正电压："<< volinitial;
                     setvol =":VOLT " + QString::number(volinitial);
                     sendData(setvol);
                     tag=1;
                 }
             }
             else{
                 if(tag==0){
                   volinitial=volinitial+1;
                   qDebug()<< "输出正电压:" << volinitial;
                   setvol =":VOLT " + QString::number(volinitial);
                   sendData(setvol);
                   tag = 1;
                  }
                 else{
                     volinitial=volinitial-1;
                     qDebug()<< "输出负电压:"<< volinitial;
                     setvol =":VOLT " + QString::number(volinitial);
                     sendData(setvol);
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
      device1.initializeDevice();
      QString commandAskVOL = ":APPL? CH1,VOLT";
      double answervol = device1.getVoltage(1);                       // 读取回复
      volinitial  = answervol;                 // Qt 内输出结果

      qDebug() << "开始自定义程序";
//    myfile.open(QIODevice::WriteOnly | QIODevice::Text);
//    QString commandAskVOL = ":APPL? CH1,VOLT";
//    sendData(commandAskVOL);
//    QThread::msleep(100);                              // 加延时，不然读不到数据
//    QString answervol = readData();                       // 读取回复
//    volinitial  = answervol.toDouble();                 // Qt 内输出结果

//  /*  QString setcurrport = ":CURR:PROT 5.3";
//    sendData(setcurrport);
//    QThread::msleep(100);
//    QString setcurrporton = ":CURR:PROT:STAT ON";
//    sendData(setcurrporton);
//    QThread::msleep(100);
//    QString setinitialvol = ":APPL CH1,19,2";              //设置初始电压，需要实验前先测试出消光的电压
//    sendData(setinitialvol);
//    QThread::msleep(100);
//    QString setoutputon = ":OUTP CH1,ON";            //打开输出
//    sendData(setoutputon);
//    QThread::msleep(100);
//*/
}

//  停止 按钮被按下
void ExternalApplicationsWidget::customizedSPcommands_stop()
{
//    qDebug() << "停止自定义程序";
      myfile.close();
      device1.closeDevice();
}

//  单道计数刷新时
void ExternalApplicationsWidget::dealSingleCountTimeup()
{
//    qDebug() << "单道计数刷新";
//      qDebug() << tr("单道计数：") << *nbrSCC;
//    qDebug() << tr("第一个与单道计数同步的符合面板的计数：") << *(vNbrCoin.at(0));

      QString setvol,setvolch3;
      static int i=0;      //用于数秒
      static int k=0;      //用于数每次调整到最大干涉需要调几次电压
      static int coin10=0,coin01=0,coin11=0,coin00=0,tag=0;     //各对通道的符合计数
      static double th1=0,th2=0,th3,error1=0,error2=0,error3=0;           //用于温度求和
      int coin10r=0,coin01r=0,coin11r=0,coin00r=0;
      double phi1;
      i++;
      coin10r=*vNbrCoin.at(0);                 //读取一次符合计数  符合面板打开顺序要与其一致。
      coin01r=*vNbrCoin.at(2);
      coin11r=*vNbrCoin.at(1);
      coin00r=*vNbrCoin.at(3);
//      th1r = tsp->getTemperature();               //读取温度
//    qDebug()<< "输出电压"<<volinitial;

    if(i<12){
   coin10=coin10+coin10r;                //累计10s内的数据
   coin01=coin01+coin01r;
   coin11=coin11+coin11r;
   coin00=coin00+coin00r;
    }
    else{
       th1 = tsp->getTemperature();
      QString str;
       str = QString::number(th1,'f',2);
       th1 = str.toDouble();
       error1 = (double)(coin10+coin01)/(coin00+coin11+coin01+coin10);
       phi1 = 1-(acos(2*error1-1))/3.1415926535897;
       qDebug()<< "误码"<< error1;
       qDebug()<< "温度"<< th1;
       QTextStream stream(&myfile);
       stream << coin10 << "\t";
       stream << coin01 << "\t";
       stream << coin00 << "\t";
       stream << coin11 << "\t";
       stream << error1 << "\t";
       stream << ";" << "\t";
     if(error1<0.025){
         qDebug() << "k=" << k;    //后续要补充写入文档
//         stream << th1 << "\t";
         stream << k << endl;
         k=1;
     }
     else{
         k++;
         if(k==1){
//             if((th1-th2)>0){
//                 tag = 0;
//                 volinitial=volinitial-phi1/0.07;
//                 qDebug()<<"输出负电压:"<<volinitial;
//                 qDebug()<<"phi1" << phi1;
//                 if(volinitial<0){
//                     volinitial=volinitial+31;
//                 }
//                 else{

//                 }
//                 setvol =":VOLT " + QString::number(volinitial);
//                 sendData(setvol);
//             }
//             else{
//                 if((th1-th2)==0){
//                     if((th1-th3)>0){
//                         tag = 0;
//                         volinitial=volinitial-phi1/0.07;
//                         qDebug()<<"输出负电压:"<<volinitial;
//                         qDebug()<<"phi1" << phi1;
//                         if(volinitial<0){
//                             volinitial=volinitial+31;
//                         }
//                         else{

//                         }
//                         setvol =":VOLT " + QString::number(volinitial);
//                         sendData(setvol);
//                     }
//                     else{
//                         tag = 1;
//                         volinitial=volinitial+phi1/0.07;
//                         if(volinitial>33){
//                             volinitial=volinitial-31;
//                         }
//                         else{

//                         }
//                         setvol =":VOLT " + QString::number(volinitial);
//                         sendData(setvol);
//                         qDebug()<<"输出正电压:"<<volinitial;
//                     }
//                 }
//                 else{
//                     tag = 1;
//                     volinitial=volinitial+phi1/0.07;
//                     qDebug()<<"输出正电压:"<<volinitial;
//                     if(volinitial>33){
//                         volinitial=volinitial-31;
//                     }
//                     else{

//                     }
//                   setvol =":VOLT " + QString::number(volinitial);
//                     sendData(setvol);
//                 }
//             }                                                                        //根据温度确定调节方向
               if(tag==0){
              //     volinitial=volinitial-phi1/0.07;          //AB1 参数
                   volinitial=volinitial-phi1/0.02;             //AB2 参数
                   qDebug()<< "输出负电压："<< volinitial;
                   if(volinitial<0){
                       volinitial=volinitial+32;
                   }
                   else{

                   }
                   if(volinitial>32){
                       volch1=32;
                       volch3=volinitial-32;
//                      setvol =":APPL CH1," + QString::number(volch1);
//                      setvolch3 =":APPL CH3," + QString::number(volch3);
//                      sendData(setvol);
//                      QThread::msleep(100);
//                      sendData(setvolch3);
                       device1.setVoltage(1,volch1);
                       device1.setVoltage(3,volch3);
                   }
                   else{
                       if(volch3==0){
                           volch1=volinitial;
//                           setvol =":APPL CH1," + QString::number(volch1);
//                           sendData(setvol);
                        device1.setVoltage(1,volch1);

                       }
                       else{
                           volch3=0;
                           volch1=volinitial;
//                           setvol =":APPL CH1," + QString::number(volch1);
//                           setvolch3 =":APPL CH3," + QString::number(volch3);
 //                          sendData(setvol);
//                          QThread::msleep(100);
//                           sendData(setvolch3);
                           device1.setVoltage(1,volch1);
                           device1.setVoltage(3,volch3);
                       }
                   }
                   tag=0;
                   qDebug()<<"输出正电压:"<<volinitial;
               }
               else{
  //               volinitial=volinitial+phi1/0.07;      //AB2 参数
                   volinitial=volinitial+phi1/0.02;         //AB1 参数

                   if(volinitial>37){
                       volinitial=volinitial-32;
                   }
                   else{

                   }
                   if(volinitial>32){
                       volch1=32;
                       volch3=volinitial-32;
//                       setvol =":APPL CH1," + QString::number(volch1);
//                       setvolch3 =":APPL CH3," + QString::number(volch3);
//                       sendData(setvol);
//                       QThread::msleep(100);
//                       sendData(setvolch3);
                       device1.setVoltage(1,volch1);
                       device1.setVoltage(3,volch3);
                   }
                   else{
                       if(volch3==0){
                           volch1=volinitial;
//                           setvol =":APPL CH1," + QString::number(volch1);
//                           sendData(setvol);
                           device1.setVoltage(1,volch1);
                       }
                       else{
                           volch3=0;
                           volch1=volinitial;
//                           setvol =":APPL CH1," + QString::number(volch1);
//                           setvolch3 =":APPL CH3," + QString::number(volch3);
//                           sendData(setvol);
//                           QThread::msleep(100);
//                           sendData(setvolch3);
                           device1.setVoltage(1,volch1);
                           device1.setVoltage(3,volch3);
                       }
                   }
                   tag=1;
                   qDebug()<< "输出正电压："<< volinitial;
               }
         }
         else{
             if(k<5){
                 if(error2>error1){
                     if(tag==0){
                        // volinitial=volinitial-phi1/0.07;
                         volinitial=volinitial-phi1/0.1;             //AB1 参数
                         if(volinitial<0){
                             volinitial=volinitial+32;
                         }
                         else{

                         }
                         if(volinitial>32){
                             volch1=32;
                             volch3=volinitial-32;
//                             setvol =":APPL CH1," + QString::number(volch1);
//                             setvolch3 =":APPL CH3," + QString::number(volch3);
//                             sendData(setvol);
//                             QThread::msleep(100);
//                             sendData(setvolch3);
                             device1.setVoltage(1,volch1);
                             device1.setVoltage(3,volch3);
                         }
                         else{
                             if(volch3==0){
                                 volch1=volinitial;
//                                 setvol =":APPL CH1," + QString::number(volch1);
//                                 sendData(setvol);
                                 device1.setVoltage(1,volch1);
                             }
                             else{
                                 volch3=0;
                                 volch1=volinitial;
//                                 setvol =":APPL CH1," + QString::number(volch1);
//                                 setvolch3 =":APPL CH3," + QString::number(volch3);
//                                 sendData(setvol);
//                                 QThread::msleep(100);
//                                 sendData(setvolch3);
                                 device1.setVoltage(1,volch1);
                                 device1.setVoltage(3,volch3);
                             }
                         }
                         tag=0;
                         qDebug()<< "输出负电压："<< volinitial;
                     }
                     else{
 //                        volinitial=volinitial+phi1/0.07;
                         volinitial=volinitial+phi1/0.1;         //AB1 参数
                         if(volinitial>37){
                             volinitial=volinitial-32;
                         }
                         else{

                         }
                         if(volinitial>32){
                             volch1=32;
                             volch3=volinitial-32;
//                             setvol =":APPL CH1," + QString::number(volch1);
//                             setvolch3 =":APPL CH3," + QString::number(volch3);
//                             sendData(setvol);
//                             QThread::msleep(100);
//                             sendData(setvolch3);
                             device1.setVoltage(1,volch1);
                             device1.setVoltage(3,volch3);
                         }
                         else{
                             if(volch3==0){
                                 volch1=volinitial;
//                                 setvol =":APPL CH1," + QString::number(volch1);
//                                 sendData(setvol);
                                  device1.setVoltage(1,volch1);
                             }
                             else{
                                 volch3=0;
                                 volch1=volinitial;
//                                 setvol =":APPL CH1," + QString::number(volch1);
//                                 setvolch3 =":APPL CH3," + QString::number(volch3);
//                                 sendData(setvol);
//                                 QThread::msleep(100);
//                                 sendData(setvolch3);
                                 device1.setVoltage(1,volch1);
                                 device1.setVoltage(3,volch3);
                             }
                         }
                         tag=1;
                         qDebug()<< "输出正电压："<< volinitial;
                     }
                 }
                 else{
                     if(tag==0){
//                       volinitial=volinitial+phi1/0.07;
                       volinitial=volinitial+phi1/0.1;         //AB1 参数
                       if(volinitial>37){
                           volinitial=volinitial-32;
                       }
                       else{

                       }
                       if(volinitial>32){
                           volch1=32;
                           volch3=volinitial-32;
//                           setvol =":APPL CH1," + QString::number(volch1);
//                           setvolch3 =":APPL CH3," + QString::number(volch3);
//                           sendData(setvol);
//                           QThread::msleep(100);
//                           sendData(setvolch3);
                           device1.setVoltage(1,volch1);
                           device1.setVoltage(3,volch3);
                       }
                       else{
                           if(volch3==0){
                               volch1=volinitial;
//                               setvol =":APPL CH1," + QString::number(volch1);
//                               sendData(setvol);
                               device1.setVoltage(1,volch1);
                           }
                           else{
                               volch3=0;
                               volch1=volinitial;
//                               setvol =":APPL CH1," + QString::number(volch1);
//                               setvolch3 =":APPL CH3," + QString::number(volch3);
//                               sendData(setvol);
//                               QThread::msleep(100);
//                               sendData(setvolch3);
                               device1.setVoltage(1,volch1);
                               device1.setVoltage(3,volch3);
                           }
                       }
                       qDebug()<< "输出正电压:" << volinitial;
                       tag = 1;
                      }
                     else{
//                         volinitial=volinitial-phi1/0.07;
                         volinitial=volinitial-phi1/0.1;             //AB1 参数
                         if(volinitial<0){
                             volinitial=volinitial+32;
                         }
                         else{

                         }
                         if(volinitial>32){
                             volch1=32;
                             volch3=volinitial-32;
//                             setvol =":APPL CH1," + QString::number(volch1);
//                             setvolch3 =":APPL CH3," + QString::number(volch3);
//                             sendData(setvol);
//                             QThread::msleep(100);
//                             sendData(setvolch3);
                             device1.setVoltage(1,volch1);
                             device1.setVoltage(3,volch3);
                         }
                         else{
                             if(volch3==0){
                                 volch1=volinitial;
//                                 setvol =":APPL CH1," + QString::number(volch1);
//                                 sendData(setvol);
                                  device1.setVoltage(1,volch1);
                             }
                             else{
                                 volch3=0;
                                 volch1=volinitial;
//                                 setvol =":APPL CH1," + QString::number(volch1);
//                                 setvolch3 =":APPL CH3," + QString::number(volch3);
//                                 sendData(setvol);
//                                 QThread::msleep(100);
//                                 sendData(setvolch3);
                                 device1.setVoltage(1,volch1);
                                 device1.setVoltage(3,volch3);
                             }
                         }
                       qDebug()<< "输出负电压:"<< volinitial;
                       tag = 0;
                      }
                 }
             }
             else{
                 if(error2>error1){
                     if(tag==0){
                         volinitial=volinitial-1;
                         if(volinitial<0){
                             volinitial=volinitial+32;
                         }
                         else{

                         }
                         if(volinitial>32){
                             volch1=32;
                             volch3=volinitial-32;
//                             setvol =":APPL CH1," + QString::number(volch1);
//                             setvolch3 =":APPL CH3," + QString::number(volch3);
//                             sendData(setvol);
//                             QThread::msleep(100);
//                             sendData(setvolch3);
                             device1.setVoltage(1,volch1);
                             device1.setVoltage(3,volch3);
                         }
                         else{
                             if(volch3==0){
                                 volch1=volinitial;
//                                 setvol =":APPL CH1," + QString::number(volch1);
//                                 sendData(setvol);
                                  device1.setVoltage(1,volch1);
                             }
                             else{
                                 volch3=0;
                                 volch1=volinitial;
//                                 setvol =":APPL CH1," + QString::number(volch1);
//                                 setvolch3 =":APPL CH3," + QString::number(volch3);
//                                 sendData(setvol);
//                                 QThread::msleep(100);
//                                 sendData(setvolch3);
                                 device1.setVoltage(1,volch1);
                                 device1.setVoltage(3,volch3);
                             }
                         }
                         tag=0;
                         qDebug()<< "输出负电压："<< volinitial;
                     }
                     else{
                         volinitial=volinitial+1;
                         if(volinitial>37){
                             volinitial=volinitial-32;
                         }
                         else{

                         }
                         if(volinitial>32){
                             volch1=32;
                             volch3=volinitial-32;
//                             setvol =":APPL CH1," + QString::number(volch1);
//                             setvolch3 =":APPL CH3," + QString::number(volch3);
//                             sendData(setvol);
//                             QThread::msleep(100);
//                             sendData(setvolch3);
                             device1.setVoltage(1,volch1);
                             device1.setVoltage(3,volch3);
                         }
                         else{
                             if(volch3==0){
                                 volch1=volinitial;
//                                 setvol =":APPL CH1," + QString::number(volch1);
//                                 sendData(setvol);
                                  device1.setVoltage(1,volch1);
                             }
                             else{
                                 volch3=0;
                                 volch1=volinitial;
//                                 setvol =":APPL CH1," + QString::number(volch1);
//                                 setvolch3 =":APPL CH3," + QString::number(volch3);
//                                 sendData(setvol);
//                                 QThread::msleep(100);
//                                 sendData(setvolch3);
                                 device1.setVoltage(1,volch1);
                                 device1.setVoltage(3,volch3);
                             }
                         }
                         tag=1;
                         qDebug()<< "输出正电压："<< volinitial;
                     }
                 }
                 else{
                     if(tag==0){
                       volinitial=volinitial+1;
                       if(volinitial>37){
                           volinitial=volinitial-32;
                       }
                       else{

                       }
                       if(volinitial>32){
                           volch1=32;
                           volch3=volinitial-32;
//                           setvol =":APPL CH1," + QString::number(volch1);
//                           setvolch3 =":APPL CH3," + QString::number(volch3);
//                           sendData(setvol);
//                           QThread::msleep(100);
//                           sendData(setvolch3);
                           device1.setVoltage(1,volch1);
                           device1.setVoltage(3,volch3);
                       }
                       else{
                           if(volch3==0){
                               volch1=volinitial;
//                               setvol =":APPL CH1," + QString::number(volch1);
//                               sendData(setvol);
                                device1.setVoltage(1,volch1);
                           }
                           else{
                               volch3=0;
                               volch1=volinitial;
//                               setvol =":APPL CH1," + QString::number(volch1);
//                               setvolch3 =":APPL CH3," + QString::number(volch3);
//                               sendData(setvol);
//                               QThread::msleep(100);
//                               sendData(setvolch3);
                               device1.setVoltage(1,volch1);
                               device1.setVoltage(3,volch3);
                           }
                       }
                       qDebug()<< "输出正电压:" << volinitial;
                       tag = 1;
                      }
                     else{
                         volinitial=volinitial-1;
                         if(volinitial<0){
                             volinitial=volinitial+32;
                         }
                         else{

                         }
                         if(volinitial>32){
                             volch1=32;
                             volch3=volinitial-32;
//                             setvol =":APPL CH1," + QString::number(volch1);
//                             setvolch3 =":APPL CH3," + QString::number(volch3);
//                             sendData(setvol);
//                             QThread::msleep(100);
//                             sendData(setvolch3);
                             device1.setVoltage(1,volch1);
                             device1.setVoltage(3,volch3);
                         }
                         else{
                             if(volch3==0){
                                 volch1=volinitial;
//                                 setvol =":APPL CH1," + QString::number(volch1);
//                                 sendData(setvol);
                                  device1.setVoltage(1,volch1);
                             }
                             else{
                                 volch3=0;
                                 volch1=volinitial;
//                                 setvol =":APPL CH1," + QString::number(volch1);
//                                 setvolch3 =":APPL CH3," + QString::number(volch3);
//                                 sendData(setvol);
//                                 QThread::msleep(100);
//                                 sendData(setvolch3);
                                 device1.setVoltage(1,volch1);
                                 device1.setVoltage(3,volch3);
                             }
                         }
                       qDebug()<< "输出负电压:"<< volinitial;
                       tag = 0;
                      }
                 }
             }
         }
     }
     qDebug()<<"使用步数"<<k;
//     if(k==20){                                        //因为电压源的行程只有33V，还差一点覆盖一个周期，当相位漂移动pi无法纠回
//         QString setoutputoff = ":OUTP CH1,OFF";            //关闭输出
//         sendData(setoutputoff);                          //超过12步无法纠回时，关闭电压源，等待相位再漂移一会儿。
//         QThread::msleep(100);
//         stream << "off" << "\n";
//     }
//     else{
//         if(k>20){
//             if((error1-error2)>0.01&&(error2-error3)>0&&(error1>0.03)){
//                 setvol =":VOLT 16";
//                 sendData(setvol);
//                 QThread::msleep(50);
//                   QString setoutputon = ":OUTP CH1,ON";            //打开输出
//                   sendData(setoutputon);
//                   k=2;
//                   stream << "on" << "\n";

//             }
//             else{

//             }
//         }
//         else{

//         }
//     }
     i=0;
     th3 = th2;
     th2 = th1;
     error3 = error2;
     error2 = error1;

     coin10=coin10r;
     coin01=coin01r;
     coin11=coin11r;
     coin00=coin00r;
    }

}

