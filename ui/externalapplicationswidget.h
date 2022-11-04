#ifndef EXTERNALAPPLICATIONSWIDGET_H
#define EXTERNALAPPLICATIONSWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QListView>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include "QSimpleLed.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

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
    QSimpleLed *statusIndicator;

private:
    QSerialPort *serial;
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
    bool openSerialPort(QSerialPortInfo);
    void readData();
    void sendData();

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
};

#endif // EXTERNALAPPLICATIONSWIDGET_H
