#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include "stdafx.h"

namespace Ui {
class StatisticsWidget;
}

class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(QWidget *parent = nullptr);
    ~StatisticsWidget();

signals:
    void sendReturnSignal();

private slots:
    void on_buttonReturn_released();
    void on_buttonStart_released();

private:
    Ui::StatisticsWidget *ui;

private:
    QVector<QString> vChannelName;
//    QVector<int*> vCountPtr001;
    QVector<int> vCountStd;

public:
    void addChannel(QString channelName, int* countPtr);
};

#endif // STATISTICSWIDGET_H
