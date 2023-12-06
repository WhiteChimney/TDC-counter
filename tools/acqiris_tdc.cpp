#include "acqiris_tdc.h"

Acqiris_TDC::Acqiris_TDC(QString m_resourceName,
                         QObject *parent) : QObject(parent)
{
    resourceName = m_resourceName;
    status = -1;

    readParamPtr = new AqT3ReadParameters();
    acqThread = new Acqiris_AcquisitionThread(instrId, readParamPtr);
    connect(acqThread,&Acqiris_AcquisitionThread::acqThreadBankSwitch,this,&Acqiris_TDC::dealAcqThreadBankSwitch);
    connect(acqThread,&Acqiris_AcquisitionThread::acquisitionStarted,this,&Acqiris_TDC::dealAcqThreadStarted);
    connect(acqThread,&Acqiris_AcquisitionThread::acquisitionFinished,this,&Acqiris_TDC::dealAcqThreadFinished);
}

int Acqiris_TDC::initialize()
{
    status = Acqrs_InitWithOptions(resourceName.toLatin1().data(), VI_FALSE,
                                   VI_FALSE, "CAL=0", &instrId);
    acqThread->setInstrId(instrId);
    return status;
}

int Acqiris_TDC::config(bool(&channelConfig)[NUM_CHANNELS+1],
                        double(&level)[NUM_CHANNELS+1],
                        int(&slope)[NUM_CHANNELS+1],
                        int countEvents)
{
    // 未初始化，直接返回错误
    if (status != VI_SUCCESS) return status;

    //设定连续运行模式并允许内部测试信号
    AcqrsT3_configMode(instrId, 2, 1, 0);//TC890 mode2, multiple acquisitions, 第三个数据为0：internal reference clock，1：external reference clock

    //设定一个（较长的）超时时间8秒
    AcqrsT3_configAcqConditions(instrId, 8, 0, 0);// start timeout counter on Arm

    //设置bank switch(最大8MBit)
    ViInt32 memorySize = 1 * 1024 * 1024;
    AcqrsT3_configMemorySwitch(instrId,2,countEvents,memorySize, 0);
//    第二个参数
//    2, switch on count of events on common channel
//    4, switch on memory size limit
//    countEvents, number of events on the common channel
//    e.g. countEvents为100k，那么若start频率200kHz，则0.5s后切换内存
//    切换内存后可以处理数据

    //设置信道。Configure channels, common on negative slope, other left on positive
    long long channelConfigCode[NUM_CHANNELS+1] = {0x80000000};
    for (int i = 0; i < NUM_CHANNELS+1; i++)
        if (channelConfig[i])
            channelConfigCode[i] = slope[i];
       qDebug()<<slope[0];

    status = AcqrsT3_configChannel(instrId, -1, channelConfigCode[0], level[0], 0);//channel common,positive slope,vth=1.0V
    for (int i = 1; i < NUM_CHANNELS+1; i++)
        status += AcqrsT3_configChannel(instrId, i, channelConfigCode[i], level[i], 0);//channel 1

    // 生成读取结构AqT3ReadParameters dataArray
    // dataArray 中包括了 start 信号与 marker 信号
    // dataSizeInBytes 表示 dataArray 所占用的空间，
    // 最大为 8MB = 8*1024*1024 = 8388608 bit
    // 而每个 start 或 marker 将占用 4 byte = 32 bit

    size_t const arraySize = memorySize;
    char * const dataArrayP = new char[arraySize];

    memset(readParamPtr, 0, sizeof(*readParamPtr));
    readParamPtr->dataArray = dataArrayP;
    readParamPtr->dataSizeInBytes = arraySize;
    readParamPtr->nbrSamples = 0;
    readParamPtr->dataType = ReadRawData;
    readParamPtr->readMode = AqT3ReadContinuous;

    return status;

}

void Acqiris_TDC::startAcquisition()
{
    // 检查仪器状态是否有问题
    if (status != VI_SUCCESS) return;

    acqThread->startAcquisition(&waitCond);
}

void Acqiris_TDC::wakeToAquireData()
{
    waitCond.wakeOne();
}

void Acqiris_TDC::dealAcqThreadStarted()
{
    emit acquisitionStarted();
}

void Acqiris_TDC::stopAcquisition()
{
    acqThread->stopAcquisition();
}

int Acqiris_TDC::getStatus()
{
    return status;
}

bool Acqiris_TDC::isAcquringData()
{
    return acqThread->isAcquringData();
}

int Acqiris_TDC::close()
{
    status = Acqrs_closeAll();
    return status;
}

void Acqiris_TDC::dealAcqThreadBankSwitch(AqT3DataDescriptor* dataDescPtr)
{
    emit dataReturned(dataDescPtr);
}

void Acqiris_TDC::dealAcqThreadFinished(ViStatus m_status)
{
    status = m_status;
    emit acquisitionFinished(status);
}
