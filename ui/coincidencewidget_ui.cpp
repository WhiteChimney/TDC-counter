#include "coincidencewidget.h"
#include "ui_coincidencewidget.h"

void CoincidenceWidget::fetchUiData()
{
//    双通道的数据
    device1 = ui->comboDevice1->currentIndex();
    device2 = ui->comboDevice2->currentIndex();
    channel1 = ui->coinChannel1->currentText().toInt();
    channel2 = ui->coinChannel2->currentText().toInt();
    delay = int(20*ui->delayCoin->text().toDouble());
    delayAcc = int(20*ui->delayAccCoin->text().toDouble());
    accumulateTime = ui->accumTimeCoin->text().toDouble();
    enableAccumulateTime = ui->checkboxAccumlateTime->isChecked();
    tolerance = int(20*ui->tolerance->text().toDouble());

//    多通道的数据
    channelMark[0] = ui->checkBoxCN1->isChecked(); delayMulti[0] = int(20*ui->textDelayCN1->text().toDouble());
    channelMark[1] = ui->checkBoxCN2->isChecked(); delayMulti[1] = int(20*ui->textDelayCN2->text().toDouble());
    channelMark[2] = ui->checkBoxCN3->isChecked(); delayMulti[2] = int(20*ui->textDelayCN3->text().toDouble());
    channelMark[3] = ui->checkBoxCN4->isChecked(); delayMulti[3] = int(20*ui->textDelayCN4->text().toDouble());
    channelMark[4] = ui->checkBoxCN5->isChecked(); delayMulti[4] = int(20*ui->textDelayCN5->text().toDouble());
    channelMark[5] = ui->checkBoxCN6->isChecked(); delayMulti[5] = int(20*ui->textDelayCN6->text().toDouble());
    channelMark_2[0] = ui->checkBoxCN1_2->isChecked(); delayMulti_2[0] = int(20*ui->textDelayCN1_2->text().toDouble());
    channelMark_2[1] = ui->checkBoxCN2_2->isChecked(); delayMulti_2[1] = int(20*ui->textDelayCN2_2->text().toDouble());
    channelMark_2[2] = ui->checkBoxCN3_2->isChecked(); delayMulti_2[2] = int(20*ui->textDelayCN3_2->text().toDouble());
    channelMark_2[3] = ui->checkBoxCN4_2->isChecked(); delayMulti_2[3] = int(20*ui->textDelayCN4_2->text().toDouble());
    channelMark_2[4] = ui->checkBoxCN5_2->isChecked(); delayMulti_2[4] = int(20*ui->textDelayCN5_2->text().toDouble());
    channelMark_2[5] = ui->checkBoxCN6_2->isChecked(); delayMulti_2[5] = int(20*ui->textDelayCN6_2->text().toDouble());

    if (ui->stackCoin->currentIndex()==0) // 双通道模式计算符合计数
        computeMode = device1 + device2;
    else
        computeMode = not(channelMark[0] or channelMark[1] or channelMark[2] or channelMark[3] or channelMark[4] or channelMark[5])
                + (channelMark_2[0] or channelMark_2[1] or channelMark_2[2] or channelMark_2[3] or channelMark_2[4] or channelMark_2[5]);

    accumulateTimeMulti = ui->accumTimeCoin_Multi->text().toDouble();
    enableAccumulateTimeMulti = ui->checkboxAccumlateTime_Multi->isChecked();
    toleranceMulti = int(20*ui->tolerance_Multi->text().toDouble());

    acquireComDelay = ui->textAcquireComDelay->text().toInt();
    pushUiData();
}

void CoincidenceWidget::pushUiData()
{
//    双通道的数据
    ui->comboDevice1->setCurrentIndex(device1);
    ui->comboDevice2->setCurrentIndex(device2);
    ui->coinChannel1->setCurrentText(QString::number(channel1));
    ui->coinChannel2->setCurrentText(QString::number(channel2));
    ui->delayCoin->setText(QString::number(delay/20.0));
    ui->delayAccCoin->setText(QString::number(delayAcc/20.0));
    ui->checkboxAccumlateTime->setChecked(enableAccumulateTime);
    ui->accumTimeCoin->setText(QString::number(accumulateTime));
    ui->accumTimeCoin->setEnabled(!enableAccumulateTime);
    ui->tolerance->setText(QString::number(tolerance/20.0));

//    多通道的数据
    ui->checkBoxCN1->setChecked(channelMark[0]); ui->textDelayCN1->setText(QString::number(delayMulti[0]/20.0));
    ui->checkBoxCN2->setChecked(channelMark[1]); ui->textDelayCN2->setText(QString::number(delayMulti[1]/20.0));
    ui->checkBoxCN3->setChecked(channelMark[2]); ui->textDelayCN3->setText(QString::number(delayMulti[2]/20.0));
    ui->checkBoxCN4->setChecked(channelMark[3]); ui->textDelayCN4->setText(QString::number(delayMulti[3]/20.0));
    ui->checkBoxCN5->setChecked(channelMark[4]); ui->textDelayCN5->setText(QString::number(delayMulti[4]/20.0));
    ui->checkBoxCN6->setChecked(channelMark[5]); ui->textDelayCN6->setText(QString::number(delayMulti[5]/20.0));
    ui->checkBoxCN1_2->setChecked(channelMark_2[0]); ui->textDelayCN1_2->setText(QString::number(delayMulti_2[0]/20.0));
    ui->checkBoxCN2_2->setChecked(channelMark_2[1]); ui->textDelayCN2_2->setText(QString::number(delayMulti_2[1]/20.0));
    ui->checkBoxCN3_2->setChecked(channelMark_2[2]); ui->textDelayCN3_2->setText(QString::number(delayMulti_2[2]/20.0));
    ui->checkBoxCN4_2->setChecked(channelMark_2[3]); ui->textDelayCN4_2->setText(QString::number(delayMulti_2[3]/20.0));
    ui->checkBoxCN5_2->setChecked(channelMark_2[4]); ui->textDelayCN5_2->setText(QString::number(delayMulti_2[4]/20.0));
    ui->checkBoxCN6_2->setChecked(channelMark_2[5]); ui->textDelayCN6_2->setText(QString::number(delayMulti_2[5]/20.0));

    ui->checkboxAccumlateTime_Multi->setChecked(enableAccumulateTimeMulti);
    ui->accumTimeCoin_Multi->setText(QString::number(accumulateTimeMulti));
    ui->accumTimeCoin_Multi->setEnabled(!enableAccumulateTimeMulti);
    ui->tolerance_Multi->setText(QString::number(toleranceMulti/20.0));

    ui->textAcquireComDelay->setText(QString::number(acquireComDelay));
}

void CoincidenceWidget::saveToIni()
{
    fetchUiData();

    QSettings *configIni = new QSettings(iniName, QSettings::IniFormat);
    configIni->setValue("符合计数配置/currentPage",ui->stackCoin->currentIndex());

    // 双通道
    configIni->setValue("符合计数配置/device1",device1);
    configIni->setValue("符合计数配置/device2",device2);
    configIni->setValue("符合计数配置/channel1",channel1);
    configIni->setValue("符合计数配置/channel2",channel2);
    configIni->setValue("符合计数配置/delay",delay);
    configIni->setValue("符合计数配置/delayAcc",delayAcc);
    configIni->setValue("符合计数配置/accumulateTime",accumulateTime);
    configIni->setValue("符合计数配置/enableAccumulateTime",enableAccumulateTime);
    configIni->setValue("符合计数配置/tolerance",tolerance);

    // 多通道
    configIni->setValue("符合计数配置/channelMark1",channelMark[0]); configIni->setValue("符合计数配置/delayMulti1",delayMulti[0]);
    configIni->setValue("符合计数配置/channelMark2",channelMark[1]); configIni->setValue("符合计数配置/delayMulti2",delayMulti[1]);
    configIni->setValue("符合计数配置/channelMark3",channelMark[2]); configIni->setValue("符合计数配置/delayMulti3",delayMulti[2]);
    configIni->setValue("符合计数配置/channelMark4",channelMark[3]); configIni->setValue("符合计数配置/delayMulti4",delayMulti[3]);
    configIni->setValue("符合计数配置/channelMark5",channelMark[4]); configIni->setValue("符合计数配置/delayMulti5",delayMulti[4]);
    configIni->setValue("符合计数配置/channelMark6",channelMark[5]); configIni->setValue("符合计数配置/delayMulti6",delayMulti[5]);
    configIni->setValue("符合计数配置/channelMark1_2",channelMark_2[0]); configIni->setValue("符合计数配置/delayMulti1_2",delayMulti_2[0]);
    configIni->setValue("符合计数配置/channelMark2_2",channelMark_2[1]); configIni->setValue("符合计数配置/delayMulti2_2",delayMulti_2[1]);
    configIni->setValue("符合计数配置/channelMark3_2",channelMark_2[2]); configIni->setValue("符合计数配置/delayMulti3_2",delayMulti_2[2]);
    configIni->setValue("符合计数配置/channelMark4_2",channelMark_2[3]); configIni->setValue("符合计数配置/delayMulti4_2",delayMulti_2[3]);
    configIni->setValue("符合计数配置/channelMark5_2",channelMark_2[4]); configIni->setValue("符合计数配置/delayMulti5_2",delayMulti_2[4]);
    configIni->setValue("符合计数配置/channelMark6_2",channelMark_2[5]); configIni->setValue("符合计数配置/delayMulti6_2",delayMulti_2[5]);
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
    device1 = configIni->value("符合计数配置/device1").toInt();
    device2 = configIni->value("符合计数配置/device2").toInt();
    channel1 = configIni->value("符合计数配置/channel1").toInt();
    channel2 = configIni->value("符合计数配置/channel2").toInt();
    delay = configIni->value("符合计数配置/delay").toInt();
    delayAcc = configIni->value("符合计数配置/delayAcc").toInt();
    accumulateTime = configIni->value("符合计数配置/accumulateTime").toDouble();
    enableAccumulateTime = configIni->value("符合计数配置/enableAccumulateTime").toBool();
    tolerance = configIni->value("符合计数配置/tolerance").toInt();

    // 多通道
    channelMark[0] = configIni->value("符合计数配置/channelMark1").toBool(); delayMulti[0] = configIni->value("符合计数配置/delayMulti1").toInt();
    channelMark[1] = configIni->value("符合计数配置/channelMark2").toBool(); delayMulti[1] = configIni->value("符合计数配置/delayMulti2").toInt();
    channelMark[2] = configIni->value("符合计数配置/channelMark3").toBool(); delayMulti[2] = configIni->value("符合计数配置/delayMulti3").toInt();
    channelMark[3] = configIni->value("符合计数配置/channelMark4").toBool(); delayMulti[3] = configIni->value("符合计数配置/delayMulti4").toInt();
    channelMark[4] = configIni->value("符合计数配置/channelMark5").toBool(); delayMulti[4] = configIni->value("符合计数配置/delayMulti5").toInt();
    channelMark[5] = configIni->value("符合计数配置/channelMark6").toBool(); delayMulti[5] = configIni->value("符合计数配置/delayMulti6").toInt();
    channelMark_2[0] = configIni->value("符合计数配置/channelMark1_2").toBool(); delayMulti_2[0] = configIni->value("符合计数配置/delayMulti1_2").toInt();
    channelMark_2[1] = configIni->value("符合计数配置/channelMark2_2").toBool(); delayMulti_2[1] = configIni->value("符合计数配置/delayMulti2_2").toInt();
    channelMark_2[2] = configIni->value("符合计数配置/channelMark3_2").toBool(); delayMulti_2[2] = configIni->value("符合计数配置/delayMulti3_2").toInt();
    channelMark_2[3] = configIni->value("符合计数配置/channelMark4_2").toBool(); delayMulti_2[3] = configIni->value("符合计数配置/delayMulti4_2").toInt();
    channelMark_2[4] = configIni->value("符合计数配置/channelMark5_2").toBool(); delayMulti_2[4] = configIni->value("符合计数配置/delayMulti5_2").toInt();
    channelMark_2[5] = configIni->value("符合计数配置/channelMark6_2").toBool(); delayMulti_2[5] = configIni->value("符合计数配置/delayMulti6_2").toInt();
    accumulateTimeMulti = configIni->value("符合计数配置/accumulateTimeMulti").toDouble();
    enableAccumulateTimeMulti = configIni->value("符合计数配置/enableAccumulateTimeMulti").toBool();
    toleranceMulti = configIni->value("符合计数配置/toleranceMulti").toInt();
    delete configIni;

    pushUiData();
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

void CoincidenceWidget::on_buttonChangeToMulti_released()
{
    ui->stackCoin->setCurrentIndex(1);
}


void CoincidenceWidget::on_buttonChangeToDual_released()
{
    ui->stackCoin->setCurrentIndex(0);
}
