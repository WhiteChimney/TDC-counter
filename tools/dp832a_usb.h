#ifndef DP832A_USB_H
#define DP832A_USB_H

#include <QObject>
#include <QString>
#include <visa.h>

#define MAX_REC_SIZE 255

class DP832A_USB : public QObject
{
    Q_OBJECT
public:
    explicit DP832A_USB(QString resourceName, QObject *parent = nullptr);
    ~DP832A_USB();

private:

    QString resourceName;
//    VISA session
    ViStatus status = VI_SUCCESS - 1;
    ViSession defaultRM = 0;
    ViSession instr;
    ViUInt32 retCount = 0;

//    读取与写入 buffer
    char* SendAddr = NULL;
    char* SendBuf = NULL;
    unsigned char RecBuf[MAX_REC_SIZE];


public:
    bool initializeDevice();
    bool closeDevice();
    bool sendCommand(QString command);
    bool readReply(QString* reply);
    QString readReply();

    bool setVoltage(int channel, double voltage);
};

#endif // DP832A_USB_H
