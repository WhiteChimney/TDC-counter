#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include "stdafx.h"
#include "statisticschannel.h"

namespace Ui {
class StatisticsWidget;
}

class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(double unitTime, QWidget *parent = nullptr);
    ~StatisticsWidget();

signals:
    void sendReturnSignal();
    void statisticsRequestSync();
    void statisticsRequestStopSync();

public slots:
    void dealTimeOut();

private slots:
    void on_buttonReturn_released();
    void on_buttonStart_released();
    void on_buttonStop_released();

private:
    Ui::StatisticsWidget *ui;

private:
    QVector<StatisticsChannel*> vStatChannel;
    double unitTime, statTime;
    int stepsTotal, stepCurrent;

public:
    void addChannel(QString channelName, int* countPtr);
};

#endif // STATISTICSWIDGET_H
