#include "heraldqkdwidget.h"
#include "ui_heraldqkdwidget.h"

HeraldQkdWidget::HeraldQkdWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeraldQkdWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);        // 在父窗口上显示独立的子窗口

    this->setupLcdCounts();
}

void HeraldQkdWidget::setupLcdCounts()
{
    ClipLcdNumber *c0;
    c0 = new ClipLcdNumber(0,this);
    ui->layoutYields->addWidget(c0,0,0,1,1);
    ClipLcdNumber *c1;
    c1 = new ClipLcdNumber(1,this);
    ui->layoutYields->addWidget(c1,1,1,1,1);
    ClipLcdNumber *c2;
    c2 = new ClipLcdNumber(2,this);
    ui->layoutYields->addWidget(c2,2,0,1,1);
    ClipLcdNumber *c3;
    c3 = new ClipLcdNumber(3,this);
    ui->layoutYields->addWidget(c3,3,1,1,1);
    ClipLcdNumber *c4;
    c4 = new ClipLcdNumber(4,this);
    ui->layoutYields->addWidget(c4,4,0,1,1);
    ClipLcdNumber *c5;
    c5 = new ClipLcdNumber(5,this);
    ui->layoutYields->addWidget(c5,5,1,1,1);
    ClipLcdNumber *c6;
    c6 = new ClipLcdNumber(6,this);
    ui->layoutYields->addWidget(c6,6,0,1,1);
    ClipLcdNumber *c7;
    c7 = new ClipLcdNumber(7,this);
    ui->layoutYields->addWidget(c7,7,1,1,1);
}

HeraldQkdWidget::~HeraldQkdWidget()
{
    delete ui;
}

void HeraldQkdWidget::on_buttonStart_released()
{
    qDebug() << "started";

    QAction *a;
    a = new QAction(this);

    ui->groupYields->addAction(a);


    qDebug() << "count: " << ui->layoutYields->count() << endl
             << "row count: " << ui->layoutYields->rowCount() << endl
             << "column count: " << ui->layoutYields->columnCount() << endl;

}


void HeraldQkdWidget::on_buttonStop_released()
{

}


void HeraldQkdWidget::on_buttonReturn_released()
{
    emit sendReturnSignal();
}

