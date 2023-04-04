#include "externalapplicationswidget.h"
#include "ui_externalapplicationswidget.h"

void ExternalApplicationsWidget::setupSPIndicator()
{
    SPstatusIndicator->setCustomOnColor0(QSimpleLed::GREEN);
    SPstatusIndicator->setCustomOffColor0(QSimpleLed::RED);
    QSizePolicy p1;
    p1.setHorizontalPolicy(QSizePolicy::Minimum);
    p1.setVerticalPolicy(QSizePolicy::Preferred);
    SPstatusIndicator->setSizePolicy(p1);
    ui->horizontalLayout_3->insertWidget(0,SPstatusIndicator);
    SPstatusIndicator->setStates(QSimpleLed::OFF);
}

void ExternalApplicationsWidget::refreshPorts()
{
    ui->comboboxSPList->clear();
    spList.clear();
    spList = QSerialPortInfo::availablePorts();
    for (int i = 0; i < spList.size(); i++)
        ui->comboboxSPList->addItem(spList.at(i).portName());
}

void ExternalApplicationsWidget::on_buttonRefresh_released()
{
    this->refreshPorts();
}

void ExternalApplicationsWidget::fetchUiData()
{
    switch (ui->comboBoxBaudRateList->currentIndex()) {
    case 0: baudRate = QSerialPort::Baud1200; break;
    case 1: baudRate = QSerialPort::Baud2400; break;
    case 2: baudRate = QSerialPort::Baud4800; break;
    case 3: baudRate = QSerialPort::Baud9600; break;
    case 4: baudRate = QSerialPort::Baud19200; break;
    case 5: baudRate = QSerialPort::Baud38400; break;
    case 6: baudRate = QSerialPort::Baud57600; break;
    case 7: baudRate = QSerialPort::Baud115200; break;
    default: baudRate = QSerialPort::UnknownBaud; break;
    }

    switch (ui->comboBoxDataBitsList->currentIndex()) {
    case 0: dataBits = QSerialPort::Data5; break;
    case 1: dataBits = QSerialPort::Data6; break;
    case 2: dataBits = QSerialPort::Data7; break;
    case 3: dataBits = QSerialPort::Data8; break;
    default: dataBits = QSerialPort::UnknownDataBits; break;
    }

    switch (ui->comboBoxStopBitsList->currentIndex()) {
    case 0: stopBits = QSerialPort::OneStop; break;
    case 1: stopBits = QSerialPort::OneAndHalfStop; break;
    case 2: stopBits = QSerialPort::TwoStop; break;
    default: stopBits = QSerialPort::UnknownStopBits; break;
    }

    switch (ui->comboBoxParityBitsList->currentIndex()) {
    case 0: parity = QSerialPort::NoParity; break;
    case 1: parity = QSerialPort::EvenParity; break;
    case 2: parity = QSerialPort::OddParity; break;
    case 3: parity = QSerialPort::SpaceParity; break;
    case 4: parity = QSerialPort::MarkParity; break;
    default: parity = QSerialPort::UnknownParity; break;
    }
}

void ExternalApplicationsWidget::pushUiData()
{
    switch (baudRate) {
    case QSerialPort::Baud1200: ui->comboBoxBaudRateList->setCurrentIndex(0); break;
    case QSerialPort::Baud2400: ui->comboBoxBaudRateList->setCurrentIndex(1); break;
    case QSerialPort::Baud4800: ui->comboBoxBaudRateList->setCurrentIndex(2); break;
    case QSerialPort::Baud9600: ui->comboBoxBaudRateList->setCurrentIndex(3); break;
    case QSerialPort::Baud19200: ui->comboBoxBaudRateList->setCurrentIndex(4); break;
    case QSerialPort::Baud38400: ui->comboBoxBaudRateList->setCurrentIndex(5); break;
    case QSerialPort::Baud57600: ui->comboBoxBaudRateList->setCurrentIndex(6); break;
    case QSerialPort::Baud115200: ui->comboBoxBaudRateList->setCurrentIndex(7); break;
    default: break;
    }

    switch (dataBits) {
    case QSerialPort::Data5: ui->comboBoxDataBitsList->setCurrentIndex(0); break;
    case QSerialPort::Data6: ui->comboBoxDataBitsList->setCurrentIndex(1); break;
    case QSerialPort::Data7: ui->comboBoxDataBitsList->setCurrentIndex(2); break;
    case QSerialPort::Data8: ui->comboBoxDataBitsList->setCurrentIndex(3); break;
    default: break;
    }

    switch (stopBits) {
    case QSerialPort::OneStop: ui->comboBoxStopBitsList->setCurrentIndex(0); break;
    case QSerialPort::OneAndHalfStop: ui->comboBoxStopBitsList->setCurrentIndex(1); break;
    case QSerialPort::TwoStop: ui->comboBoxStopBitsList->setCurrentIndex(2); break;
    default: break;
    }

    switch (parity) {
    case QSerialPort::NoParity: ui->comboBoxParityBitsList->setCurrentIndex(0); break;
    case QSerialPort::EvenParity: ui->comboBoxParityBitsList->setCurrentIndex(1); break;
    case QSerialPort::OddParity: ui->comboBoxParityBitsList->setCurrentIndex(2); break;
    case QSerialPort::SpaceParity: ui->comboBoxParityBitsList->setCurrentIndex(3); break;
    case QSerialPort::MarkParity: ui->comboBoxParityBitsList->setCurrentIndex(4); break;
    default: break;
    }
}


void ExternalApplicationsWidget::on_buttonOpenSP_released()
{
    this->fetchUiData();
    if (SPstatusIndicator->states() == QSimpleLed::OFF)
    {
        dp832_sp = new DP832A_Serial(this);
        if (dp832_sp->initializeDevice(spList.at(ui->comboboxSPList->currentIndex()),
                                       baudRate,
                                       dataBits,
                                       stopBits,
                                       parity))
            SPstatusIndicator->setStates(QSimpleLed::ON);
    }
}

void ExternalApplicationsWidget::on_buttonCloseSP_released()
{
    if (SPstatusIndicator->states() == QSimpleLed::ON)
    {
        dp832_sp->closeDevice();
        SPstatusIndicator->setStates(QSimpleLed::OFF);
    }
}

void ExternalApplicationsWidget::on_buttonSend_released()
{
    if (SPstatusIndicator->states() == QSimpleLed::ON)
        dp832_sp->sendCommand(ui->textDataSent->text());
}

void ExternalApplicationsWidget::on_buttonReceive_released()
{
    if (SPstatusIndicator->states() == QSimpleLed::ON)
        ui->labelDataReceived->setText(dp832_sp->readData());
}
