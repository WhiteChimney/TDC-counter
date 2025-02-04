#include "heraldqkdwidget.h"
#include "ui_heraldqkdwidget.h"

HeraldQkdWidget::HeraldQkdWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeraldQkdWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);        // 在父窗口上显示独立的子窗口
    this->setWindowTitle(tr("Relay 实验计数器"));

    this->setupLcdCounts();

    ui->labelPic->setStyleSheet("border-image: url(:/pic/heraldQkd/herald_MDI_detection.jpeg);");

    QString iniPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/AcqirisTDC_qt";
    iniName = iniPath + "/Configurations/heraldQkd.ini";
    QFileInfo iniInfo(iniName);
    if (iniInfo.isFile())
        loadFromIni();
    else
        saveToIni();

    fileName = iniPath + "/Configurations/heraldQkd_data.txt";
    fSave->setFileName(fileName);
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

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            vCounts[i][j] = 0;
            vLcdCounts[i][j]->display(0);
        }
    }

    lcdMainPeak->display(0);
    lcdSidePeak->display(0);

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
    double delayTotal[6] = {0.0}, delayTotalPre[6] = {0.0}, delayTotalAft[6] = {0.0};
    double minDelay = delayCN[0];
    for (int i = 0; i < 6; i++)
    {
        delayTotalPre[i] = delayCN[i] + delayUi[i]/20.0/1000.0;
        delayTotal[i] = delayTotalPre[i] + extraDelay/20.0/1000.0;
        delayTotalAft[i] = delayTotal[i] + extraDelay/20.0/1000.0;
        if (delayTotalPre[i] < minDelay)
            minDelay = delayTotalPre[i];
    }
    int maxNbrCOMdelay = 0;
    for (int i = 0; i < 6; i++)
    {
        delayTotalPre[i] -= minDelay;
        delayTotal[i] -= minDelay;             // 保证所有延时均为非负
        delayTotalAft[i] -= minDelay;
        nbrCOMdelayPre[i] = floor(delayTotalPre[i]/timeCOM);
        nbrCOMdelay[i] = floor(delayTotal[i]/timeCOM);
        nbrCOMdelayAft[i] = floor(delayTotalAft[i]/timeCOM);
        if (nbrCOMdelayAft[i] > maxNbrCOMdelay)
            maxNbrCOMdelay = nbrCOMdelayAft[i];
        delayInCOMPre[i] = int(20*1000.0*(delayTotalPre[i] - timeCOM*nbrCOMdelayPre[i]));
        delayInCOM[i] = int(20*1000.0*(delayTotal[i] - timeCOM*nbrCOMdelay[i]));
        delayInCOMAft[i] = int(20*1000.0*(delayTotalAft[i] - timeCOM*nbrCOMdelayAft[i]));
    }

//    时间序列所需要保存的 COM 周期数量为 nbrCOMdelay 中的最大值 +2 再乘 3
    resizeSeqLength(&timeSeq, 3*(maxNbrCOMdelay+2));
    resizeSeqLength(&channelSeq, 3*(maxNbrCOMdelay+2));
    COM_HEAD = 0;

    emit heraldQkdRequestSync();

    fSave->open(QIODevice::WriteOnly | QIODevice::Text);
    fStream.setDevice(fSave);

    fStream << "Time (s)\t"
            << "S1\t" << "S2\t" << "S3\t" << "S4\t" << "S5\t" << "S6\t"
            << "C11\t" << "C12\t" << "C13\t"
            << "C21\t" << "C22\t" << "C23\t"
            << "C31\t" << "C32\t" << "C33\t"
            << "C41\t" << "C42\t" << "C43\t"
            << "C51\t" << "C52\t" << "C53\t"
            << "C61\t" << "C62\t" << "C63\t"
            << "C71\t" << "C72\t" << "C73\t"
            << "C81\t" << "C82\t" << "C83\t"
            << "C91\t" << "C92\t" << "C93\t";
}

void HeraldQkdWidget::dealDataReturned(AqT3DataDescriptor *dataDescPtr)
{
    // 对每个响应事件进行分类累积
    computeHeraldMdiCounts(dataDescPtr, timeSeq, channelSeq, vCounts,
        tolerance, nbrCOMdelayPre, delayInCOMPre, nbrCOMdelay,
        delayInCOM, nbrCOMdelayAft, delayInCOMAft, timeCOMunit, &COM_HEAD);
}

void HeraldQkdWidget::dealTimeOut()
{
    int currentSeconds = ui->lcdTimeElapsed->intValue()+1;

    fStream << QString::number(currentSeconds);

    for (int i = 0; i < 6; i++)
        fStream << "\t" << QString::number(nbrSCC[i]);

    // 更新计数显示
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            fStream << "\t"
                    << QString::number(vCounts[i][j] -
                                       vLcdCounts[i][j]->intValue());
            vLcdCounts[i][j]->display(
                        double(vCounts[i][j]));
        }
    }
    fStream << "\n";

    ui->lcdTimeElapsed->display(currentSeconds);
}
