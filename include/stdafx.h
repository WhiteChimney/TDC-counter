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
         AqT3DataDescriptor* dataDescPtr,
         double *delayCN,
         int freqCOM, int countEvents);

//void computeCoincidenceCount
//        (AqT3DataDescriptor* dataDescPtr,
//         QList<QVector<int>> timeSeq,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
//         QList<QVector<int>> timeSeqAcc,
//         QList<QVector<int>> channelSeq,    // 升序排列后的时间，与通道编号一一对应
//         QList<QVector<int>> channelSeqAcc,
//         int nbrChannels,
//         int* channels,
//         int* nbrCoin,
//         int tolerance,
//         int* nbrCoinAcc,
//         int *nbrCOMdelay, int *nbrCOMdelayAcc,
//         int *delayInCOM, int *delayInCOMAcc,
//         int timeCOMunit);

void computeCoincidenceCount
        (AqT3DataDescriptor* dataDescPtr,
         QList<std::vector<int>> timeSeq,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
         QList<std::vector<int>> timeSeqAcc,
         QList<std::vector<int>> channelSeq,    // 升序排列后的时间，与通道编号一一对应
         QList<std::vector<int>> channelSeqAcc,
         int nbrChannels,
         int* channels,
         int* nbrCoin,
         int tolerance,
         int* nbrCoinAcc,
         int *nbrCOMdelay, int *nbrCOMdelayAcc,
         int *delayInCOM, int *delayInCOMAcc,
         int timeCOMunit);

void clear2DintVector(QList<std::vector<int>> *v);

void resizeSeqLength(QList<std::vector<int>> *v, int l);

void computeHistogramCount
        (AqT3DataDescriptor* dataDescPtr,
         int channel1, int channel2, double delay,
         double timeStart, double binWidth, int nbrIntervals, int* binHeight);

enum RecordMode {notRecording, SingleCountOnly, CoinCountOnly, SingleAndCoinCount};

#endif // STDAFX_H
