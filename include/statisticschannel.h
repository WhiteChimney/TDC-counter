#ifndef STATISTICSCHANNEL_H
#define STATISTICSCHANNEL_H

#include "stdafx.h"

class StatisticsChannel
{
public:
    StatisticsChannel();

private:
    QString channelName;
    int* countPtr;
    int stepsTotal, stepCurrent;
    int* counts;
    double countsAvg, countsAvgOld;
    double countsVar, countStd;
    void calcCurrentStats();

public:
    QLabel* labelChannelName;
    QLCDNumber* lcdCount;
    QLCDNumber* lcdCountStd;
    void setChannelName(QString channelName0);
    void setCountPtr(int* countPtr0);
    void updateLcdCount(int stepCurrent);
    void setNbrSteps(int stepsTotal);
};

#endif // STATISTICSCHANNEL_H
