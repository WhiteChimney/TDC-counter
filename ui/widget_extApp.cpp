#include "widget.h"
#include "ui_widget.h"

void Widget::setupExtAppWidget()
{
    extAppW = new ExternalApplicationsWidget(this);
    extAppW->setWindowTitle("外接功能");
    connect(extAppW,&ExternalApplicationsWidget::requestData,this,&Widget::dealExtAppRequestData);
}

void Widget::on_buttonExternalApplications_released()
{
    extAppW->show();
}

void Widget::dealExtAppRequestData()
{
    connect(this,&Widget::mainAppClosing,extAppW,&ExternalApplicationsWidget::dealMainAppClosed);
    connect(extAppW,&ExternalApplicationsWidget::dataReceived,this,&Widget::dealExpAppDataReceived);
    connect(this,&Widget::sendExpAppRequestedData,extAppW,&ExternalApplicationsWidget::dealRequestedData);
    connect(extAppW,&ExternalApplicationsWidget::externalAppStopped,this,&Widget::dealExpAppStopped);
    QVector<int*> vNbrCoin;
    for (int i = 0; i < vCoinWidget.size(); i++)
    {
        if (vCoinWidget.at(i)->windowState() == Qt::WindowActive)
            vNbrCoin.append(vCoinWidget.at(i)->getCoinCountPtr());
    }
    emit sendExpAppRequestedData(nbrSCC, vNbrCoin);
}

void Widget::dealExpAppDataReceived()
{
    disconnect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);
    for (int i = 0; i < vCoinWidget.size(); i++)
        if (vCoinWidget.at(i)->windowState() == Qt::WindowActive)
            disconnect(timerCount,&QTimer::timeout,vCoinWidget.at(i),&CoincidenceWidget::dealTimeOut);
    connect(timerCount, &QTimer::timeout, extAppW, &ExternalApplicationsWidget::dealSingleCountTimeup);
    for (int i = 0; i < vCoinWidget.size(); i++)
        if (vCoinWidget.at(i)->windowState() == Qt::WindowActive)
            connect(timerCount,&QTimer::timeout,vCoinWidget.at(i),&CoincidenceWidget::dealTimeOut);
    connect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);
}

void Widget::dealExpAppStopped()
{
    disconnect(timerCount, &QTimer::timeout, extAppW, &ExternalApplicationsWidget::dealSingleCountTimeup);
}
