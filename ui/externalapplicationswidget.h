#ifndef EXTERNALAPPLICATIONSWIDGET_H
#define EXTERNALAPPLICATIONSWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QThread>
#include <QListView>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QFile>
#include <QComboBox>
#include "QSimpleLed.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "tsp01.h"
#include "dp832a_serial.h"
#include "dp832a_usb.h"
#include "smc100cc_serial.h"
#include "qaskydelayboard.h"

namespace Ui {
class ExternalApplicationsWidget;
}

class ExternalApplicationsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExternalApplicationsWidget(QWidget *parent = nullptr);
    ~ExternalApplicationsWidget();

private:
    Ui::ExternalApplicationsWidget *ui;

    // 串口
private:
    QSimpleLed *SPstatusIndicator;
//    DP832A_Serial* spDevice;
    SMC100CC_Serial* spDevice;
    QList<QSerialPortInfo> spList;
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity parity;
    QByteArray receivedBytes, sentBytes;

public:
    void setupSPIndicator();
    void fetchUiData();
    void pushUiData();
    void saveToIni();
    void loadFromIni();
    void refreshPorts();

    // TSP01
private:
    QSimpleLed *TSP01statusIndicator;
    TSP01 *tsp;
    QList<QString> tspList;
    double temperature, temperatureOffset;
    double humidity, humidityOffset;

public:
    void setupTSPIndicator();
    void refreshTSPlist();

    // dp832a
private:
    QSimpleLed *DP832UsbIndicator;
    DP832A_USB* dp832usb;

public:
    void setupDP832UsbIndicator();

    // qasky_delayboard
private:
    QaskyDelayBoard *delayBoard;
    QList<QLineEdit*> countChannelList;
    QList<QLineEdit*> delayChannelList;
    QList<int> delayAdjustDirection;
    QList<double> countBefore;
    QList<double> countCurrent;
    int currentRound;

signals:
    void requestDelayFeedback();
    void requestStopDelayFeedback();

public slots:
    void dealDelayFeedbackDataReceived(int* m_nbrSCC);
    void doSingleCountTimeoutFeedback();


    // qasky_delayboard
private:
    SMC100CC_Serial *smc;
    QList<QLineEdit*> smcCountChannelList;
    QList<QLineEdit*> smcTargetCountList;
    QList<QLineEdit*> smcChannelList;
    QList<QComboBox*> smcAngleDirList;
//    QList<int> angleAdjustDirection;
//    QList<double> smcCountBefore;
    QList<double> smcCountCurrent;
    int smcCurrentRound;

    bool enableErrorFeedback = false;
    long long *errorCountPtr[2], *correctCountPtr[2];
    double errorRateCurrent, errorRateBefore;
    int errorCurrentRound;
    int errorFeedbackDirection;
    double correctCountCurrent, correctCountBefore;
    double errorCountCurrent, errorCountBefore;

signals:
    void requestAngleFeedback();
    void requestStopAngleFeedback();

public slots:
    void dealAngleFeedbackDataReceived(int* m_nbrSCC);
    void doSmcSingleCountTimeoutFeedback();
    void dealHeraldQkdErrorFeedback(long long *m_errorCountPtr[2],
                                    long long *m_correctCountPtr[2]);
    void dealHeraldQkdStopErrorFeedback();

    // 通用
private:
    int *nbrSCC;
    QVector<int*> vNbrCoin;
    bool dataRec = false;

signals:
    void requestData();
    void dataReceived();
    void externalAppStopped();

public:
    void customizedSPcommands_start();
    void customizedSPcommands_stop();

public slots:
    void dealRequestedData(int* nbrSCC, QVector<int*> vNbrCoin);
    void dealSingleCountTimeup();
    void dealMainAppClosed();

private slots:
    void on_checkboxSPcustomize_stateChanged(int checkState);
    void on_buttonOpenSP_released();
    void on_buttonStart_released();
    void on_buttonStop_released();
    void on_buttonSend_released();
    void on_buttonCloseSP_released();
    void on_buttonRefresh_released();
    void on_buttonReceive_released();
    void on_buttonTest_released();
    void on_buttonRefreshTSP01_released();
    void on_buttonOpenTSP01_released();
    void on_buttonCloseTSP01_released();
    void on_buttonTestTSP01_released();
    void on_buttonRefreshDataTSP01_released();
    void on_buttonDP832UsbInitiate_released();
    void on_buttonDP832UsbClose_released();
    void on_buttonDP832UsbTest_released();
    void on_buttonDP832UsbSendCmd_released();
    void on_buttonSetDelay_released();
    void on_comboDelayboardSpList_activated(int index);
    void on_buttonDelayFeedbackAdd_released();
    void on_buttonDelayFeedbackRemove_released();
    void on_buttonDelayFeedbackStart_released();
    void on_buttonSmcSetAngle_released();
    void on_buttonDelayFeedbackStop_released();
    void on_comboSmcSpList_activated(int index);
    void on_buttonAngleFeedbackAdd_released();
    void on_buttonAngleFeedbackRemove_released();
    void on_buttonAngleFeedbackStart_released();
    void on_buttonAngleFeedbackStop_released();
    void on_buttonSmcOpenPort_released();
    void on_buttonSmcClosePort_released();
    void on_buttonSmcHoming_released();
};

#endif // EXTERNALAPPLICATIONSWIDGET_H
