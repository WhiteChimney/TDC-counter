#include "acqiris_acquisitionthread.h"

Acqiris_AcquisitionThread::Acqiris_AcquisitionThread(ViSession m_instrId,
                                                     AqT3ReadParameters* m_readParamPtr)
{
    acqStop = true;
    instrId = m_instrId;
    readParamPtr = m_readParamPtr;
    connect(this,&Acqiris_AcquisitionThread::runThreadFinished,this,&Acqiris_AcquisitionThread::dealRunThreadFinished);
}

void Acqiris_AcquisitionThread::setInstrId(ViSession m_instrId)
{
    instrId = m_instrId;
}

void Acqiris_AcquisitionThread::startAcquisition(QWaitCondition *m_waitCond)
{
    waitCond = m_waitCond;
    this->start();
}

void Acqiris_AcquisitionThread::stopAcquisition()
{
    acqStop = true;
}

bool Acqiris_AcquisitionThread::isAcquringData()
{
    return !acqStop;
}

void Acqiris_AcquisitionThread::run()
{
    // Calibrate instrument (as we specified not to on init)
    status = Acqrs_calibrate(instrId);
    if (status != VI_SUCCESS) return;

    // 暂停线程，等待同步
    mutex.lock();
    emit acquisitionStarted();
    waitCond->wait(&mutex);
    mutex.unlock();

    QDateTime time = QDateTime::currentDateTime();
    qDebug() << "current time: " << time.currentMSecsSinceEpoch();
    qDebug() << "acquisition started: " << instrId;

    // Start acquisitions
    status = AcqrsT3_acquire(instrId);
    if (status != VI_SUCCESS) return;

    acqStop = false;

    while(!acqStop)
    {
        // 等待读满 bank switch, 8秒超时
        status = AcqrsT3_waitForEndOfAcquisition(instrId, 8000);
        if (status != VI_SUCCESS)
        {
            acqStop = true;
            return;
        }

        // 读取 acquired 数据 (acquisition continues on other bank)
        dataDescPtr = new AqT3DataDescriptor();
        memset(dataDescPtr, 0, sizeof(*dataDescPtr));
        status = AcqrsT3_readData(instrId, 0, readParamPtr, dataDescPtr);
        if (status != VI_SUCCESS)
        {
            acqStop = true;
            return;
        }

        status = AcqrsT3_acquire(instrId);
        if (status != VI_SUCCESS)
        {
            acqStop = true;
            return;
        }

        emit acqThreadBankSwitch(dataDescPtr);

        if (status != VI_SUCCESS)
        {
            acqStop = true;
            return;
        }

    }

    emit runThreadFinished();
}

void Acqiris_AcquisitionThread::dealRunThreadFinished()
{
    status = AcqrsT3_waitForEndOfAcquisition(instrId, 8000);
    status = AcqrsT3_stopAcquisition(instrId);
    this->quit();
    this->wait();
    emit acquisitionFinished(status);
}
