#include "widget.h"
#include "ui_widget.h"

void Widget::on_buttonStartAcquisition_released()
{
    fetchUiData();
    tdc->config(channelConfig,level,slope,countEvents);
    tdc_2->config(channelConfig,level,slope,countEvents);

    // start Acquisition
    if (tdc->getStatus() == VI_SUCCESS)
        tdc->startAcquisition();

    if (tdc_2->getStatus() == VI_SUCCESS)
        tdc_2->startAcquisition();
}

void Widget::on_buttonStopAcq_released()
{
    instrIds.clear();

    if (tdc->isAcquringData())
        tdc->stopAcquisition();

    if (tdc_2->isAcquringData())
        tdc_2->stopAcquisition();
}

void Widget::dealAcqThreadStarted()
{
    if (tdc->getStatus() == VI_SUCCESS)
        statusIndicator->setStates(QSimpleLed::ON);
}

void Widget::dealAcqThreadStarted_2()
{
    if (tdc_2->getStatus() == VI_SUCCESS)
        statusIndicator_2->setStates(QSimpleLed::ON);
}

void Widget::dealAcqThreadFinished()
{
    statusIndicator->setStates(QSimpleLed::OFF);
}

void Widget::dealAcqThreadFinished_2()
{
    statusIndicator_2->setStates(QSimpleLed::OFF);
}

void Widget::startAcquisitionSync(ViSession instrId)
{
    QWaitCondition waitCond;
    qDebug() << instrId;
    if (!instrIds.contains(instrId))
        instrIds.append(instrId);
    if (instrIds.size() > 1)
    {
        qDebug() << "waking up";
        waitCond.wakeAll();
    }
}
