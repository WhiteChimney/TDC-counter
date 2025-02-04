#include "heraldqkdwidget.h"
#include "ui_heraldqkdwidget.h"

void HeraldQkdWidget::setupLcdCounts()
{
    // 特殊字符串
    // 第 1、2、3 个字符分别为上划线、无、下划线
    // c1、c2 添加了下标
    QStringList aList = { QString::fromUtf8("A\u0305 "), QString::fromUtf8("A "), QString::fromUtf8("A\u0332 ")};
    QStringList bList = { QString::fromUtf8("B\u0305 "), QString::fromUtf8("B "), QString::fromUtf8("B\u0332 ")};
    QStringList c1List = { QString::fromUtf8("C\u0305\u2081 "), QString::fromUtf8("C\u2081 "), QString::fromUtf8("C\u0332\u2081 ")};
    QStringList c2List = { QString::fromUtf8("C\u0305\u2082"), QString::fromUtf8("C\u2082"), QString::fromUtf8("C\u0332\u2082")};

    QLabel *l;
    ClipLcdNumber *c;
    for (int i = 0; i < 9; i++)
    {
        QList<ClipLcdNumber*> v;
        vLcdCounts.append(v);
        for (int j = 0; j < 3; j++)
        {
            l = new QLabel(aList[1]+bList[j]+c1List[i/3]+c2List[i%3],this);
            l->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
            ui->layoutYields->addWidget(l,i,2*j+1,1,1);
            c = new ClipLcdNumber(9,this);
            vLcdCounts[i].append(c);
            ui->layoutYields->addWidget(c,i,2*j+2,1,1);
        }
    }

    l = new QLabel("累积计数",this);
    l->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
    ui->gridLayoutTotalCounts->addWidget(l,0,0,1,1);

    l = new QLabel("主峰",this);
    l->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
    ui->gridLayoutTotalCounts->addWidget(l,1,0,1,1);

    lcdMainPeak = new ClipLcdNumber(9,this);
    ui->gridLayoutTotalCounts->addWidget(lcdMainPeak,2,0,2,1);
    lcdMainPeak->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    lcdMainPeak->setStyleSheet("background-color: skyblue;");

    l = new QLabel("侧峰（之和）",this);
    l->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
    ui->gridLayoutTotalCounts->addWidget(l,4,0,1,1);

    lcdSidePeak = new ClipLcdNumber(9,this);
    ui->gridLayoutTotalCounts->addWidget(lcdSidePeak,5,0,2,1);
    lcdSidePeak->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    lcdSidePeak->setStyleSheet("background-color: skyblue;");
}

void HeraldQkdWidget::fetchUiData()
{

    delayCN[0] = int(20*ui->textDelay1->text().toDouble());
    delayCN[1] = int(20*ui->textDelay2->text().toDouble());
    delayCN[2] = int(20*ui->textDelay3->text().toDouble());
    delayCN[3] = int(20*ui->textDelay4->text().toDouble());
    delayCN[4] = int(20*ui->textDelay5->text().toDouble());
    delayCN[5] = int(20*ui->textDelay6->text().toDouble());

    tolerance = int(20*ui->textTolerance->text().toDouble());

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
    delete configIni;

    pushUiData();
}
