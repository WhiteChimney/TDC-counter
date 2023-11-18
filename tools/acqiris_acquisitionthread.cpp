#include "acqiris_acquisitionthread.h"

Acqiris_AcquisitionThread::Acqiris_AcquisitionThread(ViSession m_instrId,
                                                     AqT3ReadParameters* m_readParamPtr)
{
    acqStop = true;
    instrId = m_instrId;
    readParamPtr = m_readParamPtr;
    connect(this,&Acqiris_AcquisitionThread::runThreadFinished,this,&Acqiris_AcquisitionThread::dealRunThreadFinished);
}

void Acqiris_AcquisitionThread::startAcquisition()
{
    acqStop = false;
    this->start();
}

void Acqiris_AcquisitionThread::stopAcquisition()
{
    acqStop = true;
}

void Acqiris_AcquisitionThread::run()
{
    // Calibrate instrument (as we specified not to on init)
    Acqrs_calibrate(instrId);

    // Start acquisitions
    AcqrsT3_acquire(instrId);

    while(!acqStop)
    {
        // 等待读满 bank switch, 8秒超时
        status += AcqrsT3_waitForEndOfAcquisition(instrId, 8000);

        // 读取 acquired 数据 (acquisition continues on other bank)
        dataDescPtr = new AqT3DataDescriptor();
        memset(dataDescPtr, 0, sizeof(*dataDescPtr));
        status += AcqrsT3_readData(instrId, 0, readParamPtr, dataDescPtr);

        status += AcqrsT3_acquire(instrId);

        emit acqThreadBankSwitch(dataDescPtr);

        if (status != VI_SUCCESS)
            acqStop = true;

    }
    emit runThreadFinished();
}

void Acqiris_AcquisitionThread::dealRunThreadFinished()
{
//    status += AcqrsT3_waitForEndOfAcquisition(instrId, 8000);
    status += AcqrsT3_stopAcquisition(instrId);
    this->quit();
    this->wait();
    emit acquisitionFinished(status);
}
