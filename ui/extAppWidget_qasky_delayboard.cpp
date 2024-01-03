#include "externalapplicationswidget.h"
#include "ui_externalapplicationswidget.h"

void ExternalApplicationsWidget::on_comboDelayboardSpList_activated(int index)
{
    if (index == ui->comboDelayboardSpList->count()-1)
    {
        ui->comboDelayboardSpList->clear();
        spList.clear();
        spList = QSerialPortInfo::availablePorts();
        for (int i = 0; i < spList.size(); i++)
            ui->comboDelayboardSpList->addItem(spList.at(i).portName());
        ui->comboDelayboardSpList->addItem("刷新");
    }
}

void ExternalApplicationsWidget::on_buttonSetDelay_released()
{
    delayBoard->initializeDevice(
                spList.at(ui->comboDelayboardSpList->currentIndex()));
    delayBoard->setVoltage(
                ui->comboDelayBoardChannel->currentIndex(),
                ui->textDelayboardVoltage->text().toDouble());
    delayBoard->setDelay(
                ui->comboDelayBoardChannel->currentIndex(),
                ui->textDelayboardDelay->text().toDouble());
    delayBoard->sendCommand();
    delayBoard->closeDevice();
}

void ExternalApplicationsWidget::on_buttonDelayFeedbackAdd_released()
{
    QLineEdit *a, *b;
    a = new QLineEdit(this);
    b = new QLineEdit(this);
    countChannelList.append(a);
    delayChannelList.append(b);
    ui->layoutDelayFeedback->addWidget(
                a,ui->layoutDelayFeedback->rowCount(),0,1,1);
    ui->layoutDelayFeedback->addWidget(
                b,ui->layoutDelayFeedback->rowCount()-1,1,1,1);
}

void ExternalApplicationsWidget::on_buttonDelayFeedbackRemove_released()
{
    if (countChannelList.isEmpty())
        return;

    ui->layoutDelayFeedback->removeWidget(countChannelList.last());
    ui->layoutDelayFeedback->removeWidget(delayChannelList.last());
    countChannelList.removeLast();
    delayChannelList.removeLast();
}

void ExternalApplicationsWidget::on_buttonDelayFeedbackStart_released()
{
    emit requestDelayFeedback();
}

void ExternalApplicationsWidget::dealDelayFeedbackDataReceived(int *m_nbrSCC)
{
    nbrSCC = m_nbrSCC;
}

void ExternalApplicationsWidget::doSingleCountTimeoutFeedback()
{
    // 对每一对绑定的通道，进行延时反馈
    for (int i = 0; i < countChannelList.size(); i++)
    {
//        delayBoard->setDelay(countChannelList[i],
//                             amountDelay(countChannelList[i],delayChannelList[i],nbrSCC));
    }
    delayBoard->sendCommand();
}

void ExternalApplicationsWidget::on_buttonDelayFeedbackStop_released()
{
    emit requestStopDelayFeedback();
}

