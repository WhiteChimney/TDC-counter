#include "externalapplicationswidget.h"
#include "ui_externalapplicationswidget.h"


void ExternalApplicationsWidget::on_comboQskySpadSpList_activated(int index)
{
    if (index == ui->comboQskySpadSpList->count()-1)
    {
        ui->comboQskySpadSpList->clear();
        spList.clear();
        spList = QSerialPortInfo::availablePorts();
        for (int i = 0; i < spList.size(); i++)
            ui->comboQskySpadSpList->addItem(spList.at(i).portName());
        ui->comboQskySpadSpList->addItem("刷新");
    }
}

void ExternalApplicationsWidget::on_buttonQskySpadSetDelay_released()
{
    qskyspad->initializeDevice(
        spList.at(ui->comboQskySpadSpList->currentIndex()));
    qskyspad->set_delay(
        ui->textQskySpadDelay->text().toDouble());
    qskyspad->closeDevice();

}

void ExternalApplicationsWidget::on_buttonQskySpadDelayFeedbackStart_released()
{
    qskyspad->initializeDevice(
        spList.at(ui->comboQskySpadSpList->currentIndex()));

    fSave->open(QIODevice::WriteOnly | QIODevice::Text);
    fStream.setDevice(fSave);

    emit requestDelayFeedback2();
}

void ExternalApplicationsWidget::dealDelayFeedbackDataReceived2(int *m_nbrSCC)
{
    ui->buttonQskySpadDelayFeedbackStart->setEnabled(false);
    ui->buttonQskySpadDelayFeedbackStop->setEnabled(true);

    nbrSCC = m_nbrSCC;
    delayAdjDirection = 1;
    countBefore2 = 0;
    countCurrent2 = 0;
    currentRound2 = 0;
}

void ExternalApplicationsWidget::doSingleCountTimeoutFeedback2()
{
    currentRound2++;

    // 每轮计算均值
    countCurrent2 = (1-1.0/currentRound2)*countCurrent2
                    + double(nbrSCC[ui->textQskySpadSingleChannel->text().toInt()-1])/currentRound2;

    if (currentRound2 >= ui->textQskySpadAverageRounds->text().toInt())
        // 进行延时反馈
    {
        if (countCurrent2 < countBefore2)
            delayAdjDirection = -delayAdjDirection;
        double delayNow = ui->textQskySpadDelay->text().toDouble();
        delayNow += delayAdjDirection*ui->textQskySpadDelayAdj->text().toDouble()/1000.0;
        qskyspad->set_delay(delayNow);
        ui->textQskySpadDelay->setText(QString::number(delayNow));
        // qDebug() << countBefore2 << countCurrent2 << delayAdjDirection;
        countBefore2 = countCurrent2;
        countCurrent2 = 0;
        currentRound2 = 0;

        fStream << delayNow << "\n";
    }
}

void ExternalApplicationsWidget::on_buttonQskySpadDelayFeedbackStop_released()
{
    fSave->close();

    emit requestStopDelayFeedback2();
    qskyspad->closeDevice();

    ui->buttonQskySpadDelayFeedbackStop->setEnabled(false);
    ui->buttonQskySpadDelayFeedbackStart->setEnabled(true);
}
