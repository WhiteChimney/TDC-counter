#include "widget.h"
#include "ui_widget.h"

void Widget::on_buttonStartCount_released()
{
    if (!acqThread->isRunning())
        on_buttonStartAcquisition_released();
    this->dealCountTimeOut();
    if (! *acqStopPtr) // 当采集已经开启时
    {
        // 开始计算单道计数
        connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,this,&Widget::dealAcqThreadBankSwitchSCC);
        // 启动定时器
        timerCount->start(1000.0*accumulateTime);
    }
}

void Widget::dealAcqThreadBankSwitchSCC(AqT3DataDescriptor* dataDescPtr)
{
    computeSingleChannelCount(nbrSCC,nbrSCCfuture,dataDescPtr,delayCN,freqCOM,countEvents);
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
}

void Widget::on_buttonStopCount_released()
{
    timerCount->stop();
    disconnect(acqThread,&AcquisitionThread::acqThreadBankSwitch,this,&Widget::dealAcqThreadBankSwitchSCC);
}

int* Widget::getSingleCountPtr()
{
    return nbrSCC;
}
