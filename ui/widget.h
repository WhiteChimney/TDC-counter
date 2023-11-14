#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "projectInfo.h"
#include "stdafx.h"
#include "testwidget.h"
#include "coincidencewidget.h"
#include "acquisitionthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
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
    // 处理采集线程开启
    void dealAcqThreadFinished();
    // 采集线程关闭后续
//    void dealAcqThreadBankSwitch(AqT3DataDescriptor*);
    // 当发生 Bank 切换时，可以处理数据
    void dealAcqThreadBankSwitchSCC(AqT3DataDescriptor*);
    // 计算单道计数
    void dealCountTimeOut();
    // 定时刷新单道计数
    void dealCoincidenceReturn(int index);
    // 关闭符合子窗口
    void dealRequestCoinParam(int index);

    void dealAskDealAcqBankSwitchCoin(int index);
    // 将 Bank 切换信号与符合子窗口的槽对接
    void dealAskStopDealAcqBankSwitchCoin(int index);
    // 停止对接 Bank 切换信号与符合子窗口的槽
    void dealCoinTimerNeedsSync(int index);
    // 将符合子窗口的定时转为单道计数主时钟，以同步
    void dealCoinTimerStopsSync(int index);
    // 符合子窗口同步停止

private slots:
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
    void on_buttonCoincidence_released();
    // 开启符合子窗口

private:
    Ui::Widget *ui;
    // 测试子窗口（序列）
    QVector<TestWidget*> vTestWidget;        TestWidget *testW;
    // 符合子窗口（序列）
    QVector<CoincidenceWidget*> vCoinWidget; CoincidenceWidget *coinW;
    QVector<bool> vCoinWidgetSyncState;

signals:
    void countParamReady(bool*, int*);
    void coinParamReady(int index, double *delayCN, double freqCOM);

public:
    void setupAcqIndicator();
    void fetchUiData(), pushUiData();

    // 需要配置的 TDC 参数
private:
    double freqCOM = 1000.0;             // kHz
    int countEvents = 10;           // kHz
    bool channelConfig[7];
    double level[7] = {0.5};
    int slope[7] = {0};

    // 采集过程用到的参数
signals:
    void acqParamReady(bool*,ViSession,AqT3ReadParameters*);
private:
    AcquisitionThread *acqThread;
    bool* acqStopPtr = new bool();
    ViSession idInstr;
    ViStatus status;
    ViStatus configStatus = -1;
    AqT3ReadParameters* readParamPtr = new AqT3ReadParameters();
    QSimpleLed *statusIndicator = new QSimpleLed(this);
    QVector<AqT3DataDescriptor*> dataPtrList;

    // 单道计数参数
private:
    int nbrSCC[6] = {0};
    int nbrSCCfuture[6] = {0};
    double delayCN[7] = {0.0};
    QTimer *timerCount;
    double accumulateTime = 1.0;
    bool countSavable = false;
    // 符合计数指针变量，辅助统计
    QString coinChannelName;
    int *nbrCoinPtr = new int();
    int *nbrAccCoinPtr = new int();
    int nbrCOMbuffer = 1;
public:
    int* getSingleCountPtr();


};
#endif // WIDGET_H
