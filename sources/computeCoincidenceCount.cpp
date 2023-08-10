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

int checkCoincidence(int* channels, int nbrChannels, QVector<int> channelSeq, int start, int end)
{
    bool channelMark[6] = {0};
    for (int i = start; i <= end; i++)         // 判断 channelSeq[start,end-1] 这个序列中有哪些通道号
    {
        channelMark[channelSeq[i]] = true;
    }
                                               // 到这一步 channelMark 数组中包含了哪几个通道为符合通道

    for (int i = 0; i < nbrChannels; i++)                // 再判断 channelMark 数组中的通道是否包含所要求的符合通道 channelMulti
    {
        if (!channelMark[channels[i]])
            return 0;
    }
    return 1;
}


void computeCoincidenceCount
        (QVector<AqT3DataDescriptor*> dataPtrList,
         int nbrChannels,
         int* channels,
         int* nbrCoin,
         int tolerance,
         int* delayMulti,
         int* nbrCoinAcc = new int (),
         int delayAcc = 0)
{
//    假设每个 COM 周期内的原始数据的时间是升序排列的

//    判定通道数是否合法
    if (nbrChannels < 2 or nbrChannels > 6) return;

//    先读取时间数据
    AqT3DataDescriptor *dataDescPtr = dataPtrList.last();
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
                    if (checkCoincidence(channels, nbrChannels, channelSeq, i, i+spacing)) // 查看该跨度内是否有符合
                    {
                        (*nbrCoin)++;
                        i = i+spacing;                                     // 计算过符合的区间可以跳过
                    }
                }
                if (nbrChannels == 2)
                {
                    for (int i = 0; i < timeSeqAcc.size()-2+1; i++)
                    {
                        spacing = findSpacing(timeSeqAcc, i, tolerance);          // 求符合窗口在起始位置处的跨度
                        if (checkCoincidence(channels, 2, channelSeqAcc, i, i+spacing)) // 查看该跨度内是否有符合
                        {
                            (*nbrCoinAcc)++;
                            i = i+spacing;                                     // 计算过符合的区间可以跳过
                        }
                    }
                }
            }
            mark = false;
            timeSeq.clear();
            channelSeq.clear();
            if (nbrChannels == 2)
            {
                timeSeqAcc.clear();
                channelSeqAcc.clear();
            }
        }
    }
}
