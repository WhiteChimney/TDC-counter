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

void Widget::on_checkboxEnableCountEvents_released()
{
    if (ui->checkboxEnableCountEvents->isChecked())
        ui->textCountEvents->setEnabled(true);
    else
        ui->textCountEvents->setEnabled(false);
}
