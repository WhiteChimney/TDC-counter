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
    fetchUiData();
    if (ui->stackCoin->currentIndex()==0) // 双通道模式
    {
        emit askDealAcqBankSwitchCoin(index,&delayCN,&freqCOM,&countEvents);
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

        emit askDealAcqBankSwitchCoin(index,&delayCN,&freqCOM,&countEvents);
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
    clear2DintVector(timeSeq);
    clear2DintVector(timeSeqAcc);
    clear2DintVector(channelSeq);
    clear2DintVector(channelSeqAcc);
}

void CoincidenceWidget::dealAcqThreadBankSwitchCoin(QVector<AqT3DataDescriptor*> dataPtrList)
{
    int channels[6] = {0};
    int nbrChannels = 0;
    int *nbrCoinCalc;
    int toleranceCalc;
    int *delayCalc;
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
    computeCoincidenceCount
            (dataPtrList,
             timeSeq, timeSeqAcc,
             channelSeq, channelSeqAcc,
             nbrChannels, channels, nbrCoinCalc,
             toleranceCalc, delayCalc, &nbrAccCoin, delayAcc,
             delayCN, freqCOM, countEvents);
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
