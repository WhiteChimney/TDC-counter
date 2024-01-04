#include "externalapplicationswidget.h"
#include "ui_externalapplicationswidget.h"

void ExternalApplicationsWidget::on_comboSmcSpList_activated(int index)
{
    if (index == ui->comboSmcSpList->count()-1)
    {
        ui->comboSmcSpList->clear();
        spList.clear();
        spList = QSerialPortInfo::availablePorts();
        for (int i = 0; i < spList.size(); i++)
            ui->comboSmcSpList->addItem(spList.at(i).portName());
        ui->comboSmcSpList->addItem("刷新");
    }
}

void ExternalApplicationsWidget::on_buttonSmcOpenPort_released()
{
    smc->initializeDevice(
                spList.at(ui->comboSmcSpList->currentIndex()));
}

void ExternalApplicationsWidget::on_buttonSmcClosePort_released()
{
    smc->closeDevice();
}

void ExternalApplicationsWidget::on_buttonSmcHoming_released()
{
    smc->homingDevice(ui->textSmcAddr->text().toInt());
}

void ExternalApplicationsWidget::on_buttonSmcSetAngle_released()
{
    smc->setAbsoluteAngle(
                ui->textSmcAddr->text().toInt(),
                ui->textSmcAngle->text().toDouble());
}

void ExternalApplicationsWidget::on_buttonAngleFeedbackAdd_released()
{
    QLineEdit *a, *b, *c;
    a = new QLineEdit(this);
    b = new QLineEdit(this);
    c = new QLineEdit(this);
    smcCountChannelList.append(a);
    smcTargetCountList.append(b);
    smcChannelList.append(c);
    ui->layoutAngleFeedback->addWidget(
                a,ui->layoutAngleFeedback->rowCount(),0,1,1);
    ui->layoutAngleFeedback->addWidget(
                b,ui->layoutAngleFeedback->rowCount()-1,1,1,1);
    ui->layoutAngleFeedback->addWidget(
                c,ui->layoutAngleFeedback->rowCount()-1,2,1,1);
}

void ExternalApplicationsWidget::on_buttonAngleFeedbackRemove_released()
{
    if (smcCountChannelList.isEmpty())
        return;

    ui->layoutAngleFeedback->removeWidget(smcCountChannelList.last());
    ui->layoutAngleFeedback->removeWidget(smcTargetCountList.last());
    ui->layoutAngleFeedback->removeWidget(smcChannelList.last());
    smcCountChannelList.removeLast();
    smcTargetCountList.removeLast();
    smcChannelList.removeLast();
}

void ExternalApplicationsWidget::on_buttonAngleFeedbackStart_released()
{
    emit requestAngleFeedback();
}

void ExternalApplicationsWidget::dealAngleFeedbackDataReceived(int *m_nbrSCC)
{
    nbrSCC = m_nbrSCC;
    for (int i = 0; i < countChannelList.size(); i++)
    {
        angleAdjustDirection.append(1);
        smcCountBefore.append(0);
        smcCountCurrent.append(0);
        smcCurrentRound = 0;
    }
}

void ExternalApplicationsWidget::doSmcSingleCountTimeoutFeedback()
{
    smcCurrentRound++;

    // 每轮计算均值
    for (int i = 0; i < smcCountChannelList.size(); i++)
        smcCountCurrent[i] = (1-1.0/smcCurrentRound)*smcCountCurrent[i]
                + double(nbrSCC[smcCountChannelList[i]->text().toInt()-1])/smcCurrentRound;

    if (smcCurrentRound >= ui->textAverageRounds->text().toInt())
        // 对每一对绑定的通道，进行延时反馈
    {
        for (int i = 0; i < smcCountChannelList.size(); i++)
        {
            if (smcCountCurrent[i] < smcCountBefore[i])
                angleAdjustDirection[i] = -angleAdjustDirection[i];
            smc->setRelativeAngle(smcChannelList[i]->text().toInt(),
                angleAdjustDirection[i]*ui->textAngleAdj->text().toDouble()/1000.0);
            qDebug() << smcCountBefore[i] << smcCountCurrent[i]
                     << angleAdjustDirection[i]
                     << smc->getAbsoluteAngle(smcChannelList[i]->text().toInt());
            smcCountBefore[i] = smcCountCurrent[i];
            smcCountCurrent[i] = 0;
        }
        smcCurrentRound = 0;
    }
}

void ExternalApplicationsWidget::on_buttonAngleFeedbackStop_released()
{
    emit requestStopAngleFeedback();
}

