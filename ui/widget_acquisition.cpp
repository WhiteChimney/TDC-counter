#include "widget.h"
#include "ui_widget.h"

void Widget::on_buttonStartAcquisition_released()
{
    fetchUiData();
    if (configStatus != VI_SUCCESS)
        configStatus = configTDC
                (idInstr, countEvents, channelConfig, level, slope, readParamPtr);

    // start Acquisition
    if (configStatus == VI_SUCCESS)
        acqThread->start();
}

void Widget::dealAcqThreadStarted()
{
    statusIndicator->setStates(QSimpleLed::ON);
    *acqStopPtr = false;
    if (configStatus == VI_SUCCESS)
        emit acqParamReady
            (acqStopPtr,idInstr,readParamPtr);  // 收到采集线程的开启信号后，将参数传送过去
}

void Widget::on_buttonStopAcq_released()
{
    *acqStopPtr = true;
    configStatus = -1;
}

void Widget::dealAcqThreadFinished()
{
    AcqrsT3_waitForEndOfAcquisition(idInstr, 8000);
    AcqrsT3_stopAcquisition(idInstr);
    acqThread->quit();
    acqThread->wait();
    statusIndicator->setStates(QSimpleLed::OFF);
}

void Widget::updateDataPtrList(AqT3DataDescriptor *dataPtr0)
{
    if (dataPtrList.size() == nbrCOMbuffer)
    {
        delete dataPtrList.at(0);
        dataPtrList.removeFirst();
    }
    dataPtrList.append(dataPtr0);
    emit dataPtrListUpdated(dataPtrList);
}
