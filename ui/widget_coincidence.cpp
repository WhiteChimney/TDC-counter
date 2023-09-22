#include "widget.h"
#include "ui_widget.h"

void Widget::on_buttonCoincidence_released()
{
    int index = vCoinWidget.count();
    coinW = new CoincidenceWidget(0,index);
    vCoinWidget.append(coinW);
    vCoinWidgetSyncState.append(false);
    connect(coinW,&CoincidenceWidget::returnSignal,this,&Widget::dealCoincidenceReturn);
    connect(coinW,&CoincidenceWidget::requestCoinParam,this,&Widget::dealRequestCoinParam);
    connect(this,&Widget::coinParamReady,coinW,&CoincidenceWidget::dealRequestCoinParam);
    connect(coinW,&CoincidenceWidget::askDealAcqBankSwitchCoin,this,&Widget::dealAskDealAcqBankSwitchCoin);
    connect(coinW,&CoincidenceWidget::askStopDealAcqBankSwitchCoin,this,&Widget::dealAskStopDealAcqBankSwitchCoin);
    connect(coinW,&CoincidenceWidget::coinTimerNeedsSync,this,&Widget::dealCoinTimerNeedsSync);
    connect(coinW,&CoincidenceWidget::coinTimerStopsSync,this,&Widget::dealCoinTimerStopsSync);
    coinW->setWindowTitle("符合计数"+QString::number(index+1));
    coinW->setWindowState(Qt::WindowActive); // 设置窗口活动状态，以便后续处理，如保存数据
    coinW->show();
}

void Widget::dealCoincidenceReturn(int index)
{
    coinW = vCoinWidget.at(index);
    coinW->setWindowState(Qt::WindowNoState);
    disconnect(coinW,&CoincidenceWidget::returnSignal,this,&Widget::dealCoincidenceReturn);
    disconnect(coinW,&CoincidenceWidget::requestCoinParam,this,&Widget::dealRequestCoinParam);
    disconnect(this,&Widget::coinParamReady,coinW,&CoincidenceWidget::dealRequestCoinParam);
    disconnect(coinW,&CoincidenceWidget::askDealAcqBankSwitchCoin,this,&Widget::dealAskDealAcqBankSwitchCoin);
    disconnect(coinW,&CoincidenceWidget::askStopDealAcqBankSwitchCoin,this,&Widget::dealAskStopDealAcqBankSwitchCoin);
    disconnect(coinW,&CoincidenceWidget::coinTimerNeedsSync,this,&Widget::dealCoinTimerNeedsSync);
    coinW->close();
}

void Widget::dealRequestCoinParam(int index)
{
    emit coinParamReady(index, delayCN, freqCOM);
}

void Widget::dealAskDealAcqBankSwitchCoin(int index)
{
    coinW = vCoinWidget.at(index);
    connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,coinW,&CoincidenceWidget::dealAcqThreadBankSwitchCoin);
}

void Widget::dealAskStopDealAcqBankSwitchCoin(int index)
{
    coinW = vCoinWidget.at(index);
    disconnect(acqThread,&AcquisitionThread::acqThreadBankSwitch,coinW,&CoincidenceWidget::dealAcqThreadBankSwitchCoin);
}

void Widget::dealCoinTimerNeedsSync(int index)
{
    coinW = vCoinWidget.at(index);
    vCoinWidgetSyncState.replace(index,true);
    connect(timerCount,&QTimer::timeout,coinW,&CoincidenceWidget::dealTimeOut); // 同步记录数据
}

void Widget::dealCoinTimerStopsSync(int index)
{
    coinW = vCoinWidget.at(index);
    vCoinWidgetSyncState.replace(index,false);
    disconnect(timerCount,&QTimer::timeout,coinW,&CoincidenceWidget::dealTimeOut);
}
