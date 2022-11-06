#ifndef TSP01_H
#define TSP01_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QThread>

#include "visa.h"
#include "TLTSP.h"

class TSP01 : public QObject
{
    Q_OBJECT
public:
    explicit TSP01(QObject *parent = nullptr);

signals:

private:
    ViUInt32 TIMEOUT_MILLISEC = 5000;
    ViSession ihdl;
    ViStatus err = !VI_SUCCESS;
    ViReal64 errValue = 1958.0;

public:
    QList<ViChar*> findInstruments();
    ViStatus initializeDevice(ViChar* rscPtr);
    ViStatus closeDevice();
    QString checkDeviceInfo();
    ViStatus getErrorCode();
    ViReal64 getTemperatureOffset();
    ViReal64 getHumidityOffset();
    ViReal64 getTemperature();
    ViReal64 getHumidity();
};

#endif // TSP01_H
