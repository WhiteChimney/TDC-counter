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
#include <QTextCodec>
#include <QSizePolicy>
#include <QLCDNumber>

#include "QSimpleLed.h"

void initializeConfigurations();

ViStatus configTDC
        (ViSession idInstr,
         int countEvents,
         bool *channelConfig,
         double *level,
         int *slope,
         AqT3ReadParameters* readParamPtr);

void computeSingleChannelCount
        (int* nbrSCC, int* nbrSCCfuture,
         QVector<AqT3DataDescriptor*> dataPtrList,
         double *delayCN,
         int freqCOM, int countEvents);

void computeCoincidenceCount
        (QVector<AqT3DataDescriptor*> dataPtrList,
         QVector<QVector<int>> timeSeq,
         QVector<QVector<int>> timeSeqAcc,
         QVector<QVector<int>> channelSeq,
         QVector<QVector<int>> channelSeqAcc,
         int nbrChannels,
         int* channels,
         int* nbrCoin,
         int tolerance,
         int* delayMulti,
         int* nbrCoinAcc,
         int delayAcc,
         double *delayCN, int freqCOM, int countEvents);

void clear2DintVector(QVector<QVector<int>> v);

void computeHistogramCount
        (AqT3DataDescriptor* dataDescPtr,
         int channel1, int channel2, double delay,
         double timeStart, double binWidth, int nbrIntervals, int* binHeight);

enum RecordMode {notRecording, SingleCountOnly, CoinCountOnly, SingleAndCoinCount};

#endif // STDAFX_H
