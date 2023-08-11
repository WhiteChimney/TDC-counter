#include "AcqirisImport.h"
#include "AcqirisT3Import.h"
#include <cmath>
#include <QVector>
#include <QDebug>

// 计算符合计数

void resizeSeqLength(QVector<QVector<int>> *v, int l)
{
    if (v->size() < l)
    {
        QVector<int> v0;
        while (v->size() < l)
            v->append(v0);
    }
    else if (v->size() > l)
    {
        while (v->size() > l)
            v->removeFirst();
    }
}

bool channelToBeCalculated(int channel, int *channels, int nbrChannels)
{
    for (int i = 0; i < nbrChannels; i++)
        if (channel-1 == channels[i])
            return true;
    return false;
}

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
         QVector<QVector<int>> timeSeq,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
         QVector<QVector<int>> timeSeqAcc,
         QVector<QVector<int>> channelSeq,    // 升序排列后的时间，与通道编号一一对应
         QVector<QVector<int>> channelSeqAcc,
         int nbrChannels,
         int* channels,
         int* nbrCoin,
         int tolerance,
         int* nbrCoinAcc,
         int *nbrCOMdelay, int *nbrCOMdelayAcc,
         int maxNbrCOMdelay, int maxNbrCOMdelayAcc,
         int *delayInCOM, int *delayInCOMAcc,
         int timeCOMunit)
{
//    判定通道数是否合法
    if (nbrChannels < 2 or nbrChannels > 6) return;

//    时间序列所需要保存的 COM 周期数量为 nbrCOMdelay 中的最大值 +2
    resizeSeqLength(&timeSeq, maxNbrCOMdelay+2);
    resizeSeqLength(&channelSeq, maxNbrCOMdelay+2);
    if (nbrChannels == 2)
    {
        resizeSeqLength(&timeSeqAcc, maxNbrCOMdelayAcc+2);
        resizeSeqLength(&channelSeqAcc, maxNbrCOMdelayAcc+2);
    }

//    读取时间数据
    AqT3DataDescriptor *dataDescPtr = dataPtrList.last();
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
            if (nbrChannels == 2)
                TimeOfFlightAcc = TimeOfFlight + delayInCOMAcc[channel-1];
            TimeOfFlight += delayInCOM[channel-1];
            // channel-1 通道所插入的序列编号应为 nbrCOMdelay[channel-1]
            if (TimeOfFlight > timeCOMunit)
            {
                TimeOfFlight -= timeCOMunit;
                indexCOM = nbrCOMdelay[channel-1] + 1;
            }
            else
                indexCOM = nbrCOMdelay[channel-1];
            index = findInsertPosition(timeSeq[indexCOM], TimeOfFlight);        // 按时间升序排列
            timeSeq[indexCOM].insert(index, TimeOfFlight);
            channelSeq[indexCOM].insert(index, channel-1);
            if (nbrChannels == 2)
            {
                if (TimeOfFlightAcc > timeCOMunit)
                {
                    TimeOfFlightAcc -= timeCOMunit;
                    indexCOMAcc = nbrCOMdelayAcc[channel-1] + 1;
                }
                else
                    indexCOMAcc = nbrCOMdelayAcc[channel-1];
                index = findInsertPosition(timeSeqAcc[indexCOMAcc], TimeOfFlightAcc);
                timeSeqAcc[indexCOMAcc].insert(index, TimeOfFlightAcc);
                channelSeqAcc[indexCOMAcc].insert(index, channel-1);
            }
            mark = true;
        }
        else
        {
            if (mark and timeSeq.first().size() >= nbrChannels)                            // 计算上一轮符合
            {
                for (int i = 0; i < timeSeq.first().size()-nbrChannels+1; i++)
                {
                    spacing = findSpacing(timeSeq.first(), i, tolerance);          // 求符合窗口在起始位置处的跨度
                    if (checkCoincidence(channels, nbrChannels, channelSeq.first(), i, i+spacing)) // 查看该跨度内是否有符合
                    {
                        (*nbrCoin)++;
                        i = i+spacing;                                     // 计算过符合的区间可以跳过
                    }
                }
                if (nbrChannels == 2)
                {
                    for (int i = 0; i < timeSeqAcc.first().size()-2+1; i++)
                    {
                        spacing = findSpacing(timeSeqAcc.first(), i, tolerance);          // 求符合窗口在起始位置处的跨度
                        if (checkCoincidence(channels, 2, channelSeqAcc.first(), i, i+spacing)) // 查看该跨度内是否有符合
                        {
                            (*nbrCoinAcc)++;
                            i = i+spacing;                                     // 计算过符合的区间可以跳过
                        }
                    }
                }
            }
            mark = false;
            QVector<int> v0;
            timeSeq.first().clear(); timeSeq.removeFirst(); timeSeq.append(v0);
            channelSeq.first().clear(); channelSeq.removeFirst(); channelSeq.append(v0);
            if (nbrChannels == 2)
            {
                timeSeqAcc.first().clear(); timeSeqAcc.removeFirst(); timeSeqAcc.append(v0);
                channelSeqAcc.first().clear(); channelSeqAcc.removeFirst(); channelSeqAcc.append(v0);
            }
        }
    }
}
