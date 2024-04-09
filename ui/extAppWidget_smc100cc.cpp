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
    QComboBox *d;
    a = new QLineEdit(this);
    b = new QLineEdit(this);
    c = new QLineEdit(this);
    d = new QComboBox(this);
    d->addItem("1");
    d->addItem("-1");
    smcCountChannelList.append(a);
    smcTargetCountList.append(b);
    smcChannelList.append(c);
    smcAngleDirList.append(d);
    ui->layoutAngleFeedback->addWidget(
                a,ui->layoutAngleFeedback->rowCount(),0,1,1);
    ui->layoutAngleFeedback->addWidget(
                b,ui->layoutAngleFeedback->rowCount()-1,1,1,1);
    ui->layoutAngleFeedback->addWidget(
                c,ui->layoutAngleFeedback->rowCount()-1,2,1,1);
    ui->layoutAngleFeedback->addWidget(
                d,ui->layoutAngleFeedback->rowCount()-1,3,1,1);
}

void ExternalApplicationsWidget::on_buttonAngleFeedbackRemove_released()
{
    if (smcCountChannelList.isEmpty())
        return;

    ui->layoutAngleFeedback->removeWidget(smcCountChannelList.last());
    ui->layoutAngleFeedback->removeWidget(smcTargetCountList.last());
    ui->layoutAngleFeedback->removeWidget(smcChannelList.last());
    ui->layoutAngleFeedback->removeWidget(smcAngleDirList.last());
    smcCountChannelList.removeLast();
    smcTargetCountList.removeLast();
    smcChannelList.removeLast();
    smcAngleDirList.removeLast();
}

void ExternalApplicationsWidget::on_buttonAngleFeedbackStart_released()
{
    emit requestAngleFeedback();
}

void ExternalApplicationsWidget::dealAngleFeedbackDataReceived(int *m_nbrSCC)
{
    ui->buttonAngleFeedbackStart->setEnabled(false);
    ui->buttonAngleFeedbackStop->setEnabled(true);

    nbrSCC = m_nbrSCC;
    for (int i = 0; i < countChannelList.size(); i++)
    {
//        angleAdjustDirection.append(1);
//        smcCountBefore.append(0);
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
            double countDiff = 1-smcCountCurrent[i]/smcTargetCountList[i]->text().toDouble();
            double angleAdj =                                               // 计算角度调整量
                    countDiff/0.1*ui->textAngleAdj->text().toDouble()       // 调整量为 偏移百分比是 10% 的多少倍，就把 10% 角度偏移量乘多少倍
                    *smcAngleDirList[i]->currentText().toInt();             // 再乘上提前试好的角度调节方向
            smc->setRelativeAngle(smcChannelList[i]->text().toInt(),angleAdj);
//            qDebug() << "angle: " << angleAdj;
            smcCountCurrent[i] = 0;
        }
        smcCurrentRound = 0;
    }

    if (enableErrorFeedback)
    {
        errorCurrentRound++;
        double errorRateNow = double(*(correctCountPtr[0]) + *(correctCountPtr[0]))
                / (*(errorCountPtr[0]) + *(errorCountPtr[0]));
        errorRateNow = 1.0/(1+errorRateNow);
        errorRateCurrent = (1-1.0/errorCurrentRound)*errorRateCurrent
                + errorRateNow/errorCurrentRound;
        if (errorCurrentRound >= ui->textErrorFeedbackRounds->text().toInt())
        {
            if (errorRateCurrent > errorRateBefore)
                errorFeedbackDirection = -errorFeedbackDirection;
            smc->setRelativeAngle(ui->spinBoxErrorFeedbackChannel->value(),
                                  errorFeedbackDirection*ui->textErrorFeedbackAngle->text().toDouble());
            errorRateBefore = errorRateCurrent;
            errorRateCurrent = 0.0;
            errorCurrentRound = 0;
        }
    }
}

void ExternalApplicationsWidget::on_buttonAngleFeedbackStop_released()
{
    emit requestStopAngleFeedback();

    ui->buttonAngleFeedbackStop->setEnabled(false);
    ui->buttonAngleFeedbackStart->setEnabled(true);
}

void ExternalApplicationsWidget::dealHeraldQkdErrorFeedback(long long *m_errorCountPtr[2],
                                long long *m_correctCountPtr[2])
{
    errorCountPtr[0] = m_errorCountPtr[0];
    errorCountPtr[1] = m_errorCountPtr[1];
    correctCountPtr[0] = m_correctCountPtr[0];
    correctCountPtr[1] = m_correctCountPtr[1];
    errorCurrentRound = 0;
    errorRateCurrent = 0.0;
    errorRateBefore = 0.0;
    errorFeedbackDirection = 1;
    enableErrorFeedback = true;
}

void ExternalApplicationsWidget::dealHeraldQkdStopErrorFeedback()
{
    enableErrorFeedback = false;
}
