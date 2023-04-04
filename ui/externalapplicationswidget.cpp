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

void ExternalApplicationsWidget::dealMainAppClosed()
{
    if (serial->isOpen())
        serial->close();
    tsp->closeDevice();
}

