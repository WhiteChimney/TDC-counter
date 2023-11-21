#ifndef ACQUISITIONTHREAD_H
#define ACQUISITIONTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QWaitCondition>
#include <QDateTime>

#include "AcqirisImport.h"
#include "AcqirisT3Import.h"

class Acqiris_AcquisitionThread : public QThread
{
    Q_OBJECT
public:
    Acqiris_AcquisitionThread(ViSession m_idInstr,
                              AqT3ReadParameters* m_readParamPtr);

protected:
    void run();

private:
    ViStatus status = VI_SUCCESS;      // 设备状态
    ViSession instrId;                 // 设备 id
    AqT3ReadParameters* readParamPtr;
    AqT3DataDescriptor* dataDescPtr;   // 生成读取数据指针结构AqT3DataDescriptor

    bool acqStop;                  // 控制采集进程停止

    QMutex mutex;                      // 用于进程间通信同步
    QWaitCondition *waitCond;

public:
    void setInstrId(ViSession m_instrId);
    void startAcquisition(QWaitCondition *waitCond);
    void stopAcquisition();
    bool isAcquringData();

signals:
    void acqThreadBankSwitch(AqT3DataDescriptor*); // 内存切换
    void acquisitionStarted();
    void runThreadFinished();          // 进程结束
    void acquisitionFinished(ViStatus status);

public slots:
    void dealRunThreadFinished();
};

#endif // ACQUISITIONTHREAD_H
