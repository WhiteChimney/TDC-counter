#ifndef EXTERNALAPPLICATIONSWIDGET_H
#define EXTERNALAPPLICATIONSWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QThread>
#include <QListView>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include "QSimpleLed.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "tsp01.h"

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
    QSimpleLed *SPstatusIndicator, *TSP01statusIndicator;

private:
    QSerialPort *serial;
    QList<QSerialPortInfo> spList;
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity parity;
    QByteArray receivedBytes, sentBytes;

public:
    void setupSPIndicator(), setupTSPIndicator();
    void fetchUiData();
    void pushUiData();
    void saveToIni();
    void loadFromIni();

    void refreshPorts();
    bool openSerialPort(QSerialPortInfo);
    QString readData();
    void sendData(QString dataText);

private:
    TSP01 *tsp;
    double temperature, temperatureOffset;
    double humidity, humidityOffset;

public:
    void refreshTSPlist();

signals:
    void requestData();

private:
    int *nbrSCC;
    QVector<int*> vNbrCoin;

public slots:
    void dealRequestedData(int* nbrSCC, QVector<int*> vNbrCoin);
    void dealSingleCountTimeup();

public:
    void customizedSPcommands_start();
    void customizedSPcommands_stop();

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
};

#endif // EXTERNALAPPLICATIONSWIDGET_H
