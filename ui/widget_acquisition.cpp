#include "widget.h"
#include "ui_widget.h"

void Widget::on_buttonStartAcquisition_released()
{
    fetchUiData();
    tdc->config(channelConfig,level,slope,countEvents);

    // start Acquisition
    if (tdc->getStatus() == VI_SUCCESS)
        tdc->startAcquisition();
}

//void Widget::dealAcqThreadStarted_2()
//{
////    statusIndicator->setStates(QSimpleLed::ON);
//    *acqStopPtr_2 = false;
//    if (configStatus_2 == VI_SUCCESS)
//        emit acqParamReady_2
//            (acqStopPtr_2,idInstr_2,readParamPtr_2);  // 收到采集线程的开启信号后，将参数传送过去
//}

void Widget::on_buttonStopAcq_released()
{
    tdc->stopAcquisition();
}

void Widget::dealAcqThreadStarted()
{
    if (tdc->getStatus() == VI_SUCCESS)
        statusIndicator->setStates(QSimpleLed::ON);
}

void Widget::dealAcqThreadFinished()
{
    statusIndicator->setStates(QSimpleLed::OFF);
}

//void Widget::dealAcqThreadFinished_2()
//{
//    AcqrsT3_waitForEndOfAcquisition(idInstr_2, 8000);
//    AcqrsT3_stopAcquisition(idInstr_2);
//    acqThread->quit();
//    acqThread->wait();
//    statusIndicator->setStates(QSimpleLed::OFF);
//}
