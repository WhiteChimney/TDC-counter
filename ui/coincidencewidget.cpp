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

void CoincidenceWidget::fetchUiData()
{
//    双通道的数据
    channel1 = ui->coinChannel1->currentText().toInt();
    channel2 = ui->coinChannel2->currentText().toInt();
    delay = int(20*ui->delayCoin->text().toDouble());
    delayAcc = int(20*ui->delayAccCoin->text().toDouble());
    accumulateTime = ui->accumTimeCoin->text().toDouble();
    enableAccumulateTime = ui->checkboxAccumlateTime->isChecked();
    tolerance = int(20*ui->tolerance->text().toDouble());

//    多通道的数据
    channelMulti[0] = ui->checkBoxCN1->isChecked(); delayMulti[0] = int(20*ui->textDelayCN1->text().toDouble());
    channelMulti[1] = ui->checkBoxCN2->isChecked(); delayMulti[1] = int(20*ui->textDelayCN2->text().toDouble());
    channelMulti[2] = ui->checkBoxCN3->isChecked(); delayMulti[2] = int(20*ui->textDelayCN3->text().toDouble());
    channelMulti[3] = ui->checkBoxCN4->isChecked(); delayMulti[3] = int(20*ui->textDelayCN4->text().toDouble());
    channelMulti[4] = ui->checkBoxCN5->isChecked(); delayMulti[4] = int(20*ui->textDelayCN5->text().toDouble());
    channelMulti[5] = ui->checkBoxCN6->isChecked(); delayMulti[5] = int(20*ui->textDelayCN6->text().toDouble());
    accumulateTimeMulti = ui->accumTimeCoin_Multi->text().toDouble();
    enableAccumulateTimeMulti = ui->checkboxAccumlateTime_Multi->isChecked();
    toleranceMulti = int(20*ui->tolerance_Multi->text().toDouble());

    pushUiData();
}

void CoincidenceWidget::pushUiData()
{
//    双通道的数据
    ui->coinChannel1->setCurrentText(QString::number(channel1));
    ui->coinChannel2->setCurrentText(QString::number(channel2));
    ui->delayCoin->setText(QString::number(delay/20.0));
    ui->delayAccCoin->setText(QString::number(delayAcc/20.0));
    ui->checkboxAccumlateTime->setChecked(enableAccumulateTime);
    ui->accumTimeCoin->setText(QString::number(accumulateTime));
    ui->accumTimeCoin->setEnabled(!enableAccumulateTime);
    ui->tolerance->setText(QString::number(tolerance/20.0));

//    多通道的数据
    ui->checkBoxCN1->setChecked(channelMulti[0]); ui->textDelayCN1->setText(QString::number(delayMulti[0]/20.0));
    ui->checkBoxCN2->setChecked(channelMulti[1]); ui->textDelayCN2->setText(QString::number(delayMulti[1]/20.0));
    ui->checkBoxCN3->setChecked(channelMulti[2]); ui->textDelayCN3->setText(QString::number(delayMulti[2]/20.0));
    ui->checkBoxCN4->setChecked(channelMulti[3]); ui->textDelayCN4->setText(QString::number(delayMulti[3]/20.0));
    ui->checkBoxCN5->setChecked(channelMulti[4]); ui->textDelayCN5->setText(QString::number(delayMulti[4]/20.0));
    ui->checkBoxCN6->setChecked(channelMulti[5]); ui->textDelayCN6->setText(QString::number(delayMulti[5]/20.0));
    ui->checkboxAccumlateTime_Multi->setChecked(enableAccumulateTimeMulti);
    ui->accumTimeCoin_Multi->setText(QString::number(accumulateTimeMulti));
    ui->accumTimeCoin_Multi->setEnabled(!enableAccumulateTimeMulti);
    ui->tolerance_Multi->setText(QString::number(toleranceMulti/20.0));

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
        emit askDealAcqBankSwitchCoin(index);
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
            if (channelMulti[i])
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

        emit askDealAcqBankSwitchCoin(index);
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
}

void CoincidenceWidget::createTempDataFile()
{
    // 将配置写入临时文件
    fCoin->open(QIODevice::WriteOnly | QIODevice::Text);

    // 双通道
    if (ui->stackCoin->currentIndex()==0)
    {
        fStream << tr("符合计数") << index+1 << "\n"
                << tr("当前时间：") << QDateTime::currentDateTime().toString() << "\n"
                << tr("符合通道：Channel ") << channel1 << tr(" & ") << channel2 << "\n"
                << tr("符合门宽：") << tolerance/20.0 << tr("ns") << "\n"
                << tr("延时：") << delay/20.0 << tr("ns") << "\n"
                << tr("偶然符合额外延时：") << delayAcc/20.0 << tr("ns") << "\n";
        if (enableAccumulateTime)
        {
            fStream << tr("计时与单道计数同步") << "\n"
                    << "\n"
                    << "\n"
                    << tr("符合计数") << index+1 << "\n";
        }
        else
        {
            fStream << tr("计时未与单道计数同步") << "\n"
                    << tr("累计时间：") << accumulateTime << tr("s") << "\n"
                    << "\n"
                    << tr("时间/ms\t计数") << "\n";
        }
    }
    else
    {
        QString strChannel = "符合通道：Channel ";
        QString strDelay = "延时：";
        for (int i = 0; i < 6; i++)
        {
            if (channelMulti[i])
            {
                strChannel.append(QString::number(i+1));
                strChannel.append(" & ");
                strDelay.append(QString::number(delayMulti[i]/20.0));
                strDelay.append("ns, ");
            }
        }
        strChannel.remove(strChannel.length()-3,3);
        strDelay.remove(strDelay.length()-2,2);

        fStream << tr("符合计数") << index+1 << "\n"
                << tr("当前时间：") << QDateTime::currentDateTime().toString() << "\n"
                << tr("符合门宽：") << toleranceMulti/20.0 << tr("ns") << "\n"
                << strChannel << "\n"
                << strDelay << "\n";
        if (enableAccumulateTimeMulti)
        {
            fStream << tr("计时与单道计数同步") << "\n"
                    << "\n"
                    << "\n"
                    << "\n"
                    << tr("符合计数") << index+1 << "\n";
        }
        else
        {
            fStream << tr("计时未与单道计数同步") << "\n"
                    << tr("累计时间：") << accumulateTimeMulti << tr("s") << "\n"
                    << "\n"
                    << "\n"
                    << tr("时间/ms\t计数") << "\n";
        }
    }
    fCoin->close();
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

void CoincidenceWidget::dealSaveCoinData()
{
    if (coinSavable)
    {
        fCoin->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
        if (! ui->checkboxAccumlateTime->isChecked())
        {// 如果不与单道同步，则记录时间；否则不记录，之后合并
            timeToc = QDateTime::currentMSecsSinceEpoch();
            fStream << timeToc - timeTic << "\t";
        }
        if (ui->stackCoin->currentIndex()==0) // 双通道模式
            fStream << nbrCoin << "\n";
        else
            fStream << nbrCoinMulti << "\n";
        fCoin->close();
    }
}

void CoincidenceWidget::dealAcqThreadBankSwitchCoin(QVector<AqT3DataDescriptor*> dataPtrList)
{
    AqT3DataDescriptor *dataDescPtr = dataPtrList.last();
    if (ui->stackCoin->currentIndex()==0) // 双通道模式计算符合计数
    {
        int channels[2] = {channel1-1, channel2-1};
        int delayDual[2] = {0, delay};
        computeCoincidenceCount
            (dataPtrList, 2, channels, tolerance, &nbrCoin, &nbrAccCoin, delayAcc);
//        computeCoincidenceCount
//            (dataDescPtr, &nbrCoin, &nbrAccCoin, channel1, channel2, tolerance, delay, delayAcc);
    }
    else                                   // 多通道模式计算符合计数
    {
        int nbrChannels = 0;
        int channels[6] = {0};
        for (int i = 0; i < 6; i++)
        {
            if (channelMulti[i])
            {
                channels[nbrChannels] = i;
                nbrChannels++;
            }
        }
        computeCoincidenceCount(dataPtrList, nbrChannels, channels, tolerance, &nbrCoinMulti);
//        computeCoincidenceCountMulti(dataDescPtr, &nbrCoinMulti, channelMulti, toleranceMulti, delayMulti);
    }
}

void CoincidenceWidget::startRecordCoinLocal()
{
    disconnect(timerCoin,&QTimer::timeout,this,&CoincidenceWidget::dealTimeOut);
    connect(timerCoin,&QTimer::timeout,this,&CoincidenceWidget::dealSaveCoinData);
    connect(timerCoin,&QTimer::timeout,this,&CoincidenceWidget::dealTimeOut);
}

void CoincidenceWidget::stopRecordCoinLocal()
{
    disconnect(timerCoin,&QTimer::timeout,this,&CoincidenceWidget::dealSaveCoinData);
}

void CoincidenceWidget::on_coinChannel1_currentIndexChanged(int index)
{
    while (ui->coinChannel2->count())
        ui->coinChannel2->removeItem(0);
    for (int i = 1; i < 7; i++)
        if (i != index+1)
            ui->coinChannel2->addItem(QString::number(i));
}

void CoincidenceWidget::on_checkboxAccumlateTime_stateChanged(int checkState)
{
    if (checkState == Qt::Checked)
        ui->accumTimeCoin->setEnabled(false);
    else
        ui->accumTimeCoin->setEnabled(true);
}

void CoincidenceWidget::on_checkboxAccumlateTime_Multi_stateChanged(int checkState)
{
    if (checkState == Qt::Checked)
        ui->accumTimeCoin_Multi->setEnabled(false);
    else
        ui->accumTimeCoin_Multi->setEnabled(true);
}

void CoincidenceWidget::saveToIni()
{
    fetchUiData();

    QSettings *configIni = new QSettings(iniName, QSettings::IniFormat);
    configIni->setValue("符合计数配置/currentPage",ui->stackCoin->currentIndex());

    // 双通道
    configIni->setValue("符合计数配置/channel1",channel1);
    configIni->setValue("符合计数配置/channel2",channel2);
    configIni->setValue("符合计数配置/delay",delay);
    configIni->setValue("符合计数配置/delayAcc",delayAcc);
    configIni->setValue("符合计数配置/accumulateTime",accumulateTime);
    configIni->setValue("符合计数配置/enableAccumulateTime",enableAccumulateTime);
    configIni->setValue("符合计数配置/tolerance",tolerance);

    // 多通道
    configIni->setValue("符合计数配置/channelMulti1",channelMulti[0]); configIni->setValue("符合计数配置/delayMulti1",delayMulti[0]);
    configIni->setValue("符合计数配置/channelMulti2",channelMulti[1]); configIni->setValue("符合计数配置/delayMulti2",delayMulti[1]);
    configIni->setValue("符合计数配置/channelMulti3",channelMulti[2]); configIni->setValue("符合计数配置/delayMulti3",delayMulti[2]);
    configIni->setValue("符合计数配置/channelMulti4",channelMulti[3]); configIni->setValue("符合计数配置/delayMulti4",delayMulti[3]);
    configIni->setValue("符合计数配置/channelMulti5",channelMulti[4]); configIni->setValue("符合计数配置/delayMulti5",delayMulti[4]);
    configIni->setValue("符合计数配置/channelMulti6",channelMulti[5]); configIni->setValue("符合计数配置/delayMulti6",delayMulti[5]);
    configIni->setValue("符合计数配置/accumulateTimeMulti",accumulateTimeMulti);
    configIni->setValue("符合计数配置/enableAccumulateTimeMulti",enableAccumulateTimeMulti);
    configIni->setValue("符合计数配置/toleranceMulti",toleranceMulti);

    delete configIni;
}

void CoincidenceWidget::loadFromIni()
{
    QSettings *configIni = new QSettings(iniName, QSettings::IniFormat);
    ui->stackCoin->setCurrentIndex(configIni->value("符合计数配置/currentPage").toInt());

    // 双通道
    channel1 = configIni->value("符合计数配置/channel1").toInt();
    channel2 = configIni->value("符合计数配置/channel2").toInt();
    delay = configIni->value("符合计数配置/delay").toInt();
    delayAcc = configIni->value("符合计数配置/delayAcc").toInt();
    accumulateTime = configIni->value("符合计数配置/accumulateTime").toDouble();
    enableAccumulateTime = configIni->value("符合计数配置/enableAccumulateTime").toBool();
    tolerance = configIni->value("符合计数配置/tolerance").toInt();

    // 多通道
    channelMulti[0] = configIni->value("符合计数配置/channelMulti1").toBool(); delayMulti[0] = configIni->value("符合计数配置/delayMulti1").toInt();
    channelMulti[1] = configIni->value("符合计数配置/channelMulti2").toBool(); delayMulti[1] = configIni->value("符合计数配置/delayMulti2").toInt();
    channelMulti[2] = configIni->value("符合计数配置/channelMulti3").toBool(); delayMulti[2] = configIni->value("符合计数配置/delayMulti3").toInt();
    channelMulti[3] = configIni->value("符合计数配置/channelMulti4").toBool(); delayMulti[3] = configIni->value("符合计数配置/delayMulti4").toInt();
    channelMulti[4] = configIni->value("符合计数配置/channelMulti5").toBool(); delayMulti[4] = configIni->value("符合计数配置/delayMulti5").toInt();
    channelMulti[5] = configIni->value("符合计数配置/channelMulti6").toBool(); delayMulti[5] = configIni->value("符合计数配置/delayMulti6").toInt();
    accumulateTimeMulti = configIni->value("符合计数配置/accumulateTimeMulti").toDouble();
    enableAccumulateTimeMulti = configIni->value("符合计数配置/enableAccumulateTimeMulti").toBool();
    toleranceMulti = configIni->value("符合计数配置/toleranceMulti").toInt();
    delete configIni;

    pushUiData();
}

void CoincidenceWidget::on_buttonChangeToMulti_released()
{
    ui->stackCoin->setCurrentIndex(1);
}


void CoincidenceWidget::on_buttonChangeToDual_released()
{
    ui->stackCoin->setCurrentIndex(0);
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
            if (channelMulti[i])
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
