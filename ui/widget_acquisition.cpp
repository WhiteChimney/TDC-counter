#include "widget.h"
#include "ui_widget.h"

void Widget::on_buttonStartAcquisition_released()
{
    fetchUiData();
    if (configStatus != VI_SUCCESS)
        configStatus = configTDC
                (idInstr, countEvents, channelConfig, level, slope, readParamPtr);

    if (configStatus_2 != VI_SUCCESS)
        configStatus_2 = configTDC
                (idInstr_2, countEvents, channelConfig_2, level_2, slope_2, readParamPtr_2);

    // start Acquisition
    if (configStatus == VI_SUCCESS and configStatus_2 == VI_SUCCESS)
    {
        acqThread->start();
        acqThread_2->start();
    }
}

void Widget::dealAcqThreadStarted()
{
//    statusIndicator->setStates(QSimpleLed::ON);
    *acqStopPtr = false;
    if (configStatus == VI_SUCCESS)
        emit acqParamReady
            (acqStopPtr,idInstr,readParamPtr);  // 收到采集线程的开启信号后，将参数传送过去
}

void Widget::dealAcqThreadStarted_2()
{
//    statusIndicator->setStates(QSimpleLed::ON);
    *acqStopPtr_2 = false;
    if (configStatus_2 == VI_SUCCESS)
        emit acqParamReady_2
            (acqStopPtr_2,idInstr_2,readParamPtr_2);  // 收到采集线程的开启信号后，将参数传送过去
}

void Widget::on_buttonStopAcq_released()
{
    *acqStopPtr = true;
    *acqStopPtr_2 = true;
    configStatus = -1;
    configStatus_2 = -1;
}

void Widget::dealAcqThreadFinished()
{
    AcqrsT3_waitForEndOfAcquisition(idInstr, 8000);
    AcqrsT3_stopAcquisition(idInstr);
    acqThread->quit();
    acqThread->wait();
    statusIndicator->setStates(QSimpleLed::OFF);
}

void Widget::dealAcqThreadFinished_2()
{
    AcqrsT3_waitForEndOfAcquisition(idInstr_2, 8000);
    AcqrsT3_stopAcquisition(idInstr_2);
    acqThread->quit();
    acqThread->wait();
    statusIndicator->setStates(QSimpleLed::OFF);
}
