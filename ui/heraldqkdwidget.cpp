#include "heraldqkdwidget.h"
#include "ui_heraldqkdwidget.h"

HeraldQkdWidget::HeraldQkdWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeraldQkdWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);        // 在父窗口上显示独立的子窗口
    this->setWindowTitle(tr("标记 MDI 计数器"));

    this->setupLcdCounts();

    ui->labelPic->setStyleSheet("border-image: url(:/pic/heraldQkd/herald_MDI_detection.jpeg);");
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

    ui->lcdTimeElapsed->display(0);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            vCounts[i][j] = 0;
            vLcdCounts[i][j]->display(0);
        }
    }

    emit heraldQkdRequestParam();
}

void HeraldQkdWidget::on_buttonStop_released()
{
    emit heraldQkdRequestStopSync();
}

void HeraldQkdWidget::dealQkdParamReceived(double *m_delayCN, double m_freqCOM)
{
    for (int i = 0; i < 6; i++)
        delayCN[i] = m_delayCN[i];
    freqCOM = m_freqCOM;

    //    预处理 TDC 参数
    double timeCOM = 1000000.0/freqCOM;           // 单位为 us
    timeCOMunit = int(20*1000.0*timeCOM);         // TDC 内部单位，50 ps
    tolerance = int(20*ui->textTolerance->text().toDouble());
    double delayUi[6] = {0.0};
    delayUi[0] = ui->textDelay1->text().toDouble();
    delayUi[1] = ui->textDelay2->text().toDouble();
    delayUi[2] = ui->textDelay3->text().toDouble();
    delayUi[3] = ui->textDelay4->text().toDouble();
    delayUi[4] = ui->textDelay5->text().toDouble();
    delayUi[5] = ui->textDelay6->text().toDouble();
    double delayTotal[6] = {0.0};
    double minDelay = delayCN[0];
    for (int i = 0; i < 6; i++)
    {
        delayTotal[i] = delayCN[i] + delayUi[i]/1000.0;
        if (delayTotal[i] < minDelay)
            minDelay = delayTotal[i];
    }
    int maxNbrCOMdelay = 0;
    for (int i = 0; i < 6; i++)
    {
        delayTotal[i] -= minDelay;             // 保证所有延时均为非负
        nbrCOMdelay[i] = floor(delayTotal[i]/timeCOM);
        if (nbrCOMdelay[i] > maxNbrCOMdelay)
            maxNbrCOMdelay = nbrCOMdelay[i];
        delayInCOM[i] = int(20*1000.0*delayTotal[i] - timeCOM*nbrCOMdelay[i]);
    }

//    时间序列所需要保存的 COM 周期数量为 nbrCOMdelay 中的最大值 +2
    resizeSeqLength(&timeSeq, maxNbrCOMdelay+2);
    resizeSeqLength(&channelSeq, maxNbrCOMdelay+2);
    COM_HEAD = 0;

    emit heraldQkdRequestSync();
}

void HeraldQkdWidget::dealDataReturned(AqT3DataDescriptor *dataDescPtr)
{
    // 对每个响应事件进行分类累积
    computeHeraldMdiCounts(dataDescPtr, timeSeq, channelSeq, vCounts,
             tolerance,
             nbrCOMdelay,
             delayInCOM,
             timeCOMunit,
             &COM_HEAD);
}

void HeraldQkdWidget::dealTimeOut()
{
    // 更新计数显示
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            vLcdCounts[i][j]->display(double(vCounts[i][j]));

    ui->lcdTimeElapsed->display(ui->lcdTimeElapsed->intValue()+1);
}
