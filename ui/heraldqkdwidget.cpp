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
    for (int i = 0; i < 4; i++)
    {
        QList<ClipLcdNumber*> v;
        vLcdCounts.append(v);
        for (int j = 0; j < 4; j++)
        {
            ClipLcdNumber *c;
            c = new ClipLcdNumber(15,this);
            vLcdCounts[i].append(c);
            ui->layoutYields->addWidget(c,2*i+1,j+1,1,1);
        }
    }
}

HeraldQkdWidget::~HeraldQkdWidget()
{
    delete ui;
}

void HeraldQkdWidget::on_buttonReturn_released()
{
    emit sendReturnSignal();
}

void HeraldQkdWidget::on_buttonStart_released()
{
    on_buttonStop_released();

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            vCounts[i][j] = 0;
            vLcdCounts[i][j]->display(0);
        }
    }

    emit heraldQkdRequestSync();

    qDebug() << "started";

}

void HeraldQkdWidget::on_buttonStop_released()
{
    emit heraldQkdRequestStopSync();
}

void HeraldQkdWidget::dealTimeOut()
{
    // 更新计数显示
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            vLcdCounts[i][j]->display(vCounts[i][j]);
}

void HeraldQkdWidget::dealDataReturned(AqT3DataDescriptor *dataDescPtr)
{
    // 对每个响应事件进行分类累积
}
