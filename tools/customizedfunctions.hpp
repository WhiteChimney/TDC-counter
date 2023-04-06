#ifndef CUSTOMIZEDFUNCTIONS_HPP
#define CUSTOMIZEDFUNCTIONS_HPP

#include "dp832a_serial.h"
#include "dp832a_usb.h"
#include "tsp01.h"

int sum(int a, int b)
{
    return a+b;
}

void computerA(DP832A_Serial* dp832, TSP01* tsp)
{
    if (tsp->getTemperature() > 21)
    {
        dp832->setVoltage(1, 1.2);
    }
    else
    {
        dp832->setVoltage(1, 1.1);
    }
}

void computerB(DP832A_USB* dp832, TSP01* tsp)
{
    if (tsp->getTemperature() > 21)
    {
        dp832->setVoltage(1, 1.2);
    }
    else
    {
        dp832->setVoltage(1, 1.1);
    }
}























#endif // CUSTOMIZEDFUNCTIONS_HPP
