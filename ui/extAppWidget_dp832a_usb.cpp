#include "externalapplicationswidget.h"
#include "ui_externalapplicationswidget.h"

void ExternalApplicationsWidget::setupDP832UsbIndicator()
{
    DP832UsbIndicator->setCustomOnColor0(QSimpleLed::GREEN);
    DP832UsbIndicator->setCustomOffColor0(QSimpleLed::RED);
    QSizePolicy p1;
    p1.setHorizontalPolicy(QSizePolicy::Minimum);
    p1.setVerticalPolicy(QSizePolicy::Preferred);
    DP832UsbIndicator->setSizePolicy(p1);
    ui->gridLayoutDP832Usb->addWidget(DP832UsbIndicator,0,0,2,1,0);
    DP832UsbIndicator->setStates(QSimpleLed::OFF);
}

void ExternalApplicationsWidget::refreshDP832UsbList()
{
    ui->comboBoxDP832UsbList->clear();

    QList<QString> usbDevices = TSP01::findInstruments();

    for (int i = 0; i < usbDevices.size(); i++)
        ui->comboBoxDP832UsbList->addItem(usbDevices.at(i));
}

void ExternalApplicationsWidget::on_buttonDP832UsbRefresh_released()
{
    refreshDP832UsbList();
}

void ExternalApplicationsWidget::on_buttonDP832UsbInitiate_released()
{
    if (DP832UsbIndicator->states() == QSimpleLed::OFF)
    {
        dp832usb = new DP832A_USB(ui->comboBoxDP832UsbList->currentText(), this);
        if (dp832usb->initializeDevice())
            DP832UsbIndicator->setStates(QSimpleLed::ON);
    }
}

void ExternalApplicationsWidget::on_buttonDP832UsbClose_released()
{
    if (DP832UsbIndicator->states() == QSimpleLed::ON)
    {
        dp832usb->closeDevice();
        DP832UsbIndicator->setStates(QSimpleLed::OFF);
    }
}

void ExternalApplicationsWidget::on_buttonDP832UsbTest_released()
{
    if (DP832UsbIndicator->states() == QSimpleLed::ON)
    {
        on_buttonDP832UsbClose_released();
    }
    DP832A_USB instr1(QString("USB0::0x1AB1::0x0E11::DP8B240700265::INSTR"),this);
    instr1.initializeDevice();
    QString command = ui->textDP832UsbCmdSent->text();
    QString result = ui->labelDP832UsbMsgRecv->text();
    instr1.sendCommand(&command,&result);
    ui->labelDP832UsbMsgRecv->setText(result);
    instr1.closeDevice();
}

void ExternalApplicationsWidget::on_buttonDP832UsbSendCmd_released()
{
    if (DP832UsbIndicator->states() == QSimpleLed::ON)
    {
        QString command = ui->textDP832UsbCmdSent->text();
        QString result = ui->labelDP832UsbMsgRecv->text();
        dp832usb->sendCommand(&command,&result);
        ui->labelDP832UsbMsgRecv->setText(result);
    }
}
