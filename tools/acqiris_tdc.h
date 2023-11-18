#ifndef ACQIRIS_TDC_H
#define ACQIRIS_TDC_H

#include <QObject>

class Acqiris_TDC : public QObject
{
    Q_OBJECT
public:
    explicit Acqiris_TDC(QObject *parent = nullptr);
};

#endif // ACQIRIS_TDC_H
