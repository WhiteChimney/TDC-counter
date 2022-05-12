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

//    vChannelName = new QVector<QString>();
//    vCountPtr = new QVector<int*>();
//    vCountStd = new QVector<int>();
//    vChannelName.clear();
//    vCountPtr.clear();
//    vCountStd.clear();
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
//    vChannelName.append(channelName0);
//    vCountPtr.append(countPtr0);
//    int countStd0 = 0;
//    vCountStd.append(countStd0);
//    QLabel *labelChannelName = new QLabel(channelName0);
//    QLCDNumber *lcdCount = new QLCDNumber(15);
//    QLCDNumber *lcdCountStd = new QLCDNumber(9);
//    ui->gridLayoutMain->addWidget(labelChannelName,1,1);
//    ui->gridLayoutMain->addWidget(lcdCount,1,2);
//    ui->gridLayoutMain->addWidget(lcdCountStd,1,3);
//    labelChannelName->setText(channelName0);
//    lcdCount->display(0);
//    lcdCountStd->display(0);
}

void StatisticsWidget::on_buttonStart_released()
{
    int a = 59;
    this->addChannel("shit",&a);

}

