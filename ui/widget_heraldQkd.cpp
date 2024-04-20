#include "widget.h"
#include "ui_widget.h"


void Widget::on_buttonQkd_released()
{
//    初始化统计 Widget
    if (qkdWidgetLaunched)
    {
        qkdW->show();
        qkdW->activateWindow();
    }
    else
    {
        qkdW = new HeraldQkdWidget(this);

        connect(qkdW,&HeraldQkdWidget::sendReturnSignal,this,&Widget::dealHeraldQkdReturn);
        connect(qkdW,&HeraldQkdWidget::heraldQkdRequestParam,this,&Widget::dealHeraldQkdRequestParam);
        connect(this,&Widget::qkdParamReady,qkdW,&HeraldQkdWidget::dealQkdParamReceived);
        connect(qkdW,&HeraldQkdWidget::heraldQkdRequestSync,this,&Widget::dealHeraldQkdRequestSync);
        connect(qkdW,&HeraldQkdWidget::heraldQkdRequestStopSync,this,&Widget::dealHeraldQkdRequestStopSync);
        connect(qkdW,&HeraldQkdWidget::requestErrorFeedback,extAppW,&ExternalApplicationsWidget::dealHeraldQkdErrorFeedback);
        connect(qkdW,&HeraldQkdWidget::requestStopErrorFeedback,extAppW,&ExternalApplicationsWidget::dealHeraldQkdStopErrorFeedback);

        qkdW->show();
        qkdWidgetLaunched = true;
    }
}

void Widget::dealHeraldQkdReturn()
{
    qkdW->close();
    qkdWidgetLaunched = false;
    qkdW->deleteLater();
}

void Widget::dealHeraldQkdRequestParam()
{
    emit qkdParamReady(delayCN, freqCOM, nbrSCC);
}

void Widget::dealHeraldQkdRequestSync()
{
    connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,qkdW,&HeraldQkdWidget::dealDataReturned);
    connect(timerCount,&QTimer::timeout,qkdW,&HeraldQkdWidget::dealTimeOut);
}

void Widget::dealHeraldQkdRequestStopSync()
{
    disconnect(acqThread,&AcquisitionThread::acqThreadBankSwitch,qkdW,&HeraldQkdWidget::dealDataReturned);
    disconnect(timerCount,&QTimer::timeout,qkdW,&HeraldQkdWidget::dealTimeOut);
}
