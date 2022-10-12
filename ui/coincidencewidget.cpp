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
    channelMulti[0] = ui->checkBoxCN1->isEnabled(); delayMulti[0] = int(20*ui->textDelayCN1->text().toDouble());
    channelMulti[1] = ui->checkBoxCN2->isEnabled(); delayMulti[1] = int(20*ui->textDelayCN2->text().toDouble());
    channelMulti[2] = ui->checkBoxCN3->isEnabled(); delayMulti[2] = int(20*ui->textDelayCN3->text().toDouble());
    channelMulti[3] = ui->checkBoxCN4->isEnabled(); delayMulti[3] = int(20*ui->textDelayCN4->text().toDouble());
    channelMulti[4] = ui->checkBoxCN5->isEnabled(); delayMulti[4] = int(20*ui->textDelayCN5->text().toDouble());
    channelMulti[5] = ui->checkBoxCN6->isEnabled(); delayMulti[5] = int(20*ui->textDelayCN6->text().toDouble());
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
    ui->checkBoxCN1->setEnabled(channelMulti[0]); ui->textDelayCN1->setText(QString::number(delayMulti[0]/20.0));
    ui->checkBoxCN2->setEnabled(channelMulti[1]); ui->textDelayCN2->setText(QString::number(delayMulti[1]/20.0));
    ui->checkBoxCN3->setEnabled(channelMulti[2]); ui->textDelayCN3->setText(QString::number(delayMulti[2]/20.0));
    ui->checkBoxCN4->setEnabled(channelMulti[3]); ui->textDelayCN4->setText(QString::number(delayMulti[3]/20.0));
    ui->checkBoxCN5->setEnabled(channelMulti[4]); ui->textDelayCN5->setText(QString::number(delayMulti[4]/20.0));
    ui->checkBoxCN6->setEnabled(channelMulti[5]); ui->textDelayCN6->setText(QString::number(delayMulti[5]/20.0));
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
    emit askDealAcqBankSwitchCoin(index);
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
    coinSavable = true; // 此时可保存数据
}

void CoincidenceWidget::on_buttonStop_released()
{
    coinSavable = false;
    emit askStopDealAcqBankSwitchCoin(index);
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

void CoincidenceWidget::createTempDataFile()
{
    // 将配置写入临时文件
    fCoin->open(QIODevice::WriteOnly | QIODevice::Text);
    fStream << tr("符合计数") << index+1 << endl
            << tr("当前时间：") << QDateTime::currentDateTime().toString() << endl
            << tr("符合通道：Channel") << channel1 << tr("与Channel") << channel2 << endl
            << tr("符合门宽：") << tolerance/20.0 << tr("ns") << endl
            << tr("延时：") << delay/20.0 << tr("ns") << endl
            << tr("偶然符合额外延时：") << delayAcc/20.0 << tr("ns") << endl;
    if (enableAccumulateTime)
    {
        fStream << tr("计时与单道计数同步") << endl
                << endl
                << endl
                << tr("符合计数") << index+1 << endl;
    }
    else
    {
        fStream << tr("计时未与单道计数同步") << endl
                << tr("累计时间：") << accumulateTime << endl
                << endl
                << tr("时间/ms\t计数") << endl;
    }
    fCoin->close();
}

void CoincidenceWidget::dealTimeOut()
{
    ui->lcdCoin->display(nbrCoin);
    ui->lcdAccCoin->display(nbrAccCoin);
    memset(&nbrCoin,0,sizeof(nbrCoin));
    memset(&nbrAccCoin,0,sizeof(nbrAccCoin));
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
        fStream << nbrCoin << endl;
        fCoin->close();
    }
}

void CoincidenceWidget::dealAcqThreadBankSwitchCoin(AqT3DataDescriptor* dataDescPtr)
{
    computeCoincidenceCount
        (dataDescPtr, &nbrCoin, &nbrAccCoin, channel1, channel2, tolerance, delay, delayAcc);
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

void CoincidenceWidget::saveToIni()
{
    fetchUiData();

    QSettings *configIni = new QSettings(iniName, QSettings::IniFormat);
    configIni->setValue("符合计数配置/channel1",channel1);
    configIni->setValue("符合计数配置/channel2",channel2);
    configIni->setValue("符合计数配置/delay",delay);
    configIni->setValue("符合计数配置/delayAcc",delayAcc);
    configIni->setValue("符合计数配置/accumulateTime",accumulateTime);
    configIni->setValue("符合计数配置/enableAccumulateTime",enableAccumulateTime);
    configIni->setValue("符合计数配置/tolerance",tolerance);
    delete configIni;
}

void CoincidenceWidget::loadFromIni()
{
    QSettings *configIni = new QSettings(iniName, QSettings::IniFormat);
    channel1 = configIni->value("符合计数配置/channel1").toInt();
    channel2 = configIni->value("符合计数配置/channel2").toInt();
    delay = configIni->value("符合计数配置/delay").toInt();
    delayAcc = configIni->value("符合计数配置/delayAcc").toInt();
    accumulateTime = configIni->value("符合计数配置/accumulateTime").toDouble();
    enableAccumulateTime = configIni->value("符合计数配置/enableAccumulateTime").toBool();
    tolerance = configIni->value("符合计数配置/tolerance").toInt();
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

void CoincidenceWidget::getCoinParam(QString* coinChannelName, int **nbrCoinPtr, int **nbrAccCoinPtr)
{
    fetchUiData();
    *coinChannelName = "Channel" + QString::number(channel1) + "&" + QString::number(channel2);
    **nbrCoinPtr = nbrCoin;
    **nbrAccCoinPtr = nbrAccCoin;
}

//void CoincidenceWidget::getCoinParam(QString* coinChannelName, int **nbrCoinPtr)
//{
//    fetchUiData();
//    *coinChannelName = "Channel" + QString::number(channel1) + "&" + QString::number(channel2);
//    **nbrCoinPtr = nbrCoin;
//}
