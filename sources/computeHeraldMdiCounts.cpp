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
    // 使用十八位的二进制数 channelMark 来表示所有通道的存在性
    // 最低位表示通道 1，最高位表示通道 6
    // 每位如果是 0 则表示不存在该通道
    int channelMark = 0;
    for (int i = start; i <= end; i++)
        channelMark += pow(2,channelSeq[i]);

    // 接下来仅筛选出 relay 中所需要的几种响应组合
    // 注：上单引号为 C++14 引入的新特性，为数字分隔符
    switch (channelMark) {
    case 0b000000'010000'101100: nbrCounts[0][0]++; break; // B- A  2- 1-
    case 0b000000'010100'101000: nbrCounts[1][0]++; break; // B- A  2- 1
    case 0b000100'010000'101000: nbrCounts[2][0]++; break; // B- A  2- 1+
    case 0b000000'011000'100100: nbrCounts[3][0]++; break; // B- A  2  1-
    case 0b000000'011100'100000: nbrCounts[4][0]++; break; // B- A  2  1
    case 0b000100'011000'100000: nbrCounts[5][0]++; break; // B- A  2  1+
    case 0b001000'010000'100100: nbrCounts[6][0]++; break; // B- A  2+ 1-
    case 0b001000'010100'100000: nbrCounts[7][0]++; break; // B- A  2+ 1
    case 0b001100'010000'100000: nbrCounts[8][0]++; break; // B- A  2+ 1+
    case 0b000000'110000'001100: nbrCounts[0][1]++; break; // B  A  2- 1-
    case 0b000000'110100'001000: nbrCounts[1][1]++; break; // B  A  2- 1
    case 0b000100'110000'001000: nbrCounts[2][1]++; break; // B  A  2- 1+
    case 0b000000'111000'000100: nbrCounts[3][1]++; break; // B  A  2  1-
    case 0b000000'111100'000000: nbrCounts[4][1]++; break; // B  A  2  1
    case 0b000100'111000'000000: nbrCounts[5][1]++; break; // B  A  2  1+
    case 0b001000'110000'000100: nbrCounts[6][1]++; break; // B  A  2+ 1-
    case 0b001000'110100'000000: nbrCounts[7][1]++; break; // B  A  2+ 1
    case 0b001100'110000'000000: nbrCounts[8][1]++; break; // B  A  2+ 1+
    case 0b100000'010000'001100: nbrCounts[0][2]++; break; // B+ A  2- 1-
    case 0b100000'010100'001000: nbrCounts[1][2]++; break; // B+ A  2- 1
    case 0b100100'010000'001000: nbrCounts[2][2]++; break; // B+ A  2- 1+
    case 0b100000'011000'000100: nbrCounts[3][2]++; break; // B+ A  2  1-
    case 0b100000'011100'000000: nbrCounts[4][2]++; break; // B+ A  2  1
    case 0b100100'011000'000000: nbrCounts[5][2]++; break; // B+ A  2  1+
    case 0b101000'010000'000100: nbrCounts[6][2]++; break; // B+ A  2+ 1-
    case 0b101000'010100'000000: nbrCounts[7][2]++; break; // B+ A  2+ 1
    case 0b101100'010000'000000: nbrCounts[8][2]++; break; // B+ A  2+ 1+
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
         int *nbrCOMdelay,
         int *delayInCOM,
         int extraDelay,
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
        int TimeOfFlightP, TimeOfFlightPP;            // 分别用来添加一次和两次 extraDelay

        // flag 为 0 表示 COM 信号，channel 为 7 表示内存切换
        if (channel != 0 and channel != 7)                        // Channel=1-6 are the physical channels;
                                                // Data = an integer giving the time value in units of 50 ps
                                                // Channel=7 is for marker data.
        {
            mark = true;

            int indexCOM;

            TimeOfFlight += delayInCOM[channel-1];
            TimeOfFlightP = TimeOfFlight + extraDelay;
            TimeOfFlightPP = TimeOfFlightP + extraDelay;

            // channel-1 通道所插入的序列编号应为 nbrCOMdelay[channel-1]

            // 插入加了一次 extraDelay 的，作为基准时间
            if (TimeOfFlightP > timeCOMunit)
            {
                TimeOfFlightP -= timeCOMunit;
                indexCOM = (nbrCOMdelay[channel-1] + 1 + *COM_HEAD) % timeSeq.size();
            }
            else
                indexCOM = (nbrCOMdelay[channel-1] + *COM_HEAD) % timeSeq.size();
            index = findInsertPosition(timeSeq[indexCOM], TimeOfFlightP);        // 按时间升序排列
            timeSeq[indexCOM].insert(index, TimeOfFlightP);
            channelSeq[indexCOM].insert(index, channel-1+6);      // 通道号加 6
            if (channel == 5) continue;                           // 如果是通道 5，即 A 的信号，则无需继续做延时差

            // 插入未加 extraDelay 的，作为减少延时的信号
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

            // 插入加了两次 extraDelay 的，作为增加延时的信号
            if (TimeOfFlightPP > timeCOMunit)
            {
                TimeOfFlightPP -= timeCOMunit;
                indexCOM = (nbrCOMdelay[channel-1] + 1 + *COM_HEAD) % timeSeq.size();
            }
            else
                indexCOM = (nbrCOMdelay[channel-1] + *COM_HEAD) % timeSeq.size();
            index = findInsertPosition(timeSeq[indexCOM], TimeOfFlightPP);        // 按时间升序排列
            timeSeq[indexCOM].insert(index, TimeOfFlightPP);
            channelSeq[indexCOM].insert(index, channel-1+12);      // 通道号加 12
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
