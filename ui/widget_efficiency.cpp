#include "widget.h"
#include "ui_widget.h"
#include <QVector>
#include <QtMath>
#include "AcqirisImport.h"
#include "AcqirisT3Import.h"

void Widget::on_buttonStarteff_released()
{

    fetchUiData();
    tdc->config(channelConfig,level,slope,countEvents);


    // start Acquisition
    if (tdc->getStatus() == VI_SUCCESS)
        tdc->startAcquisition();

    this->dealEffTimeOut();
    if (tdc->isAcquringData()) // 当采集已经开启时
    {
        // 开始计算单道计数
        connect(tdc,&Acqiris_TDC::dataReturned,this,&Widget::dealAcqThreadBankSwitchEff);
        // 启动定时器
        connect(timerCount,&QTimer::timeout,this,&Widget::dealEffTimeOut);
        timerCount->start(1000.0*accumulateTime);
    }
}



void Widget::on_buttonStopCount_2_released()
{
     disconnect(tdc,&Acqiris_TDC::dataReturned,this,&Widget::dealAcqThreadBankSwitchEff);
     if (tdc->isAcquringData())
         tdc->stopAcquisition();
}

void Widget::computeSingleChannelEff(AqT3DataDescriptor* dataDescPtr)
{

    long nbrSamples = dataDescPtr->nbrSamples;
    for (long n = 0 ; n < nbrSamples ; ++n)
    {
        int sample = ((long *)dataDescPtr->dataPtr)[n];  //dataPtr指向time value data buffer
        int channel = (sample & 0x70000000) >> 28;   //右移28位为channel位
        int TimeOfFlight = sample & 0x0FFFFFFF;             //最右侧28位为计数值
        // flag 为 0 表示 COM 信号，channel 为 7 表示内存切换
        if (channel != 0 && channel != 7)                        // Channel=1-6 are the physical channels;
                                                // Data = an integer giving the time value in units of 50 ps
                                                // Channel=7 is for marker data.
         {
            if(TimeOfFlight%200==1)    // 200为开门周期，
            nbrPhoton ++;    // 光子计数
            else
            nbrAP ++;     //后脉冲计数
         }
    }
}




void Widget::dealAcqThreadBankSwitchEff(AqT3DataDescriptor* dataDescPtr)
{
    computeSingleChannelEff(dataDescPtr);
}




void Widget::dealEffTimeOut()
{
    ui->lcdSPC1_3->display((double)nbrPhoton/1000000);
    ui->lcdSPC1_4->display((double)nbrAP/nbrPhoton);

    memset(&nbrPhoton,0,sizeof(nbrPhoton));
    memset(&nbrAP, 0, sizeof(nbrAP));

}



