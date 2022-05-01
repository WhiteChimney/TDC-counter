#include "testwidget.h"
#include "ui_testwidget.h"
#include <QDebug>

#include "stdafx.h"

TestWidget::TestWidget(QWidget *parent, int index0) :
    QWidget(parent),
    ui(new Ui::TestWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);        // 在父窗口上显示独立的子窗口
    index = index0;
}

TestWidget::~TestWidget()
{
    delete ui;
}

void TestWidget::on_buttonStart_released()
{
    ui->textResult->setText("start test");
    ui->textResult->setText(testTC890());
    ui->textResult->adjustSize();
}

void TestWidget::on_buttonReturn_released()
{
    emit returnSignal(index);
}
