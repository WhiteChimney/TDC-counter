#include "widget.h"
#include "ui_widget.h"

void Widget::setupExtAppWidget()
{
    extAppW = new ExternalApplicationsWidget(this);
    extAppW->setWindowTitle("外接功能");

    connect(extAppW,&ExternalApplicationsWidget::requestData,this,&Widget::dealExtAppRequestData);

    connect(extAppW,&ExternalApplicationsWidget::requestDelayFeedback,this,&Widget::dealExtAppRequestDelayFeedback);
    connect(this,&Widget::sendExtAppDelayFeedbackData,extAppW,&ExternalApplicationsWidget::dealDelayFeedbackDataReceived);
    connect(extAppW,&ExternalApplicationsWidget::requestStopDelayFeedback,this,&Widget::dealExtAppStopRequestDelayFeedback);

    connect(extAppW,&ExternalApplicationsWidget::requestDelayFeedback2,this,&Widget::dealExtAppRequestDelayFeedback2);
    connect(this,&Widget::sendExtAppDelayFeedbackData2,extAppW,&ExternalApplicationsWidget::dealDelayFeedbackDataReceived2);
    connect(extAppW,&ExternalApplicationsWidget::requestStopDelayFeedback2,this,&Widget::dealExtAppStopRequestDelayFeedback2);

    connect(extAppW,&ExternalApplicationsWidget::requestAngleFeedback,this,&Widget::dealExtAppRequestAngleFeedback);
    connect(this,&Widget::sendExtAppAngleFeedbackData,extAppW,&ExternalApplicationsWidget::dealAngleFeedbackDataReceived);
    connect(extAppW,&ExternalApplicationsWidget::requestStopAngleFeedback,this,&Widget::dealExtAppStopRequestAngleFeedback);
}

void Widget::on_buttonExternalApplications_released()
{
    extAppW->show();
}

void Widget::dealExtAppRequestData()
{
    connect(this,&Widget::mainAppClosing,extAppW,&ExternalApplicationsWidget::dealMainAppClosed);
    connect(extAppW,&ExternalApplicationsWidget::dataReceived,this,&Widget::dealExtAppDataReceived);
    connect(this,&Widget::sendExtAppRequestedData,extAppW,&ExternalApplicationsWidget::dealRequestedData);
    connect(extAppW,&ExternalApplicationsWidget::externalAppStopped,this,&Widget::dealExtAppStopped);
    QVector<int*> vNbrCoin;
    for (int i = 0; i < vCoinWidget.size(); i++)
    {
        if (vCoinWidget.at(i)->windowState() == Qt::WindowActive)
            vNbrCoin.append(vCoinWidget.at(i)->getCoinCountPtr());
    }
    emit sendExtAppRequestedData(nbrSCC, vNbrCoin);
}

void Widget::dealExtAppDataReceived()
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

void Widget::dealExtAppStopped()
{
    disconnect(timerCount, &QTimer::timeout, extAppW, &ExternalApplicationsWidget::dealSingleCountTimeup);
}

void Widget::dealExtAppRequestDelayFeedback()
{
    emit sendExtAppDelayFeedbackData(nbrSCC);
    disconnect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);
    connect(timerCount, &QTimer::timeout, extAppW, &ExternalApplicationsWidget::doSingleCountTimeoutFeedback);
    connect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);
}

void Widget::dealExtAppStopRequestDelayFeedback()
{
    disconnect(timerCount, &QTimer::timeout, extAppW, &ExternalApplicationsWidget::doSingleCountTimeoutFeedback);
}

void Widget::dealExtAppRequestDelayFeedback2()
{
    emit sendExtAppDelayFeedbackData2(nbrSCC);
    disconnect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);
    connect(timerCount, &QTimer::timeout, extAppW, &ExternalApplicationsWidget::doSingleCountTimeoutFeedback2);
    connect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);
}

void Widget::dealExtAppStopRequestDelayFeedback2()
{
    disconnect(timerCount, &QTimer::timeout, extAppW, &ExternalApplicationsWidget::doSingleCountTimeoutFeedback2);
}

void Widget::dealExtAppRequestAngleFeedback()
{
    emit sendExtAppAngleFeedbackData(nbrSCC);
    disconnect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);
    connect(timerCount, &QTimer::timeout, extAppW, &ExternalApplicationsWidget::doSmcSingleCountTimeoutFeedback);
    connect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);
}

void Widget::dealExtAppStopRequestAngleFeedback()
{
    disconnect(timerCount, &QTimer::timeout, extAppW, &ExternalApplicationsWidget::doSmcSingleCountTimeoutFeedback);
}
