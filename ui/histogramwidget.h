#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>
#include <QwtPlot>
#include <QwtPlotHistogram>
#include "stdafx.h"
#include "projectInfo.h"
#include "acqiris_tdc.h"

namespace Ui {
class HistogramWidget;
}

class HistogramWidget : public QWidget
{
    Q_OBJECT

public:
    int index;          // 辅助窗口计数
    explicit HistogramWidget(QWidget *parent = nullptr, int index = 0, int comOffset = 0);
    ~HistogramWidget();

signals:
    void returnSignal(int index);                  // 返回信号
    void requestHistParam(int index);              // 请求用于画直方图的参数
    void askDealAcqBankSwitchHist(int index, int computeMode);      // 通知主窗口接上内存切换信号
    void askStopDealAcqBankSwitchHist(int index);  // 通知主窗口断掉内存切换信号

public slots:
    void dealTimeOut();                           // 时间到刷新图
    void dealRequestHistParam(int index,
                              double *delayCN,
                              double *delayCN_2,
                              double freqCOM, int countEvents);
    void dealAcqThreadBankSwitchHist(AqT3DataDescriptor* dataDescPtr); // 内存切换时累计计数
    void dealAcqThreadBankSwitchHist_2(AqT3DataDescriptor* dataDescPtr_2); // 内存切换时累计计数

private slots:
    void on_buttonReturn_released();
    void on_buttonStart_released();
    void on_buttonStop_released();

private:
    Ui::HistogramWidget *ui;

    // UI 参数
    int device1, device2;
    int computeMode;
    int channel1, channel2;        // 通道
    double accumulateTime = 1.0;   // 累计时间
    double delay = 0.0;            // 延时

    // TDC 参数
    QVector<QVector<double>> timeSeq1, timeSeq2, timeSeq1_2, timeSeq2_2;
    QVector<QVector<double>> timeSeqX1, timeSeqX2;
    int nbrCOMdelay[NUM_CHANNELS] = {0}, nbrCOMdelay_2[NUM_CHANNELS] = {0};
    int maxNbrCOMdelay = 0;
    int delayInCOM[NUM_CHANNELS] = {0}, delayInCOM_2[NUM_CHANNELS] = {0};
    int timeCOMunit;
    int COM_HEAD = 0, COM_HEAD_2 = 0;
    int COM_HEAD_X1 = 0, COM_HEAD_X2 = 0, COM_HEAD_compute = 0, COM_offset = 0;
    double *delayCN, *delayCN_2;           // 各通道固有延时
    double freqCOM;
    int countEvents;               // TDC COM 重复频率
//    int dataToBeRemoved = 0, dataToBeRemoved_2 = 0;
    int comRange = 0;
    int COM_START = 0, COM_START_2 = 0; // 这个是两个 TDC 固有的 COM 编号差（比如因启动时间差异导致）

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
    void changeComOffset(int newOffset);
};

#endif // HISTOGRAMWIDGET_H
