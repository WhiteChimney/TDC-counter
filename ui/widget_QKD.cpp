#include "widget.h"
#include "ui_widget.h"

void Widget::on_pushButtonQKD_released()
{
    int index = vQKDWidget.count();
    QKDW = new hyperentanglementQKD(0,index,comOffset);
    vQKDWidget.append(QKDW);
    vQKDWidgetSyncState.append(false);
    connect(QKDW,&hyperentanglementQKD::returnSignal,this,&Widget::dealQKDReturn);
    connect(QKDW,&hyperentanglementQKD::requestQKDParam,this,&Widget::dealRequestQKDParam);
    connect(this,&Widget::QKDParamReady,QKDW,&hyperentanglementQKD::dealRequestQKDParam);
    connect(QKDW,&hyperentanglementQKD::askDealAcqBankSwitchQKD,this,&Widget::dealAskDealAcqBankSwitchQKD);
    connect(QKDW,&hyperentanglementQKD::askStopDealAcqBankSwitchQKD,this,&Widget::dealAskStopDealAcqBankSwitchCoin);
    connect(QKDW,&hyperentanglementQKD::QKDTimerNeedsSync,this,&Widget::dealQKDTimerNeedsSync);
    connect(QKDW,&hyperentanglementQKD::QKDTimerStopsSync,this,&Widget::dealQKDTimerStopsSync);
//    connect(QKDW,&hyperentanglementQKD::QKDSaveDataNeedsSync,this,&Widget::dealQKDSaveDataNeedsSync);
//    connect(QKDW,&hyperentanglementQKD::QKDSaveDataStopsSync,this,&Widget::dealQKDSaveDataStopsSync);

    QKDW->setWindowTitle("hyperEBQKD"+QString::number(index+1));
    QKDW->setWindowState(Qt::WindowActive); // 设置窗口活动状态，以便后续处理，如保存数据
    QKDW->show();
}

void Widget::dealQKDReturn(int index)
{
    QKDW = vQKDWidget.at(index);
    QKDW->setWindowState(Qt::WindowNoState);
    disconnect(QKDW,&hyperentanglementQKD::returnSignal,this,&Widget::dealQKDReturn);
    disconnect(this,&Widget::coinParamReady,coinW,&CoincidenceWidget::dealRequestCoinParam);
    disconnect(QKDW,&hyperentanglementQKD::askDealAcqBankSwitchQKD,this,&Widget::dealAskDealAcqBankSwitchQKD);
    disconnect(QKDW,&hyperentanglementQKD::askStopDealAcqBankSwitchQKD,this,&Widget::dealAskStopDealAcqBankSwitchQKD);
//    disconnect(QKDW,&hyperentanglementQKD::QKDTimerNeedsSync,this,&Widget::dealQKDTimerNeedsSync);
//   disconnect(QKDW,&hyperentanglementQKD::QKDSaveDataNeedsSync,this,&Widget::dealQKDSaveDataNeedsSync);

    QKDW->close();
}


void Widget::dealRequestQKDParam(int index)
{
    emit QKDParamReady(index, delayCN, delayCN_2, freqCOM, countEvents);
}


void Widget::dealAskDealAcqBankSwitchQKD(int index)
{
    QKDW= vQKDWidget.at(index);
    connect(tdc,&Acqiris_TDC::dataReturned,QKDW,&hyperentanglementQKD::dealAcqThreadBankSwitchQKD);
    connect(tdc_2,&Acqiris_TDC::dataReturned,QKDW,&hyperentanglementQKD::dealAcqThreadBankSwitchQKD_2);
}

void Widget::dealAskStopDealAcqBankSwitchQKD(int index)
{
    QKDW = vQKDWidget.at(index);
    disconnect(tdc,&Acqiris_TDC::dataReturned,QKDW,&hyperentanglementQKD::dealAcqThreadBankSwitchQKD);
    disconnect(tdc_2,&Acqiris_TDC::dataReturned,QKDW,&hyperentanglementQKD::dealAcqThreadBankSwitchQKD_2);
}

void Widget::dealQKDTimerNeedsSync(int index)
{
    QKDW = vQKDWidget.at(index);
    vQKDWidgetSyncState.replace(index,true);
    connect(timerCount,&QTimer::timeout,QKDW,&hyperentanglementQKD::dealTimeOut); // 同步记录数据
}

void Widget::dealQKDTimerStopsSync(int index)
{
    QKDW = vQKDWidget.at(index);
    vQKDWidgetSyncState.replace(index,false);
    disconnect(timerCount,&QTimer::timeout,QKDW,&hyperentanglementQKD::dealTimeOut);
}


/*void Widget::dealQKDSaveDataNeedsSync(int index)
{
    QKDW = vQKDWidget.at(index);
    vQKDWidgetSyncState.replace(index,true);
//    connect(timerCount,&QTimer::timeout,QKDW,&hyperentanglementQKD::dealSaveDataTimeOut); // 同步记录数据
}

void Widget::dealQKDSaveDataStopsSync(int index)
{
    QKDW = vQKDWidget.at(index);
    vQKDWidgetSyncState.replace(index,false);
//    disconnect(timerCount,&QTimer::timeout,QKDW,&hyperentanglementQKD::dealSaveDataTimeOut);
}*/
