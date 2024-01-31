#include "cliplcdnumber.h"

ClipLcdNumber::ClipLcdNumber(QWidget *parent)
    : QLCDNumber(parent)
{
    setupUi();
}

ClipLcdNumber::ClipLcdNumber(uint numDigits, QWidget *parent)
    : QLCDNumber(parent)
{
    this->setDigitCount(numDigits);
    setupUi();
}

void ClipLcdNumber::setupUi()
{
    this->setSegmentStyle(QLCDNumber::Flat);
    this->setToolTip("点击复制");
    this->setCursor(Qt::PointingHandCursor);
}

void ClipLcdNumber::mousePressEvent(QMouseEvent*)
{
    setCursor(Qt::WaitCursor);
    QClipboard *clip = QApplication::clipboard();
    clip->setText(QString::number(this->value(),'g',12));
    QTimer *timer = new QTimer();
    connect(timer,&QTimer::timeout,this,[=](){
        this->setCursor(Qt::PointingHandCursor);
        delete timer;
    });
    timer->start(100);
}
