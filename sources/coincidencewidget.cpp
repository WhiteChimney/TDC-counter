#include "coincidencewidget.h"
#include "ui_coincidencewidget.h"

CoincidenceWidget::CoincidenceWidget(QWidget *parent, int index0) :
    QWidget(parent),
    ui(new Ui::CoincidenceWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);        // 在父窗口上显示独立的子窗口
    index = index0;                    // 标记当前符合窗口

    // 时钟
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
    // 读取符合配置
    channel1 = ui->coinChannel1->currentText().toInt();
    channel2 = ui->coinChannel2->currentText().toInt();
    tolerance = int(20*ui->tolerance->text().toDouble());
    delay = int(20*ui->delayCoin->text().toDouble());
    delayAcc = int(20*ui->delayAccCoin->text().toDouble());
    emit askDealAcqBankSwitchCoin(index);
    if (ui->checkboxAccumlateTime->isChecked())
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

    // 将配置写入临时文件
    fCoin->open(QIODevice::WriteOnly | QIODevice::Text);
    fStream << "符合计数" << index << endl
            << "当前时间：" << QDateTime::currentDateTime().toString() << endl
            << "符合通道：Channel" << channel1 << "与Channel" << channel2 << endl
            << "符合门宽：" << tolerance/20.0 << "ns" << endl
            << "延时：" << delay/20.0 << "ns" << endl
            << "偶然符合额外延时：" << delayAcc/20.0 << "ns" << endl;
    if (ui->checkboxAccumlateTime->isChecked())
    {
        fStream << "计时与单道计数同步" << endl
                << endl
                << endl
                << "符合计数" << index << endl;
    }
    else
    {
        fStream << "计时未与单道计数同步" << endl
                << "累计时间：" << accumulateTime << endl
                << endl
                << "时间/ms\t计数" << endl;
    }
    fCoin->close();
    coinSavable = true; // 此时可保存数据
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
        {
            timeToc = QDateTime::currentMSecsSinceEpoch();
            fStream << timeToc - timeTic << "\t";
        }
        fStream << nbrCoin << endl;
        fCoin->close();
    }
}

void CoincidenceWidget::dealAcqThreadBankSwitchCoin(AqT3DataDescriptor* dataDescPtr)
{
    algorithmCoincidenceCount
        (dataDescPtr, &nbrCoin, &nbrAccCoin, channel1, channel2, tolerance, delay, delayAcc);
}

void CoincidenceWidget::on_buttonStop_released()
{
    coinSavable = false;
    emit askStopDealAcqBankSwitchCoin(index);
    if (! ui->checkboxAccumlateTime->isChecked())
        timerCoin->stop();
}

void CoincidenceWidget::startRecordCoinLocal()
{
    connect(timerCoin,&QTimer::timeout,this,&CoincidenceWidget::dealSaveCoinData);
}

void CoincidenceWidget::stopRecordCoinLocal()
{
    disconnect(timerCoin,&QTimer::timeout,this,&CoincidenceWidget::dealSaveCoinData);
}

void CoincidenceWidget::closeRecordFile()
{
    fCoin->close();
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
    channel1 = ui->coinChannel1->currentText().toInt();
    channel2 = ui->coinChannel2->currentText().toInt();
    delay = int(20*ui->delayCoin->text().toDouble());
    delayAcc = int(20*ui->delayAccCoin->text().toDouble());
    accumulateTime = ui->accumTimeCoin->text().toDouble();
    enableAccumulateTime = ui->checkboxAccumlateTime->isChecked();
    tolerance = int(20*ui->tolerance->text().toDouble());

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

    ui->coinChannel1->setCurrentText(QString::number(channel1));
    ui->coinChannel2->setCurrentText(QString::number(channel2));
    ui->delayCoin->setText(QString::number(delay/20.0));
    ui->delayAccCoin->setText(QString::number(delayAcc/20.0));
    ui->checkboxAccumlateTime->setChecked(enableAccumulateTime);
    ui->accumTimeCoin->setText(QString::number(accumulateTime));
    ui->accumTimeCoin->setEnabled(!enableAccumulateTime);
    ui->tolerance->setText(QString::number(tolerance/20.0));
}

