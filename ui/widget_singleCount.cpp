#include "widget.h"
#include "ui_widget.h"

void Widget::on_buttonStartCount_released()
{
    this->dealCountTimeOut();
    if (tdc->isAcquringData()) // 当采集已经开启时
    {
        // 开始计算单道计数
        connect(tdc,&Acqiris_TDC::dataReturned,this,&Widget::dealAcqThreadBankSwitchSCC);
        // 启动定时器
        fetchUiData();
        timerCount->start(1000.0*accumulateTime);
    }

    if (tdc_2->isAcquringData()) // 当采集已经开启时
    {
        // 开始计算单道计数
        connect(tdc_2,&Acqiris_TDC::dataReturned,this,&Widget::dealAcqThreadBankSwitchSCC_2);
        // 启动定时器
        fetchUiData();
        timerCount->start(1000.0*accumulateTime);
    }
}

void Widget::dealAcqThreadBankSwitchSCC(AqT3DataDescriptor* dataDescPtr)
{
    computeSingleChannelCount(nbrSCC,nbrSCCfuture,dataDescPtr,delayCN,freqCOM,countEvents);
}

void Widget::dealAcqThreadBankSwitchSCC_2(AqT3DataDescriptor* dataDescPtr_2)
{
    computeSingleChannelCount(nbrSCC_2,nbrSCCfuture_2,dataDescPtr_2,delayCN_2,freqCOM,countEvents);
}

void Widget::dealCountTimeOut()
{
    ui->lcdSPC1->display(nbrSCC[0]);
    ui->lcdSPC2->display(nbrSCC[1]);
    ui->lcdSPC3->display(nbrSCC[2]);
    ui->lcdSPC4->display(nbrSCC[3]);
    ui->lcdSPC5->display(nbrSCC[4]);
    ui->lcdSPC6->display(nbrSCC[5]);
    memcpy(nbrSCC,nbrSCCfuture,6*sizeof(nbrSCC[0]));
    memset(nbrSCCfuture, 0, 6*sizeof(nbrSCCfuture[0]));

    ui->lcdSPC1_2->display(nbrSCC_2[0]);
    ui->lcdSPC2_2->display(nbrSCC_2[1]);
    ui->lcdSPC3_2->display(nbrSCC_2[2]);
    ui->lcdSPC4_2->display(nbrSCC_2[3]);
    ui->lcdSPC5_2->display(nbrSCC_2[4]);
    ui->lcdSPC6_2->display(nbrSCC_2[5]);
    memcpy(nbrSCC_2,nbrSCCfuture_2,6*sizeof(nbrSCC_2[0]));
    memset(nbrSCCfuture_2, 0, 6*sizeof(nbrSCCfuture_2[0]));
}

void Widget::on_buttonStopCount_released()
{
    timerCount->stop();
    disconnect(tdc,&Acqiris_TDC::dataReturned,this,&Widget::dealAcqThreadBankSwitchSCC);
    disconnect(tdc_2,&Acqiris_TDC::dataReturned,this,&Widget::dealAcqThreadBankSwitchSCC_2);
}

int* Widget::getSingleCountPtr()
{
    return nbrSCC;
}
