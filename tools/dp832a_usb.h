#ifndef DP832A_USB_H
#define DP832A_USB_H

#include <visa.h>
#include <QString>
#include <QDebug>

#define MAX_SCPI_LEN 255
#define DEFAULT_TMO  5000

class DP832A_USB : public QObject
{
    Q_OBJECT
public:
    explicit DP832A_USB(QString resourceName, QObject *parent = nullptr);
    ~DP832A_USB();

private:

//    VISA session
    ViStatus nRetStatus;
    ViSession rmSession = 0;
    ViSession pInstrHandle;
    ViRsrc rscName;

//    读取与写入 buffer
    ViByte wrBuff[MAX_SCPI_LEN];
    ViByte rdBuff[DEFAULT_TMO];

    int retCount = 0;

public:
    bool initializeDevice();
    bool closeDevice();
    bool sendCommand(QString* command, QString* result);
    bool sendCommand2(QString command);

    void setVoltage(double voltage);
};

#endif // DP832A_USB_H
