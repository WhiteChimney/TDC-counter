#include "externalapplicationswidget.h"
#include "ui_externalapplicationswidget.h"

void ExternalApplicationsWidget::on_buttonSetDelay_released()
{
    qDebug() << QSerialPortInfo::availablePorts().at(
                    ui->textDelayboardSp->text().toInt()
                    ).portName();
    qDebug() << delayBoard->initializeDevice(
                QSerialPortInfo::availablePorts().at(
                    ui->textDelayboardSp->text().toInt()
                    ));
    delayBoard->setVoltage(
                ui->comboDelayBoardChannel->currentIndex(),
                ui->textDelayboardVoltage->text().toDouble());
    delayBoard->setDelay(
                ui->comboDelayBoardChannel->currentIndex(),
                ui->textDelayboardDelay->text().toDouble());
    delayBoard->sendCommand();
    delayBoard->closeDevice();
}

