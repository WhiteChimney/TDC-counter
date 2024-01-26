#ifndef COINCIDENCEWIDGET_H
#define COINCIDENCEWIDGET_H

#include <QWidget>
#include "stdafx.h"
#include "projectInfo.h"

namespace Ui {
class CoincidenceWidget;
}

class CoincidenceWidget : public QWidget
{
    Q_OBJECT

public:
    int index;        // 用以辅助标记窗口号
    explicit CoincidenceWidget(QWidget *parent = nullptr, int index = 0, int comOffset = 0);
    ~CoincidenceWidget();

signals:
    void returnSignal(int index);                  // 处理返回按键按下
    void requestCoinParam(int index);              // 请求符合用数据
    void askDealAcqBankSwitchCoin(int index, int computeMode);
                                                    // 告知主窗口将内存切换信号与本窗口连接
    void askStopDealAcqBankSwitchCoin(int index);  // 告知主窗口断开内存切换信号与本窗口的连接
    void coinTimerNeedsSync(int index);            // 告知主窗口时钟需要同步
    void coinTimerStopsSync(int index);            // 告知主窗口停止同步时钟

public slots:
    void dealTimeOut();                                     // 累计时间到，刷新计数
    void dealRequestCoinParam(int index,
                              double *delayCN,
                              double *delayCN_2,
                              double freqCOM,
                              int countEvents);
    void dealAcqThreadBankSwitchCoin(AqT3DataDescriptor* dataDescPtr);  // 内存切换，计算计数
    void dealAcqThreadBankSwitchCoin_2(AqT3DataDescriptor* dataDescPtr_2);  // 内存切换，计算计数
    void dealSaveCoinData();                                // 保存数据

private slots:
    void on_buttonReturn_released();                       // 返回按键按下
    void on_buttonStart_released();                        // 开始按键按下
    void on_buttonStop_released();                         // 停止按键按下
    void on_coinChannel1_currentIndexChanged(int index);   // 辅助选择符合通道，避免产生两个通道选择一样的情况
    void on_checkboxAccumlateTime_stateChanged(int checkState);  // 勾选同步时，时间输入框不可编辑
    void on_buttonChangeToMulti_released();
    void on_buttonChangeToDual_released();

    void on_checkboxAccumlateTime_Multi_stateChanged(int arg1);

private:
    Ui::CoincidenceWidget *ui;

    // 用于计算的符合参数
    int channels[6] = {0}, channels_2[6] = {0};
    int nbrChannels = 0, nbrChannels_2 = 0, nbrChannels_X = 0;
    int *nbrCoinCalc;
    int toleranceCalc;
    int *delayCalc, *delayCalc_2;
    int nbrCOMdelay[6] = {0}, nbrCOMdelayAcc[6] = {0};
    int nbrCOMdelay_2[6] = {0}, nbrCOMdelayAcc_2[6] = {0};
    int maxNbrCOMdelay = 0, maxNbrCOMdelayAcc = 0;
    int delayInCOM[6] = {0}, delayInCOMAcc[6] = {0};
    int delayInCOM_2[6] = {0}, delayInCOMAcc_2[6] = {0};
    int timeCOMunit;
    int COM_HEAD = 0, COM_HEAD_2 = 0;
    int COM_HEAD_X1 = 0, COM_HEAD_X2 = 0, COM_HEAD_compute = 0, COM_offset = 0;
    int COM_START = 0, COM_START_2 = 0; // 这个是两个 TDC 固有的 COM 编号差（比如因启动时间差异导致）
    bool COM_START_REGISTERED = false, COM_START_2_REGISTERED = false;

    // 双通道
    int device1, device2;
    int channel1, channel2;      // 通道
    int tolerance;               // 符合门宽
    int delay, delayAcc;         // 延时，偶然符合额外延时
    bool enableAccumulateTime;   // 时钟同步勾选框
    double accumulateTime = 1.0; // 累计时间（不同步）
    int computeMode;

    // 多通道
    bool channelMark[6] = {0}; // 是否选择该通道
    int delayMulti[6] = {0}; // 延时
    bool channelMark_2[6] = {0}; // 是否选择该通道
    int delayMulti_2[6] = {0}; // 延时
    int toleranceMulti;               // 符合门宽
    bool enableAccumulateTimeMulti;   // 时钟同步勾选框
    double accumulateTimeMulti = 1.0; // 累计时间（不同步）

    // 计数参数
    QTimer* timerCoin;         // 符合专用时钟
    int nbrCoin = 0;           // 符合计数
    int nbrAccCoin = 0;        // 偶然符合计数
    int nbrCoinMulti = 0;      // 多通道符合计数
    QList<QVector<int>> timeSeq, timeSeqAcc;       // 用于存放时间序列
    QList<QVector<int>> channelSeq, channelSeqAcc; // 用于存放通道序列
    QList<QVector<int>> timeSeq_2, timeSeqAcc_2;       // 用于存放时间序列
    QList<QVector<int>> channelSeq_2, channelSeqAcc_2; // 用于存放通道序列
    QList<QVector<int>> timeSeqX1, timeSeqAccX1, timeSeqX2, timeSeqAccX2;       // 用于存放时间序列
    QList<QVector<int>> channelSeqX1, channelSeqAccX1, channelSeqX2, channelSeqAccX2; // 用于存放通道序列
    double *delayCN, *delayCN_2;           // 各通道固有延时
    double freqCOM;               // TDC COM 重复频率
    int countEvents;

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
    bool getCoinParam(QString* coinChannelName, int** nbrCoinPtr, int** nbrAccCoinPtr);
    int* getCoinCountPtr();
    int* getAccCoinCountPtr();

    void changeComOffset(int newOffset);
};

#endif // COINCIDENCEWIDGET_H
