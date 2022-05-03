#include "AcqirisImport.h"
#include "AcqirisT3Import.h"

// 计算单道计数

void computeSingleChannelCount(AqT3DataDescriptor* dataDescPtr, int* nbrSCC)
{
    long nbrSamples = dataDescPtr->nbrSamples;
    for (long n = 0 ; n < nbrSamples ; ++n)
    {
        int sample = ((long *)dataDescPtr->dataPtr)[n];  //dataPtr指向time value data buffer
        int channel = (sample & 0x70000000) >> 28;   //右移28位为channel位

        // flag 为 0 表示 COM 信号，channel 为 7 表示内存切换
        if (channel != 0 && channel != 7)                        // Channel=1-6 are the physical channels;
                                                // Data = an integer giving the time value in units of 50 ps
                                                // Channel=7 is for marker data.
            nbrSCC[channel-1]++;
    }
}
