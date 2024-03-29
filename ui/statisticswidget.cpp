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
    if (stepCurrent+1 == stepsTotal)
        on_buttonStop_released();
    for (int i = 0; i < vStatChannel.count() ; i++ )
    {
        vStatChannel.at(i)->updateLcdCount(stepCurrent,unitTime);
    }
    stepCurrent++;

    // 设置进度条，显示两位小数
    ui->progressBarStat->setMaximum(100*stepsTotal);
    double progressValue = 100.0*stepCurrent;
    ui->progressBarStat->setValue(progressValue);
    int noDecimal = ceil(log10(stepsTotal/100.0));
    if (noDecimal < 0) noDecimal = 0;
    ui->progressBarStat->setFormat
            (QString::number(progressValue/stepsTotal,'f',noDecimal)+"%");
    ui->progressBarStat->setAlignment(Qt::AlignCenter);
}
