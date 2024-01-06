#ifndef HERALDQKDWIDGET_H
#define HERALDQKDWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QAction>

#include "cliplcdnumber.h"

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

signals:
    void sendReturnSignal();
    void heraldQkdRequestSync();
    void heraldQkdRequestStopSync();

public slots:

public:
    void setupLcdCounts();
};

#endif // HERALDQKDWIDGET_H
