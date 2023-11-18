#ifndef ACQIRIS_TDC_H
#define ACQIRIS_TDC_H

#include <QObject>

#include "AcqirisImport.h"
#include "AcqirisT3Import.h"

#include "acqiris_acquisitionthread.h"

#define COUNT_EVENTS "10"

class Acqiris_TDC : public QObject
{
    Q_OBJECT
public:
    explicit Acqiris_TDC(QString resourceName,
                         ViSession *instrId,
                         QObject *parent = nullptr);

signals:
    void acqParamReady(bool*,ViSession,AqT3ReadParameters*);

private:
//    仪器参数
    ViRsrc resourceName;
    ViSession *instrId;
    ViStatus status;
    ViStatus configStatus = -1;

//    配置参数
//    double freqCOM;             // kHz
    int countEvents;           // kHz
    bool channelConfig[7];
    double level[7];
    int slope[7];

//    采集参数
    bool* acqStopPtr;
    Acqiris_AcquisitionThread *acqThread;
    AqT3ReadParameters* readParamPtr;

public:
//    仪器初始化
    int initialize();
//    配置参数
    int config();
//    开始采集
    int startAcquisition();
//    停止采集
    int stopAcquisition();
//    关闭仪器
    int close();

    // 处理采集线程开启
    void dealAcqThreadStarted();
    // 采集线程关闭后续
    void dealAcqThreadFinished();
    // 当发生 Bank 切换时，可以处理数据
    void dealAcqThreadBankSwitchSCC(AqT3DataDescriptor*);

    void dealRequestCoinParam(int index);

};

#endif // ACQIRIS_TDC_H
