#ifndef ACQUISITIONTHREAD_H
#define ACQUISITIONTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "AcqirisImport.h"
#include "AcqirisT3Import.h"

class Acqiris_AcquisitionThread : public QThread
{
    Q_OBJECT
public:
    Acqiris_AcquisitionThread();

protected:
    void run();

private:
    ViStatus status = VI_SUCCESS;      // 设备状态
    ViSession idInstr;                 // 设备 id
    AqT3ReadParameters* readParamPtr;
    AqT3DataDescriptor* dataDescPtr;   // 生成读取数据指针结构AqT3DataDescriptor

    bool* acqStopPtr;                  // 控制采集进程停止
    bool acqParamReady = false;        // 判断采集数据传入状态
    QMutex mutex;                      // 用于进程间通信同步
    QWaitCondition waitCond;

signals:
    void acqThreadStarted();           // 进程开始
    void acqThreadFinished();          // 进程结束
    void acqThreadBankSwitch(AqT3DataDescriptor*); // 内存切换

public slots:
    void dealAcqParamReady
        (bool* acqStopPtr0,
         ViSession idInstr0,
         AqT3ReadParameters* readParamPtr0); // 处理传入的采集参数

};

#endif // ACQUISITIONTHREAD_H
