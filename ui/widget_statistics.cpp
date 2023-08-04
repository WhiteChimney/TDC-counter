#include "widget.h"
#include "ui_widget.h"

void Widget::on_buttonStatistics_released()
{
//    初始化统计 Widget
    if (statWidgetLaunched)
    {
        statW->show();
        statW->activateWindow();
    }
    else
    {
        statW = new StatisticsWidget(accumulateTime,this);
        setupStatUi();
        connect(statW,&StatisticsWidget::sendReturnSignal,this,&Widget::dealStatisticsReturn);
        connect(statW,&StatisticsWidget::statisticsRequestSync,this,&Widget::dealStatisticsRequestSync);
        connect(statW,&StatisticsWidget::statisticsRequestStopSync,this,&Widget::dealStatisticsRequestStopSync);
        statW->show();
        statWidgetLaunched = true;
    }
}

void Widget::dealStatisticsReturn()
{
    statW->close();
    statWidgetLaunched = false;
}

void Widget::setupStatUi()
{
    if (ui->checkBoxCN1->isChecked()) statW->addChannel("单道 1",nbrSCC+0);
    if (ui->checkBoxCN2->isChecked()) statW->addChannel("单道 2",nbrSCC+1);
    if (ui->checkBoxCN3->isChecked()) statW->addChannel("单道 3",nbrSCC+2);
    if (ui->checkBoxCN4->isChecked()) statW->addChannel("单道 4",nbrSCC+3);
    if (ui->checkBoxCN5->isChecked()) statW->addChannel("单道 5",nbrSCC+4);
    if (ui->checkBoxCN6->isChecked()) statW->addChannel("单道 6",nbrSCC+5);
    for (int i = 0; i < vCoinWidget.count(); i++)
    {
        if (vCoinWidgetSyncState.at(i))
        {
            bool checkMulti;
            checkMulti = vCoinWidget.at(i)->getCoinParam(&coinChannelName,&nbrCoinPtr,&nbrAccCoinPtr);
            statW->addChannel("符合 "+coinChannelName,nbrCoinPtr);
            if (!checkMulti)
                statW->addChannel("偶然符合 "+coinChannelName,nbrAccCoinPtr);
        }
    }
}

void Widget::dealStatisticsRequestSync()
{
    disconnect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);
    for (int i = 0; i < vCoinWidget.count(); i++)
        if (vCoinWidgetSyncState.at(i))
            disconnect(timerCount,&QTimer::timeout,vCoinWidget.at(i),&CoincidenceWidget::dealTimeOut);
    connect(timerCount,&QTimer::timeout,statW,&StatisticsWidget::dealTimeOut);
    connect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);
    for (int i = 0; i < vCoinWidget.count(); i++)
        if (vCoinWidgetSyncState.at(i))
            connect(timerCount,&QTimer::timeout,vCoinWidget.at(i),&CoincidenceWidget::dealTimeOut);
}

void Widget::dealStatisticsRequestStopSync()
{
    disconnect(timerCount,&QTimer::timeout,statW,&StatisticsWidget::dealTimeOut);
}
