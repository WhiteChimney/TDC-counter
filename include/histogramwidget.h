#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>
#include <QwtPlot>
#include <QwtPlotHistogram>
#include "stdafx.h"

namespace Ui {
class HistogramWidget;
}

class HistogramWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistogramWidget(QWidget *parent = nullptr, int index = 0);
    ~HistogramWidget();

signals:
    void returnSignal(int index);
    void askDealAcqBankSwitchHist(int index);
    void askStopDealAcqBankSwitchHist(int index);

public slots:
    void dealTimeOut();
    void dealAcqThreadBankSwitchHist(AqT3DataDescriptor*);

private slots:
    void on_buttonReturn_released();
    void on_buttonStart_released();
    void on_buttonStop_released();

private:
    Ui::HistogramWidget *ui;
    int index;
    QwtPlotHistogram* qwtHistPlot;
    QTimer* timerHist;
    int channel1, channel2;
    double accumulateTime = 1.0;
    double delay = 0.0;
    double timeStart, timeStop;
    double binWidth;
    int* binHeight;
    int nbrIntervals;
    QwtInterval* histIntervals;

    QString iniPath, iniName;
    void saveToIni();
    void loadFromIni();
};

#endif // HISTOGRAMWIDGET_H
