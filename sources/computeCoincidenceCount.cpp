#include "AcqirisImport.h"
#include "AcqirisT3Import.h"
#include <cmath>
#include <QVector>
#include <QDebug>

// 计算符合计数

bool channelToBeCalculated(int channel, int *channels, int nbrChannels);

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
    bool channelMark[12] = {0};
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
        (AqT3DataDescriptor* dataDescPtr,
         QList<QVector<int>> &timeSeq,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
         QList<QVector<int>> &timeSeqAcc,
         QList<QVector<int>> &channelSeq,    // 升序排列后的时间，与通道编号一一对应
         QList<QVector<int>> &channelSeqAcc,
         int nbrChannels,
         int* channels,
         int* nbrCoin,
         int tolerance,
         int* nbrCoinAcc,
         int *nbrCOMdelay, int *nbrCOMdelayAcc,
         int *delayInCOM, int *delayInCOMAcc,
         int timeCOMunit,
         int *COM_HEAD)
{
//    判定通道数是否合法
    if (nbrChannels < 2 or nbrChannels > 6) return;

//    读取时间数据
    long nbrSamples = dataDescPtr->nbrSamples;
    bool mark = false; // 用于标记上轮是否有计数来判断是否需要进行符合计算
    int index = 0; // 用于标记元素插入位置
    int spacing = 0; // 用于表示符合窗口跨度
    for (long n = 0 ; n < nbrSamples ; ++n)
    {
        int sample = ((long *)dataDescPtr->dataPtr)[n];  //dataPtr指向time value data buffer
//        int flag = (sample & 0x80000000) >> 31;      //“sample”值右移31位为flag位
        int channel = (sample & 0x70000000) >> 28;   //右移28位为channel位
        int TimeOfFlight = sample & 0x0FFFFFFF;             //最右侧28位为计数值

        int TimeOfFlightAcc = 0;
        // flag 为 0 表示 COM 信号，channel 为 7 表示内存切换
        if (channel != 0 and channel != 7)                        // Channel=1-6 are the physical channels;
                                                // Data = an integer giving the time value in units of 50 ps
                                                // Channel=7 is for marker data.
        {
            if (!channelToBeCalculated(channel,channels,nbrChannels)) continue;

            int indexCOM, indexCOMAcc;
            if (timeSeqAcc.size() == timeSeq.size())
                TimeOfFlightAcc = TimeOfFlight + delayInCOMAcc[channel-1];
            TimeOfFlight += delayInCOM[channel-1];
            // channel-1 通道所插入的序列编号应为 nbrCOMdelay[channel-1]
            if (TimeOfFlight > timeCOMunit)
            {
                TimeOfFlight -= timeCOMunit;
                indexCOM = (nbrCOMdelay[channel-1] + 1 + *COM_HEAD) % timeSeq.size();
            }
            else
                indexCOM = (nbrCOMdelay[channel-1] + *COM_HEAD) % timeSeq.size();
            index = findInsertPosition(timeSeq[indexCOM], TimeOfFlight);        // 按时间升序排列
            timeSeq[indexCOM].insert(index, TimeOfFlight);
            channelSeq[indexCOM].insert(index, channel-1);
            if (timeSeqAcc.size() == timeSeq.size())
            {
                if (TimeOfFlightAcc > timeCOMunit)
                {
                    TimeOfFlightAcc -= timeCOMunit;
                    indexCOMAcc = (nbrCOMdelayAcc[channel-1] + 1 + *COM_HEAD) % timeSeq.size();
                }
                else
                    indexCOMAcc = (nbrCOMdelayAcc[channel-1] + *COM_HEAD) % timeSeq.size();
                index = findInsertPosition(timeSeqAcc[indexCOMAcc], TimeOfFlightAcc);
                timeSeqAcc[indexCOMAcc].insert(index, TimeOfFlightAcc);
                channelSeqAcc[indexCOMAcc].insert(index, channel-1);
            }
            mark = true;
        }
        else
        {
            if (mark and timeSeq[*COM_HEAD].size() >= nbrChannels)                            // 计算上一轮符合
            {
                for (int i = 0; i < timeSeq[*COM_HEAD].size()-nbrChannels+1; i++)
                {
                    spacing = findSpacing(timeSeq[*COM_HEAD], i, tolerance);          // 求符合窗口在起始位置处的跨度
                    if (checkCoincidence(channels, nbrChannels, channelSeq[*COM_HEAD], i, i+spacing)) // 查看该跨度内是否有符合
                    {
                        (*nbrCoin)++;
                        i = i+spacing;                                     // 计算过符合的区间可以跳过
                    }
                }
                if (timeSeqAcc.size() == timeSeq.size())
                {
                    for (int i = 0; i < timeSeqAcc[*COM_HEAD].size()-2+1; i++)
                    {
                        spacing = findSpacing(timeSeqAcc[*COM_HEAD], i, tolerance);          // 求符合窗口在起始位置处的跨度
                        if (checkCoincidence(channels, 2, channelSeqAcc[*COM_HEAD], i, i+spacing)) // 查看该跨度内是否有符合
                        {
                            (*nbrCoinAcc)++;
                            i = i+spacing;                                     // 计算过符合的区间可以跳过
                        }
                    }
                }
            }
            mark = false;
            timeSeq[*COM_HEAD].clear();
            channelSeq[*COM_HEAD].clear();
            if (timeSeqAcc.size() == timeSeq.size())
            {
                timeSeqAcc[*COM_HEAD].clear();
                channelSeqAcc[*COM_HEAD].clear();
            }
            *COM_HEAD = ((*COM_HEAD)+1) % timeSeq.size();
        }
    }
}

void computeCoincidenceCountAcrossDevices_HOLD
            (AqT3DataDescriptor* dataDescPtr,
             QList<QVector<int>>& timeSeq,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
             QList<QVector<int>>& timeSeqAcc,
             QList<QVector<int>>& channelSeq,    // 升序排列后的时间，与通道编号一一对应
             QList<QVector<int>>& channelSeqAcc,
             int nbrChannels,
             int* channels,
             int *nbrCOMdelay, int *nbrCOMdelayAcc,
             int *delayInCOM, int *delayInCOMAcc,
             int timeCOMunit,
             int *COM_HEAD)
{
//    读取时间数据
    long nbrSamples = dataDescPtr->nbrSamples;

    int index = 0; // 用于标记元素插入位置
    for (long n = 0 ; n < nbrSamples ; ++n)
    {
        int sample = ((long *)dataDescPtr->dataPtr)[n];  //dataPtr指向time value data buffer
//        int flag = (sample & 0x80000000) >> 31;      //“sample”值右移31位为flag位
        int channel = (sample & 0x70000000) >> 28;   //右移28位为channel位
        int TimeOfFlight = sample & 0x0FFFFFFF;             //最右侧28位为计数值
        int TimeOfFlightAcc = 0;

        // flag 为 0 表示 COM 信号，channel 为 7 表示内存切换
        if (channel != 0 and channel != 7)                        // Channel=1-6 are the physical channels;
                                                // Data = an integer giving the time value in units of 50 ps
                                                // Channel=7 is for marker data.
        {
            if (!channelToBeCalculated(channel,channels,nbrChannels)) continue;

            int indexCOM, indexCOMAcc;
            if (timeSeqAcc.size() == timeSeq.size())
                TimeOfFlightAcc = TimeOfFlight + delayInCOMAcc[channel-1];
            TimeOfFlight += delayInCOM[channel-1];
            // channel-1 通道所插入的序列编号应为 nbrCOMdelay[channel-1]
            if (TimeOfFlight > timeCOMunit)
            {
                TimeOfFlight -= timeCOMunit;
                indexCOM = (nbrCOMdelay[channel-1] + 1 + *COM_HEAD) % timeSeq.size();
            }
            else
                indexCOM = (nbrCOMdelay[channel-1] + *COM_HEAD) % timeSeq.size();
            index = findInsertPosition(timeSeq[indexCOM], TimeOfFlight);        // 按时间升序排列
            timeSeq[indexCOM].insert(index, TimeOfFlight);
            channelSeq[indexCOM].insert(index, channel-1);

            if (timeSeqAcc.size() == timeSeq.size())
            {
                if (TimeOfFlightAcc > timeCOMunit)
                {
                    TimeOfFlightAcc -= timeCOMunit;
                    indexCOMAcc = (nbrCOMdelayAcc[channel-1] + 1 + *COM_HEAD) % timeSeq.size();
                }
                else
                    indexCOMAcc = (nbrCOMdelayAcc[channel-1] + *COM_HEAD) % timeSeq.size();
                index = findInsertPosition(timeSeqAcc[indexCOMAcc], TimeOfFlightAcc);
                timeSeqAcc[indexCOMAcc].insert(index, TimeOfFlightAcc);
                channelSeqAcc[indexCOMAcc].insert(index, channel-1);
            }
        }
        else
            *COM_HEAD = ((*COM_HEAD)+1) % timeSeq.size();
    }
}

void computeCoincidenceCountAcrossDevices_COMPUTE(
                 QList<QVector<int>> &timeSeqX1,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
                 QList<QVector<int>> &timeSeqAccX1,
                 QList<QVector<int>> &channelSeqX1,    // 升序排列后的时间，与通道编号一一对应
                 QList<QVector<int>> &channelSeqAccX1,
                 QList<QVector<int>> &timeSeqX2,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
                 QList<QVector<int>> &timeSeqAccX2,
                 QList<QVector<int>> &channelSeqX2,    // 升序排列后的时间，与通道编号一一对应
                 QList<QVector<int>> &channelSeqAccX2,
                 int nbrChannelsX1, int nbrChannelsX2,
                 int* channelsX1, int *channelsX2,
                 int* nbrCoin,
                 int tolerance,
                 int* nbrCoinAcc,
                 int *COM_HEAD_X1, int *COM_HEAD_X2, int *COM_HEAD_compute)
{
    int vectorSize = timeSeqX1.size();
    int computeLength = *COM_HEAD_X1 - *COM_HEAD_compute;

    if (*COM_HEAD_X1 < *COM_HEAD_compute and *COM_HEAD_X2 < *COM_HEAD_compute)
    {
        if (*COM_HEAD_X1 > *COM_HEAD_X2)        // 2 < 1 < X
            computeLength = vectorSize - *COM_HEAD_compute + *COM_HEAD_X2;
        else                                // 1 < 2 < X
            computeLength = vectorSize - *COM_HEAD_compute + *COM_HEAD_X1;

    }
    else
    {
        if (*COM_HEAD_X1 < *COM_HEAD_compute)        // 1 < X < 2
            computeLength = *COM_HEAD_X2 - *COM_HEAD_compute;
        else
        {
            if (*COM_HEAD_X1 > *COM_HEAD_X2 and *COM_HEAD_X2 > *COM_HEAD_compute)
                                            // X < 2 < 1
                computeLength = *COM_HEAD_X2 - *COM_HEAD_compute;
        }
    }
    int nbrChannels = nbrChannelsX1 + nbrChannelsX2;
    int *channels = new int[nbrChannels]();
    for (int i = 0; i < nbrChannelsX1; i++)
        channels[i] = channelsX1[i];
    for (int i = 0; i < nbrChannelsX2; i++)
        channels[i+nbrChannelsX1] = channelsX2[i] + 6;

    int index, spacing;

    for (int m = *COM_HEAD_compute; m < *COM_HEAD_compute + computeLength; m++)
    {
        int mm = m % vectorSize;

        if (timeSeqX1[mm].size() + timeSeqX2[mm].size() >= nbrChannels)                            // 计算上一轮符合
        {
            // 先将 X2 的时间及通道数据合并到 X1 向量中
            for (int j = 0; j < timeSeqX2[mm].size(); j++)
            {
                index = findInsertPosition(timeSeqX1[mm], timeSeqX2[mm][j]);
                timeSeqX1[mm].insert(index, timeSeqX2[mm][j]);
                channelSeqX1[mm].insert(index, channelSeqX2[mm][j] + 6);
                if (timeSeqAccX1.size() == timeSeqX1.size())
                {
                    index = findInsertPosition(timeSeqAccX1[mm], timeSeqAccX2[mm][j]);
                    timeSeqAccX1[mm].insert(index, timeSeqAccX2[mm][j]);
                    channelSeqAccX1[mm].insert(index, channelSeqAccX2[mm][j] + 6);
                }
            }

            for (int i = 0; i < timeSeqX1[mm].size()-nbrChannels+1; i++)
            {
                spacing = findSpacing(timeSeqX1[mm], i, tolerance);          // 求符合窗口在起始位置处的跨度
                if (checkCoincidence(channels, nbrChannels, channelSeqX1[mm], i, i+spacing)) // 查看该跨度内是否有符合
                {
                    (*nbrCoin)++;
                    i = i+spacing;                                     // 计算过符合的区间可以跳过
                }
            }
            if (timeSeqAccX1.size() == timeSeqX1.size())
            {
                for (int i = 0; i < timeSeqAccX1[mm].size()-2+1; i++)
                {
                    spacing = findSpacing(timeSeqAccX1[mm], i, tolerance);          // 求符合窗口在起始位置处的跨度
                    if (checkCoincidence(channels, 2, channelSeqAccX1[mm], i, i+spacing)) // 查看该跨度内是否有符合
                    {
                        (*nbrCoinAcc)++;
                        i = i+spacing;                                     // 计算过符合的区间可以跳过
                    }
                }
            }
        }

        timeSeqX1[mm].clear();
        channelSeqX1[mm].clear();
        timeSeqX2[mm].clear();
        channelSeqX2[mm].clear();
        if (timeSeqAccX1.size() == timeSeqX1.size())
        {
            timeSeqAccX1[mm].clear();
            channelSeqAccX1[mm].clear();
            timeSeqAccX2[mm].clear();
            channelSeqAccX2[mm].clear();
        }
    }
    *COM_HEAD_compute = (*COM_HEAD_compute + computeLength) % vectorSize;
    delete[] channels;
}
