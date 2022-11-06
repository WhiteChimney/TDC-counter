#include "tsp01.h"

TSP01::TSP01(QObject *parent) : QObject(parent)
{
}


/*---------------------------------------------------------------------------
  Find Instruments, return the list of found instruments
---------------------------------------------------------------------------*/
QList<QString> TSP01::findInstruments()
{
    ViUInt32       findCnt;
    ViChar         *rscPtr;
    QList<QString> vResource;
    ViUInt32       cnt;

    switch(err = TLTSP_getDeviceCount(VI_NULL, &findCnt)) {
    case VI_SUCCESS:
         break;
    case VI_ERROR_RSRC_NFOUND:
         return vResource;
      default:
         return vResource;
    }

    rscPtr = new ViChar[VI_FIND_BUFLEN];

    for(cnt = 0; cnt < findCnt; cnt++)
    {
        if((err = TLTSP_getDeviceResourceString(VI_NULL, cnt, rscPtr)))
        {
            rscPtr -= cnt * VI_FIND_BUFLEN;
            delete [] rscPtr;
            return vResource;
        }
        vResource.append(QString(rscPtr));
        rscPtr += VI_FIND_BUFLEN;
    }
    rscPtr -= cnt * VI_FIND_BUFLEN;
    delete [] rscPtr;
    return vResource;
}

/*===========================================================================
 Initialize Device
===========================================================================*/
ViStatus TSP01::initializeDevice(QString resourceName)
{
    err = TLTSP_init(resourceName.toLocal8Bit().data(), VI_ON, VI_ON, &ihdl);
    if (!err)
        viSetAttribute(ihdl, VI_ATTR_TMO_VALUE, TIMEOUT_MILLISEC);

    return err;
}

ViStatus TSP01::closeDevice()
{
    return err = TLTSP_close(ihdl);
}

/*===========================================================================
 GET Device Information
===========================================================================*/
QString TSP01::checkDeviceInfo()
{
    ViChar   nameBuf[TLTSP_BUFFER_SIZE];
    ViChar   snBuf[TLTSP_BUFFER_SIZE];
    ViChar   revBuf[TLTSP_BUFFER_SIZE];
    QString deviceInfo = "";

    if((err = TLTSP_identificationQuery (ihdl, VI_NULL, nameBuf, snBuf, revBuf)))
        return tr("Error");

    deviceInfo += "Instrument:    " + QString(nameBuf);
    deviceInfo += "\n";
    deviceInfo += "Serial number: " + QString(snBuf);
    deviceInfo += "\n";
    deviceInfo += "Firmware:      V" + QString(revBuf);
    deviceInfo += "\n";

    if((err = TLTSP_revisionQuery (ihdl, revBuf, VI_NULL)))
        return tr("Error");

    deviceInfo += "Driver:        V" + QString(revBuf);
    deviceInfo += "\n";

    if((err = TLTSP_calibrationMessage (ihdl, revBuf)))
        return tr("Error");

    deviceInfo += "Cal message:   " + QString(revBuf);
    deviceInfo += "\n";

    return deviceInfo;
}

ViStatus TSP01::getErrorCode()
{
    return err;
}

/*---------------------------------------------------------------------------
  Get Temperature Offset Values
---------------------------------------------------------------------------*/
ViReal64 TSP01::getTemperatureOffset()
{
    if (!err)
    {
        ViReal64 temperature_offset = 0.0;
        err = TLTSP_getTempSensOffset (ihdl, TLTSP_MEAS_TEMP1, TLTSP_ATTR_SET_VAL, &temperature_offset);
        if (err == VI_SUCCESS)
            return temperature_offset;
    }
    return errValue;
}

/*---------------------------------------------------------------------------
  Get Humidity Offset Values
---------------------------------------------------------------------------*/
ViReal64 TSP01::getHumidityOffset()
{
    if (!err)
    {
        ViReal64 humidity_offset = 0.0;
        err = TLTSP_getHumSensOffset (ihdl, TLTSP_ATTR_SET_VAL, &humidity_offset);
        if (err == VI_SUCCESS)
            return humidity_offset;
    }
    return errValue;
}

/*---------------------------------------------------------------------------
  Get Temperature Value
---------------------------------------------------------------------------*/
ViReal64 TSP01::getTemperature()
{
    if (!err)
    {
        ViReal64 temperature_value;
        err = TLTSP_measTemperature (ihdl, TLTSP_MEAS_TEMP1, &temperature_value);
        if(!err)
            return temperature_value;
    }
    return errValue;
}


/*---------------------------------------------------------------------------
  Get Humidity Value
---------------------------------------------------------------------------*/
ViReal64 TSP01::getHumidity()
{
    if (!err)
    {
        ViReal64 humidity_value;
        err = TLTSP_getHumidityData (ihdl, TLTSP_ATTR_SET_VAL, &humidity_value);
        if(!err)
            return humidity_value;
    }
    return errValue;
}

