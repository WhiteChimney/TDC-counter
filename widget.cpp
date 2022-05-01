#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Acqiris TDC 计数程序");

//    清空临时文件
    QDir tempDataPath(iniPath + "/Data");
    tempDataPath.removeRecursively();

//    检查是否存在配置文件（ini）
    QFileInfo iniInfo(iniName);
    if (iniInfo.isFile())
        loadFromIni();   // 存在配置则加载
    else
    {
        ui->textDataDirectory->setCurrentText(
                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
        saveToIni();     // 否则保存当前配置
    }
    fSingleCount->setFileName(tempFileName_SCC);
    fStream.setDevice(fSingleCount);

//     初始化设备
    status = Acqrs_InitWithOptions((ViRsrc)"PCI::INSTR0", VI_FALSE,
            VI_FALSE, "CAL=0", &idInstr);

    if (status != VI_SUCCESS)
    {
        QMessageBox::critical(this,
                              QString("警告"),
                              QString("未发现可操控设备"),
                              QMessageBox::Ok);
    }

//    数据采集线程
    acqThread = new AcquisitionThread();
    connect(acqThread,&AcquisitionThread::acqThreadStarted,this,&Widget::dealAcqThreadStarted);
    connect(this,&Widget::acqParamReady,acqThread,&AcquisitionThread::dealAcqParamReady);
    connect(acqThread,&AcquisitionThread::acqThreadFinished,this,&Widget::dealAcqThreadFinished);
    connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,this,&Widget::dealAcqThreadBankSwitch);

//    单道计数时钟，主时钟
    timerCount = new QTimer(this);
    connect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);

//    数据保存时的倒计时时钟
    timerFile = new QTimer(this);
    connect(timerFile,&QTimer::timeout,this,&Widget::dealRecordTimeOut);

    qRegisterMetaType<AqT3DataDescriptor>("AqT3DataDescriptor");

}

Widget::~Widget()
{
    on_buttonExit_released();
    saveToIni();
    delete ui;
}

void Widget::on_buttonTempFilePath_released()
// 打开临时文件目录
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(iniPath));
}

void Widget::on_buttonExit_released()
{
    on_buttonStopRecord_released();
    on_buttonStopCount_released();
    on_buttonStopAcq_released();
    // Stops the acquisition & close instruments
    Acqrs_closeAll();
    this->close();
}

void Widget::on_buttonTest_released()
// 生成测试子 widget
{
    int index = vTestWidget.count();
    testW = new TestWidget(this,index);
    vTestWidget.append(testW);
    connect(testW,&TestWidget::returnSignal,this,&Widget::dealTestReturn);
    testW->setWindowTitle("测试"+QString::number(index+1));
    testW->setWindowState(Qt::WindowActive);
    testW->show();
}

void Widget::dealTestReturn(int index)
// 关闭测试子widget
{
    testW = vTestWidget.at(index);
    testW->setWindowState(Qt::WindowNoState);
    testW->close();
}

void Widget::on_buttonApplyConfig_released()
// 应用 TDC 配置
{
    // config TDC
    freqCOM = int(1000*ui->textFreqCOM->text().toDouble());
    if (ui->checkboxEnableCountEvents->isChecked())
        countEvents = int(1000*ui->textCountEvents->text().toDouble());
    else
    {
        countEvents = freqCOM / 10;
        ui->textCountEvents->setText(QString::number(countEvents/1000));
    }
    channelConfig[0] = true;
    channelConfig[1] = ui->checkBoxCN1->isChecked();
    channelConfig[2] = ui->checkBoxCN2->isChecked();
    channelConfig[3] = ui->checkBoxCN3->isChecked();
    channelConfig[4] = ui->checkBoxCN4->isChecked();
    channelConfig[5] = ui->checkBoxCN5->isChecked();
    channelConfig[6] = ui->checkBoxCN6->isChecked();
    level[0] = ui->levelTextCNCOM->text().toDouble();
    level[1] = ui->levelTextCN1->text().toDouble();
    level[2] = ui->levelTextCN2->text().toDouble();
    level[3] = ui->levelTextCN3->text().toDouble();
    level[4] = ui->levelTextCN4->text().toDouble();
    level[5] = ui->levelTextCN5->text().toDouble();
    level[6] = ui->levelTextCN6->text().toDouble();
    slope[0] = ui->slopeCNCOM->currentIndex();
    slope[1] = ui->slopeCN1->currentIndex();
    slope[2] = ui->slopeCN2->currentIndex();
    slope[3] = ui->slopeCN3->currentIndex();
    slope[4] = ui->slopeCN4->currentIndex();
    slope[5] = ui->slopeCN5->currentIndex();
    slope[6] = ui->slopeCN6->currentIndex();
    configStatus = configTDC(idInstr, countEvents, channelConfig, level, slope, readParamPtr);
}

void Widget::on_checkboxEnableCountEvents_released()
// 配置 TDC 时，未勾选手动设置 Count Events 会将输入框变灰
{
    if (ui->checkboxEnableCountEvents->isChecked())
        ui->textCountEvents->setEnabled(true);
    else
        ui->textCountEvents->setEnabled(false);
}

void Widget::on_buttonStartAcquisition_released()
// 开始采集
{
    if (configStatus != VI_SUCCESS)
        on_buttonApplyConfig_released();

    // start Acquisition
    if (configStatus == VI_SUCCESS)
        acqThread->start();
}

void Widget::dealAcqThreadStarted()
{
    *acqStopPtr = false;
//    TOFDataSavable = true;
    if (configStatus == VI_SUCCESS)
        emit acqParamReady
            (acqStopPtr,idInstr,readParamPtr);  // 收到采集线程的开启信号后，将参数传送过去
}

void Widget::dealAcqThreadBankSwitch(AqT3DataDescriptor* dataDescPtr)
// 当发生 Bank 切换时，可以处理数据
{
    ui->labelNumSamples->setText(QString::number(dataDescPtr->nbrSamples));
    long sample = ((long*)dataDescPtr->dataPtr)[0];
    ui->labelSample->setText(QString::number(sample,16));
}

void Widget::on_buttonStopAcq_released()
// 停止采集线程
{
    *acqStopPtr = true;
    configStatus = -1;
}

void Widget::dealAcqThreadFinished()
// 采集线程关闭后续
{
    AcqrsT3_waitForEndOfAcquisition(idInstr, 8000);
    AcqrsT3_stopAcquisition(idInstr);
    acqThread->quit();
    acqThread->wait();
}

void Widget::on_buttonStartCount_released()
// 开启计数功能
{
    if (! *acqStopPtr) // 当采集已经开启时
    {
        // 开始计算单道计数
        connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,this,&Widget::dealAcqThreadBankSwitchSCC);
        // 启动定时器
        accumulateTime = ui->accumulateTimeText->text().toDouble();
        timerCount->start(1000.0*accumulateTime);
        // 此时单道计数可被记录，将元数据写入临时文件
        fSingleCount->open(QIODevice::WriteOnly | QIODevice::Text);
        fStream << "单道计数" << endl
                << "当前时间：" << dateTime.currentDateTime().toString() << endl
                << "计数累计时间：" << accumulateTime << endl
                << endl
                << "时间/ms\tChannel 1\tChannel 2\tChannel 3\tChannel 4\tChannel 5\tChannel 6" << endl;
        fSingleCount->close();
    }
}

void Widget::dealAcqThreadBankSwitchSCC(AqT3DataDescriptor* dataDescPtr)
// 计算单道计数
{
    algorithmSingleChannelCount(dataDescPtr,nbrSCC);
}

void Widget::dealCountTimeOut()
// 定时刷新单道计数
{
    ui->lcdSPC1->display(nbrSCC[0]);
    ui->lcdSPC2->display(nbrSCC[1]);
    ui->lcdSPC3->display(nbrSCC[2]);
    ui->lcdSPC4->display(nbrSCC[3]);
    ui->lcdSPC5->display(nbrSCC[4]);
    ui->lcdSPC6->display(nbrSCC[5]);
    memset(nbrSCC, 0, 6*sizeof(nbrSCC[0]));
}

void Widget::on_buttonStopCount_released()
// 停止单道计数与计时
{
    timerCount->stop();
    disconnect(acqThread,&AcquisitionThread::acqThreadBankSwitch,this,&Widget::dealAcqThreadBankSwitchSCC);
}

void Widget::on_buttonCoincidence_released()
// 开启符合子窗口
{
    int index = vCoinWidget.count();
    coinW = new CoincidenceWidget(this,index);
    vCoinWidget.append(coinW);
    connect(coinW,&CoincidenceWidget::returnSignal,this,&Widget::dealCoincidenceReturn);
    connect(coinW,&CoincidenceWidget::askDealAcqBankSwitchCoin,this,&Widget::dealAskDealAcqBankSwitchCoin);
    connect(coinW,&CoincidenceWidget::askStopDealAcqBankSwitchCoin,this,&Widget::dealAskStopDealAcqBankSwitchCoin);
    connect(coinW,&CoincidenceWidget::coinTimerNeedsSync,this,&Widget::dealCoinTimerNeedsSync);
    coinW->setWindowTitle("符合计数"+QString::number(index+1));
    coinW->setWindowState(Qt::WindowActive); // 设置窗口活动状态，以便后续处理，如保存数据
    coinW->show();
}

void Widget::dealCoincidenceReturn(int index)
// 关闭符合子窗口
{
    coinW = vCoinWidget.at(index);
    coinW->setWindowState(Qt::WindowNoState);
    disconnect(coinW,&CoincidenceWidget::returnSignal,this,&Widget::dealCoincidenceReturn);
    disconnect(coinW,&CoincidenceWidget::askDealAcqBankSwitchCoin,this,&Widget::dealAskDealAcqBankSwitchCoin);
    disconnect(coinW,&CoincidenceWidget::askStopDealAcqBankSwitchCoin,this,&Widget::dealAskStopDealAcqBankSwitchCoin);
    disconnect(coinW,&CoincidenceWidget::coinTimerNeedsSync,this,&Widget::dealCoinTimerNeedsSync);
    coinW->close();
}

void Widget::dealAskDealAcqBankSwitchCoin(int index)
// 将 Bank 切换信号与符合子窗口的槽对接
{
    coinW = vCoinWidget.at(index);
    connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,coinW,&CoincidenceWidget::dealAcqThreadBankSwitchCoin);
}

void Widget::dealAskStopDealAcqBankSwitchCoin(int index)
// 停止对接 Bank 切换信号与符合子窗口的槽
{
    coinW = vCoinWidget.at(index);
    disconnect(acqThread,&AcquisitionThread::acqThreadBankSwitch,coinW,&CoincidenceWidget::dealAcqThreadBankSwitchCoin);
}

void Widget::dealCoinTimerNeedsSync(int index)
// 将符合子窗口的定时转为单道计数主时钟，以同步
{
    coinW = vCoinWidget.at(index);
    connect(timerCount,&QTimer::timeout,coinW,&CoincidenceWidget::dealTimeOut); // 同步记录数据
}

void Widget::on_buttonHistogram_released()
// 开启直方图子窗口
{
    int index = vHistWidget.count();
    histW = new HistogramWidget(this, index);
    vHistWidget.append(histW);
    connect(histW,&HistogramWidget::returnSignal,this,&Widget::dealHistogramReturn);
    connect(histW,&HistogramWidget::askDealAcqBankSwitchHist,this,&Widget::dealAskDealAcqBankSwitchHist);
    connect(histW,&HistogramWidget::askStopDealAcqBankSwitchHist,this,&Widget::dealAskStopDealAcqBankSwitchHist);
    histW->setWindowTitle("直方图"+QString::number(index+1));
    histW->setWindowState(Qt::WindowActive);
    histW->show();
}

void Widget::dealHistogramReturn(int index)
// 关闭直方图子窗口
{
    histW = vHistWidget.at(index);
    histW->setWindowState(Qt::WindowNoState);
    histW->close();
}

void Widget::dealAskDealAcqBankSwitchHist(int index)
// 对接 Bank 切换与直方图槽函数
{
    histW = vHistWidget.at(index);
    connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,histW,&HistogramWidget::dealAcqThreadBankSwitchHist);
}

void Widget::dealAskStopDealAcqBankSwitchHist(int index)
// 停止将数据传入直方图
{
    histW = vHistWidget.at(index);
    disconnect(acqThread,&AcquisitionThread::acqThreadBankSwitch,histW,&HistogramWidget::dealAcqThreadBankSwitchHist);
}

void Widget::on_buttonOpenDataDir_released()
{

    QDesktopServices::openUrl(QUrl::fromLocalFile(iniPath));
}

void Widget::on_buttonDataDirectory_released()
// 保存文件时路径选择
{
    pathName = QFileDialog::getExistingDirectory
                        (this,tr("Open Directory"),
                         QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                         QFileDialog::ShowDirsOnly
                         | QFileDialog::DontResolveSymlinks);
    ui->textDataDirectory->setCurrentText(pathName);
    ui->textDataDirectory->addItem(pathName);
}

void Widget::on_buttonStartRecord_released()
// 保存数据
{
    // 如果选择记录单道
    if (ui->checkboxSCC->isChecked())
    {
        if (timerCount->isActive()) // 如果单道计数打开了，才记录
        {
            if (ui->checkboxCC->isChecked()) // 如果符合计数也打开了，进入同步时钟记录模式
                modeRecord = SingleAndCoinCount;
            else                              // 否则仅记录单道
                modeRecord = SingleCountOnly;
        }
    }
    else if (ui->checkboxCC->isChecked()) // 如果单道没勾选，但勾选了符合，那么记录符合，无需同步时钟
    {
        modeRecord = CoinCountOnly;
    }

    switch (modeRecord)
    {
    case SingleAndCoinCount:
        timeTic = dateTime.currentMSecsSinceEpoch();
        fSingleCount->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
        disconnect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);
        connect(timerCount,&QTimer::timeout,this,&Widget::dealRecordSingleCount); // 先记录数据，再更新 UI 数据，防止数据被清空
        connect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);
        for (int i = 0; i < vCoinWidget.count(); i++)
        {
            coinW = vCoinWidget.at(i);
            if (coinW->windowState() != Qt::WindowActive)
                continue;
            disconnect(timerCount,&QTimer::timeout,coinW,&CoincidenceWidget::dealTimeOut);
            connect(timerCount,&QTimer::timeout,coinW,&CoincidenceWidget::dealSaveCoinData);
            connect(timerCount,&QTimer::timeout,coinW,&CoincidenceWidget::dealTimeOut);
        }
        break;
    case SingleCountOnly:
        timeTic = dateTime.currentMSecsSinceEpoch();
        fSingleCount->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
        disconnect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);
        connect(timerCount,&QTimer::timeout,this,&Widget::dealRecordSingleCount);
        connect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);
        break;
    case CoinCountOnly:
        for (int i = 0; i < vCoinWidget.count(); i++)
        {
            coinW = vCoinWidget.at(i);
            if (coinW->windowState() != Qt::WindowActive)
                continue;
            coinW->startRecordCoinLocal();
        }
        break;
    default:
        return;
        break;
    }

    enableRecordTime = ui->checkboxRecordTime->isChecked();
    if (enableRecordTime)
        timerFile->start(1000.0*ui->textRecordTime->text().toDouble());
}

void Widget::dealRecordSingleCount()
// 记录单道计数
{
    fSingleCount->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
    timeToc = dateTime.currentMSecsSinceEpoch();
    timeRelative = timeToc - timeTic;
    fStream << timeRelative;
    for (int i = 0; i < 6; i++)
    {
        fStream << "\t" << nbrSCC[i];
    }
    fStream << endl;
    fSingleCount->close();
}

void Widget::on_buttonStopRecord_released()
// 停止记录单道计数
{
    switch (modeRecord)
    {
    case SingleAndCoinCount:
        disconnect(timerCount,&QTimer::timeout,this,&Widget::dealRecordSingleCount);
        fSingleCount->close();
        for (int i = 0; i < vCoinWidget.count(); i++)
        {
            coinW = vCoinWidget.at(i);
            if (coinW->windowState() != Qt::WindowActive)
                continue;
            disconnect(timerCount,&QTimer::timeout,coinW,&CoincidenceWidget::dealSaveCoinData);
            coinW->closeRecordFile();
        }
        break;
    case SingleCountOnly:
        disconnect(timerCount,&QTimer::timeout,this,&Widget::dealRecordSingleCount);
        fSingleCount->close();
        break;
    case CoinCountOnly:
        for (int i = 0; i < vCoinWidget.count(); i++)
        {
            coinW = vCoinWidget.at(i);
            if (coinW->windowState() != Qt::WindowActive)
                continue;
            coinW->stopRecordCoinLocal();
            coinW->closeRecordFile();
        }
        break;
    default:
        return;
        break;
    }

//    合并数据文件，并保存至指定位置
    pathName = ui->textDataDirectory->currentText();
    fileName = pathName + "/" + ui->textDataFileName->text() +
                "." + ui->comboDataFileSuffix->currentText();
    QTextDocument* document = ui->textMetaData->document();
    QTextBlock textBlock = document->begin();
    QString metaData = "";
    while (textBlock != document->end())
    {
        metaData += textBlock.text();
        textBlock = textBlock.next();
    }
    metaData += "\r\n\r\n";

    mergeDataFiles(modeRecord, fileName, metaData, vCoinWidget);

    modeRecord = notRecording;

    QDesktopServices::openUrl(QUrl::fromLocalFile(pathName));
    if (ui->checkboxRecordTime->isChecked())
    {
        timerFile->stop();
    }
}

void Widget::dealRecordTimeOut()
// 数据记录倒计时结束
{
    on_buttonStopRecord_released();
}

void Widget::on_checkboxRecordTime_released()
// 当未选择记录时长时，不可更改记录时间
{
    if (ui->checkboxRecordTime->isChecked())
        ui->textRecordTime->setEnabled(true);
    else
        ui->textRecordTime->setEnabled(false);
}

void Widget::saveToIni()
// 保存当前配置到配置文件
{
    freqCOM = int(1000*ui->textFreqCOM->text().toDouble());
    enableCountEvents = ui->checkboxEnableCountEvents->isChecked();
    if (enableCountEvents)
        countEvents = int(1000*ui->textCountEvents->text().toDouble());
    else
    {
        countEvents = freqCOM / 10;
        ui->textCountEvents->setText(QString::number(countEvents/1000));
    }
    channelConfig[0] = true;
    channelConfig[1] = ui->checkBoxCN1->isChecked();
    channelConfig[2] = ui->checkBoxCN2->isChecked();
    channelConfig[3] = ui->checkBoxCN3->isChecked();
    channelConfig[4] = ui->checkBoxCN4->isChecked();
    channelConfig[5] = ui->checkBoxCN5->isChecked();
    channelConfig[6] = ui->checkBoxCN6->isChecked();
    level[0] = ui->levelTextCNCOM->text().toDouble();
    level[1] = ui->levelTextCN1->text().toDouble();
    level[2] = ui->levelTextCN2->text().toDouble();
    level[3] = ui->levelTextCN3->text().toDouble();
    level[4] = ui->levelTextCN4->text().toDouble();
    level[5] = ui->levelTextCN5->text().toDouble();
    level[6] = ui->levelTextCN6->text().toDouble();
    slope[0] = ui->slopeCNCOM->currentIndex();
    slope[1] = ui->slopeCN1->currentIndex();
    slope[2] = ui->slopeCN2->currentIndex();
    slope[3] = ui->slopeCN3->currentIndex();
    slope[4] = ui->slopeCN4->currentIndex();
    slope[5] = ui->slopeCN5->currentIndex();
    slope[6] = ui->slopeCN6->currentIndex();

    accumulateTime = ui->accumulateTimeText->text().toDouble();

    pathName = ui->textDataDirectory->currentText();
    fileName = ui->textDataFileName->text();
    QTextDocument* document = ui->textMetaData->document();
    QTextBlock textBlock = document->begin();
    QString metaData = "";
    while (textBlock != document->end())
    {
        metaData += textBlock.text();
        metaData += "\r\n";
        textBlock = textBlock.next();
    }
    enableRecordTime = ui->checkboxRecordTime->isChecked();
    recordTime = ui->textRecordTime->text().toDouble();

    QSettings *configIni = new QSettings(iniName, QSettings::IniFormat);

    //    configure TDC
    configIni->setValue("TDC配置/freqCOM",freqCOM);
    configIni->setValue("TDC配置/enableCountEvents",enableCountEvents);
    configIni->setValue("TDC配置/countEvents",countEvents);
    for(int i = 0; i < 7; i++)
    {
        configIni->setValue("TDC配置/channelConfig"+QString::number(i),channelConfig[i]);
        configIni->setValue("TDC配置/level"+QString::number(i),level[i]);
        configIni->setValue("TDC配置/slope"+QString::number(i),slope[i]);
    }

    //    configure single count
    configIni->setValue("计数/accumulateTime",accumulateTime);

    //    configure data saving
    configIni->setValue("保存数据/pathName",pathName);
    configIni->setValue("保存数据/fileName",fileName);
    configIni->setValue("保存数据/metaData",metaData);
    configIni->setValue("保存数据/enableRecordTime",enableRecordTime);
    configIni->setValue("保存数据/recordTime",recordTime);

    delete configIni;

}

void Widget::loadFromIni()
// 从配置文件加载配置
{
    QSettings *configIni = new QSettings(iniName, QSettings::IniFormat);

//    configure TDC
    freqCOM = configIni->value("TDC配置/freqCOM").toInt();
    enableCountEvents = configIni->value("TDC配置/enableCountEvents").toBool();
    countEvents = configIni->value("TDC配置/countEvents").toInt();
    for(int i = 0; i < 7; i++)
    {
        channelConfig[i] = configIni->value("TDC配置/channelConfig"+QString::number(i)).toBool();
        level[i] = configIni->value("TDC配置/level"+QString::number(i)).toDouble();
        slope[i] = configIni->value("TDC配置/slope"+QString::number(i)).toInt();
    }

//    configure single count
    accumulateTime = configIni->value("计数/accumulateTime").toDouble();

//    configure data saving
    pathName = configIni->value("保存数据/pathName").toString();
    fileName = configIni->value("保存数据/fileName").toString();
    metaData = configIni->value("保存数据/metaData").toString();
    enableRecordTime = configIni->value("保存数据/enableRecordTime").toBool();
    recordTime = configIni->value("保存数据/recordTime").toDouble();

    delete configIni;

//    将配置更新到 UI
    ui->textFreqCOM->setText(QString::number(freqCOM/1000));
    ui->checkboxEnableCountEvents->setChecked(enableCountEvents);
    ui->textCountEvents->setEnabled(enableCountEvents);
    ui->textCountEvents->setText(QString::number(countEvents/1000));
    ui->checkBoxCN1->setChecked(channelConfig[1]);
    ui->checkBoxCN2->setChecked(channelConfig[2]);
    ui->checkBoxCN3->setChecked(channelConfig[3]);
    ui->checkBoxCN4->setChecked(channelConfig[4]);
    ui->checkBoxCN5->setChecked(channelConfig[5]);
    ui->checkBoxCN6->setChecked(channelConfig[6]);
    ui->levelTextCNCOM->setText(QString::number(level[0]));
    ui->levelTextCN1->setText(QString::number(level[1]));
    ui->levelTextCN2->setText(QString::number(level[2]));
    ui->levelTextCN3->setText(QString::number(level[3]));
    ui->levelTextCN4->setText(QString::number(level[4]));
    ui->levelTextCN5->setText(QString::number(level[5]));
    ui->levelTextCN6->setText(QString::number(level[6]));
    ui->slopeCNCOM->setCurrentIndex(slope[0]);
    ui->slopeCN1->setCurrentIndex(slope[1]);
    ui->slopeCN2->setCurrentIndex(slope[2]);
    ui->slopeCN3->setCurrentIndex(slope[3]);
    ui->slopeCN4->setCurrentIndex(slope[4]);
    ui->slopeCN5->setCurrentIndex(slope[5]);
    ui->slopeCN6->setCurrentIndex(slope[6]);

    ui->accumulateTimeText->setText(QString::number(accumulateTime));

    ui->textDataDirectory->setCurrentText(pathName);
    ui->textDataFileName->setText(fileName);
    ui->textMetaData->setPlainText(metaData);
    ui->checkboxRecordTime->setChecked(enableRecordTime);
    ui->textRecordTime->setEnabled(enableRecordTime);
    ui->textRecordTime->setText(QString::number(recordTime));

}

