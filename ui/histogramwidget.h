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
    int index;          // 辅助窗口计数
    explicit HistogramWidget(QWidget *parent = nullptr, int index = 0);
    ~HistogramWidget();

signals:
    void returnSignal(int index);                  // 返回信号
    void askDealAcqBankSwitchHist(int index);      // 通知主窗口接上内存切换信号
    void askStopDealAcqBankSwitchHist(int index);  // 通知主窗口断掉内存切换信号

public slots:
    void dealTimeOut();                           // 时间到刷新图
    void dealAcqThreadBankSwitchHist(AqT3DataDescriptor*); // 内存切换时累计计数

private slots:
    void on_buttonReturn_released();
    void on_buttonStart_released();
    void on_buttonStop_released();

private:
    Ui::HistogramWidget *ui;

    // UI 参数
    int channel1, channel2;        // 通道
    double accumulateTime = 1.0;   // 累计时间
    double delay = 0.0;            // 延时

    // 画图参数
    double timeStart, timeStop;    // 起止时间
    double binWidth;               // bin 宽
    int* binHeight;                // bin 高
    int nbrIntervals;              // 区间个数
    QwtPlotHistogram* qwtHistPlot; // 直方图
    QwtInterval* histIntervals;
    QTimer* timerHist;
    void setupPlotStyle();         // 设置画图样式
    void testPlot();               // 画图样式测试

    // 配置文件
    QString iniPath, iniName;

public:
    void fetchUiData(), pushUiData(); // 获取与推送 ui 数据
    void saveToIni(), loadFromIni();  // 保存与读取配置文件
};

#endif // HISTOGRAMWIDGET_H
