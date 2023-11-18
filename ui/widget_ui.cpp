#include "widget.h"
#include "ui_widget.h"

void Widget::setupAcqIndicator()
{
    QSizePolicy p1;
    p1.setHorizontalPolicy(QSizePolicy::Expanding);
    p1.setVerticalPolicy(QSizePolicy::Expanding);

    statusIndicator = new QSimpleLed(this);
    statusIndicator->setCustomOnColor0(QSimpleLed::GREEN);
    statusIndicator->setCustomOffColor0(QSimpleLed::RED);
    statusIndicator->setSizePolicy(p1);
    ui->horizontalLayout_6->insertWidget(0,statusIndicator);
    statusIndicator->setStates(QSimpleLed::OFF);

    statusIndicator_2 = new QSimpleLed(this);
    statusIndicator_2->setCustomOnColor0(QSimpleLed::GREEN);
    statusIndicator_2->setCustomOffColor0(QSimpleLed::RED);
    statusIndicator_2->setSizePolicy(p1);
    ui->horizontalLayout_6->insertWidget(1,statusIndicator_2);
    statusIndicator_2->setStates(QSimpleLed::OFF);
}

void Widget::on_buttonExit_released()
{
    emit mainAppClosing();
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
    tdc->close();
    this->close();
}

void Widget::fetchUiData()
{
    // TDC 设置
    freqCOM = 1000.0*ui->textFreqCOM->text().toDouble();
    enableCountEvents = ui->checkboxEnableCountEvents->isChecked();
    if (enableCountEvents)
        countEvents = int(1000*ui->textCountEvents->text().toDouble());
    else
    {
        countEvents = int(freqCOM/100.0);
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
    delayCN[0] = ui->textDelayCN1->text().toDouble();
    delayCN[1] = ui->textDelayCN2->text().toDouble();
    delayCN[2] = ui->textDelayCN3->text().toDouble();
    delayCN[3] = ui->textDelayCN4->text().toDouble();
    delayCN[4] = ui->textDelayCN5->text().toDouble();
    delayCN[5] = ui->textDelayCN6->text().toDouble();
    int minDelay = delayCN[0], maxDelay = delayCN[0];
    for (int k = 1; k < 6; k++)
    {
        if (delayCN[k] < minDelay)
            minDelay = delayCN[k];
        if (delayCN[k] > maxDelay)
            maxDelay = delayCN[k];
    }
    for (int k = 0; k < 6; k++)
    {
        delayCN[k] = delayCN[k] - minDelay;
    }
    delayCN[6] = maxDelay - minDelay;
    ui->textDelayCN1->setText(QString::number(delayCN[0]));
    ui->textDelayCN2->setText(QString::number(delayCN[1]));
    ui->textDelayCN3->setText(QString::number(delayCN[2]));
    ui->textDelayCN4->setText(QString::number(delayCN[3]));
    ui->textDelayCN5->setText(QString::number(delayCN[4]));
    ui->textDelayCN6->setText(QString::number(delayCN[5]));


    channelConfig_2[0] = true;
    channelConfig_2[1] = ui->checkBoxCN1_2->isChecked();
    channelConfig_2[2] = ui->checkBoxCN2_2->isChecked();
    channelConfig_2[3] = ui->checkBoxCN3_2->isChecked();
    channelConfig_2[4] = ui->checkBoxCN4_2->isChecked();
    channelConfig_2[5] = ui->checkBoxCN5_2->isChecked();
    channelConfig_2[6] = ui->checkBoxCN6_2->isChecked();
    level_2[0] = ui->levelTextCNCOM_2->text().toDouble();
    level_2[1] = ui->levelTextCN1_2->text().toDouble();
    level_2[2] = ui->levelTextCN2_2->text().toDouble();
    level_2[3] = ui->levelTextCN3_2->text().toDouble();
    level_2[4] = ui->levelTextCN4_2->text().toDouble();
    level_2[5] = ui->levelTextCN5_2->text().toDouble();
    level_2[6] = ui->levelTextCN6_2->text().toDouble();
    slope_2[0] = ui->slopeCNCOM_2->currentIndex();
    slope_2[1] = ui->slopeCN1_2->currentIndex();
    slope_2[2] = ui->slopeCN2_2->currentIndex();
    slope_2[3] = ui->slopeCN3_2->currentIndex();
    slope_2[4] = ui->slopeCN4_2->currentIndex();
    slope_2[5] = ui->slopeCN5_2->currentIndex();
    slope_2[6] = ui->slopeCN6_2->currentIndex();
    delayCN_2[0] = ui->textDelayCN1_2->text().toDouble();
    delayCN_2[1] = ui->textDelayCN2_2->text().toDouble();
    delayCN_2[2] = ui->textDelayCN3_2->text().toDouble();
    delayCN_2[3] = ui->textDelayCN4_2->text().toDouble();
    delayCN_2[4] = ui->textDelayCN5_2->text().toDouble();
    delayCN_2[5] = ui->textDelayCN6_2->text().toDouble();
    minDelay = delayCN_2[0]; maxDelay = delayCN_2[0];
    for (int k = 1; k < 6; k++)
    {
        if (delayCN_2[k] < minDelay)
            minDelay = delayCN_2[k];
        if (delayCN_2[k] > maxDelay)
            maxDelay = delayCN_2[k];
    }
    for (int k = 0; k < 6; k++)
    {
        delayCN_2[k] = delayCN_2[k] - minDelay;
    }
    delayCN_2[6] = maxDelay - minDelay;
    ui->textDelayCN1_2->setText(QString::number(delayCN_2[0]));
    ui->textDelayCN2_2->setText(QString::number(delayCN_2[1]));
    ui->textDelayCN3_2->setText(QString::number(delayCN_2[2]));
    ui->textDelayCN4_2->setText(QString::number(delayCN_2[3]));
    ui->textDelayCN5_2->setText(QString::number(delayCN_2[4]));
    ui->textDelayCN6_2->setText(QString::number(delayCN_2[5]));

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
    ui->textFreqCOM->setText(QString::number(freqCOM/1000.0));
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
    ui->textDelayCN1->setText(QString::number(delayCN[0]));
    ui->textDelayCN2->setText(QString::number(delayCN[1]));
    ui->textDelayCN3->setText(QString::number(delayCN[2]));
    ui->textDelayCN4->setText(QString::number(delayCN[3]));
    ui->textDelayCN5->setText(QString::number(delayCN[4]));
    ui->textDelayCN6->setText(QString::number(delayCN[5]));

    ui->checkBoxCN1_2->setChecked(channelConfig_2[1]);
    ui->checkBoxCN2_2->setChecked(channelConfig_2[2]);
    ui->checkBoxCN3_2->setChecked(channelConfig_2[3]);
    ui->checkBoxCN4_2->setChecked(channelConfig_2[4]);
    ui->checkBoxCN5_2->setChecked(channelConfig_2[5]);
    ui->checkBoxCN6_2->setChecked(channelConfig_2[6]);
    ui->levelTextCNCOM_2->setText(QString::number(level_2[0]));
    ui->levelTextCN1_2->setText(QString::number(level_2[1]));
    ui->levelTextCN2_2->setText(QString::number(level_2[2]));
    ui->levelTextCN3_2->setText(QString::number(level_2[3]));
    ui->levelTextCN4_2->setText(QString::number(level_2[4]));
    ui->levelTextCN5_2->setText(QString::number(level_2[5]));
    ui->levelTextCN6_2->setText(QString::number(level_2[6]));
    ui->slopeCNCOM_2->setCurrentIndex(slope_2[0]);
    ui->slopeCN1_2->setCurrentIndex(slope_2[1]);
    ui->slopeCN2_2->setCurrentIndex(slope_2[2]);
    ui->slopeCN3_2->setCurrentIndex(slope_2[3]);
    ui->slopeCN4_2->setCurrentIndex(slope_2[4]);
    ui->slopeCN5_2->setCurrentIndex(slope_2[5]);
    ui->slopeCN6_2->setCurrentIndex(slope_2[6]);
    ui->textDelayCN1_2->setText(QString::number(delayCN_2[0]));
    ui->textDelayCN2_2->setText(QString::number(delayCN_2[1]));
    ui->textDelayCN3_2->setText(QString::number(delayCN_2[2]));
    ui->textDelayCN4_2->setText(QString::number(delayCN_2[3]));
    ui->textDelayCN5_2->setText(QString::number(delayCN_2[4]));
    ui->textDelayCN6_2->setText(QString::number(delayCN_2[5]));

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

void Widget::saveToIni()
{
    fetchUiData();

    QSettings *configIni = new QSettings(iniName, QSettings::IniFormat);

    configIni->setValue("UI设置/currentPage",ui->tabWidget->currentIndex());  // 记录当前所在页面

    //    configure TDC
    configIni->setValue("TDC配置/freqCOM",freqCOM);
    configIni->setValue("TDC配置/enableCountEvents",enableCountEvents);
    configIni->setValue("TDC配置/countEvents",countEvents);
    for(int i = 0; i < 7; i++)
    {
        configIni->setValue("TDC配置/channelConfig"+QString::number(i),channelConfig[i]);
        configIni->setValue("TDC配置/level"+QString::number(i),level[i]);
        configIni->setValue("TDC配置/slope"+QString::number(i),slope[i]);
        configIni->setValue("TDC配置/delayCN"+QString::number(i),delayCN[i]);

        configIni->setValue("TDC配置/channelConfig_2"+QString::number(i),channelConfig_2[i]);
        configIni->setValue("TDC配置/level_2"+QString::number(i),level_2[i]);
        configIni->setValue("TDC配置/slope_2"+QString::number(i),slope_2[i]);
        configIni->setValue("TDC配置/delayCN_2"+QString::number(i),delayCN_2[i]);
    }

    //    configure single count
    configIni->setValue("计数/accumulateTime",accumulateTime);

    //    configure data saving
    configIni->setValue("保存数据/pathName",pathName);
    configIni->setValue("保存数据/fileName",fileName);
    configIni->setValue("保存数据/metaData",metaData);
    configIni->setValue("保存数据/enableRecordTime",enableRecordTime);
    configIni->setValue("保存数据/recordTime",recordTime);
    configIni->setValue("保存数据/checkboxSCC",ui->checkboxSCC->isChecked());
    configIni->setValue("保存数据/checkboxCC",ui->checkboxCC->isChecked());

    delete configIni;

}

void Widget::loadFromIni()
{
    QSettings *configIni = new QSettings(iniName, QSettings::IniFormat);

    ui->tabWidget->setCurrentIndex(configIni->value("UI设置/currentPage").toInt());

//    configure TDC
    freqCOM = configIni->value("TDC配置/freqCOM").toDouble();
    enableCountEvents = configIni->value("TDC配置/enableCountEvents").toBool();
    countEvents = configIni->value("TDC配置/countEvents").toInt();
    for(int i = 0; i < 7; i++)
    {
        channelConfig[i] = configIni->value("TDC配置/channelConfig"+QString::number(i)).toBool();
        level[i] = configIni->value("TDC配置/level"+QString::number(i)).toDouble();
        slope[i] = configIni->value("TDC配置/slope"+QString::number(i)).toInt();
        delayCN[i] = configIni->value("TDC配置/delayCN"+QString::number(i)).toDouble();

        channelConfig_2[i] = configIni->value("TDC配置/channelConfig_2"+QString::number(i)).toBool();
        level_2[i] = configIni->value("TDC配置/level_2"+QString::number(i)).toDouble();
        slope_2[i] = configIni->value("TDC配置/slope_2"+QString::number(i)).toInt();
        delayCN_2[i] = configIni->value("TDC配置/delayCN_2"+QString::number(i)).toDouble();
    }

//    configure single count
    accumulateTime = configIni->value("计数/accumulateTime").toDouble();

//    configure data saving
    pathName = configIni->value("保存数据/pathName").toString();
    fileName = configIni->value("保存数据/fileName").toString();
    metaData = configIni->value("保存数据/metaData").toString();
    enableRecordTime = configIni->value("保存数据/enableRecordTime").toBool();
    recordTime = configIni->value("保存数据/recordTime").toDouble();
    ui->checkboxSCC->setChecked(configIni->value("保存数据/checkboxSCC").toBool());
    ui->checkboxCC->setChecked(configIni->value("保存数据/checkboxCC").toBool());

    delete configIni;

    pushUiData();
}

void Widget::on_buttonTempFilePath_released()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(iniPath));
}

void Widget::on_buttonCheckUpdate_released()
{
    QUrl myUrl = QUrl(PROJECT_RELEASE_PAGE);
    QDesktopServices::openUrl(myUrl);
}
