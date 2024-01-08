#include "coincidencewidget.h"
#include "ui_coincidencewidget.h"

CoincidenceWidget::CoincidenceWidget(QWidget *parent, int index0) :
    QWidget(parent),
    ui(new Ui::CoincidenceWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);        // 在父窗口上显示独立的子窗口
//    this->setAttribute(Qt::WA_DeleteOnClose);   // 退出时执行析构函数
    index = index0;                    // 标记当前符合窗口

    // 符合专用时钟
    timerCoin = new QTimer(this);

    // 设置配置文件与临时文件名
    iniPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/AcqirisTDC_qt";
    iniName = iniPath + "/Configurations/coincidence" + QString::number(index) +".ini";
    tempFileName = iniPath + "/Data/Coincidence" + QString::number(index) +".txt";
    fCoin->setFileName(tempFileName);
    fStream.setDevice(fCoin);
    QFileInfo iniInfo(iniName);
    if (iniInfo.isFile())
        loadFromIni();
    else
        saveToIni();
}

CoincidenceWidget::~CoincidenceWidget()
{
    on_buttonStop_released();
    on_buttonReturn_released();
    saveToIni();
    delete ui;
}

void CoincidenceWidget::on_buttonReturn_released()
{
    on_buttonStop_released();
    emit returnSignal(index);
}

void CoincidenceWidget::on_buttonStart_released()
{
    on_buttonStop_released();
    fetchUiData();
    if (ui->stackCoin->currentIndex()==0) // 双通道模式
    {
        ui->buttonChangeToMulti->setEnabled(false);
        if (enableAccumulateTime)
        { // 如果需要与单道计数同步，发送同步请求
            emit coinTimerNeedsSync(index);
        }
        else
        {// 否则使用自己的时钟
            connect(timerCoin,&QTimer::timeout,this,&CoincidenceWidget::dealTimeOut);
            accumulateTime = ui->accumTimeCoin->text().toDouble();
            timerCoin->start(1000.0*accumulateTime);
            timeTic = QDateTime::currentMSecsSinceEpoch();
        }
    }
    else
    {
        bool existChannel = false;
        for (int i = 0; i < 6; i++)
        {
            if (channelMark[i])
            {
                existChannel = true;
                break;
            }
        }
        if (!existChannel)
        {
            QMessageBox::critical(this,
                                  QString("警告"),
                                  QString("未选择通道！"),
                                  QMessageBox::Ok);
            return;
        }
        ui->buttonChangeToDual->setEnabled(false);
        if (enableAccumulateTimeMulti)
        { // 如果需要与单道计数同步，发送同步请求
            emit coinTimerNeedsSync(index);
        }
        else
        {// 否则使用自己的时钟
            connect(timerCoin,&QTimer::timeout,this,&CoincidenceWidget::dealTimeOut);
            accumulateTimeMulti = ui->accumTimeCoin_Multi->text().toDouble();
            timerCoin->start(1000.0*accumulateTimeMulti);
            timeTic = QDateTime::currentMSecsSinceEpoch();
        }
    }
    coinSavable = true; // 此时可保存数据

    emit requestCoinParam(index);
}

void CoincidenceWidget::on_buttonStop_released()
{
    coinSavable = false;
    emit askStopDealAcqBankSwitchCoin(index); // 双通道模式
    if (ui->stackCoin->currentIndex()==0)
    {
        if (enableAccumulateTime)
        { // 如果需要与单道计数同步，发送同步请求
            emit coinTimerStopsSync(index);
        }
        else
        {// 否则使用自己的时钟
            disconnect(timerCoin,&QTimer::timeout,this,&CoincidenceWidget::dealTimeOut);
            timerCoin->stop();
        }
    }
    else
    {
        if (enableAccumulateTimeMulti)
        { // 如果需要与单道计数同步，发送同步请求
            emit coinTimerStopsSync(index);
        }
        else
        {// 否则使用自己的时钟
            disconnect(timerCoin,&QTimer::timeout,this,&CoincidenceWidget::dealTimeOut);
            timerCoin->stop();
        }
    }
    clear2DintVector(&timeSeq);
    clear2DintVector(&timeSeqAcc);
    clear2DintVector(&channelSeq);
    clear2DintVector(&channelSeqAcc);
}

void CoincidenceWidget::dealRequestCoinParam(int index0, double *delayCN0, double freqCOM0)
{
    if (index == index0)
    {
        delayCN = new double[6]();
        for (int i = 0; i < 6; i++)
            delayCN[i] = delayCN0[i];
        freqCOM = freqCOM0;
        if (ui->stackCoin->currentIndex()==0) // 双通道模式计算符合计数
        {
            channels[0] = channel1-1;
            channels[1] = channel2-1;
            nbrChannels = 2;
            nbrCoinCalc = &nbrCoin;
            toleranceCalc = tolerance;
            delayCalc = new int[6]();
            delayCalc[channel2-1] = delay;
        }
        else                                   // 多通道模式计算符合计数
        {
            nbrChannels = 0;
            for (int i = 0; i < 6; i++)
            {
                if (channelMark[i])
                {
                    channels[nbrChannels] = i;
                    nbrChannels++;
                }
            }
            nbrCoinCalc = &nbrCoinMulti;
            toleranceCalc = toleranceMulti;
            delayCalc = delayMulti;
        }

        //    预处理 TDC 参数
        double timeCOM = 1000000.0/freqCOM;           // 单位为 us
        timeCOMunit = int(20*1000.0*timeCOM);         // TDC 内部单位，50 ps
        double delayTotal[6] = {0.0}, delayTotalAcc[6] = {0.0};
        double minDelay = delayCN[0] + delayCalc[0]/20.0/1000.0;
        for (int i = 0; i < 6; i++)
        {
            delayTotal[i] = delayCN[i] + delayCalc[i]/20.0/1000.0;
            if (delayTotal[i] < minDelay)
                minDelay = delayTotal[i];
            if (nbrChannels == 2)
            {
                delayTotalAcc[i] = delayTotal[i];
                if(i == channels[1])
                    delayTotalAcc[i] += delayAcc/20.0/1000.0;
            }
        }
        if (nbrChannels == 2 && delayAcc < 0)
            minDelay += delayAcc;
        int maxNbrCOMdelay = 0, maxNbrCOMdelayAcc = 0;
        for (int i = 0; i < 6; i++)
        {
            delayTotal[i] -= minDelay;             // 保证所有延时均为非负
            nbrCOMdelay[i] = floor(delayTotal[i]/timeCOM);
            if (nbrCOMdelay[i] > maxNbrCOMdelay)
                maxNbrCOMdelay = nbrCOMdelay[i];
            delayInCOM[i] = int(20*1000.0*delayTotal[i] - timeCOM*nbrCOMdelay[i]);
            if (nbrChannels == 2)
            {
                delayTotalAcc[i] -= minDelay;
                nbrCOMdelayAcc[i] = floor(delayTotalAcc[i]/timeCOM);
                if (nbrCOMdelayAcc[i] > maxNbrCOMdelayAcc)
                    maxNbrCOMdelayAcc = nbrCOMdelayAcc[i];
                delayInCOMAcc[i] = int(20*1000.0*delayTotalAcc[i] - timeCOM*nbrCOMdelayAcc[i]);
            }
        }

    //    时间序列所需要保存的 COM 周期数量为 nbrCOMdelay 中的最大值 +2
        resizeSeqLength(&timeSeq, maxNbrCOMdelay+2);
        resizeSeqLength(&channelSeq, maxNbrCOMdelay+2);
        if (nbrChannels == 2)
        {
            resizeSeqLength(&timeSeqAcc, maxNbrCOMdelayAcc+2);
            resizeSeqLength(&channelSeqAcc, maxNbrCOMdelayAcc+2);
        }
        COM_HEAD = 0;

        emit askDealAcqBankSwitchCoin(index);
    }
}

void CoincidenceWidget::dealAcqThreadBankSwitchCoin(AqT3DataDescriptor* dataDescPtr)
{
    computeCoincidenceCount
            (dataDescPtr,
             timeSeq, timeSeqAcc,
             channelSeq, channelSeqAcc,
             nbrChannels, channels, nbrCoinCalc,
             toleranceCalc, &nbrAccCoin,
             nbrCOMdelay, nbrCOMdelayAcc,
             delayInCOM, delayInCOMAcc,
             timeCOMunit, &COM_HEAD);
}

void CoincidenceWidget::dealTimeOut()
{
    if (ui->stackCoin->currentIndex()==0) // 双通道模式
    {
        ui->lcdCoin->display(nbrCoin);
        ui->lcdAccCoin->display(nbrAccCoin);
        memset(&nbrCoin,0,sizeof(nbrCoin));
        memset(&nbrAccCoin,0,sizeof(nbrAccCoin));
    }
    else
    {
        ui->lcdCoin_Multi->display(nbrCoinMulti);
        memset(&nbrCoinMulti,0,sizeof(nbrCoinMulti));
    }
}

bool CoincidenceWidget::getCoinParam(QString* coinChannelNamePtr, int **nbrCoinPtrPtr, int **nbrAccCoinPtrPtr)
{
    fetchUiData();
    if (ui->stackCoin->currentIndex()==0)
    {
        *coinChannelNamePtr = QString::number(channel1) + "&" + QString::number(channel2);
        *nbrCoinPtrPtr = &nbrCoin;
        *nbrAccCoinPtrPtr = &nbrAccCoin;
        return false;
    }
    else
    {
        *coinChannelNamePtr = "";
        for (int i = 0; i < 6; i++)
        {
            if (channelMark[i])
                *coinChannelNamePtr += QString::number(i+1) + "&";
        }
        coinChannelNamePtr->remove(coinChannelNamePtr->length()-1,1);
        *nbrCoinPtrPtr = &nbrCoinMulti;
        return true;
    }
}

int* CoincidenceWidget::getCoinCountPtr()
{
    if (ui->stackCoin->currentIndex()==0)
        return &nbrCoin;
    else
        return &nbrCoinMulti;
}

int* CoincidenceWidget::getAccCoinCountPtr()
{
    return &nbrAccCoin;
}
