#include "widget.h"
#include "ui_widget.h"

void Widget::on_buttonHistogram_released()
{
    int index = vHistWidget.count();
    histW = new HistogramWidget(0, index);
    vHistWidget.append(histW);
    connect(histW,&HistogramWidget::returnSignal,this,&Widget::dealHistogramReturn);
    connect(histW,&HistogramWidget::askDealAcqBankSwitchHist,this,&Widget::dealAskDealAcqBankSwitchHist);
    connect(histW,&HistogramWidget::askStopDealAcqBankSwitchHist,this,&Widget::dealAskStopDealAcqBankSwitchHist);
    histW->setWindowTitle("直方图"+QString::number(index+1));
    histW->setWindowState(Qt::WindowActive);
    histW->show();
}

void Widget::dealHistogramReturn(int index)
{
    histW = vHistWidget.at(index);
    histW->setWindowState(Qt::WindowNoState);
    histW->close();
}

void Widget::dealAskDealAcqBankSwitchHist(int index)
{
    histW = vHistWidget.at(index);
    connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,histW,&HistogramWidget::dealAcqThreadBankSwitchHist);
}

void Widget::dealAskStopDealAcqBankSwitchHist(int index)
{
    histW = vHistWidget.at(index);
    disconnect(acqThread,&AcquisitionThread::acqThreadBankSwitch,histW,&HistogramWidget::dealAcqThreadBankSwitchHist);
}
