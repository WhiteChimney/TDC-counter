#ifndef HERALDQKDWIDGET_H
#define HERALDQKDWIDGET_H

#include <QWidget>
#include <QDebug>

#include "cliplcdnumber.h"

#include "AcqirisImport.h"
#include "AcqirisT3Import.h"

namespace Ui {
class HeraldQkdWidget;
}

class HeraldQkdWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HeraldQkdWidget(QWidget *parent = nullptr);
    ~HeraldQkdWidget();

private slots:
    void on_buttonStart_released();
    void on_buttonStop_released();
    void on_buttonReturn_released();

private:
    Ui::HeraldQkdWidget *ui;
    QList<QList<ClipLcdNumber*>> vLcdCounts;
    double vCounts[4][4] = {{0.0}};

signals:
    void sendReturnSignal();
    void heraldQkdRequestSync();
    void heraldQkdRequestStopSync();

public slots:
    void dealDataReturned(AqT3DataDescriptor *dataDescPtr);
    void dealTimeOut();

public:
    void setupLcdCounts();
};

#endif // HERALDQKDWIDGET_H
