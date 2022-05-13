#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Acqiris TDC 计数程序");
    this->setupAcqIndicator();
    ui->tabWidget->setCurrentWidget(ui->pageSettings);

//    清空临时文件
    QDir tempDataPath(iniPath + "/Data");
    if (tempDataPath.isReadable())
    {
        if (! tempDataPath.isEmpty())
        {
            tempDataPath.removeRecursively();
            QThread::msleep(100);
            tempDataPath.mkdir(iniPath + "/Data");
        }
    }
    else
        tempDataPath.mkdir(iniPath + "/Data");

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
//    connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,this,&Widget::dealAcqThreadBankSwitch);

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
    QDir tempDataPath(iniPath + "/Data");
    tempDataPath.removeRecursively();
    QThread::msleep(100);
    tempDataPath.mkdir(iniPath + "/Data");
    delete ui;
}

void Widget::setupAcqIndicator()
{
    statusIndicator->setCustomOnColor0(QSimpleLed::GREEN);
    statusIndicator->setCustomOffColor0(QSimpleLed::RED);
    QSizePolicy p1;
    p1.setHorizontalPolicy(QSizePolicy::Expanding);
    p1.setVerticalPolicy(QSizePolicy::Expanding);
    statusIndicator->setSizePolicy(p1);
    ui->horizontalLayout_6->insertWidget(0,statusIndicator);
    statusIndicator->setStates(QSimpleLed::OFF);
}

void Widget::fetchUiData()
{
    // TDC 设置
    freqCOM = int(1000*ui->textFreqCOM->text().toDouble());
    enableCountEvents = ui->checkboxEnableCountEvents->isChecked();
    if (enableCountEvents)
        countEvents = int(1000*ui->textCountEvents->text().toDouble());
    else
    {
        countEvents = freqCOM / 100;
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

    // 单道计数设置
    accumulateTime = ui->accumulateTimeText->text().toDouble();

    // 数据保存设置
    pathName = ui->textDataDirectory->currentText();
    fileName = ui->textDataFileName->text();
    metaData = ui->textMetaData->toPlainText();
    enableRecordTime = ui->checkboxRecordTime->isChecked();
    recordTime = ui->textRecordTime->text().toDouble();
}

void Widget::pushUiData()
{
    // TDC 设置
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

    // 单道计数设置
    ui->accumulateTimeText->setText(QString::number(accumulateTime));

    // 数据保存设置
    ui->textDataDirectory->setCurrentText(pathName);
    ui->textDataFileName->setText(fileName);
    ui->textMetaData->setPlainText(metaData);
    ui->checkboxRecordTime->setChecked(enableRecordTime);
    ui->textRecordTime->setEnabled(enableRecordTime);
    ui->textRecordTime->setText(QString::number(recordTime));
}

void Widget::on_buttonTempFilePath_released()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(iniPath));
}

void Widget::on_buttonExit_released()
{
    for (int i = vCoinWidget.count()-1; i >= 0; i--)
    {
        vCoinWidget.at(i)->saveToIni();
        if (vCoinWidget.at(i)->windowState() != Qt::WindowNoState)
            dealCoincidenceReturn(i);
        vCoinWidget.removeLast();
        vCoinWidgetSyncState.removeLast();
    }
    for (int i = vHistWidget.count()-1; i >= 0; i--)
    {
        vHistWidget.at(i)->saveToIni();
        if (vHistWidget.at(i)->windowState() != Qt::WindowNoState)
            dealHistogramReturn(i);
        vHistWidget.removeLast();
    }
    on_buttonStopRecord_released();
    on_buttonStopCount_released();
    on_buttonStopAcq_released();
    QThread::msleep(100);     // 等待进程完全停止
    // Stops the acquisition & close instruments
    Acqrs_closeAll();
    this->close();
}

void Widget::on_buttonTest_released()
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
{
    testW = vTestWidget.at(index);
    testW->setWindowState(Qt::WindowNoState);
    testW->close();
}

void Widget::on_buttonApplyConfig_released()
{
    fetchUiData();
    configStatus = configTDC(idInstr, countEvents, channelConfig, level, slope, readParamPtr);
}

void Widget::on_checkboxEnableCountEvents_released()
{
    if (ui->checkboxEnableCountEvents->isChecked())
        ui->textCountEvents->setEnabled(true);
    else
        ui->textCountEvents->setEnabled(false);
}

void Widget::on_buttonStartAcquisition_released()
{
    if (configStatus != VI_SUCCESS)
        on_buttonApplyConfig_released();

    // start Acquisition
    if (configStatus == VI_SUCCESS)
        acqThread->start();
}

void Widget::dealAcqThreadStarted()
{
    statusIndicator->setStates(QSimpleLed::ON);
    *acqStopPtr = false;
    if (configStatus == VI_SUCCESS)
        emit acqParamReady
            (acqStopPtr,idInstr,readParamPtr);  // 收到采集线程的开启信号后，将参数传送过去
}

void Widget::on_buttonStopAcq_released()
{
    *acqStopPtr = true;
    configStatus = -1;
}

void Widget::dealAcqThreadFinished()
{
    AcqrsT3_waitForEndOfAcquisition(idInstr, 8000);
    AcqrsT3_stopAcquisition(idInstr);
    acqThread->quit();
    acqThread->wait();
    statusIndicator->setStates(QSimpleLed::OFF);
}

void Widget::on_buttonStartCount_released()
{
    this->dealCountTimeOut();
    if (! *acqStopPtr) // 当采集已经开启时
    {
        // 开始计算单道计数
        connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,this,&Widget::dealAcqThreadBankSwitchSCC);
        // 启动定时器
        fetchUiData();
        timerCount->start(1000.0*accumulateTime);
    }
}

void Widget::createTempDataFile()
{
    fSingleCount->open(QIODevice::WriteOnly | QIODevice::Text);
    fStream << tr("单道计数") << endl
            << tr("当前时间：") << dateTime.currentDateTime().toString() << endl
            << tr("计数累计时间：") << accumulateTime << endl
            << endl
            << tr("时间/ms\tChannel 1\tChannel 2\tChannel 3\tChannel 4\tChannel 5\tChannel 6") << endl;
    fSingleCount->close();
}

void Widget::dealAcqThreadBankSwitchSCC(AqT3DataDescriptor* dataDescPtr)
{
    computeSingleChannelCount(dataDescPtr,nbrSCC);
}

void Widget::dealCountTimeOut()
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
{
    timerCount->stop();
    disconnect(acqThread,&AcquisitionThread::acqThreadBankSwitch,this,&Widget::dealAcqThreadBankSwitchSCC);
}

void Widget::on_buttonCoincidence_released()
{
    int index = vCoinWidget.count();
    coinW = new CoincidenceWidget(0,index);
    vCoinWidget.append(coinW);
    vCoinWidgetSyncState.append(false);
    connect(coinW,&CoincidenceWidget::returnSignal,this,&Widget::dealCoincidenceReturn);
    connect(coinW,&CoincidenceWidget::askDealAcqBankSwitchCoin,this,&Widget::dealAskDealAcqBankSwitchCoin);
    connect(coinW,&CoincidenceWidget::askStopDealAcqBankSwitchCoin,this,&Widget::dealAskStopDealAcqBankSwitchCoin);
    connect(coinW,&CoincidenceWidget::coinTimerNeedsSync,this,&Widget::dealCoinTimerNeedsSync);
    connect(coinW,&CoincidenceWidget::coinTimerStopsSync,this,&Widget::dealCoinTimerStopsSync);
    coinW->setWindowTitle("符合计数"+QString::number(index+1));
    coinW->setWindowState(Qt::WindowActive); // 设置窗口活动状态，以便后续处理，如保存数据
    coinW->show();
}

void Widget::dealCoincidenceReturn(int index)
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
{
    coinW = vCoinWidget.at(index);
    connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,coinW,&CoincidenceWidget::dealAcqThreadBankSwitchCoin);
}

void Widget::dealAskStopDealAcqBankSwitchCoin(int index)
{
    coinW = vCoinWidget.at(index);
    disconnect(acqThread,&AcquisitionThread::acqThreadBankSwitch,coinW,&CoincidenceWidget::dealAcqThreadBankSwitchCoin);
}

void Widget::dealCoinTimerNeedsSync(int index)
{
    coinW = vCoinWidget.at(index);
    vCoinWidgetSyncState.replace(index,true);
    connect(timerCount,&QTimer::timeout,coinW,&CoincidenceWidget::dealTimeOut); // 同步记录数据
}

void Widget::dealCoinTimerStopsSync(int index)
{
    coinW = vCoinWidget.at(index);
    vCoinWidgetSyncState.replace(index,false);
    disconnect(timerCount,&QTimer::timeout,coinW,&CoincidenceWidget::dealTimeOut);
}

void Widget::on_buttonHistogram_released()
{
    int index = vHistWidget.count();
    histW = new HistogramWidget(0, index);
    vHistWidget.append(histW);
    connect(histW,&HistogramWidget::returnSignal,this,&Widget::dealHistogramReturn);
    connect(histW,&HistogramWidget::askDealAcqBankSwitchHist,this,&Widget::dealAskDealAcqBankSwitchHist);
    connect(histW,&HistogramWidget::askStopDealAcqBankSwitchHist,this,&Widget::dealAskStopDealAcqBankSwitchHist);
    histW->setWindowTitle("直方图"+QString::number(index+1));
    histW->setWindowState(Qt::WindowActive);
    histW->show();
}

void Widget::dealHistogramReturn(int index)
{
    histW = vHistWidget.at(index);
    histW->setWindowState(Qt::WindowNoState);
    histW->close();
}

void Widget::dealAskDealAcqBankSwitchHist(int index)
{
    histW = vHistWidget.at(index);
    connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,histW,&HistogramWidget::dealAcqThreadBankSwitchHist);
}

void Widget::dealAskStopDealAcqBankSwitchHist(int index)
{
    histW = vHistWidget.at(index);
    disconnect(acqThread,&AcquisitionThread::acqThreadBankSwitch,histW,&HistogramWidget::dealAcqThreadBankSwitchHist);
}

void Widget::on_buttonOpenDataDir_released()
{
    fetchUiData();
    QDesktopServices::openUrl(QUrl::fromLocalFile(pathName));
}

void Widget::on_buttonDataDirectory_released()
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
{
    fetchUiData();
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
        this->createTempDataFile();
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
            coinW->createTempDataFile();
            disconnect(timerCount,&QTimer::timeout,coinW,&CoincidenceWidget::dealTimeOut);
            connect(timerCount,&QTimer::timeout,coinW,&CoincidenceWidget::dealSaveCoinData);
            connect(timerCount,&QTimer::timeout,coinW,&CoincidenceWidget::dealTimeOut);
        }
        break;
    case SingleCountOnly:
        this->createTempDataFile();
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
            coinW->createTempDataFile();
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
{
    timeToc = dateTime.currentMSecsSinceEpoch();
    timeRelative = timeToc - timeTic;
    fStream << timeRelative;
    for (int i = 0; i < 6; i++)
    {
        fStream << "\t" << nbrSCC[i];
    }
    fStream << endl;
}

void Widget::on_buttonStopRecord_released()
{
    fetchUiData();

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
        }
        break;
    default:
        return;
        break;
    }

    QString fullFileName = pathName + "/" + fileName + "." + ui->comboDataFileSuffix->currentText();
    mergeDataFiles(modeRecord, fullFileName, metaData, vCoinWidget);

    modeRecord = notRecording;

    QDesktopServices::openUrl(QUrl::fromLocalFile(pathName));
    if (enableRecordTime)
    {
        timerFile->stop();
    }
}

void Widget::dealRecordTimeOut()
{
    on_buttonStopRecord_released();
}

void Widget::on_checkboxRecordTime_released()
{
    if (ui->checkboxRecordTime->isChecked())
        ui->textRecordTime->setEnabled(true);
    else
        ui->textRecordTime->setEnabled(false);
}

void Widget::saveToIni()
{
    fetchUiData();

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

    pushUiData();
}

void Widget::on_buttonStatistics_released()
{
//    初始化统计 Widget
    if (statWidgetLaunched)
    {
        statW->show();
        statW->activateWindow();
    }
    else
    {
        statW = new StatisticsWidget(this);
        setupStatUi();
        connect(statW,&StatisticsWidget::sendReturnSignal,this,&Widget::dealStatisticsReturn);
        statW->show();
        statWidgetLaunched = true;
    }
}

void Widget::dealStatisticsReturn()
{
    statW->close();
    statWidgetLaunched = false;
}

void Widget::setupStatUi()
{
    if (ui->checkBoxCN1->isChecked()) statW->addChannel("单道 1",nbrSCC+0);
    if (ui->checkBoxCN2->isChecked()) statW->addChannel("单道 2",nbrSCC+1);
    if (ui->checkBoxCN3->isChecked()) statW->addChannel("单道 3",nbrSCC+2);
    if (ui->checkBoxCN4->isChecked()) statW->addChannel("单道 4",nbrSCC+3);
    if (ui->checkBoxCN5->isChecked()) statW->addChannel("单道 5",nbrSCC+4);
    if (ui->checkBoxCN6->isChecked()) statW->addChannel("单道 6",nbrSCC+5);
    for (int i = 0; i < vCoinWidget.count(); i++)
    {
        if (vCoinWidgetSyncState.at(i))
        {
            QString coinChannelName;
            int *nbrCoin = new int();
            int *nbrAccCoin = new int();
            vCoinWidget.at(i)->getCoinParam(&coinChannelName,&nbrCoin,&nbrAccCoin);
            statW->addChannel("符合"+coinChannelName,nbrCoin);
            statW->addChannel("偶然符合"+coinChannelName,nbrAccCoin);
            delete nbrCoin;
            delete nbrAccCoin;
        }
    }
}
