#include "widget.h"
#include "ui_widget.h"

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

void Widget::on_checkBoxNbrCOMbuffer_stateChanged(int arg1)
{
    ui->textNbrCOMbuffer->setEnabled(arg1);
}

void Widget::on_textNbrCOMbuffer_editingFinished()
{
    nbrCOMbuffer = ui->textNbrCOMbuffer->text().toInt();
    if (nbrCOMbuffer < 1)
        nbrCOMbuffer = 1;
    ui->textNbrCOMbuffer->setText(QString::number(nbrCOMbuffer));
}
