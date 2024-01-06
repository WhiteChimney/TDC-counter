#include "heraldqkdwidget.h"
#include "ui_heraldqkdwidget.h"

HeraldQkdWidget::HeraldQkdWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeraldQkdWidget)
{
    ui->setupUi(this);
}

HeraldQkdWidget::~HeraldQkdWidget()
{
    delete ui;
}
