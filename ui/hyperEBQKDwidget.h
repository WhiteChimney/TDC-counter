#ifndef HYPEREBQKDWIDGET_H
#define HYPEREBQKDWIDGET_H

#include <QWidget>
#include "stdafx.h"
#include "projectInfo.h"

namespace Ui {
class hyperentanglementQKD;
}

class hyperentanglementQKD : public QWidget
{
    Q_OBJECT

public:
    int index;
    explicit hyperentanglementQKD(QWidget *parent = nullptr, int index_QKD=0, int comOffset=0);
    ~hyperentanglementQKD();

signals:
    void returnSignal(int index);
    void requestQKDParam(int index);
    void askDealAcqBankSwitchQKD(int index); // 告知主窗口将内存切换信号与本窗口连接
    void askStopDealAcqBankSwitchQKD(int index);  // 告知主窗口断开内存切换信号与本窗口的连接
    void QKDTimerNeedsSync(int index);            // 告知主窗口时钟需要同步
    void QKDTimerStopsSync(int index);            // 告知主窗口停止同步时钟
//    void QKDSaveDataNeedsSync(int index);       //保存数据的时钟
//    void QKDSaveDataStopsSync(int index);
 public slots:
      void dealAcqThreadBankSwitchQKD(AqT3DataDescriptor* dataDescPtr);  // 内存切换，计算计数
      void dealAcqThreadBankSwitchQKD_2(AqT3DataDescriptor* dataDescPtr_2);  // 内存切换，计算计数
      void dealTimeOut();
      void dealRequestQKDParam(int index,
                                double *delayCN,
                                double *delayCN_2,
                                double freqCOM,
                                int countEvents);
//      void dealSaveDataTimeOut();


private slots:
    void on_ButtonStartQKD_released();     //开始键按下
    void on_ButtonStopQKD_released();      //停止按键按下
    void on_buttonStartRecord_released();  //开始记录按键按下
    void on_buttonStopRecord_released();   //停止记录按键按下
    void on_ButtonRetornQKD_released();

    void on_buttonDataDirectory_released();

    void on_buttonOpenDataDir_released();



private:
    Ui::hyperentanglementQKD *ui;

    // 用于计算
    int channels[6] = {0}, channels_2[6] = {0};
    int nbrChannels = 0, nbrChannels_2 = 0, nbrChannels_X = 0;
    int *axbx11calc, *axbx12calc, *axbx22calc, *axbx11errorcalc, *axbx22errorcalc;
    int *azbz11calc, *azbz12calc, *azbz22calc, *azbz11errorcalc, *azbz22errorcalc;
    int *abandoncalc;
    int toleranceCalc;
    int *delayCalc, *delayCalc_2;
    int nbrCOMdelay[6] = {0};
    int nbrCOMdelay_2[6] = {0};
    int maxNbrCOMdelay = 0;
    int delayInCOM[6] = {0};
    int delayInCOM_2[6] = {0};
    int timeCOMunit;
    int COM_HEAD_X1 = 0, COM_HEAD_X2 = 0, COM_HEAD_compute = 0;

    bool channelMark[6] = {0}; // 是否选择该通道
    int delayMulti[6] = {0}; // 延时
    bool channelMark_2[6] = {0}; // 是否选择该通道
    int delayMulti_2[6] = {0}; // 延时
    double accumulateTimeMulti = 1.0; // 累计时间（不同步）
    int toleranceMulti;               // 符合门宽
    int zperiod;

    int device1, device2;     // 实验中设备1是Alice设备2是Bob的信号
//    int coinwindow;           //符合门宽
    int COM_offset = 0;

    // 用于保存数据
    bool QKDSavable = false;     //判断是否可以保存
    qint64 timeTic, timeToc, timeRelative;   // 记录时间用
    QFile *fQKD = new QFile();
    QTextStream fStream;
    QDateTime dateTime;            // 记录当前时间

    //计算参数
    int nbraxbx11 = 0, nbraxbx12 = 0, nbraxbx22 = 0, nbraxbx11error = 0, nbraxbx22error = 0;
    int nbrazbz11 = 0, nbrazbz12 = 0, nbrazbz22 = 0, nbrazbz11error = 0, nbrazbz22error = 0;
    int nbrnbrabandon = 0;

    // 统计不同密钥结果
    QList<QVector<int>> timeSeq, timeSeqAcc;       // 用于存放时间序列
    QList<QVector<int>> channelSeq, channelSeqAcc; // 用于存放通道序列
    QList<QVector<int>> timeSeq_2, timeSeqAcc_2;       // 用于存放时间序列
    QList<QVector<int>> channelSeq_2, channelSeqAcc_2; // 用于存放通道序列
    QList<QVector<int>> timeSeqX1, timeSeqAccX1, timeSeqX2, timeSeqAccX2;       // 用于存放时间序列
    QList<QVector<int>> channelSeqX1, channelSeqAccX1, channelSeqX2, channelSeqAccX2; // 用于存放通道序列
    double *delayCN, *delayCN_2;           // 各通道固有延时
    double freqCOM;               // TDC COM 重复频率
    int countEvents;

 private:
    QString pathName = "D:/Data";
    QString fileName = "test1";
    QString metaData = "# e.g. 实验条件：……";


public:
    void fetchUiData(), pushUiData(); // 获取与推送 ui 数据
    void changeComOffset(int newOffset);
    void createTempDataFile();


    void computeQKDAcrossDevices_HOLD
                (AqT3DataDescriptor* dataDescPtr,
                 QList<QVector<int>>& timeSeq,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
                 QList<QVector<int>>& channelSeq,    // 升序排列后的时间，与通道编号一一对应
                 int *nbrCOMdelay,
                 int *delayInCOM,
                 int timeCOMunit,
                 int *COM_HEAD);

    void computeQKDAcrossDevices_COMPUTE(
                     QList<QVector<int>> &timeSeqX1,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
                     QList<QVector<int>> &channelSeqX1,    // 升序排列后的时间，与通道编号一一对应
                     QList<QVector<int>> &timeSeqX2,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
                     QList<QVector<int>> &channelSeqX2,    // 升序排列后的时间，与通道编号一一对应
                     int nbrChannelsX1, int nbrChannelsX2,
                     int* channelsX1, int *channelsX2,
                     int* axbx11, int* axbx12, int* axbx22, int* axbx11error, int* axbx22error,
                     int* azbz11, int* azbz12, int* azbz22, int* azbz11error, int* azbz22error,
                     int* abandon,
                     int  period,
                     int tolerance,
                     int *COM_HEAD, int *COM_HEAD_2, int *COM_HEAD_X);

};

#endif // HYPEREBQKDWIDGET_H
