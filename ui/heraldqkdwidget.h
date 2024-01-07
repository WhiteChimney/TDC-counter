#ifndef HERALDQKDWIDGET_H
#define HERALDQKDWIDGET_H

#include <QWidget>
#include <QDebug>

#include "cliplcdnumber.h"

#include "stdafx.h"

namespace Ui {
class HeraldQkdWidget;
}

class HeraldQkdWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HeraldQkdWidget(QWidget *parent = nullptr);
    ~HeraldQkdWidget();

private slots:
    void on_buttonStart_released();
    void on_buttonStop_released();
    void on_buttonReturn_released();

private:
    Ui::HeraldQkdWidget *ui;

    int nbrCOMdelay[6] = {0};
    int maxNbrCOMdelay = 0;
    int delayInCOM[6] = {0};
    int timeCOMunit;
    int COM_HEAD = 0;

    int tolerance;
    int delayCN[6] = {0};

    QList<QList<ClipLcdNumber*>> vLcdCounts;
    long long vCounts[4][4] = {{0}};
    QList<QVector<int>> timeSeq;       // 用于存放时间序列
    QList<QVector<int>> channelSeq; // 用于存放通道序列
    double freqCOM;

signals:
    void sendReturnSignal();
    void heraldQkdRequestParam();
    void heraldQkdRequestSync();
    void heraldQkdRequestStopSync();

public slots:
    void dealQkdParamReceived(double *m_delayCN, double m_freqCOM);
    void dealDataReturned(AqT3DataDescriptor *dataDescPtr);
    void dealTimeOut();

public:
    void setupLcdCounts();
};

#endif // HERALDQKDWIDGET_H
