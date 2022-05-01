#ifndef COINCIDENCEWIDGET_H
#define COINCIDENCEWIDGET_H

#include <QWidget>
#include "stdafx.h"

namespace Ui {
class CoincidenceWidget;
}

class CoincidenceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CoincidenceWidget(QWidget *parent = nullptr, int index = 0);
    ~CoincidenceWidget();
    int index;

signals:
    void returnSignal(int index);
    void askDealAcqBankSwitchCoin(int index);
    void askStopDealAcqBankSwitchCoin(int index);
    void coinTimerNeedsSync(int index);

public slots:
    void dealTimeOut();
    void dealAcqThreadBankSwitchCoin(AqT3DataDescriptor*);
    void dealSaveCoinData();

private slots:
    void on_buttonReturn_released();
    void on_buttonStart_released();
    void on_buttonStop_released();
    void on_coinChannel1_currentIndexChanged(int index);
    void on_checkboxAccumlateTime_stateChanged(int arg1);

private:
    Ui::CoincidenceWidget *ui;
    QTimer* timerCoin;
    int nbrCoin = 0;
    int nbrAccCoin = 0;
    int channel1, channel2;
    int tolerance;
    int delay, delayAcc;
    bool enableAccumulateTime;
    double accumulateTime = 1.0;
    bool coinSavable = false;

    qint64 timeTic, timeToc;

    QString iniPath, iniName, tempFileName;
    QFile *fCoin = new QFile();
    QTextStream fStream;

public:
    void saveToIni();
    void loadFromIni();
    void startRecordCoinLocal(), stopRecordCoinLocal();
    void closeRecordFile();
};

#endif // COINCIDENCEWIDGET_H
