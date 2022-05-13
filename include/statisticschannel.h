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
    int countStd;

public:
    QLabel* labelChannelName;
    QLCDNumber* lcdCount;
    QLCDNumber* lcdCountStd;
    void setChannelName(QString channelName0);
    void setCountPtr(int* countPtr0);
    void updateLcdCount();
    void updateLcdCountStd();

};

#endif // STATISTICSCHANNEL_H
