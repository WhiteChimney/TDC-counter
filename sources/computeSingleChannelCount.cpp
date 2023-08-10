#include <QVector>
#include <QtMath>
#include "AcqirisImport.h"
#include "AcqirisT3Import.h"

// 计算单道计数

// // 不计算延时的算法
//void computeSingleChannelCount(int* nbrSCC, QVector<AqT3DataDescriptor*> dataPtrList, double *delayCN)
//{
//    AqT3DataDescriptor *dataDescPtr = dataPtrList.last();
//    long nbrSamples = dataDescPtr->nbrSamples;
//    for (long n = 0 ; n < nbrSamples ; ++n)
//    {
//        int sample = ((long *)dataDescPtr->dataPtr)[n];  //dataPtr指向time value data buffer
//        int channel = (sample & 0x70000000) >> 28;   //右移28位为channel位

//        // flag 为 0 表示 COM 信号，channel 为 7 表示内存切换
//        if (channel != 0 && channel != 7)                        // Channel=1-6 are the physical channels;
//                                                // Data = an integer giving the time value in units of 50 ps
//                                                // Channel=7 is for marker data.
//            nbrSCC[channel-1]++;
//    }
//}

// // 计算延时的算法
void computeSingleChannelCount
    (int* nbrSCC, int* nbrSCCfuture,
     QVector<AqT3DataDescriptor*> dataPtrList,
     double *delayCN, int freqCOM, int countEvents)
{
    double timeCOM = 1000.0/freqCOM;           // 单位为 us
    int nbrCOMdelay[6] = {0};
    int delayInCOM[6] = {0};                  // 以 TDC 最小时间为单位，50 ps
    for (int i = 0; i < 6; i++)
    {
        nbrCOMdelay[i] = floor(delayCN[i]/timeCOM);
        delayInCOM[i] = 20*1000*int(delayCN[i] - timeCOM*nbrCOMdelay[i]);
    }
//    qDebug() << "nbrCOMdelay[" << i << "]: " << nbrCOMdelay[i];
//    qDebug() << "delayInCOM[" << i << "]:" << delayInCOM[i];

    AqT3DataDescriptor *dataDescPtr = dataPtrList.last();

    long nbrSamples = dataDescPtr->nbrSamples;
    int nCOM = 0;
    for (long n = 0 ; n < nbrSamples ; ++n)
    {
        int sample = ((long *)dataDescPtr->dataPtr)[n];  //dataPtr指向time value data buffer
        int channel = (sample & 0x70000000) >> 28;   //右移28位为channel位
        int TimeOfFlight = sample & 0x0FFFFFFF;

        // flag 为 0 表示 COM 信号，channel 为 7 表示内存切换
        if (channel == 0 || channel == 7)                        // Channel=1-6 are the physical channels;
                                                // Data = an integer giving the time value in units of 50 ps
                                                // Channel=7 is for marker data.
        {
            nCOM++;
        }
        else
        {
            if (nCOM + nbrCOMdelay[channel-1] < countEvents)
                nbrSCC[channel-1]++;
            else
            {
                if (nCOM + nbrCOMdelay[channel-1] == countEvents
                    && TimeOfFlight < delayInCOM[channel-1])
                    nbrSCC[channel-1]++;
                else
                    nbrSCCfuture[channel-1]++;
            }
        }
    }
}
