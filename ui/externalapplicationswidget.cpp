#include "externalapplicationswidget.h"
#include "ui_externalapplicationswidget.h"

ExternalApplicationsWidget::ExternalApplicationsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExternalApplicationsWidget)
{
    ui->setupUi(this);
    ui->comboBoxBaudRateList->setCurrentIndex(3);
    ui->comboBoxDataBitsList->setCurrentIndex(3);
    ui->comboBoxStopBitsList->setCurrentIndex(0);
    ui->comboBoxParityBitsList->setCurrentIndex(0);
    ui->checkboxSPcustomize->setChecked(true);

    setWindowFlags(Qt::Window);        // 在父窗口上显示独立的子窗口

    SPstatusIndicator = new QSimpleLed(this);
    this->setupSPIndicator();
    TSP01statusIndicator = new QSimpleLed(this);
    this->setupTSPIndicator();

    serial = new QSerialPort(this);
    this->refreshPorts();

    tsp = new TSP01(this);
    this->refreshTSPlist();

}

ExternalApplicationsWidget::~ExternalApplicationsWidget()
{
    delete ui;
}

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

void ExternalApplicationsWidget::setupTSPIndicator()
{
    TSP01statusIndicator->setCustomOnColor0(QSimpleLed::GREEN);
    TSP01statusIndicator->setCustomOffColor0(QSimpleLed::RED);
    QSizePolicy p1;
    p1.setHorizontalPolicy(QSizePolicy::Minimum);
    p1.setVerticalPolicy(QSizePolicy::Preferred);
    TSP01statusIndicator->setSizePolicy(p1);
    ui->gridLayout_3->addWidget(TSP01statusIndicator,0,0,2,1,0);
    TSP01statusIndicator->setStates(QSimpleLed::OFF);
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

bool ExternalApplicationsWidget::openSerialPort(QSerialPortInfo spInfo)
{
    serial->setPort(spInfo);
    serial->setBaudRate(baudRate);
    serial->setDataBits(dataBits);
    serial->setStopBits(stopBits);
    serial->setParity(parity);
    return serial->open(QIODevice::ReadWrite);
}

QString ExternalApplicationsWidget::readData()
{
    qDebug() << serial->waitForReadyRead(1000);
    QByteArray buffer = serial->readAll();
    if (buffer.size() > 0)
        buffer = buffer.remove(buffer.size()-1,1);
    return buffer;
}

void ExternalApplicationsWidget::sendData(QString dataText)
{
    serial->write(dataText.toLocal8Bit().append("\n"));
    serial->waitForBytesWritten();
}

void ExternalApplicationsWidget::on_checkboxSPcustomize_stateChanged(int checkState)
{
    if (checkState == Qt::Checked)
    {
        ui->buttonSend->setEnabled(false);
        ui->buttonReceive->setEnabled(false);
        ui->buttonTest->setEnabled(dataRec);
        ui->buttonStart->setEnabled(true);
        ui->buttonStop->setEnabled(true);
    }
    else
    {
        ui->buttonSend->setEnabled(true);
        ui->buttonReceive->setEnabled(true);
        ui->buttonTest->setEnabled(false);
        ui->buttonStart->setEnabled(false);
        ui->buttonStop->setEnabled(false);
    }
}


void ExternalApplicationsWidget::on_buttonOpenSP_released()
{
    this->fetchUiData();
    if (this->openSerialPort(spList.at(ui->comboboxSPList->currentIndex())))
    {
        SPstatusIndicator->setStates(QSimpleLed::ON);
    }
    else
    {
        SPstatusIndicator->setStates(QSimpleLed::OFF);
        QMessageBox::warning(this,
                             tr("警告"),
                             tr("串口打开失败"),
                             QMessageBox::Ok);
    }
}

void ExternalApplicationsWidget::on_buttonCloseSP_released()
{
    if (serial->isOpen())
    {
        serial->close();
        SPstatusIndicator->setStates(QSimpleLed::OFF);
    }
}

void ExternalApplicationsWidget::on_buttonSend_released()
{
    this->sendData(ui->textDataSent->text());
}

void ExternalApplicationsWidget::on_buttonReceive_released()
{
    ui->labelDataReceived->setText(this->readData());
}

void ExternalApplicationsWidget::on_buttonStart_released()
{
    emit this->requestData();
}

void ExternalApplicationsWidget::dealRequestedData(int* m_nbrSCC, QVector<int*> m_vNbrCoin)
{
    nbrSCC = new int(); nbrSCC = m_nbrSCC;
    vNbrCoin.clear();
    for (int i = 0; i < m_vNbrCoin.size(); i++)
        vNbrCoin.append(m_vNbrCoin.at(i));
    this->customizedSPcommands_start();
    emit dataReceived();
    dataRec = true;
    ui->buttonTest->setEnabled(dataRec);
}

void ExternalApplicationsWidget::on_buttonStop_released()
{
    emit externalAppStopped();
    this->customizedSPcommands_stop();
}

void ExternalApplicationsWidget::refreshTSPlist()
{
    ui->comboBoxTSP01DeviceList->clear();

    tspList = tsp->findInstruments();

    for (int i = 0; i < tspList.size(); i++)
        ui->comboBoxTSP01DeviceList->addItem(tspList.at(i));
}

void ExternalApplicationsWidget::on_buttonRefreshTSP01_released()
{
    this->refreshTSPlist();
}

void ExternalApplicationsWidget::on_buttonOpenTSP01_released()
{
    tsp->initializeDevice(tspList.at(ui->comboBoxTSP01DeviceList->currentIndex()));

    if (tsp->getErrorCode())
        TSP01statusIndicator->setStates(QSimpleLed::OFF);
    else
        TSP01statusIndicator->setStates(QSimpleLed::ON);
}


void ExternalApplicationsWidget::on_buttonCloseTSP01_released()
{
    tsp->closeDevice();
    TSP01statusIndicator->setStates(QSimpleLed::OFF);
}


void ExternalApplicationsWidget::on_buttonTestTSP01_released()
{
    ui->whiteBoard->setText(tsp->checkDeviceInfo());

    if (tsp->getErrorCode())
        TSP01statusIndicator->setStates(QSimpleLed::OFF);
    else
        TSP01statusIndicator->setStates(QSimpleLed::ON);
}


void ExternalApplicationsWidget::on_buttonRefreshDataTSP01_released()
{
    ui->lcdTemp->display(tsp->getTemperature());
    ui->lcdTempOffset->display(tsp->getTemperatureOffset());
    ui->lcdHumd->display(tsp->getHumidity());
    ui->lcdHumdOffset->display(tsp->getHumidityOffset());

    if (tsp->getErrorCode())
        TSP01statusIndicator->setStates(QSimpleLed::OFF);
    else
        TSP01statusIndicator->setStates(QSimpleLed::ON);
}

void ExternalApplicationsWidget::dealMainAppClosed()
{
    if (serial->isOpen())
        serial->close();
    tsp->closeDevice();
}
