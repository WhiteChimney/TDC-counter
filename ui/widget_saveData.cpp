#include "widget.h"
#include "ui_widget.h"

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
    ui->buttonStartRecord->setEnabled(false);
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

    ui->buttonStartRecord->setEnabled(true);
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
