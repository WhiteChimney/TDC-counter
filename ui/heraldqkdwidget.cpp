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

    QString iniPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/AcqirisTDC_qt";
    iniName = iniPath + "/Configurations/heraldQkd.ini";
    QFileInfo iniInfo(iniName);
    if (iniInfo.isFile())
        loadFromIni();
    else
        saveToIni();

    fileName = iniPath + "/Data/heraldQkd.txt";
    fSave->setFileName(fileName);
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
    saveToIni();
    delete ui;
}

void HeraldQkdWidget::on_buttonReturn_released()
{
    emit sendReturnSignal();
}

void HeraldQkdWidget::on_buttonStart_released()
{
    on_buttonStop_released();

    saveToIni();

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
    fSave->close();
    emit heraldQkdRequestStopSync();
}

void HeraldQkdWidget::dealQkdParamReceived(double *m_delayCN, double m_freqCOM, int *m_nbrSCC)
{
    for (int i = 0; i < 6; i++)
        delayCN[i] = m_delayCN[i];
    freqCOM = m_freqCOM;
    nbrSCC = m_nbrSCC;

    //    预处理 TDC 参数
    double timeCOM = 1000000.0/freqCOM;           // 单位为 us
    timeCOMunit = int(20*1000.0*timeCOM);         // TDC 内部单位，50 ps
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
        delayInCOM[i] = int(20*1000.0*(delayTotal[i] - timeCOM*nbrCOMdelay[i]));
    }

//    时间序列所需要保存的 COM 周期数量为 nbrCOMdelay 中的最大值 +2
    resizeSeqLength(&timeSeq, maxNbrCOMdelay+2);
    resizeSeqLength(&channelSeq, maxNbrCOMdelay+2);
    COM_HEAD = 0;

    emit heraldQkdRequestSync();

    fSave->open(QIODevice::WriteOnly | QIODevice::Text);
    fStream.setDevice(fSave);

    fStream << "Time (s)\t"
            << "S1\t" << "S2\t" << "S3\t" << "S4\t" << "S5\t" << "S6\t"
            << "AB12\t" << "AB34\t" << "AB14\t" << "AB23\t"
            << "12\t" << "34\t" << "14\t" << "23\t"
            << "A12\t" << "A34\t" << "A14\t" << "A23\t"
            << "B12\t" << "B34\t" << "B14\t" << "B23\n";
}

void HeraldQkdWidget::dealDataReturned(AqT3DataDescriptor *dataDescPtr)
{
    // 对每个响应事件进行分类累积
    computeHeraldMdiCounts(dataDescPtr, timeSeq, channelSeq, vCounts,
        tolerance, deadTime, nbrCOMdelay, delayInCOM, timeCOMunit, &COM_HEAD);
}

void HeraldQkdWidget::dealTimeOut()
{
    int currentSeconds = ui->lcdTimeElapsed->intValue()+1;

    fStream << QString::number(currentSeconds);

    for (int i = 0; i < 6; i++)
        fStream << "\t" << QString::number(nbrSCC[i]);

    // 更新计数显示
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            fStream << "\t"
                    << QString::number(vCounts[i][j] -
                                       vLcdCounts[i][j]->intValue());
            vLcdCounts[i][j]->display(
                        double(vCounts[i][j]));
        }
    }
    fStream << "\n";

    double errorRate;
    double errorCount, correctCount;
    double errorCountNow, correctCountNow;
    if (ui->radioButtonErrorSame->isChecked())
    {
        errorCountNow = vCounts[1][0] + vCounts[1][1];
        correctCountNow = vCounts[1][2] + vCounts[1][3];
    }
    else
    {
        errorCountNow = vCounts[1][2] + vCounts[1][3];
        correctCountNow = vCounts[1][0] + vCounts[1][1];
    }
    errorCount = errorCountNow - errorCountBefore;
    correctCount = correctCountNow - correctCountBefore;
    errorRate = errorCount / (errorCount + correctCount);
    ui->labelErrorRate->setText(QString::number(errorRate));
    errorCountBefore = errorCountNow;
    correctCountBefore = correctCountNow;

    ui->lcdTimeElapsed->display(currentSeconds);
}

void HeraldQkdWidget::on_checkBoxErrorFeedback_stateChanged(int enableFeedback)
{
    // ui->radioButtonErrorSame->setEnabled(enableFeedback);
    // ui->radioButtonErrorOppo->setEnabled(enableFeedback);

    dealErrorFeedback();
}

void HeraldQkdWidget::dealErrorFeedback()
{
    if (ui->checkBoxErrorFeedback->isChecked())
    {
        long long *errorCountPtr[2], *correctCountPtr[2];
        if (ui->radioButtonErrorSame->isChecked())
        {
            errorCountPtr[0] = vCounts[1];
            errorCountPtr[1] = vCounts[1] + 1;
            correctCountPtr[0] = vCounts[1] + 2;
            correctCountPtr[1] = vCounts[1] + 3;
        }
        else
        {
            errorCountPtr[0] = vCounts[1] + 2;
            errorCountPtr[1] = vCounts[1] + 3;
            correctCountPtr[0] = vCounts[1];
            correctCountPtr[1] = vCounts[1] + 1;
        }
        emit requestErrorFeedback(errorCountPtr,correctCountPtr);
    }
    else
        emit requestStopErrorFeedback();
}

void HeraldQkdWidget::on_radioButtonErrorSame_clicked()
{
    dealErrorFeedback();
}


void HeraldQkdWidget::on_radioButtonErrorOppo_clicked()
{
    dealErrorFeedback();
}

