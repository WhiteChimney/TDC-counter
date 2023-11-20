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

void computeCoincidenceCount
        (AqT3DataDescriptor* dataDescPtr,
         QList<QVector<int>> timeSeq,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
         QList<QVector<int>> timeSeqAcc,
         QList<QVector<int>> channelSeq,    // 升序排列后的时间，与通道编号一一对应
         QList<QVector<int>> channelSeqAcc,
         int nbrChannels,
         int* channels,
         int* nbrCoin,
         int tolerance,
         int* nbrCoinAcc,
         int *nbrCOMdelay, int *nbrCOMdelayAcc,
         int *delayInCOM, int *delayInCOMAcc,
         int timeCOMunit,
         int *COM_HEAD);

void computeCoincidenceCountAcrossDevices_HOLD
            (AqT3DataDescriptor* dataDescPtr,
             QList<QVector<int>>& timeSeq,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
             QList<QVector<int>>& timeSeqAcc,
             QList<QVector<int>>& channelSeq,    // 升序排列后的时间，与通道编号一一对应
             QList<QVector<int>>& channelSeqAcc,
             int nbrChannels,
             int* channels,
             int *nbrCOMdelay, int *nbrCOMdelayAcc,
             int *delayInCOM, int *delayInCOMAcc,
             int timeCOMunit,
             int *COM_HEAD);

void computeCoincidenceCountAcrossDevices_COMPUTE(
                 QList<QVector<int>> &timeSeqX1,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
                 QList<QVector<int>> &timeSeqAccX1,
                 QList<QVector<int>> &channelSeqX1,    // 升序排列后的时间，与通道编号一一对应
                 QList<QVector<int>> &channelSeqAccX1,
                 QList<QVector<int>> &timeSeqX2,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
                 QList<QVector<int>> &timeSeqAccX2,
                 QList<QVector<int>> &channelSeqX2,    // 升序排列后的时间，与通道编号一一对应
                 QList<QVector<int>> &channelSeqAccX2,
                 int nbrChannelsX1, int nbrChannelsX2,
                 int* channelsX1, int *channelsX2,
                 int* nbrCoin,
                 int tolerance,
                 int* nbrCoinAcc,
                 int *COM_HEAD, int *COM_HEAD_2, int *COM_HEAD_X);

void clear2DintVector(QList<QVector<int>> *v);

void resizeSeqLength(QList<QVector<int>> *v, int l);

void resizeSeqLength(QList<QVector<double>> *v, int l);

void resizeSeqLength(QVector<QVector<int>> *v, int l);

void resizeSeqLength(QVector<QVector<double>> *v, int l);

void computeHistogramCount(AqT3DataDescriptor* dataDescPtr,
                           QVector<QVector<double>> timeSeq1, QVector<QVector<double>> timeSeq2,
                           int channel1, int channel2, double delay,
                           double timeStart, double binWidth, int nbrIntervals, int* binHeight,
                           int *nbrCOMdelay,
                           int *delayInCOM,
                           int timeCOMunit,
                           int *COM_HEAD);

void computeHistogramCountAcrossDevices_HOLD
                         (AqT3DataDescriptor* dataDescPtr,
                          QVector<QVector<double>> &timeSeq1,
                          int channel1,
                          int *nbrCOMdelay,
//                          int maxNbrCOMdelay,
                          int *delayInCOM,
                          int timeCOMunit,
//                          int countEvents,
                          int *COM_HEAD);

void computeHistogramCountAcrossDevices_COMPUTE
    (QVector<QVector<double>>& timeSeqX1, QVector<QVector<double>>& timeSeqX2,
     int comRange, int timeCOMunit, int *COM_HEAD, int *COM_HEAD_2, int *COM_HEAD_X,
     double delay, double timeStart, double binWidth, int nbrIntervals, int* binHeight);

enum RecordMode {notRecording, SingleCountOnly, CoinCountOnly, SingleAndCoinCount};

#endif // STDAFX_H
