#ifndef ACQUISITIONTHREAD_H
#define ACQUISITIONTHREAD_H

#include <QObject>
#include <QThread>

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

public:
    void startAcquisition();
    void stopAcquisition();

signals:
    void acqThreadBankSwitch(AqT3DataDescriptor*); // 内存切换
    void runThreadFinished();          // 进程结束
    void acquisitionFinished(ViStatus status);

public slots:
    void dealRunThreadFinished();
};

#endif // ACQUISITIONTHREAD_H
