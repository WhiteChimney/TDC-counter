#ifndef ACQUISITIONTHREAD_H
#define ACQUISITIONTHREAD_H

#include <QObject>

#include "stdafx.h"

class AcquisitionThread : public QThread
{
    Q_OBJECT
public:
    AcquisitionThread();

protected:
    void run();

private:
    bool* acqStopPtr;
    ViStatus status = VI_SUCCESS;
    ViSession idInstr;
    AqT3ReadParameters* readParamPtr;
    AqT3DataDescriptor* dataDescPtr; //生成读取数据指针结构AqT3DataDescriptor
    bool acqParamReady = false;
    QReadWriteLock* lockDataDesc;
    QMutex mutex;
    QWaitCondition waitCond;

signals:
    void acqThreadStarted();
    void acqThreadFinished();
    void acqThreadBankSwitch(AqT3DataDescriptor*);

public slots:
    void dealAcqParamReady(bool*, ViSession, AqT3ReadParameters*);

};

#endif // ACQUISITIONTHREAD_H
