#include "statisticschannel.h"

StatisticsChannel::StatisticsChannel()
{
    channelName = "channel";
    labelChannelName = new QLabel();
    countPtr = new int();
    lcdCount = new QLCDNumber(15);
    lcdCount->setSegmentStyle(QLCDNumber::Flat);
    lcdCount->setStyleSheet("background-color: white");
    countStd = 0;
    lcdCountStd = new QLCDNumber(9);
    lcdCountStd->setSegmentStyle(QLCDNumber::Flat);
    lcdCountStd->setStyleSheet("background-color: white");
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

void StatisticsChannel::updateLcdCount()
{
    lcdCount->display(*countPtr);
}

void StatisticsChannel::updateLcdCountStd()
{
    lcdCountStd->display(countStd);
}
