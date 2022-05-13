#include "statisticswidget.h"
#include "ui_statisticswidget.h"

StatisticsWidget::StatisticsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWidget)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::Window);
    this->setWindowTitle("统计");
    ui->labelComment->setWordWrap(true);

//    vChannelName.clear();
//    vCountPtr.clear();
//    vCountStd.clear();
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
    newChannel->updateLcdCount();
    newChannel->updateLcdCountStd();
}

void StatisticsWidget::on_buttonStart_released()
{
}


void StatisticsWidget::on_buttonStop_released()
{
}

