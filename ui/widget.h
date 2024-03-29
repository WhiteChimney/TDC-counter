#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAbstractSlider>

#include "projectInfo.h"
#include "stdafx.h"
#include "testwidget.h"
#include "histogramwidget.h"
#include "coincidencewidget.h"
#include "statisticswidget.h"
#include "externalapplicationswidget.h"
#include "acqiris_tdc.h"
#include "hyperEBQKDwidget.h"
#include "heraldqkdwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void dealTestReturn(int index);
    // 关闭测试子widget
    void dealAcqThreadStarted();
    void dealAcqThreadStarted_2();
    // 处理采集线程开启
    void dealAcqThreadFinished();
    void dealAcqThreadFinished_2();
    // 采集线程关闭后续
    //    void dealAcqThreadBankSwitch(AqT3DataDescriptor*);
    // 当发生 Bank 切换时，可以处理数据
    void dealAcqThreadBankSwitchSCC(AqT3DataDescriptor *);
    void dealAcqThreadBankSwitchSCC_2(AqT3DataDescriptor *);
    // 计算单道计数
    void dealCountTimeOut();
    // 定时刷新单道计数
    void dealCoincidenceReturn(int index);
    // 关闭符合子窗口
    void dealRequestCoinParam(int index);
    void dealRequestQKDParam(int index);

    void dealAskDealAcqBankSwitchCoin(int index, int computeMode);
    // 将 Bank 切换信号与符合子窗口的槽对接
    void dealAskStopDealAcqBankSwitchCoin(int index);
    // 停止对接 Bank 切换信号与符合子窗口的槽
    void dealCoinTimerNeedsSync(int index);
    // 将符合子窗口的定时转为单道计数主时钟，以同步
    void dealCoinTimerStopsSync(int index);
    // 符合子窗口同步停止
    void dealHistogramReturn(int index);
    // 关闭直方图子窗口
    void dealRequestHistParam(int index);

    void dealAskDealAcqBankSwitchHist(int index, int computeMode);
    // 对接 Bank 切换与直方图槽函数
    void dealAskStopDealAcqBankSwitchHist(int index);
    // 停止将数据传入直方图
    void dealRecordSingleCount();
    // 记录单道计数
    void dealRecordTimeOut();
    // 数据记录倒计时结束

    void dealStatisticsReturn();
    // 关闭统计子窗口
    void dealStatisticsRequestSync();
    // 统计时钟同步
    void dealStatisticsRequestStopSync();
    // 统计时钟停止同步

    void dealQKDReturn(int index);
    // 关闭QKD子窗口
    void dealAskDealAcqBankSwitchQKD(int index);
    // 将 Bank 切换信号与符合子窗口的槽对接
    void dealAskStopDealAcqBankSwitchQKD(int index);
    void dealQKDTimerNeedsSync(int index);
    // 将符合子窗口的定时转为单道计数主时钟，以同步
    void dealQKDTimerStopsSync(int index);
    //    void dealQKDSaveDataNeedsSync(int index);
    //    void dealQKDSaveDataStopsSync(int index);

    void dealHeraldQkdReturn();
    // 关闭标记 QKD 子窗口
    void dealHeraldQkdRequestParam();
    // 处理数据传递
    void dealHeraldQkdRequestSync();
    // 标记 QKD 时钟同步
    void dealHeraldQkdRequestStopSync();
    // 标记 QKD 时钟停止同步

private slots:
    void on_buttonTempFilePath_released();
    // 打开临时文件目录
    void on_buttonTest_released();
    // 生成测试子 widget
    void on_buttonExit_released();
    // 退出按键按下
    void on_buttonStartAcquisition_released();
    // 开始采集
    void on_buttonStopAcq_released();
    // 停止采集线程
    void on_buttonStartCount_released();
    // 开启计数功能
    void on_buttonStopCount_released();
    // 停止单道计数与计时
    void on_buttonHistogram_released();
    // 开启直方图子窗口
    void on_buttonCoincidence_released();
    // 开启符合子窗口
    void on_checkboxEnableCountEvents_released();
    // 配置 TDC 时，未勾选手动设置 Count Events 会将输入框变灰
    void on_buttonOpenDataDir_released();
    // 打开数据文件夹
    void on_buttonDataDirectory_released();
    // 保存文件时路径选择
    void on_buttonStartRecord_released();
    // 保存数据
    void on_buttonStopRecord_released();
    // 停止记录单道计数
    void on_checkboxRecordTime_released();
    // 当未选择记录时长时，不可更改记录时间
    void on_buttonStatistics_released();

    void on_buttonExternalApplications_released();

    void on_buttonCheckUpdate_released();

    void on_textComOffset_editingFinished();

    void on_buttonMinus1_released();

    void on_buttonPlus1_released();

    void on_buttonMinus100_released();

    void on_buttonPlus100_released();

    void on_pushButtonQKD_released();

    void on_dialComOffset_valueChanged(int value);

    //    void on_buttonDelayStart_released();

    //    void on_buttonDelayStop_released();

    void on_buttonQkd_released();

private:
    Ui::Widget *ui;
    // 测试子窗口（序列）
    QVector<TestWidget *> vTestWidget;
    TestWidget *testW;
    // 符合子窗口（序列）
    QVector<CoincidenceWidget *> vCoinWidget;
    CoincidenceWidget *coinW;
    QVector<bool> vCoinWidgetSyncState;
    // 直方图子窗口（序列）
    QVector<HistogramWidget *> vHistWidget;
    HistogramWidget *histW;
    StatisticsWidget *statW;
    bool statWidgetLaunched = false;
    // QKD子窗口（序列）
    QVector<hyperentanglementQKD *> vQKDWidget;
    hyperentanglementQKD *QKDW;
    QVector<bool> vQKDWidgetSyncState;
    HeraldQkdWidget *qkdW;
    bool qkdWidgetLaunched = false;

signals:
    void countParamReady(bool *, int *);
    void coinParamReady(int index,
                        double *delayCN,
                        double *delayCN_2,
                        double freqCOM, int countEvents);
    void histParamReady(int index,
                        double *delayCN,
                        double *delayCN_2,
                        double freqCOM, int countEvents);
    void QKDParamReady(int index,
                       double *delayCN,
                       double *delayCN_2,
                       double freqCOM, int countEvents); // 要修改延时的设置，导入QKD面板的设置

    void qkdParamReady(double *delayCN, double freqCOM);

public:
    void setupAcqIndicator();
    void fetchUiData(), pushUiData();

    // 需要配置的 TDC 参数
private:
    double freqCOM = 1000.0; // kHz
    bool enableCountEvents = false;
    int countEvents = 10; // kHz
    bool channelConfig[NUM_CHANNELS + 1], channelConfig_2[NUM_CHANNELS + 1];
    double level[NUM_CHANNELS + 1] = {0.5}, level_2[NUM_CHANNELS + 1] = {0.5};
    int slope[NUM_CHANNELS + 1] = {0}, slope_2[NUM_CHANNELS + 1] = {0};

    // 采集过程用到的参数/
private:
    Acqiris_TDC *tdc, *tdc_2;
    QList<ViSession> instrIds;
    //    QList<QWaitCondition*> waitConds;
    bool *acqStopPtr = new bool(), *acqStopPtr_2 = new bool();
    QSimpleLed *statusIndicator, *statusIndicator_2;
    int comOffset = 0;

public:
    void updateComOffset();

    // 单道计数参数
private:
    int nbrSCC[NUM_CHANNELS] = {0}, nbrSCC_2[NUM_CHANNELS] = {0};
    int nbrSCCfuture[NUM_CHANNELS] = {0}, nbrSCCfuture_2[NUM_CHANNELS] = {0};
    double delayCN[NUM_CHANNELS + 1] = {0.0}, delayCN_2[NUM_CHANNELS + 1] = {0.0};
    QTimer *timerCount;
    double accumulateTime = 1.0;
    bool countSavable = false;
    // 符合计数指针变量，辅助统计
    QString coinChannelName;
    int *nbrCoinPtr = new int();
    int *nbrAccCoinPtr = new int();

public:
    int *getSingleCountPtr();

    // 数据保存参数
private:
    QString pathName = "D:/Data";
    QString fileName = "test1";
    QString metaData = "# e.g. 实验条件：……";
    bool enableRecordTime = true;
    double recordTime = 10.0;
    QDateTime dateTime;                    // 记录当前时间
    qint64 timeTic, timeToc, timeRelative; // 用来计时
    QTimer *timerFile;                     // 数据记录用时时钟
    RecordMode modeRecord = notRecording;  // 数据记录模式
public:
    void createTempDataFile();
    void mergeDataFiles(RecordMode modeRecord, QString fileName, QString metaData, QVector<CoincidenceWidget *>);

    // 统计参数
public:
    void setupStatUi();

    // 保存配置
private:
    QString iniPath;
    QString iniName;
    QString tempFileName_SCC; // 单道计数临时文件
    QFile *fSingleCount = new QFile();
    QTextStream fStream;

public:
    void saveToIni();
    // 保存当前配置到配置文件
    void loadFromIni();
    // 从配置文件加载配置s

private:
    // 外部功能子窗口（序列）
    ExternalApplicationsWidget *extAppW;

public:
    void setupExtAppWidget();
public slots:
    void dealExtAppRequestData();
    void dealExtAppDataReceived();
    void dealExtAppStopped();
    void dealExtAppRequestDelayFeedback();
    void dealExtAppStopRequestDelayFeedback();
    void dealExtAppRequestAngleFeedback();
    void dealExtAppStopRequestAngleFeedback();
signals:
    void sendExtAppRequestedData(int *nbrSCC, QVector<int *> vNbrCoin);
    void sendExtAppDelayFeedbackData(int *nbrSCC);
    void sendExtAppAngleFeedbackData(int *nbrSCC);
    void mainAppClosing();

    //    // 测试延时板
    // private:
    //    QFile sFile;
    //    QTextStream sStream;
    //    double freq = 9.973e6;              // 重复频率
    //    int period;        // 一个周期对应多少个 TDC 最小单位（50 ps）
    //    int nCOM = 0;
    //    int calcPeriod; // 计算达到时间的平均值，计算周期约为 1e5 个 COM，即 0.1 s
    //    double currentPeriod = 0.0;
    //    double TOF[3] = {0.0};         // 用于存放一个计算周期内的各个通道的时间均值
    //    int nTOF[3] = {0};               // 用于计数当前有多少个时间戳

    // public slots:
    //     void dealDelayTest(AqT3DataDescriptor*);
};
#endif // WIDGET_H
