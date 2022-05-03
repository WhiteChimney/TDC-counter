#include "stdafx.h"

// 配置 TDC 运行参数

ViStatus configTDC(ViSession idInstr, int countEvents, bool *channelConfig, double *level, int *slope, AqT3ReadParameters* readParamPtr)
{
    //设定连续运行模式并允许内部测试信号
    AcqrsT3_configMode(idInstr, 2, 1, 0);//TC890 mode2, multiple acquisitions, 第三个数据为0：internal reference clock，1：external reference clock

    //设定一个（较长的）超时时间8秒
    AcqrsT3_configAcqConditions(idInstr, 8, 0, 0);// start timeout counter on Arm

    //设置bank switch(最大8MBit)
    ViInt32 memorySize = 1 * 1024 * 1024;
    AcqrsT3_configMemorySwitch(idInstr,2,countEvents,memorySize, 0);
//    第二个参数
//    2, switch on count of events on common channel
//    4, switch on memory size limit
//    countEvents, number of events on the common channel
//    e.g. countEvents为100k，那么若start频率200kHz，则0.5s后切换内存
//    切换内存后可以处理数据

    //设置信道。Configure channels, common on negative slope, other left on positive
    long long channelConfigCode[7] = {0x80000000};
    for (int i = 0; i < 7; i++)
        if (channelConfig[i])
            channelConfigCode[i] = 0x00000000;

    ViStatus configStatus;
    configStatus = AcqrsT3_configChannel(idInstr, -1, channelConfigCode[0], level[0], slope[0]);//channel common,positive slope,vth=1.0V
    for (int i = 1; i < 7; i++)
    {
        ViStatus status = AcqrsT3_configChannel(idInstr, i, channelConfigCode[i], level[i], slope[i]);//channel 1
        configStatus = configStatus + status;
    }

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

    return configStatus;
}
