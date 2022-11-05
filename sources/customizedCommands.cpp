#include "externalapplicationswidget.h"
#include "ui_externalapplicationswidget.h"

namespace Ui {
class ExternalApplicationsWidget;
}

//  自定义串口操作文件
//  注: nbrSCC（单道计数的指针），vNbrCoin（包含符合计数的指针的向量），serial（串口）
//  dealSingleCountTimeup 指当单道计数计时时间到后，进行什么操作

void ExternalApplicationsWidget::customizedSPcommands_start()
{
    qDebug() << "开始自定义程序";
}

void ExternalApplicationsWidget::customizedSPcommands_stop()
{
    qDebug() << "停止自定义程序";
    ui->textCommand->setText("开始");
}

void ExternalApplicationsWidget::dealSingleCountTimeup()
{
    qDebug() << "单道计数刷新";
    qDebug() << tr("单道计数：") << *nbrSCC;
    qDebug() << tr("第一个与单道计数同步的符合面板的计数：") << *vNbrCoin.at(0);


}
