#include "AcqirisImport.h"
#include "AcqirisT3Import.h"
#include <QDebug>

// 计算统计直方图计数

void countSingle(AqT3DataDescriptor* dataDescPtr, int channel1,
                 double timeStart, double binWidth, int nbrIntervals, int* binHeight)
{
    long nbrSamples = dataDescPtr->nbrSamples;
    for (long n = 0 ; n < nbrSamples ; ++n)
    {
        int sample = ((long *)dataDescPtr->dataPtr)[n];  //dataPtr指向time value data buffer
        int channel = (sample & 0x70000000) >> 28;   //右移28位为channel位

        if (channel == channel1)
        {
            int TimeOfFlight = sample & 0x0FFFFFFF;             //最右侧28位为计数值
            double TOF = 20.0*TimeOfFlight;
            int index = int((TOF-timeStart)/binWidth);
            if (index >= 0 and index < nbrIntervals)
                binHeight[index]++;
        }
    }
}

void countDifference(AqT3DataDescriptor* dataDescPtr, int channel1, int channel2, double delay,
                 double timeStart, double binWidth, int nbrIntervals, int* binHeight)
{
    long nbrSamples = dataDescPtr->nbrSamples;
    double TOF1 = 0.0, TOF2 = 0.0;
    for (long n = 0 ; n < nbrSamples ; ++n)
    {
        int sample = ((long *)dataDescPtr->dataPtr)[n];  //dataPtr指向time value data buffer
        int channel = (sample & 0x70000000) >> 28;   //右移28位为channel位

        if (channel == channel1)
        {
            int TimeOfFlight = sample & 0x0FFFFFFF;             //最右侧28位为计数值
            TOF1 = TimeOfFlight/20.0;
        }
        else if (channel == channel2)
        {
            int TimeOfFlight = sample & 0x0FFFFFFF;             //最右侧28位为计数值
            TOF2 = TimeOfFlight/20.0;
        }
        else if (channel == 0 or channel == 7)
        {
            if (TOF1 > 0 and TOF2 > 0)
            {
                double timeDiff = TOF2 - TOF1;
                int index = int((timeDiff+delay-timeStart)/binWidth);
                if (index >= 0 and index < nbrIntervals)
                    binHeight[index]++;
            }
            TOF1 = 0.0;
            TOF2 = 0.0;
        }
    }
}

void computeHistogramCount(AqT3DataDescriptor* dataDescPtr,
                             int channel1, int channel2, double delay,
                             double timeStart, double binWidth, int nbrIntervals, int* binHeight)
{
    if (channel1 == channel2) countSingle(dataDescPtr,channel1,timeStart,binWidth,nbrIntervals,binHeight);
    else countDifference(dataDescPtr,channel1,channel2,delay,timeStart,binWidth,nbrIntervals,binHeight);
}
