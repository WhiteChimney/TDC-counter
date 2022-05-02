#include "acquisitionthread.h"

AcquisitionThread::AcquisitionThread()
{

}

void AcquisitionThread::run()
{
    emit acqThreadStarted();

    // 等待传入采集参数
    mutex.lock();
    waitCond.wait(&mutex);
    mutex.unlock();

    // Calibrate instrument (as we specified not to on init)
    Acqrs_calibrate(idInstr);

    // Start acquisitions
    AcqrsT3_acquire(idInstr);

    while(!*acqStopPtr)
    {
        // 等待读满 bank switch, 8秒超时
        status += AcqrsT3_waitForEndOfAcquisition(idInstr, 8000);

        // 读取 acquired 数据 (acquisition continues on other bank)
        dataDescPtr = new AqT3DataDescriptor();
        memset(dataDescPtr, 0, sizeof(*dataDescPtr));
        status += AcqrsT3_readData(idInstr, 0, readParamPtr, dataDescPtr);

        status += AcqrsT3_acquire(idInstr);

        if (status != VI_SUCCESS)
            *acqStopPtr = true;

        emit acqThreadBankSwitch(dataDescPtr);

    }
    emit acqThreadFinished();
}

void AcquisitionThread::dealAcqParamReady
        (bool* acqStopPtr0,
         ViSession idInstr0,
         AqT3ReadParameters* readParamPtr0)
{
    mutex.lock();
    acqStopPtr = acqStopPtr0;
    idInstr = idInstr0;
    readParamPtr = readParamPtr0;
    waitCond.wakeOne();
    mutex.unlock();
}
