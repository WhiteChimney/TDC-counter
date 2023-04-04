#include "externalapplicationswidget.h"
#include "ui_externalapplicationswidget.h"


void ExternalApplicationsWidget::on_buttonRefreshUSB_clicked()
{

}

void ExternalApplicationsWidget::refreshDP832AUSBlist()
{
    ui->comboBoxDP832AUSB->clear();

    QList<QString> usbDevices = TSP01::findInstruments();

    for (int i = 0; i < usbDevices.size(); i++)
        ui->comboBoxDP832AUSB->addItem(usbDevices.at(i));
}

void ExternalApplicationsWidget::on_buttonDP832AVISAtest_clicked()
{
    dp832a_visa instr1(ui->comboBoxDP832AUSB->currentData().toString(), this);
    instr1.initializeDevice();

    QString command = ui->textDP832Acommand->text();
    QString result = ui->labelUSBret->text();
    instr1.sendCommand(&command,&result);

    instr1.closeDevice();
}
