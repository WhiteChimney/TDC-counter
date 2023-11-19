#include "widget.h"
#include "ui_widget.h"

void Widget::on_buttonHistogram_released()
{
    int index = vHistWidget.count();
    histW = new HistogramWidget(0, index);
    vHistWidget.append(histW);
    connect(histW,&HistogramWidget::returnSignal,this,&Widget::dealHistogramReturn);
    connect(histW,&HistogramWidget::requestHistParam,this,&Widget::dealRequestHistParam);
    connect(this,&Widget::histParamReady,histW,&HistogramWidget::dealRequestHistParam);
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

void Widget::dealRequestHistParam(int index)
{
    emit histParamReady(index, delayCN, delayCN_2, freqCOM, countEvents);
}

void Widget::dealAskDealAcqBankSwitchHist(int index)
{
    histW = vHistWidget.at(index);
    connect(tdc,&Acqiris_TDC::dataReturned,histW,&HistogramWidget::dealAcqThreadBankSwitchHist);
    connect(tdc_2,&Acqiris_TDC::dataReturned,histW,&HistogramWidget::dealAcqThreadBankSwitchHist_2);
}

void Widget::dealAskStopDealAcqBankSwitchHist(int index)
{
    histW = vHistWidget.at(index);
    disconnect(tdc,&Acqiris_TDC::dataReturned,histW,&HistogramWidget::dealAcqThreadBankSwitchHist);
    disconnect(tdc_2,&Acqiris_TDC::dataReturned,histW,&HistogramWidget::dealAcqThreadBankSwitchHist_2);
}
