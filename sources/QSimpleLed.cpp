#include "QSimpleLed.h"

#include <QGradient>
#include <QPainter>
#include <QDebug>

//
// https://www.schemecolor.com/
QSimpleLed::ColorGroup QSimpleLed::smColorPalette[6] {
    // CUSTOM
    { QColor(0, 0, 0), QColor(0, 0, 0), QColor(0, 0, 0), QColor(0, 0, 0) },
    // RED
    { QColor(255, 0, 0), QColor(191, 0, 0), QColor(28, 0, 0), QColor(128, 0, 0) },
    // GREEN
    { QColor(0, 255, 0), QColor(0, 191, 0), QColor(0, 28, 0), QColor(0, 128, 0) },
    // BLUE
    { QColor(0, 0, 255), QColor(0, 0, 191), QColor(0, 0, 28), QColor(0, 0, 128) },
    // YELLOW
    { QColor(255, 255, 0), QColor(191, 191, 0), QColor(28, 28, 0), QColor(128, 128, 0) },
    // ORANGE
    { QColor(255, 165, 0), QColor(255, 113, 1), QColor(20, 8, 5), QColor(99, 39, 24) },
};

QSimpleLed::QSimpleLed(QWidget *parent, QSimpleLed::LEDCOLOR color)
    : QAbstractButton(parent)
    , mColor(color)
{
    setCheckable(true);
    setMinimumSize(20, 20);
}

void QSimpleLed::setStates(QSimpleLed::LEDSTATES states)
{
    switch (states) {
    case ON:
        resetStatus();
        setChecked(true);
        mStates = ON;
        break;

    case OFF:
        resetStatus();
        break;

    case BLINK:
        resetStatus();

        if (!mBlinkTimer) {
            mBlinkTimer = new QTimer(this);
            connect(mBlinkTimer, &QTimer::timeout, this, &QSimpleLed::onBlinkTimerTimeout);
        }
        mBlinkTimer->setInterval(mInterval);
        mBlinkTimer->start();
        mStates = BLINK;
        break;

    default:
        qDebug() << "LED - unknown states!!!";
    }

    update();
}

void QSimpleLed::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    qreal realSize = qMin(width(), height());

    QRadialGradient radialGent;

    //
    // gradient - 1
    radialGent = QRadialGradient(QPointF(-500, -500)
                                 , 1500
                                 , QPointF(-500, -500));
    radialGent.setColorAt(0, QColor(224, 224, 224));
    radialGent.setColorAt(1, QColor(28, 28, 28));

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);      // 反锯齿
    painter.translate(width()/2, height()/2);           // 绘点移到控件中心处
    painter.scale(realSize/1000, realSize/1000);
    painter.setBrush(QBrush(radialGent));
    painter.drawEllipse(QPointF(0, 0), 500, 500);

    //
    // gradient - 2
    radialGent = QRadialGradient(QPointF(500, 500)
                                 , 1500
                                 , QPointF(500, 500));
    radialGent.setColorAt(0, QColor(224, 224, 224));
    radialGent.setColorAt(1, QColor(28, 28, 28));

    painter.setBrush(QBrush(radialGent));
    painter.drawEllipse(QPointF(0, 0), 450, 450);

    if (isChecked()) {
        //
        // gradient - 3
        radialGent = QRadialGradient(QPointF(-500, -500)
                                     , 1500
                                     , QPointF(-500, -500));
        radialGent.setColorAt(0, smColorPalette[mColor].on0);
        radialGent.setColorAt(1, smColorPalette[mColor].on1);

    } else {
        //
        // gradient - 4
        radialGent = QRadialGradient(QPointF(500, 500)
                                     , 1500
                                     , QPointF(500, 500));
        radialGent.setColorAt(0, smColorPalette[mColor].off0);
        radialGent.setColorAt(1, smColorPalette[mColor].off1);
    }

    painter.setBrush(QBrush(radialGent));
    painter.drawEllipse(QPoint(0, 0), 400, 400);

    ////////////////////////////////////////////////////////////////
//    float test2 = realSize*0.5;
//    float test15 = realSize*1.5;
//    float test45 = realSize*0.45;
//    float test4 = realSize*0.4;

//    //
//    // gradient - 1
//    radialGent = QRadialGradient(QPointF(-test2, -test2)
//                                 , test15
//                                 , QPointF(-test2, -test2));
//    radialGent.setColorAt(0, QColor(224, 224, 224));
//    radialGent.setColorAt(1, QColor(28, 28, 28));

//    QPainter painter(this);

//    painter.setRenderHint(QPainter::Antialiasing);      // 反锯齿
//    painter.translate(width()/2, height()/2);           // 绘点移到控件中心处
////    painter.scale(realSize/1000, realSize/1000);
//    painter.setPen(Qt::NoPen);
//    painter.setBrush(QBrush(radialGent));
//    painter.drawEllipse(QPointF(0, 0), test2, test2);

//    //
//    // gradient - 2
//    radialGent = QRadialGradient(QPointF(test2, test2)
//                                 , test15
//                                 , QPointF(test2, test2));
//    radialGent.setColorAt(0, QColor(224, 224, 224));
//    radialGent.setColorAt(1, QColor(28, 28, 28));

//    painter.setBrush(QBrush(radialGent));
//    painter.drawEllipse(QPointF(0, 0), test45, test45);

//    if (isChecked()) {
//        //
//        // gradient - 3
//        radialGent = QRadialGradient(QPointF(-test2, -test2)
//                                     , test15
//                                     , QPointF(-test2, -test2));
//        radialGent.setColorAt(0, mOnColor1);
//        radialGent.setColorAt(1, mOnColor2);

//    } else {
//        //
//        // gradient - 4
//        radialGent = QRadialGradient(QPointF(test2, test2)
//                                     , test15
//                                     , QPointF(test2, test2));
//        radialGent.setColorAt(0, mOffColor1);
//        radialGent.setColorAt(1, mOffColor2);
//    }

//    painter.setBrush(QBrush(radialGent));
//    painter.drawEllipse(QPointF(0, 0), test4, test4);
}

void QSimpleLed::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    update();
}

void QSimpleLed::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void QSimpleLed::onBlinkTimerTimeout()
{
    setChecked(!isChecked());
}

void QSimpleLed::resetStatus()
{
    if (mBlinkTimer && mBlinkTimer->isActive()) {
        mBlinkTimer->stop();
    }

    setChecked(false);
    mStates = OFF;
}
