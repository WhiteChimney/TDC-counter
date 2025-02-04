#include "AcqirisImport.h"
#include "AcqirisT3Import.h"
#include <cmath>
#include <QVector>
#include <QDebug>

int findInsertPosition(QVector<int> timeSeq, int TimeOfFlight);

int findSpacing(QVector<int> timeSeq, int i, int toleranceMulti);

int findHeraldCoincidence
    (QVector<int> channelSeq, int start, int end, long long nbrCounts[][3])
{
    // 使用六位的十进制数 channelMark 来表示所有通道的存在性
    // 最低位表示通道 1，最高位表示通道 6
    // 每位如果是 0 则表示不存在该通道
    int channelMark = 0;
    for (int i = start; i <= end; i++)
        channelMark += pow(10,channelSeq[i]);

    // 接下来仅筛选出标记 QKD 中所需要的几种响应组合
    switch (channelMark) {
    case 110011: nbrCounts[0][0]++; break; // AB12
    case 111100: nbrCounts[0][1]++; break; // AB34
    case 111001: nbrCounts[0][2]++; break; // AB14
    case     11: nbrCounts[1][0]++; break; //   12
    case   1100: nbrCounts[1][1]++; break; //   34
    case   1001: nbrCounts[1][2]++; break; //   14
    case  10011: nbrCounts[2][0]++; break; // A 12
    case  11100: nbrCounts[2][1]++; break; // A 34
    case  11001: nbrCounts[2][2]++; break; // A 14
    case 100011: nbrCounts[3][0]++; break; //  B12
    case 101100: nbrCounts[3][1]++; break; //  B34
    case 101001: nbrCounts[3][2]++; break; //  B14
    default:      channelMark = -1; break;
    }

    return channelMark;
}

void computeHeraldMdiCounts
        (AqT3DataDescriptor *dataDescPtr,
         QList<QVector<int>> &timeSeq,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
         QList<QVector<int>> &channelSeq,    // 升序排列后的时间，与通道编号一一对应
         long long nbrCounts[][3],
         int tolerance,
         int *nbrCOMdelayPre, int *delayInCOMPre,
         int *nbrCOMdelay, int *delayInCOM,
         int *nbrCOMdelayAft, int *delayInCOMAft,
         int timeCOMunit,
         int *COM_HEAD)
{
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

        // flag 为 0 表示 COM 信号，channel 为 7 表示内存切换
        if (channel != 0 and channel != 7)                        // Channel=1-6 are the physical channels;
                                                // Data = an integer giving the time value in units of 50 ps
                                                // Channel=7 is for marker data.
        {
            int indexCOM;
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
            mark = true;
        }
        else
        {
            if (mark and timeSeq[*COM_HEAD].size() >= 2)                            // 计算上一轮符合
            {
                for (int i = 0; i < timeSeq[*COM_HEAD].size()-1; i++)
                {
                    spacing = findSpacing(timeSeq[*COM_HEAD], i, tolerance);          // 求符合窗口在起始位置处的跨度
                    if (spacing > 0) // 该跨度内有符合
                    {
                        findHeraldCoincidence
                                (channelSeq[*COM_HEAD], i, i+spacing, nbrCounts);
                        i = i+spacing;                                     // 计算过符合的区间可以跳过
                    }
                }
            }
            mark = false;
            timeSeq[*COM_HEAD].clear();
            channelSeq[*COM_HEAD].clear();
            *COM_HEAD = ((*COM_HEAD)+1) % timeSeq.size();
        }
    }
}
