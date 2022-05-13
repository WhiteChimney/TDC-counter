#include "statisticswidget.h"
#include "ui_statisticswidget.h"

StatisticsWidget::StatisticsWidget(double unitTime0, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWidget)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::Window);
    this->setWindowTitle("统计");
    ui->labelComment->setWordWrap(true);
    ui->progressBarStat->setValue(0);
    unitTime = unitTime0;
    ui->textUnitTime->setText(QString::number(unitTime));

    vStatChannel.clear();
}

StatisticsWidget::~StatisticsWidget()
{
    delete ui;
}

void StatisticsWidget::on_buttonReturn_released()
{
    emit sendReturnSignal();
}

void StatisticsWidget::addChannel(QString channelName0, int *countPtr0)
{
    StatisticsChannel* newChannel = new StatisticsChannel();
    vStatChannel.append(newChannel);
    newChannel->setChannelName(channelName0);
    newChannel->setCountPtr(countPtr0);
    int nbrRows = ui->gridLayoutMain->rowCount();
    ui->gridLayoutMain->addWidget(newChannel->labelChannelName,nbrRows,1);
    ui->gridLayoutMain->addWidget(newChannel->lcdCount,nbrRows,2);
    ui->gridLayoutMain->addWidget(newChannel->lcdCountStd,nbrRows,3);
//    newChannel->labelChannelName->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
//    newChannel->lcdCount->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
//    newChannel->lcdCountStd->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);
}

void StatisticsWidget::on_buttonStart_released()
{
    statTime = abs(ui->textStatTime->text().toDouble());
    stepsTotal = ceil(statTime/unitTime);
    statTime = unitTime*stepsTotal;
    ui->textStatTime->setText(QString::number(statTime));
    for (int i = 0; i < vStatChannel.count() ; i++ )
    {
        vStatChannel.at(i)->setNbrSteps(stepsTotal);
    }
    stepCurrent = 0;
    emit statisticsRequestSync();
}


void StatisticsWidget::on_buttonStop_released()
{
    emit statisticsRequestStopSync();
}

void StatisticsWidget::dealTimeOut()
{
    for (int i = 0; i < vStatChannel.count() ; i++ )
    {
        vStatChannel.at(i)->updateLcdCount(stepCurrent);
    }
    stepCurrent++;
    ui->progressBarStat->setValue(100*stepCurrent/stepsTotal);
}
