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
