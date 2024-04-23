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

//    spDevice = new DP832A_Serial(this);
    spDevice = new SMC100CC_Serial(this);

    TSP01statusIndicator = new QSimpleLed(this);
    this->setupTSPIndicator();
    DP832UsbIndicator = new QSimpleLed(this);
    this->setupDP832UsbIndicator();

    this->refreshPorts();

    tsp = new TSP01(this);
    this->refreshTSPlist();

    delayBoard = new QaskyDelayBoard(this);
    smc = new SMC100CC_Serial(this);
    qskyspad = new Spd300Qsky(this);
    QString iniPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/AcqirisTDC_qt";
    fileName = iniPath + "/Configurations/qskyspad_delay_feedback_data.txt";
    fSave->setFileName(fileName);
}

ExternalApplicationsWidget::~ExternalApplicationsWidget()
{
    delete ui;
}

void ExternalApplicationsWidget::dealMainAppClosed()
{
    tsp->closeDevice();
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
}

void ExternalApplicationsWidget::on_buttonStop_released()
{
    emit externalAppStopped();
    this->customizedSPcommands_stop();
}

void ExternalApplicationsWidget::on_checkboxSPcustomize_stateChanged(int checkState)
{
    if (checkState == Qt::Checked)
    {
        ui->buttonSend->setEnabled(false);
        ui->buttonReceive->setEnabled(false);
        ui->buttonStart->setEnabled(true);
        ui->buttonStop->setEnabled(true);
    }
    else
    {
        ui->buttonSend->setEnabled(true);
        ui->buttonReceive->setEnabled(true);
        ui->buttonStart->setEnabled(false);
        ui->buttonStop->setEnabled(false);
    }
}
