#ifndef CLIPLCDNUMBER_H
#define CLIPLCDNUMBER_H

#include <QLCDNumber>
#include <QWidget>
#include <QApplication>
#include <QMouseEvent>
#include <QClipboard>
#include <QCursor>
#include <QTimer>

class ClipLcdNumber : public QLCDNumber
{
    Q_OBJECT
public:
    ClipLcdNumber(QWidget *parent = nullptr);
    ClipLcdNumber(uint numDigits, QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent*);

};

#endif // CLIPLCDNUMBER_H
