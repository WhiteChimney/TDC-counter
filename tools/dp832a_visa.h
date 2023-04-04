#ifndef DP832A_VISA_H
#define DP832A_VISA_H

#include <visa.h>
#include <QString>
#include <QDebug>

#define MAX_SCPI_LEN 255
#define DEFAULT_TMO  5000

class dp832a_visa : public QObject
{
    Q_OBJECT
public:
    explicit dp832a_visa(QString resourceName, QObject *parent = nullptr);
    ~dp832a_visa();

private:

//    VISA session
    ViStatus nRetStatus;
    ViSession rmSession = 0;
    ViSession pInstrHandle;
    ViRsrc rscName;

//    通信读写 buffer
    ViByte wrBuff[MAX_SCPI_LEN];
    ViByte rdBuff[DEFAULT_TMO];

    int retCount = 0;

public:
    ViStatus initializeDevice();
    ViStatus closeDevice();
    ViStatus sendCommand(QString* command, QString* result);
};

#endif // DP832A_VISA_H
