#ifndef ACQIRIS_TDC_H
#define ACQIRIS_TDC_H

#include <QObject>
#include <QDebug>

#include "AcqirisImport.h"
#include "AcqirisT3Import.h"

#include "acqiris_acquisitionthread.h"

#define NUM_CHANNELS 6

class Acqiris_TDC : public QObject
{
    Q_OBJECT
public:
    explicit Acqiris_TDC(QString m_resourceName,
                         QObject *parent = nullptr);

private:
//    仪器参数
    QString resourceName;
    ViSession instrId;
    ViStatus status;

//    采集参数
    Acqiris_AcquisitionThread *acqThread;
    AqT3ReadParameters* readParamPtr;

public:
//    仪器初始化
    int initialize();
//    配置参数
    int config(bool(&channelConfig)[NUM_CHANNELS+1],
                    double(&level)[NUM_CHANNELS+1],
                    int(&slope)[NUM_CHANNELS+1],
                    int countEvents);
//    开始采集
    void startAcquisition();
//    停止采集
    void stopAcquisition();
//    获取仪器的状态信息
    int getStatus();
//    获取仪器的状态信息
    bool isAcquringData();
//    关闭仪器
    int close();

public slots:

    void dealReadyToAcquireData();
    // 当发生 Bank 切换时，可以处理数据
    void dealAcqThreadBankSwitch(AqT3DataDescriptor*);

    void dealAcqThreadStarted();
    // 采集线程关闭后续
    void dealAcqThreadFinished(ViStatus m_status);

signals:
    void readyToAcquireData(ViSession instrId);
    void acquisitionStarted();
    void acquisitionFinished(ViStatus status);
    void dataReturned(AqT3DataDescriptor*);

};

#endif // ACQIRIS_TDC_H
