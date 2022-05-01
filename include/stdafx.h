#ifndef STDAFX_H
#define STDAFX_H

#include "AcqirisImport.h"
#include "AcqirisT3Import.h"

#include <cstring>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QtMath>
#include <QTimer>
#include <QWaitCondition>
#include <QMutex>
#include <QFileDialog>
#include <QDateTime>
#include <QDesktopServices>
#include <QTextBlock>
#include <QTextDocument>
#include <QThread>
#include <QSettings>

void initializeConfigurations();

ViStatus configTDC
        (ViSession idInstr,
         int countEvents,
         bool *channelConfig,
         double *level,
         int *slope,
         AqT3ReadParameters* readParamPtr);

QString testTC890();

void algorithmSingleChannelCount(AqT3DataDescriptor*,int*);

void algorithmCoincidenceCount
        (AqT3DataDescriptor* dataDescPtr,
         int* nbrCoin, int* nbrAccCoin,
         int channel1, int channel2,
         int tolerance,
         int delay, int delayAcc);

void algorithmHistogramCount
        (AqT3DataDescriptor* dataDescPtr,
         int channel1, int channel2, double delay,
         double timeStart, double binWidth, int nbrIntervals, int* binHeight);

enum RecordMode {notRecording, SingleCountOnly, CoinCountOnly, SingleAndCoinCount};

#endif // STDAFX_H
