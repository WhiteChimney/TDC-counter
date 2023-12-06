#include "widget.h"
#include "ui_widget.h"

void Widget::on_buttonStartAcquisition_released()
{
    on_buttonStopAcq_released();

    fetchUiData();
    tdc->config(channelConfig,level,slope,countEvents);
    tdc_2->config(channelConfig_2,level_2,slope_2,countEvents);

    // start Acquisition
    if (tdc->getStatus() == VI_SUCCESS)
        tdc->startAcquisition();

    if (tdc_2->getStatus() == VI_SUCCESS)
        tdc_2->startAcquisition();
}

void Widget::on_buttonStopAcq_released()
{
    instrIds.clear();
//    waitConds.clear();

    if (tdc->isAcquringData())
        tdc->stopAcquisition();

    if (tdc_2->isAcquringData())
        tdc_2->stopAcquisition();
}

void Widget::dealAcqThreadStarted()
{
    if (tdc->getStatus() == VI_SUCCESS)
    {
        statusIndicator->setStates(QSimpleLed::ON);
        if (statusIndicator->states() == QSimpleLed::ON
                and statusIndicator_2->states() == QSimpleLed::ON)
        {
            tdc->wakeToAquireData();
            tdc_2->wakeToAquireData();
        }
    }
}

void Widget::dealAcqThreadStarted_2()
{
    if (tdc_2->getStatus() == VI_SUCCESS)
    {
        statusIndicator_2->setStates(QSimpleLed::ON);
        if (statusIndicator->states() == QSimpleLed::ON
                and statusIndicator_2->states() == QSimpleLed::ON)
        {
            tdc->wakeToAquireData();
            tdc_2->wakeToAquireData();
        }
    }
}

void Widget::dealAcqThreadFinished()
{
    statusIndicator->setStates(QSimpleLed::OFF);
}

void Widget::dealAcqThreadFinished_2()
{
    statusIndicator_2->setStates(QSimpleLed::OFF);
}
