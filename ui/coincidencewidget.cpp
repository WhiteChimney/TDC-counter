#include "coincidencewidget.h"
#include "ui_coincidencewidget.h"

CoincidenceWidget::CoincidenceWidget(QWidget *parent, int m_index, int m_comOffset) : QWidget(parent),
                                                                                      ui(new Ui::CoincidenceWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window); // 在父窗口上显示独立的子窗口
                                      //    this->setAttribute(Qt::WA_DeleteOnClose);   // 退出时执行析构函数
    index = m_index;                  // 标记当前符合窗口

    // 符合专用时钟
    timerCoin = new QTimer(this);

    // 设置配置文件与临时文件名
    QString appVersion = "V" + QString(PROJECT_VERSION_0) + "." + QString(PROJECT_VERSION_1) + "." + QString(PROJECT_VERSION_2);
    iniPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + appVersion;
    iniName = iniPath + "/Configurations/coincidence" + QString::number(index) + ".ini";
    tempFileName = iniPath + "/Data/Coincidence" + QString::number(index) + ".txt";
    fCoin->setFileName(tempFileName);
    fStream.setDevice(fCoin);
    QFileInfo iniInfo(iniName);
    if (iniInfo.isFile())
        loadFromIni();
    else
        saveToIni();

    COM_offset = m_comOffset;
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
    if (ui->stackCoin->currentIndex() == 0) // 双通道模式
    {
        ui->buttonChangeToMulti->setEnabled(false);
        if (enableAccumulateTime)
        { // 如果需要与单道计数同步，发送同步请求
            emit coinTimerNeedsSync(index);
        }
        else
        { // 否则使用自己的时钟
            connect(timerCoin, &QTimer::timeout, this, &CoincidenceWidget::dealTimeOut);
            accumulateTime = ui->accumTimeCoin->text().toDouble();
            timerCoin->start(1000.0 * accumulateTime);
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
        { // 否则使用自己的时钟
            connect(timerCoin, &QTimer::timeout, this, &CoincidenceWidget::dealTimeOut);
            accumulateTimeMulti = ui->accumTimeCoin_Multi->text().toDouble();
            timerCoin->start(1000.0 * accumulateTimeMulti);
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
    if (ui->stackCoin->currentIndex() == 0)
    {
        if (enableAccumulateTime)
        { // 如果需要与单道计数同步，发送同步请求
            emit coinTimerStopsSync(index);
        }
        else
        { // 否则使用自己的时钟
            disconnect(timerCoin, &QTimer::timeout, this, &CoincidenceWidget::dealTimeOut);
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
        { // 否则使用自己的时钟
            disconnect(timerCoin, &QTimer::timeout, this, &CoincidenceWidget::dealTimeOut);
            timerCoin->stop();
        }
    }
    clear2DintVector(&timeSeq);
    clear2DintVector(&timeSeqAcc);
    clear2DintVector(&channelSeq);
    clear2DintVector(&channelSeqAcc);
    clear2DintVector(&timeSeq_2);
    clear2DintVector(&timeSeqAcc_2);
    clear2DintVector(&channelSeq_2);
    clear2DintVector(&channelSeqAcc_2);
    clear2DintVector(&timeSeqX1);
    clear2DintVector(&timeSeqAccX1);
    clear2DintVector(&channelSeqX1);
    clear2DintVector(&channelSeqAccX1);
    clear2DintVector(&timeSeqX2);
    clear2DintVector(&timeSeqAccX2);
    clear2DintVector(&channelSeqX2);
    clear2DintVector(&channelSeqAccX2);
}

void CoincidenceWidget::dealRequestCoinParam(int m_index,
                                             double *m_delayCN,
                                             double *m_delayCN_2,
                                             double m_freqCOM,
                                             int m_countEvents)
{
    if (index == m_index)
    {
        delayCN = m_delayCN;
        delayCN_2 = m_delayCN_2;
        freqCOM = m_freqCOM;
        countEvents = m_countEvents;

        nbrChannels = 0;
        nbrChannels_2 = 0;

        if (ui->stackCoin->currentIndex() == 0) // 双通道模式计算符合计数
        {
            nbrCoinCalc = &nbrCoin;
            toleranceCalc = tolerance;
            delayCalc = new int[6]();
            delayCalc_2 = new int[6]();
            if (device1 == 0)
            {
                channels[nbrChannels] = channel1 - 1;
                nbrChannels++;
            }
            else
            {
                channels_2[nbrChannels_2] = channel1 - 1;
                nbrChannels_2++;
            }
            if (device2 == 0)
            {
                channels[nbrChannels] = channel2 - 1;
                nbrChannels++;
                delayCalc[channel2 - 1] = delay;
            }
            else
            {
                channels_2[nbrChannels_2] = channel2 - 1;
                nbrChannels_2++;
                delayCalc_2[channel2 - 1] = delay;
            }
        }
        else // 多通道模式计算符合计数
        {
            for (int i = 0; i < 6; i++)
            {
                if (channelMark[i])
                {
                    channels[nbrChannels] = i;
                    nbrChannels++;
                }
                if (channelMark_2[i])
                {
                    channels_2[nbrChannels_2] = i;
                    nbrChannels_2++;
                }
            }
            nbrCoinCalc = &nbrCoinMulti;
            toleranceCalc = toleranceMulti;
            delayCalc = delayMulti;
            delayCalc_2 = delayMulti_2;
        }

        //    预处理 TDC 参数
        double timeCOM = 1000000.0 / freqCOM;     // 单位为 us
        timeCOMunit = int(20 * 1000.0 * timeCOM); // TDC 内部单位，50 ps
        double delayTotal[6] = {0.0}, delayTotalAcc[6] = {0.0};
        double delayTotal_2[6] = {0.0}, delayTotalAcc_2[6] = {0.0};

        double minDelay = delayCN[0] + delayCalc[0] / 20.0 / 1000.0;
        for (int i = 0; i < 6; i++)
        {
            delayTotal[i] = delayCN[i] + delayCalc[i] / 20.0 / 1000.0;
            delayTotal_2[i] = delayCN_2[i] + delayCalc_2[i] / 20.0 / 1000.0;
            if (delayTotal[i] < minDelay)
                minDelay = delayTotal[i];
            if (delayTotal_2[i] < minDelay)
                minDelay = delayTotal_2[i];
            if (ui->stackCoin->currentIndex() == 0)
            {
                delayTotalAcc[i] = delayTotal[i];
                delayTotalAcc_2[i] = delayTotal_2[i];
                switch (computeMode)
                {
                case 0:
                    if (i == channels[1])
                        delayTotalAcc[i] += delayAcc / 20.0 / 1000.0;
                    break;
                case 1:
                    if (device2 == 0 and i == channels[0])
                        delayTotalAcc[i] += delayAcc / 20.0 / 1000.0;
                    if (device2 == 1 and i == channels_2[0])
                        delayTotalAcc_2[i] += delayAcc / 20.0 / 1000.0;
                    break;
                case 2:
                    if (i == channels_2[1])
                        delayTotalAcc_2[i] += delayAcc / 20.0 / 1000.0;
                    break;
                default:
                    break;
                }
            }
        }
        if (ui->stackCoin->currentIndex() == 0 && delayAcc < 0)
            minDelay += delayAcc;

        maxNbrCOMdelay = 0, maxNbrCOMdelayAcc = 0;
        for (int i = 0; i < 6; i++)
        {
            delayTotal[i] -= minDelay;   // 保证所有延时均为非负
            delayTotal_2[i] -= minDelay; // 保证所有延时均为非负
            nbrCOMdelay[i] = floor(delayTotal[i] / timeCOM);
            nbrCOMdelay_2[i] = floor(delayTotal_2[i] / timeCOM);
            if (nbrCOMdelay[i] > maxNbrCOMdelay)
                maxNbrCOMdelay = nbrCOMdelay[i];
            if (nbrCOMdelay_2[i] > maxNbrCOMdelay)
                maxNbrCOMdelay = nbrCOMdelay_2[i];
            delayInCOM[i] = int(20 * 1000.0 * (delayTotal[i] - timeCOM * nbrCOMdelay[i]));
            delayInCOM_2[i] = int(20 * 1000.0 * (delayTotal_2[i] - timeCOM * nbrCOMdelay_2[i]));
            if (ui->stackCoin->currentIndex() == 0)
            {
                delayTotalAcc[i] -= minDelay;
                delayTotalAcc_2[i] -= minDelay;
                nbrCOMdelayAcc[i] = floor(delayTotalAcc[i] / timeCOM);
                nbrCOMdelayAcc_2[i] = floor(delayTotalAcc_2[i] / timeCOM);
                if (nbrCOMdelayAcc[i] > maxNbrCOMdelayAcc)
                    maxNbrCOMdelayAcc = nbrCOMdelayAcc[i];
                if (nbrCOMdelayAcc_2[i] > maxNbrCOMdelayAcc)
                    maxNbrCOMdelayAcc = nbrCOMdelayAcc_2[i];
                delayInCOMAcc[i] = int(20 * 1000.0 * (delayTotalAcc[i] - timeCOM * nbrCOMdelayAcc[i]));
                delayInCOMAcc_2[i] = int(20 * 1000.0 * (delayTotalAcc_2[i] - timeCOM * nbrCOMdelayAcc_2[i]));
            }
        }

        //    时间序列所需要保存的 COM 周期数量为 nbrCOMdelay 中的最大值 +2
        resizeSeqLength(&timeSeq, maxNbrCOMdelay + 2);
        resizeSeqLength(&channelSeq, maxNbrCOMdelay + 2);
        if (ui->stackCoin->currentIndex() == 0)
        {
            resizeSeqLength(&timeSeqAcc, maxNbrCOMdelayAcc + 2);
            resizeSeqLength(&channelSeqAcc, maxNbrCOMdelayAcc + 2);
        }
        COM_HEAD = 0;

        resizeSeqLength(&timeSeq_2, maxNbrCOMdelay + 2);
        resizeSeqLength(&channelSeq_2, maxNbrCOMdelay + 2);
        if (ui->stackCoin->currentIndex() == 0)
        {
            resizeSeqLength(&timeSeqAcc_2, maxNbrCOMdelayAcc + 2);
            resizeSeqLength(&channelSeqAcc_2, maxNbrCOMdelayAcc + 2);
        }
        COM_HEAD_2 = 0;

        resizeSeqLength(&timeSeqX1, 3 * (countEvents + maxNbrCOMdelay + 2));
        resizeSeqLength(&channelSeqX1, 3 * (countEvents + maxNbrCOMdelay + 2));
        resizeSeqLength(&timeSeqX2, 3 * (countEvents + maxNbrCOMdelay + 2));
        resizeSeqLength(&channelSeqX2, 3 * (countEvents + maxNbrCOMdelay + 2));
        nbrChannels_X = nbrChannels + nbrChannels_2;
        if (ui->stackCoin->currentIndex() == 0)
        {
            resizeSeqLength(&timeSeqAccX1, 3 * (countEvents + maxNbrCOMdelayAcc + 2));
            resizeSeqLength(&channelSeqAccX1, 3 * (countEvents + maxNbrCOMdelayAcc + 2));
            resizeSeqLength(&timeSeqAccX2, 3 * (countEvents + maxNbrCOMdelayAcc + 2));
            resizeSeqLength(&channelSeqAccX2, 3 * (countEvents + maxNbrCOMdelayAcc + 2));
        }
        if (COM_offset < 0)
        {
            COM_HEAD_X1 = (-COM_offset) % timeSeqX1.size();
            COM_HEAD_X2 = 0;
        }
        else
        {
            COM_HEAD_X1 = 0;
            COM_HEAD_X2 = COM_offset % timeSeqX2.size();
        }
        COM_START = COM_HEAD_X1;
        COM_START_2 = COM_HEAD_X2;
        COM_HEAD_compute = 0;

        emit askDealAcqBankSwitchCoin(index, computeMode);
    }
}

void CoincidenceWidget::dealAcqThreadBankSwitchCoin(AqT3DataDescriptor *dataDescPtr)
{
    switch (computeMode)
    {
    case 0:
        computeCoincidenceCount(dataDescPtr,
                                timeSeq, timeSeqAcc,
                                channelSeq, channelSeqAcc,
                                nbrChannels, channels, nbrCoinCalc,
                                toleranceCalc, &nbrAccCoin,
                                nbrCOMdelay, nbrCOMdelayAcc,
                                delayInCOM, delayInCOMAcc,
                                timeCOMunit, &COM_HEAD);
        break;
    case 1:
        // TDC 1 先存数据
        computeCoincidenceCountAcrossDevices_HOLD(dataDescPtr,
                                                  timeSeqX1, // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
                                                  timeSeqAccX1,
                                                  channelSeqX1, // 升序排列后的时间，与通道编号一一对应
                                                  channelSeqAccX1,
                                                  nbrChannels,
                                                  channels,
                                                  nbrCOMdelay, nbrCOMdelayAcc,
                                                  delayInCOM, delayInCOMAcc,
                                                  timeCOMunit,
                                                  &COM_HEAD_X1,
                                                  COM_START);
        break;
    case 2:
        break;
    default:
        break;
    }
}

void CoincidenceWidget::dealAcqThreadBankSwitchCoin_2(AqT3DataDescriptor *dataDescPtr_2)
{
    switch (computeMode)
    {
    case 0:
        break;
    case 1:
        // TDC 2 先存数据
        computeCoincidenceCountAcrossDevices_HOLD(dataDescPtr_2,
                                                  timeSeqX2, // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
                                                  timeSeqAccX2,
                                                  channelSeqX2, // 升序排列后的时间，与通道编号一一对应
                                                  channelSeqAccX2,
                                                  nbrChannels_2,
                                                  channels_2,
                                                  nbrCOMdelay_2, nbrCOMdelayAcc_2,
                                                  delayInCOM_2, delayInCOMAcc_2,
                                                  timeCOMunit,
                                                  &COM_HEAD_X2,
                                                  COM_START_2);

        // TDC 2 再计算数据
        computeCoincidenceCountAcrossDevices_COMPUTE(
            timeSeqX1, // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
            timeSeqAccX1,
            channelSeqX1, // 升序排列后的时间，与通道编号一一对应
            channelSeqAccX1,
            timeSeqX2, // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
            timeSeqAccX2,
            channelSeqX2, // 升序排列后的时间，与通道编号一一对应
            channelSeqAccX2,
            nbrChannels, nbrChannels_2,
            channels, channels_2,
            nbrCoinCalc,
            tolerance,
            &nbrAccCoin,
            &COM_HEAD_X1, &COM_HEAD_X2, &COM_HEAD_compute);
        break;
    case 2:
        computeCoincidenceCount(dataDescPtr_2,
                                timeSeq_2, timeSeqAcc_2,
                                channelSeq_2, channelSeqAcc_2,
                                nbrChannels_2, channels_2, nbrCoinCalc,
                                toleranceCalc, &nbrAccCoin,
                                nbrCOMdelay_2, nbrCOMdelayAcc_2,
                                delayInCOM_2, delayInCOMAcc_2,
                                timeCOMunit, &COM_HEAD_2);
        break;
    default:
        break;
    }
}

void CoincidenceWidget::dealTimeOut()
{
    if (ui->stackCoin->currentIndex() == 0) // 双通道模式
    {
        ui->lcdCoin->display(nbrCoin);
        ui->lcdAccCoin->display(nbrAccCoin);
        memset(&nbrCoin, 0, sizeof(nbrCoin));
        memset(&nbrAccCoin, 0, sizeof(nbrAccCoin));
    }
    else
    {
        ui->lcdCoin_Multi->display(nbrCoinMulti);
        memset(&nbrCoinMulti, 0, sizeof(nbrCoinMulti));
    }
}

bool CoincidenceWidget::getCoinParam(QString *coinChannelNamePtr, int **nbrCoinPtrPtr, int **nbrAccCoinPtrPtr)
{
    fetchUiData();
    if (ui->stackCoin->currentIndex() == 0)
    {
        *coinChannelNamePtr = QString::number(channel1) + " & " + QString::number(channel2 + 6);
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
                *coinChannelNamePtr += QString::number(i + 1) + "&";
            if (channelMark_2[i])
                *coinChannelNamePtr += QString::number(i + 1 + 6) + "&";
        }
        coinChannelNamePtr->remove(coinChannelNamePtr->length() - 1, 1);
        *nbrCoinPtrPtr = &nbrCoinMulti;
        return true;
    }
}

int *CoincidenceWidget::getCoinCountPtr()
{
    if (ui->stackCoin->currentIndex() == 0)
        return &nbrCoin;
    else
        return &nbrCoinMulti;
}

int *CoincidenceWidget::getAccCoinCountPtr()
{
    return &nbrAccCoin;
}

void CoincidenceWidget::changeComOffset(int newOffset)
{
    int offsetChange = newOffset - COM_offset;
    COM_offset = newOffset;
    if (offsetChange < 0)
    {
        if (timeSeqX1.size() == 0)
            return;
        COM_START = (COM_START - offsetChange) % timeSeqX1.size();
    }
    else
    {
        if (timeSeqX2.size() == 0)
            return;
        COM_START_2 = (COM_START_2 + offsetChange) % timeSeqX2.size();
    }
}
