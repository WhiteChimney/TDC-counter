#include "heraldqkdwidget.h"
#include "ui_heraldqkdwidget.h"


void HeraldQkdWidget::fetchUiData()
{

    delayCN[0] = int(20*ui->textDelay1->text().toDouble());
    delayCN[1] = int(20*ui->textDelay2->text().toDouble());
    delayCN[2] = int(20*ui->textDelay3->text().toDouble());
    delayCN[3] = int(20*ui->textDelay4->text().toDouble());
    delayCN[4] = int(20*ui->textDelay5->text().toDouble());
    delayCN[5] = int(20*ui->textDelay6->text().toDouble());

    tolerance = int(20*ui->textTolerance->text().toDouble());

    deadTime = int(20*ui->textDeadTime->text().toDouble());

    pushUiData();
}

void HeraldQkdWidget::pushUiData()
{
    ui->textDelay1->setText(QString::number(delayCN[0]/20.0));
    ui->textDelay2->setText(QString::number(delayCN[1]/20.0));
    ui->textDelay3->setText(QString::number(delayCN[2]/20.0));
    ui->textDelay4->setText(QString::number(delayCN[3]/20.0));
    ui->textDelay5->setText(QString::number(delayCN[4]/20.0));
    ui->textDelay6->setText(QString::number(delayCN[5]/20.0));
    ui->textTolerance->setText(QString::number(tolerance/20.0));
    ui->textDeadTime->setText(QString::number(deadTime/20.0));
}

void HeraldQkdWidget::saveToIni()
{
    fetchUiData();

    QSettings *configIni = new QSettings(iniName, QSettings::IniFormat);
    configIni->setValue("标记 QKD 配置/delay1",delayCN[0]);
    configIni->setValue("标记 QKD 配置/delay2",delayCN[1]);
    configIni->setValue("标记 QKD 配置/delay3",delayCN[2]);
    configIni->setValue("标记 QKD 配置/delay4",delayCN[3]);
    configIni->setValue("标记 QKD 配置/delay5",delayCN[4]);
    configIni->setValue("标记 QKD 配置/delay6",delayCN[5]);
    configIni->setValue("标记 QKD 配置/tolerance",tolerance);
    configIni->setValue("标记 QKD 配置/deadTime",deadTime);

    delete configIni;
}

void HeraldQkdWidget::loadFromIni()
{
    QSettings *configIni = new QSettings(iniName, QSettings::IniFormat);
    delayCN[0] = configIni->value("标记 QKD 配置/delay1").toInt();
    delayCN[1] = configIni->value("标记 QKD 配置/delay2").toInt();
    delayCN[2] = configIni->value("标记 QKD 配置/delay3").toInt();
    delayCN[3] = configIni->value("标记 QKD 配置/delay4").toInt();
    delayCN[4] = configIni->value("标记 QKD 配置/delay5").toInt();
    delayCN[5] = configIni->value("标记 QKD 配置/delay6").toInt();
    tolerance = configIni->value("标记 QKD 配置/tolerance").toInt();
    deadTime = configIni->value("标记 QKD 配置/deadTime").toInt();
    delete configIni;

    pushUiData();
}
