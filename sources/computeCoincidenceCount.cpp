#include "AcqirisImport.h"
#include "AcqirisT3Import.h"
#include <cmath>

// 计算符合计数

void computeCoincidenceCount
        (AqT3DataDescriptor* dataDescPtr,
         int* nbrCoin, int* nbrAccCoin,
         int channel1, int channel2,
         int tolerance,
         int delay, int delayAcc)
{
    long nbrSamples = dataDescPtr->nbrSamples;

    int mark = 0;
    int nCOM = 0;
    int *TOF[2];                             // TOF: time of flight 单光子到达时间
    for (int i = 0; i < 2; ++i) TOF[i] = new int[nbrSamples]();

    for (long n = 0 ; n < nbrSamples ; ++n)
    {
        int sample = ((long *)dataDescPtr->dataPtr)[n];  //dataPtr指向time value data buffer
//        int flag = (sample & 0x80000000) >> 31;      //“sample”值右移31位为flag位
        int channel = (sample & 0x70000000) >> 28;   //右移28位为channel位
        int TimeOfFlight = sample & 0x0FFFFFFF;             //最右侧28位为计数值

        // flag 为 0 表示 COM 信号，channel 为 7 表示内存切换
        if (channel == channel1)                        // Channel=1-6 are the physical channels;
                                                // Data = an integer giving the time value in units of 50 ps
                                                // Channel=7 is for marker data.
        {
            TOF[0][n] = TimeOfFlight;
            mark = 1;
        }
        else if (channel == channel2)
        {
            TOF[1][n] = TimeOfFlight;
            mark = 1;
        }
        else if (channel == 0 or channel == 7)                                     // Channel = 0 is for start the next event.
        {
            if (mark == 1)                            // 计算上一轮符合
            {
                for (int i = nCOM; i < n; i++)
                    if (TOF[0][i] >= 0)
                    {
                        for (int j = nCOM; j < n; j++)
                        {
                            if(/*TOF[1][j]!=0 && */abs(TOF[0][i]-TOF[1][j]-delay)<=tolerance/2.0)
                                (*nbrCoin)++;
                            if(/*TOF[1][j]!=0 && */abs(TOF[0][i]-TOF[1][j]-delay-delayAcc)<=tolerance/2.0)
                                (*nbrAccCoin)++;
                        }
                    }
                nCOM = n+1;
                mark = 0;
            }
        }
    }
    for (int i = 0; i < 2; ++i) delete [] TOF[i];
}
