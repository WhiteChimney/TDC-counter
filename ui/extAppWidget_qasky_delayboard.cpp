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
    delayBoard->initializeDevice(
                spList.at(ui->comboDelayboardSpList->currentIndex()));
    emit requestDelayFeedback();
}

void ExternalApplicationsWidget::dealDelayFeedbackDataReceived(int *m_nbrSCC)
{
    nbrSCC = m_nbrSCC;
    for (int i = 0; i < countChannelList.size(); i++)
    {
        delayAdjustDirection.append(1);
        countBefore.append(0);
        countCurrent.append(0);
        currentRound = 0;
    }
}

void ExternalApplicationsWidget::doSingleCountTimeoutFeedback()
{
    currentRound++;

    // 每轮计算均值
    for (int i = 0; i < countChannelList.size(); i++)
        countCurrent[i] = (1-1.0/currentRound)*countCurrent[i]
                + double(nbrSCC[countChannelList[i]->text().toInt()-1])/currentRound;

    if (currentRound >= ui->textAverageRounds->text().toInt())
        // 对每一对绑定的通道，进行延时反馈
    {
        for (int i = 0; i < countChannelList.size(); i++)
        {
            if (countCurrent[i] < countBefore[i])
                delayAdjustDirection[i] = -delayAdjustDirection[i];
            delayBoard->setRelativeDelay(delayChannelList[i]->text().toInt(),
                delayAdjustDirection[i]*ui->textDelayAdj->text().toDouble()/1000.0);
//            qDebug() << countBefore[i] << countCurrent[i]
//                     << delayAdjustDirection[i]
//                     << delayBoard->getDelay(delayChannelList[i]->text().toInt());
            countBefore[i] = countCurrent[i];
            countCurrent[i] = 0;
        }
        delayBoard->sendCommand();
        currentRound = 0;
    }
}

void ExternalApplicationsWidget::on_buttonDelayFeedbackStop_released()
{
    emit requestStopDelayFeedback();
    delayBoard->closeDevice();
}

