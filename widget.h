#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "stdafx.h"
#include "testwidget.h"
#include "histogramwidget.h"
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

signals:
    void acqParamReady(bool*,ViSession,AqT3ReadParameters*);
    void countParamReady(bool*, int*);

public slots:
    void dealTestReturn(int index);
    void dealAcqThreadStarted();
    void dealAcqThreadFinished();
    void dealAcqThreadBankSwitch(AqT3DataDescriptor*);
    void dealAcqThreadBankSwitchSCC(AqT3DataDescriptor*);
    void dealCountTimeOut();
    void dealCoincidenceReturn(int index);
    void dealAskDealAcqBankSwitchCoin(int index);
    void dealAskStopDealAcqBankSwitchCoin(int index);
    void dealCoinTimerNeedsSync(int index);
    void dealHistogramReturn(int index);
    void dealAskDealAcqBankSwitchHist(int index);
    void dealAskStopDealAcqBankSwitchHist(int index);
    void dealRecordSingleCount();
    void dealRecordTimeOut();

private slots:
    void on_buttonTempFilePath_released();
    void on_buttonTest_released();
    void on_buttonExit_released();
    void on_buttonStartAcquisition_released();
    void on_buttonStopAcq_released();
    void on_buttonApplyConfig_released();
    void on_buttonStartCount_released();
    void on_buttonStopCount_released();
    void on_buttonHistogram_released();
    void on_buttonCoincidence_released();
    void on_checkboxEnableCountEvents_released();
    void on_buttonOpenDataDir_released();
    void on_buttonDataDirectory_released();
    void on_buttonStartRecord_released();
    void on_buttonStopRecord_released();
    void on_checkboxRecordTime_released();

private:
    Ui::Widget *ui;
    // 测试子窗口（序列）
    QVector<TestWidget*> vTestWidget;        TestWidget *testW;
    // 符合子窗口（序列）
    QVector<CoincidenceWidget*> vCoinWidget; CoincidenceWidget *coinW;
    // 直方图子窗口（序列）
    QVector<HistogramWidget*> vHistWidget;   HistogramWidget *histW;

    // 需要配置的 TDC 参数
private:
    int freqCOM = 1000000;
    bool enableCountEvents = false;
    int countEvents = 100000;
    bool channelConfig[7];
    double level[7] = {0.5};
    int slope[7] = {0};

    // 采集过程用到的参数
private:
    AcquisitionThread *acqThread;
    bool* acqStopPtr = new bool();
    ViSession idInstr;
    ViStatus status;
    ViStatus configStatus = 1;
    AqT3ReadParameters* readParamPtr = new AqT3ReadParameters();

    // 单道计数参数
private:
    int nbrSCC[6] = {0};
    QTimer *timerCount;
    double accumulateTime = 1.0;
    bool countSavable = false;

    // 数据保存参数
private:
    QString pathName = "D:/Data";
    QString fileName = "test1";
    QString metaData = "# e.g. 实验条件：……";
    bool enableRecordTime = true;
    double recordTime = 10.0;
    QDateTime dateTime;                        // 记录当前时间
    qint64 timeTic, timeToc, timeRelative;     // 用来计时
    QTimer* timerFile;                         // 数据记录用时时钟
    RecordMode modeRecord = notRecording;      // 数据记录模式
public:
    void mergeDataFiles(RecordMode modeRecord, QString fileName, QString metaData, QVector<CoincidenceWidget*>);

    // 保存配置
private:
    QString iniPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/AcqirisTDC_qt";
    QString iniName = iniPath + "/Configurations/general.ini";
    QString tempFileName_SCC = iniPath + "/Data/SingleCount.txt";   // 单道计数临时文件
    QFile *fSingleCount = new QFile();
    QTextStream fStream;
public:
    void saveToIni();
    void loadFromIni();

};
#endif // WIDGET_H
