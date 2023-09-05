#include "AcqirisImport.h"
#include "AcqirisT3Import.h"
#include <QDebug>

// 计算统计直方图计数

bool channelToBeCalculated(int channel, int *channels, int nbrChannels);

void countSingle(AqT3DataDescriptor* dataDescPtr,
                 QVector<QVector<double>> timeSeq1,
                 int channel1,
                 double timeStart, double binWidth, int nbrIntervals, int* binHeight,
                 int *nbrCOMdelay,
                 int *delayInCOM,
                 int timeCOMunit,
                 int *COM_HEAD)
{
    long nbrSamples = dataDescPtr->nbrSamples;
    for (long n = 0 ; n < nbrSamples ; ++n)
    {
        int sample = ((long *)dataDescPtr->dataPtr)[n];  //dataPtr指向time value data buffer
        int channel = (sample & 0x70000000) >> 28;   //右移28位为channel位
        int indexCOM;

        if (channel == channel1)
        {
            int TimeOfFlight = sample & 0x0FFFFFFF;             //最右侧28位为计数值
            TimeOfFlight += delayInCOM[channel-1];
            if (TimeOfFlight > timeCOMunit)
            {
                TimeOfFlight -= timeCOMunit;
                indexCOM = (nbrCOMdelay[channel-1] + 1 + *COM_HEAD) % timeSeq1.size();
            }
            else
                indexCOM = (nbrCOMdelay[channel-1] + *COM_HEAD) % timeSeq1.size();

            timeSeq1[indexCOM].append(TimeOfFlight/20.0);
        }
        else if (channel == 0 or channel == 7)
        {
            for (int i = 0; i < timeSeq1.size(); i++)
            {
                int index = int((timeSeq1[*COM_HEAD][i]-timeStart)/binWidth);
                if (index >= 0 and index < nbrIntervals)
                    binHeight[index]++;
            }
            timeSeq1[*COM_HEAD].clear();
            *COM_HEAD = ((*COM_HEAD)+1) % timeSeq1.size();
        }
    }
}

void countDifference(AqT3DataDescriptor* dataDescPtr,
                     QVector<QVector<double>> timeSeq1, QVector<QVector<double>> timeSeq2,
                     int channel1, int channel2, double delay,
                     double timeStart, double binWidth, int nbrIntervals, int* binHeight,
                     int *nbrCOMdelay,
                     int *delayInCOM,
                     int timeCOMunit,
                     int *COM_HEAD)
{
    long nbrSamples = dataDescPtr->nbrSamples;

    for (long n = 0 ; n < nbrSamples ; ++n)
    {
        int sample = ((long *)dataDescPtr->dataPtr)[n];  //dataPtr指向time value data buffer
        int channel = (sample & 0x70000000) >> 28;   //右移28位为channel位

        if (channel == channel1 or channel == channel2)
        {
            int indexCOM;
            int TimeOfFlight = sample & 0x0FFFFFFF;             //最右侧28位为计数值
            TimeOfFlight += delayInCOM[channel-1];
            if (TimeOfFlight > timeCOMunit)
            {
                TimeOfFlight -= timeCOMunit;
                indexCOM = (nbrCOMdelay[channel-1] + 1 + *COM_HEAD) % timeSeq1.size();
            }
            else
                indexCOM = (nbrCOMdelay[channel-1] + *COM_HEAD) % timeSeq1.size();
            if (channel == channel1)
                timeSeq1[indexCOM].append(TimeOfFlight/20.0);
            else
                timeSeq2[indexCOM].append(TimeOfFlight/20.0);
        }
        else if (channel == 0 or channel == 7)
        {
            for (int i = 0; i < timeSeq1[*COM_HEAD].size(); i++)
            {
                for (int j = 0; j < timeSeq2[*COM_HEAD].size(); j++)
                {
                    double timeDiff = timeSeq2[*COM_HEAD][i] - timeSeq1[*COM_HEAD][j];
                    int index = int((timeDiff+delay-timeStart)/binWidth);
                    if (index >= 0 and index < nbrIntervals)
                        binHeight[index]++;
                }
            }
            timeSeq1[*COM_HEAD].clear();
            timeSeq2[*COM_HEAD].clear();
            *COM_HEAD = ((*COM_HEAD)+1) % timeSeq1.size();
        }
    }
}

void computeHistogramCount(AqT3DataDescriptor* dataDescPtr,
                           QVector<QVector<double>> timeSeq1, QVector<QVector<double>> timeSeq2,
                           int channel1, int channel2, double delay,
                           double timeStart, double binWidth, int nbrIntervals, int* binHeight,
                           int *nbrCOMdelay,
                           int *delayInCOM,
                           int timeCOMunit,
                           int *COM_HEAD)
{
    if (channel1 == channel2)
        countSingle(dataDescPtr,
                    timeSeq1,
                    channel1,
                    timeStart,binWidth,nbrIntervals,binHeight,
                    nbrCOMdelay,
                    delayInCOM,
                    timeCOMunit,
                    COM_HEAD);
    else
        countDifference(dataDescPtr,
                         timeSeq1,timeSeq2,
                         channel1,channel2,delay,
                         timeStart,binWidth,nbrIntervals,binHeight,
                         nbrCOMdelay,
                         delayInCOM,
                         timeCOMunit,
                         COM_HEAD);
}
