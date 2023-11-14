#include "coincidencewidget.h"
#include "ui_coincidencewidget.h"

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
