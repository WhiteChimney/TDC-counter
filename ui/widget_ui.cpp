#include "widget.h"
#include "ui_widget.h"

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
    Acqrs_closeAll();
    this->close();
}

void Widget::on_checkBoxEnableGating1_stateChanged(int arg1)
{
    ui->textGating1->setEnabled(arg1);
    enableGating[0] = arg1;
}

void Widget::on_checkBoxEnableGating2_stateChanged(int arg1)
{
    ui->textGating2->setEnabled(arg1);
    enableGating[1] = arg1;
}

void Widget::on_checkBoxEnableGating3_stateChanged(int arg1)
{
    ui->textGating3->setEnabled(arg1);
    enableGating[2] = arg1;
}

void Widget::on_checkBoxEnableGating4_stateChanged(int arg1)
{
    ui->textGating4->setEnabled(arg1);
    enableGating[3] = arg1;
}

void Widget::on_checkBoxEnableGating5_stateChanged(int arg1)
{
    ui->textGating5->setEnabled(arg1);
    enableGating[4] = arg1;
}

void Widget::on_checkBoxEnableGating6_stateChanged(int arg1)
{
    ui->textGating6->setEnabled(arg1);
    enableGating[5] = arg1;
}

void Widget::on_spinBoxCn1_valueChanged(int arg1)
{
    ui->textDelayCN1->setText(QString::number(arg1/20000.0));
    delayCN[0] = arg1/20000.0;
}

void Widget::on_spinBoxCn2_valueChanged(int arg1)
{
    ui->textDelayCN2->setText(QString::number(arg1/20000.0));
    delayCN[1] = arg1/20000.0;
}

void Widget::on_spinBoxCn3_valueChanged(int arg1)
{
    ui->textDelayCN3->setText(QString::number(arg1/20000.0));
    delayCN[2] = arg1/20000.0;
}

void Widget::on_spinBoxCn4_valueChanged(int arg1)
{
    ui->textDelayCN4->setText(QString::number(arg1/20000.0));
    delayCN[3] = arg1/20000.0;
}

void Widget::on_spinBoxCn5_valueChanged(int arg1)
{
    ui->textDelayCN5->setText(QString::number(arg1/20000.0));
    delayCN[4] = arg1/20000.0;
}

void Widget::on_spinBoxCn6_valueChanged(int arg1)
{
    ui->textDelayCN6->setText(QString::number(arg1/20000.0));
    delayCN[5] = arg1/20000.0;
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
    delayCN[0] = int(20*1000.0*ui->textDelayCN1->text().toDouble())/20000.0;
    delayCN[1] = int(20*1000.0*ui->textDelayCN2->text().toDouble())/20000.0;
    delayCN[2] = int(20*1000.0*ui->textDelayCN3->text().toDouble())/20000.0;
    delayCN[3] = int(20*1000.0*ui->textDelayCN4->text().toDouble())/20000.0;
    delayCN[4] = int(20*1000.0*ui->textDelayCN5->text().toDouble())/20000.0;
    delayCN[5] = int(20*1000.0*ui->textDelayCN6->text().toDouble())/20000.0;
    int minDelay = delayCN[0], maxDelay = delayCN[0];
    for (int k = 1; k < 6; k++)
    {
        if (delayCN[k] < minDelay)
            minDelay = delayCN[k];
        if (delayCN[k] > maxDelay)
            maxDelay = delayCN[k];
    }
    delayCN[6] = maxDelay - minDelay;
    if (minDelay < 0)
    {
        for (int k = 0; k < 6; k++)
            delayCN[k] = delayCN[k] - minDelay;
    }

    // 单道计数设置
    accumulateTime = ui->accumulateTimeText->text().toDouble();
    enableGating[0] = ui->textGating1->isEnabled();
    enableGating[1] = ui->textGating2->isEnabled();
    enableGating[2] = ui->textGating3->isEnabled();
    enableGating[3] = ui->textGating4->isEnabled();
    enableGating[4] = ui->textGating5->isEnabled();
    enableGating[5] = ui->textGating6->isEnabled();
    gatingTime[0] = ui->textGating1->text().toDouble();
    gatingTime[1] = ui->textGating2->text().toDouble();
    gatingTime[2] = ui->textGating3->text().toDouble();
    gatingTime[3] = ui->textGating4->text().toDouble();
    gatingTime[4] = ui->textGating5->text().toDouble();
    gatingTime[5] = ui->textGating6->text().toDouble();

    // 数据保存设置
    pathName = ui->textDataDirectory->currentText();
    fileName = ui->textDataFileName->text();
    metaData = ui->textMetaData->toPlainText();
    enableRecordTime = ui->checkboxRecordTime->isChecked();
    recordTime = ui->textRecordTime->text().toDouble();

    pushUiData();
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

    // 单道计数设置
    ui->accumulateTimeText->setText(QString::number(accumulateTime));
    ui->textGating1->setEnabled(enableGating[0]);
    ui->textGating2->setEnabled(enableGating[1]);
    ui->textGating3->setEnabled(enableGating[2]);
    ui->textGating4->setEnabled(enableGating[3]);
    ui->textGating5->setEnabled(enableGating[4]);
    ui->textGating6->setEnabled(enableGating[5]);
    ui->checkBoxEnableGating1->setChecked(enableGating[0]);
    ui->checkBoxEnableGating2->setChecked(enableGating[1]);
    ui->checkBoxEnableGating3->setChecked(enableGating[2]);
    ui->checkBoxEnableGating4->setChecked(enableGating[3]);
    ui->checkBoxEnableGating5->setChecked(enableGating[4]);
    ui->checkBoxEnableGating6->setChecked(enableGating[5]);
    ui->textGating1->setText(QString::number(gatingTime[0]));
    ui->textGating2->setText(QString::number(gatingTime[1]));
    ui->textGating3->setText(QString::number(gatingTime[2]));
    ui->textGating4->setText(QString::number(gatingTime[3]));
    ui->textGating5->setText(QString::number(gatingTime[4]));
    ui->textGating6->setText(QString::number(gatingTime[5]));
    ui->spinBoxCn1->setValue(int(20000.0*ui->textDelayCN1->text().toDouble()));
    ui->spinBoxCn2->setValue(int(20000.0*ui->textDelayCN2->text().toDouble()));
    ui->spinBoxCn3->setValue(int(20000.0*ui->textDelayCN3->text().toDouble()));
    ui->spinBoxCn4->setValue(int(20000.0*ui->textDelayCN4->text().toDouble()));
    ui->spinBoxCn5->setValue(int(20000.0*ui->textDelayCN5->text().toDouble()));
    ui->spinBoxCn6->setValue(int(20000.0*ui->textDelayCN6->text().toDouble()));

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
    configIni->setValue("TDC配置/freqSignal",ui->textFreqSignal->text().toDouble());
    for(int i = 0; i < 7; i++)
    {
        configIni->setValue("TDC配置/channelConfig"+QString::number(i),channelConfig[i]);
        configIni->setValue("TDC配置/level"+QString::number(i),level[i]);
        configIni->setValue("TDC配置/slope"+QString::number(i),slope[i]);
        configIni->setValue("TDC配置/delayCN"+QString::number(i),delayCN[i]);
    }

    //    configure single count
    configIni->setValue("计数/accumulateTime",accumulateTime);
    for(int i = 0; i < 6; i++)
    {
        configIni->setValue("计数/enableGating"+QString::number(i),enableGating[i]);
        configIni->setValue("计数/gatingTime"+QString::number(i),gatingTime[i]);
    }

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
    ui->textFreqSignal->setText(QString::number(configIni->value("TDC配置/freqSignal").toDouble()));
    for(int i = 0; i < 7; i++)
    {
        channelConfig[i] = configIni->value("TDC配置/channelConfig"+QString::number(i)).toBool();
        level[i] = configIni->value("TDC配置/level"+QString::number(i)).toDouble();
        slope[i] = configIni->value("TDC配置/slope"+QString::number(i)).toInt();
        delayCN[i] = configIni->value("TDC配置/delayCN"+QString::number(i)).toDouble();
    }

//    configure single count
    accumulateTime = configIni->value("计数/accumulateTime").toDouble();
    for(int i = 0; i < 6; i++)
    {
        enableGating[i] = configIni->value("计数/enableGating"+QString::number(i)).toBool();
        gatingTime[i] = configIni->value("计数/gatingTime"+QString::number(i)).toDouble();
    }

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

void Widget::on_buttonSaveConfig_clicked()
{
    this->saveToIni();

    for (int i = 0; i < vCoinWidget.size(); i++)
        vCoinWidget.at(i)->saveToIni();

    for (int i = 0; i < vHistWidget.size(); i++)
        vHistWidget.at(i)->saveToIni();
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

void Widget::on_textDelayCN1_editingFinished()
{
    fetchUiData();
}

void Widget::on_textDelayCN2_editingFinished()
{
    fetchUiData();
}

void Widget::on_textDelayCN3_editingFinished()
{
    fetchUiData();
}

void Widget::on_textDelayCN4_editingFinished()
{
    fetchUiData();
}

void Widget::on_textDelayCN5_editingFinished()
{
    fetchUiData();
}

void Widget::on_textDelayCN6_editingFinished()
{
    fetchUiData();
}
