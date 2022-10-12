#ifndef COINCIDENCEWIDGET_H
#define COINCIDENCEWIDGET_H

#include <QWidget>
#include "stdafx.h"

namespace Ui {
class CoincidenceWidget;
}

class CoincidenceWidget : public QWidget
{
    Q_OBJECT

public:
    int index;        // 用以辅助标记窗口号
    explicit CoincidenceWidget(QWidget *parent = nullptr, int index = 0);
    ~CoincidenceWidget();

signals:
    void returnSignal(int index);                  // 处理返回按键按下
    void askDealAcqBankSwitchCoin(int index);      // 告知主窗口将内存切换信号与本窗口连接
    void askStopDealAcqBankSwitchCoin(int index);  // 告知主窗口断开内存切换信号与本窗口的连接
    void coinTimerNeedsSync(int index);            // 告知主窗口时钟需要同步
    void coinTimerStopsSync(int index);            // 告知主窗口停止同步时钟

public slots:
    void dealTimeOut();                                     // 累计时间到，刷新计数
    void dealAcqThreadBankSwitchCoin(AqT3DataDescriptor*);  // 内存切换，计算计数
    void dealSaveCoinData();                                // 保存数据

private slots:
    void on_buttonReturn_released();                       // 返回按键按下
    void on_buttonStart_released();                        // 开始按键按下
    void on_buttonStop_released();                         // 停止按键按下
    void on_coinChannel1_currentIndexChanged(int index);   // 辅助选择符合通道，避免产生两个通道选择一样的情况
    void on_checkboxAccumlateTime_stateChanged(int checkState);  // 勾选同步时，时间输入框不可编辑
    void on_buttonChangeToMulti_released();
    void on_buttonChangeToDual_released();

private:
    Ui::CoincidenceWidget *ui;

    // UI 参数
    // 双通道
    int channel1, channel2;      // 通道
    int tolerance;               // 符合门宽
    int delay, delayAcc;         // 延时，偶然符合额外延时
    bool enableAccumulateTime;   // 时钟同步勾选框
    double accumulateTime = 1.0; // 累计时间（不同步）

    // 多通道
    bool channelMulti[6] = {0}; // 是否选择该通道
    int toleranceMulti;               // 符合门宽
    int delayMulti[6] = {0}; // 延时
    bool enableAccumulateTimeMulti;   // 时钟同步勾选框
    double accumulateTimeMulti = 1.0; // 累计时间（不同步）


    // 计数参数
    QTimer* timerCoin;         // 符合专用时钟
    int nbrCoin = 0;           // 符合计数
    int nbrAccCoin = 0;        // 偶然符合计数
    int nbrCoinMulti = 0;      // 多通道符合计数

    // 数据保存
    bool coinSavable = false;  // 判断是否可保存
    qint64 timeTic, timeToc;   // 记录时间用
    QString iniPath, iniName;  // 配置文件
    QString tempFileName;      // 临时文件
    QFile *fCoin = new QFile();
    QTextStream fStream;

public:
    void fetchUiData(), pushUiData(); // 获取与推送 Ui 数据
    void createTempDataFile();
    void saveToIni(), loadFromIni();  // 保存与读取配置
    void startRecordCoinLocal(), stopRecordCoinLocal(); // 开始与停止保存计数（不与单道同步）
    void getCoinParam(QString* coinChannelName, int** nbrCoinPtr, int** nbrAccCoinPtr);
    void getCoinParam(QString* coinChannelName, int** nbrCoinPtr);
};

#endif // COINCIDENCEWIDGET_H
