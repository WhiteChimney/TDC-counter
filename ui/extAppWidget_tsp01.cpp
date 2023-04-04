#include "externalapplicationswidget.h"
#include "ui_externalapplicationswidget.h"

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
