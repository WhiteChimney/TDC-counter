#include "statisticschannel.h"

StatisticsChannel::StatisticsChannel()
{
    channelName = "channel";
    labelChannelName = new QLabel();
    countPtr = new int();
//    lcdCount = new QLCDNumber(15);
    lcdCount = new ClipLcdNumber(15);
    lcdCount->setSegmentStyle(QLCDNumber::Flat);
    lcdCount->setStyleSheet("background-color: white");
    lcdCount->display(0);
    countStd = 0;
//    lcdCountStd = new QLCDNumber(9);
    lcdCountStd = new ClipLcdNumber(9);
    lcdCountStd->setSegmentStyle(QLCDNumber::Flat);
    lcdCountStd->setStyleSheet("background-color: white");
    lcdCountStd->display(0);
}

void StatisticsChannel::setChannelName(QString channelName0)
{
    channelName = channelName0;
    labelChannelName->setText(channelName);
}

void StatisticsChannel::setCountPtr(int *countPtr0)
{
    countPtr = countPtr0;
}

void StatisticsChannel::updateLcdCount(int stepCurrent0, double unitTime)
{
    stepCurrent = stepCurrent0;
    counts[stepCurrent] = *countPtr;

    calcCurrentStats();

    lcdCount->display(countsAvg/unitTime);
    lcdCountStd->display(countStd/unitTime);
}

void StatisticsChannel::setNbrSteps(int stepsTotal0)
{
    stepsTotal = stepsTotal0;
    counts = new int[stepsTotal]();
    countsAvg = 0.0; countsAvgOld = 0.0;
    countsVar = 0.0; countStd = 0.0;
}

void StatisticsChannel::calcCurrentStats()
{
    countsAvg = stepCurrent/double(stepCurrent+1)*countsAvgOld
                + counts[stepCurrent]/double(stepCurrent+1);
    countsAvgOld = countsAvg;
//    countsVar = 0.0;
//    for (int i = 0; i <= stepCurrent; i++)
//    {
//        countsVar += 1.0/double(stepCurrent)*pow(counts[i]-countsAvg,2);
//    }
//    countStd = sqrt(countsVar);

//    改成 sqrt(N) 的偏差估计方式，提高易用性
    countStd = sqrt(countsAvg/(stepCurrent+1));
}
