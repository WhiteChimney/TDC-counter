#include "widget.h"
#include "ui_widget.h"

void Widget::setupAcqIndicator()
{
    statusIndicator->setCustomOnColor0(QSimpleLed::GREEN);
    statusIndicator->setCustomOffColor0(QSimpleLed::RED);
    QSizePolicy p1;
    p1.setHorizontalPolicy(QSizePolicy::Expanding);
    p1.setVerticalPolicy(QSizePolicy::Expanding);
    statusIndicator->setSizePolicy(p1);
    ui->horizontalLayout_6->insertWidget(0,statusIndicator);
    statusIndicator->setStates(QSimpleLed::OFF);
}

void Widget::on_buttonExit_released()
{
    for (int i = vCoinWidget.count()-1; i >= 0; i--)
    {
        if (vCoinWidget.at(i)->windowState() != Qt::WindowNoState)
            dealCoincidenceReturn(i);
        vCoinWidget.removeLast();
        vCoinWidgetSyncState.removeLast();
    }
    on_buttonStopCount_released();
    on_buttonStopAcq_released();
    QThread::msleep(100);     // 等待进程完全停止
    // Stops the acquisition & close instruments
    Acqrs_closeAll();
    this->close();
}
