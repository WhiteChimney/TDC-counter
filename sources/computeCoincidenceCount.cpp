#include "AcqirisImport.h"
#include "AcqirisT3Import.h"
#include <cmath>
#include <QVector>
#include <QDebug>

// 计算符合计数

int findInsertPosition(QVector<int> timeSeq, int TimeOfFlight)
{
    if (timeSeq.size() == 0)
        return 0;
    int index = 0;
    bool bingo = false;
    while (!bingo and index < timeSeq.size())
    {
        if (timeSeq.at(index) >= TimeOfFlight)
            bingo = true;
        else
            index++;
    }
    return index;
}

int findSpacing(QVector<int> timeSeq, int i, int toleranceMulti)
{
    int spacing = 0;
    while (timeSeq.at(i+spacing+1) - timeSeq.at(i) <= toleranceMulti and i+spacing+1 < timeSeq.size())
        spacing++;
    return spacing;
}

int checkCoincidence(int* channels, int nbrChannels, sQVector<int> channelSeq, int start, int end)
{
    bool channelMark[6] = {0};
    for (int i = start; i <= end; i++)
    {
        channelMark[channelSeq.at(i)] = true;
    }

    if (channelMark[channel1-1] and channelMark[channel2-1])
        return 1;
    else
        return 0;
}


void computeCoincidenceCount
        (AqT3DataDescriptor* dataDescPtr,
         int nbrChannels,
         int* channels,
         int* nbrCoin,
         int tolerance,
         int* delayMulti = new int (),
         int* nbrCoinAcc = new int (),
         int delayAcc = 0)
{
//    假设每个 COM 周期内的原始数据的时间是升序排列的

//    判定通道数是否合法
    if (nbrChannels < 2 or nbrChannels > 6) return;

//    先读取时间数据
    long nbrSamples = dataDescPtr->nbrSamples;

    bool mark = false; // 用于标记上轮是否有计数来判断是否需要进行符合计算
    QVector<int> channelSeq, channelSeqAcc; // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
    QVector<int> timeSeq, timeSeqAcc;    // 升序排列后的时间，与通道编号一一对应
    int index = 0; // 用于标记元素插入位置
    int spacing = 0; // 用于表示符合窗口跨度

    for (long n = 0 ; n < nbrSamples ; ++n)
    {
        int sample = ((long *)dataDescPtr->dataPtr)[n];  //dataPtr指向time value data buffer
//        int flag = (sample & 0x80000000) >> 31;      //“sample”值右移31位为flag位
        int channel = (sample & 0x70000000) >> 28;   //右移28位为channel位
        int TimeOfFlight = sample & 0x0FFFFFFF;             //最右侧28位为计数值
        int TimeOfFlightAcc;

        // flag 为 0 表示 COM 信号，channel 为 7 表示内存切换
        if (channel != 0 and channel != 7)                        // Channel=1-6 are the physical channels;
                                                // Data = an integer giving the time value in units of 50 ps
                                                // Channel=7 is for marker data.
        {
            TimeOfFlight += delayMulti[channel-1];
            index = findInsertPosition(timeSeq, TimeOfFlight);        // 按时间升序排列
            timeSeq.insert(index, TimeOfFlight);
            channelSeq.insert(index, channel-1);
            if (nbrChannels == 2)
            {
                if (channel == channels[0])
                    TimeOfFlightAcc = TimeOfFlight;
                else
                    TimeOfFlightAcc = TimeOfFlight + delayAcc;
                index = findInsertPosition(timeSeqAcc, TimeOfFlightAcc);
                timeSeqAcc.insert(index, TimeOfFlightAcc);
                channelSeqAcc.insert(index, channel-1);
            }
            mark = true;
        }
        else
        {
            if (mark and timeSeq.size() >= nbrChannels)                            // 计算上一轮符合
            {
                for (int i = 0; i < timeSeq.size()-nbrChannels+1; i++)
                {
                    spacing = findSpacing(timeSeq, i, tolerance);          // 求符合窗口在起始位置处的跨度
                    if (checkCoincidence(channelMulti, channelSeq, i, i+spacing)) // 查看该跨度内是否有符合
                    {
                        (*nbrCoinMulti)++;
                        i = i+spacing;                                     // 计算过符合的区间可以跳过
                    }
                }
            }
            mark = false;
            timeSeq.clear();
            channelSeq.clear();
        }




        // flag 为 0 表示 COM 信号，channel 为 7 表示内存切换
        if (channel == channel1 or channel == channel2)                        // Channel=1-6 are the physical channels;
                                                // Data = an integer giving the time value in units of 50 ps
                                                // Channel=7 is for marker data.
        {
            if (channel == channel1)
            {
                TimeOfFlightAcc = TimeOfFlight;
            }
            else
            {
                TimeOfFlight += delay;
                TimeOfFlightAcc = TimeOfFlight + delayAcc;
            }
            index = findInsertPosition2(timeSeq, TimeOfFlight);        // 按时间升序排列
            timeSeq.insert(index, TimeOfFlight);
            channelSeq.insert(index, channel-1);
            index = findInsertPosition2(timeSeqAcc, TimeOfFlightAcc);
            timeSeqAcc.insert(index, TimeOfFlightAcc);
            channelSeqAcc.insert(index, channel-1);
            mark = true;
        }
        else if (channel == 0 or channel == 7)
        {
            if (mark and timeSeq.size() >= 2)                            // 计算上一轮符合
            {
                for (int i = 0; i < timeSeq.size()-2+1; i++)
                {
                    spacing = findSpacing2(timeSeq, i, tolerance);          // 求符合窗口在起始位置处的跨度
                    if (checkCoincidence2(channel1, channel2, channelSeq, i, i+spacing)) // 查看该跨度内是否有符合
                    {
                        (*nbrCoin)++;
                        i = i+spacing;                                     // 计算过符合的区间可以跳过
                    }
                }
                for (int i = 0; i < timeSeqAcc.size()-2+1; i++)
                {
                    spacing = findSpacing2(timeSeqAcc, i, tolerance);          // 求符合窗口在起始位置处的跨度
                    if (checkCoincidence2(channel1, channel2, channelSeqAcc, i, i+spacing)) // 查看该跨度内是否有符合
                    {
                        (*nbrAccCoin)++;
                        i = i+spacing;                                     // 计算过符合的区间可以跳过
                    }
                }
            }
            mark = false;
            timeSeq.clear();
            channelSeq.clear();
            timeSeqAcc.clear();
            channelSeqAcc.clear();
        }
    }
}
