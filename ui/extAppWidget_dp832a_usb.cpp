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
//    dp832a_visa instr1(ui->comboBoxDP832AUSB->currentData().toString(), this);
    dp832a_visa instr1(QString("USB0::0x1AB1::0x0E11::DP8B240700265::INSTR"),this);
    qDebug() << "1";
    instr1.initializeDevice();

    qDebug() << "2";

    QString command = ui->textDP832Acommand->text();
    qDebug() << "3";
    QString result = ui->labelUSBret->text();
    qDebug() << "4";
    instr1.sendCommand(&command,&result);
    ui->labelUSBret->setText(result);
    qDebug() << "5";

    instr1.closeDevice();
    qDebug() << "6";
}
