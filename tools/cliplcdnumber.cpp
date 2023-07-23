#include "cliplcdnumber.h"

ClipLcdNumber::ClipLcdNumber(QWidget *parent)
    : QLCDNumber(parent)
{
    this->setCursor(Qt::PointingHandCursor);
}

ClipLcdNumber::ClipLcdNumber(uint numDigits, QWidget *parent)
    : QLCDNumber(parent)
{
    this->setDigitCount(numDigits);
    this->setCursor(Qt::PointingHandCursor);
}

void ClipLcdNumber::mousePressEvent(QMouseEvent* event)
{
    setCursor(Qt::WaitCursor);
    QClipboard *clip = QApplication::clipboard();
    clip->setText(QString::number(this->value()));
    QTimer *timer = new QTimer();
    connect(timer,&QTimer::timeout,this,[=](){
        this->setCursor(Qt::PointingHandCursor);
        delete timer;
    });
    timer->start(100);
}
