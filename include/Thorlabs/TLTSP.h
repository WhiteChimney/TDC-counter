/****************************************************************************

   Thorlabs TSP Series VISA instrument driver

   This driver supports TSP environment measurement devices
   
   FOR DETAILED DESCRIPTION OF THE DRIVER FUNCTIONS SEE THE ONLINE HELP FILE
   AND THE PROGRAMMERS REFERENCE MANUAL.

   Copyright:  Copyright(c) 2008-2014, Thorlabs (www.thorlabs.com)
   Author:     Michael Biebl (mbiebl@thorlabs.com),
               Diethelm Krause (dkrause@thorlabs.com),
               Thomas Schlosser (tschlosser@thorlabs.com)

   Disclaimer:
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


   Header file

   Date:          Jan-14-2014
   Built with:    NI LabWindows/CVI 12.0.0
   Software-Nr:   091.101.xxx
   Version:       1.2.0

   Changelog:     see 'readme.rtf'

****************************************************************************/


#ifndef _TLTSP_DRIVER_HEADER_
#define _TLTSP_DRIVER_HEADER_

#include <vpptype.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C"
{
#endif


/*===========================================================================

 Macros

===========================================================================*/
/*---------------------------------------------------------------------------
 USB instrument identification
---------------------------------------------------------------------------*/
#define TLTSP_VID_THORLABS             (0x1313)   // Thorlabs
	
#define TLTSP_PID_TSP_DFU           	(0x80F0)   // TSP with DFU interface enabled
#define TLTSP_PID_TSP          			(0x80F8)   // TSP without DFU interface enabled 

/*---------------------------------------------------------------------------
 Find pattern for 'viFindRsrc()'
---------------------------------------------------------------------------*/
// find TSP01
#define TLTSP_FIND_PATTERN_01           "USB?*INSTR{VI_ATTR_MANF_ID==0x1313 && (VI_ATTR_MODEL_CODE==0x80F0 || VI_ATTR_MODEL_CODE==0x80F8)}"

// find any TSP
#define TLTSP_FIND_PATTERN_ANY          "USB?*::0x1313::0x80F?::?*::INSTR"

// find bluetooth devices 
//#define PMBT_FIND_PATTERN             "ASRL?*::INSTR"

/*---------------------------------------------------------------------------
 Buffers
---------------------------------------------------------------------------*/
#define TLTSP_BUFFER_SIZE            256      // General buffer size
#define TLTSP_ERR_DESCR_BUFFER_SIZE  512      // Buffer size for error messages


/*---------------------------------------------------------------------------
 Error/Warning Codes
   Note: The instrument returns errors within the range -512 .. +1023. 
   The driver adds the value VI_INSTR_ERROR_OFFSET (0xBFFC0900). So the 
   driver returns instrument errors in the range 0xBFFC0700 .. 0xBFFC0CFF.
---------------------------------------------------------------------------*/
// Offsets
#undef VI_INSTR_WARNING_OFFSET
#undef VI_INSTR_ERROR_OFFSET

#define VI_INSTR_WARNING_OFFSET        (0x3FFC0900L)
#define VI_INSTR_ERROR_OFFSET          (_VI_ERROR + 0x3FFC0900L)   //0xBFFC0900

// Driver warnings
#undef VI_INSTR_WARN_OVERFLOW
#undef VI_INSTR_WARN_UNDERRUN
#undef VI_INSTR_WARN_NAN

#define VI_INSTR_WARN_OVERFLOW         (VI_INSTR_WARNING_OFFSET + 0x01L)   //0x3FFC0901
#define VI_INSTR_WARN_UNDERRUN         (VI_INSTR_WARNING_OFFSET + 0x02L)   //0x3FFC0902
#define VI_INSTR_WARN_NAN              (VI_INSTR_WARNING_OFFSET + 0x03L)   //0x3FFC0903


/*---------------------------------------------------------------------------
 Attributes
---------------------------------------------------------------------------*/
#define TLTSP_ATTR_SET_VAL           (0)
#define TLTSP_ATTR_MIN_VAL           (1)
#define TLTSP_ATTR_MAX_VAL           (2)
#define TLTSP_ATTR_DFLT_VAL          (3)

/*===========================================================================

 GLOBAL USER-CALLABLE FUNCTION DECLARATIONS (Exportable Functions)

===========================================================================*/
/*===========================================================================

 Init/close

===========================================================================*/
/*---------------------------------------------------------------------------
Get the number of connected devices available in your system that can be 
controlled with this driver.
instrHdl			This parameter is only needed for IVI compliant. Set to VI_NULL.
deviceCount		Receives the number of connected devices you can control with
					this driver.
return			Status code. 
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_getDeviceCount (ViSession instrHdl, ViPUInt32 deviceCount);

/*---------------------------------------------------------------------------
Gets resource string of a connected device you can control with this driver.
You don't have to open a session with the device with TLTSP_init() before you
can use this function. It returns the resource name of a desired device.
instrHdl			This parameter is only needed for IVI compliance. Set to VI_NULL.
deviceIndex		Device index to get the resource string from. Valid values 
					range from 0 to (number of connected devices - 1) (see 
					TLTSP_getDeviceCount()).
resourceName	A 256 byte string buffer to receive the ressource identification 
					string. Use this string in function TLTSP_init(). 
					You may pass VI_NULL if you don't need this value.
return			Status code.
---------------------------------------------------------------------------*/  
ViStatus _VI_FUNC TLTSP_getDeviceResourceString (ViSession instrHdl, ViUInt32 deviceIndex, ViChar resourceName[]);

/*---------------------------------------------------------------------------
 Initialize - This function initializes the instrument driver session and
 returns an instrument handle which is used in subsequent calls.
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_init (ViRsrc resourceName, ViBoolean IDQuery, ViBoolean resetDevice, ViPSession instrHdl);

/*---------------------------------------------------------------------------
 Close an instrument driver session
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_close (ViSession instrHdl);

/*===========================================================================

 Class: Configuration Functions.

===========================================================================*/
/*===========================================================================
 Subclass:  Config Func - TSP Driver - Temp Sensor - Thermistor
===========================================================================*/
/*---------------------------------------------------------------------------
 Set/get exponential parameters
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_setThermistorExpParams (ViSession instrHdl, ViUInt16 chan, ViReal64 r0_value, ViReal64 t0_value, ViReal64 beta_value);
ViStatus _VI_FUNC TLTSP_getThermistorExpParams (ViSession instrHdl, ViUInt16 chan, ViInt16 attribute, ViPReal64 r0_Value, ViPReal64 t0_Value, ViPReal64 beta_Value);

/*===========================================================================
 Subclass: Configuration Functions - System
===========================================================================*/
/*---------------------------------------------------------------------------
 Set/get TSP DFU state
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_setDfuState (ViSession instrHdl, ViInt16 val);
ViStatus _VI_FUNC TLTSP_getDfuState (ViSession instrHdl, ViPInt16 pVal);

#define TLTSP_DFU_STATE_OFF        (0)   // dfu off
#define TLTSP_DFU_STATE_ON         (1)   // dfu on
#define TLTSP_DFU_STATE_ONCE       (2)   // dfu once (dfu on for one time only - self reseting) 

/*===========================================================================
 Class: Configuration Functions - System - Instrument Registers
===========================================================================*/
/*---------------------------------------------------------------------------
 Write/read register contents
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_writeRegister (ViSession instrHdl, ViInt16 reg, ViInt16 value);
ViStatus _VI_FUNC TLTSP_readRegister (ViSession instrHdl, ViInt16 reg, ViPInt16 value);

#define TLTSP_REG_STB                (0)   // Status Byte Register
#define TLTSP_REG_SRE                (1)   // Service Request Enable
#define TLTSP_REG_ESB                (2)   // Standard Event Status Register
#define TLTSP_REG_ESE                (3)   // Standard Event Enable
#define TLTSP_REG_OPER_COND          (4)   // Operation Condition Register
#define TLTSP_REG_OPER_EVENT         (5)   // Operation Event Register
#define TLTSP_REG_OPER_ENAB          (6)   // Operation Event Enable Register
#define TLTSP_REG_OPER_PTR           (7)   // Operation Positive Transition Filter
#define TLTSP_REG_OPER_NTR           (8)   // Operation Negative Transition Filter
#define TLTSP_REG_QUES_COND          (9)   // Questionable Condition Register
#define TLTSP_REG_QUES_EVENT         (10)  // Questionable Event Register
#define TLTSP_REG_QUES_ENAB          (11)  // Questionable Event Enable Reg.
#define TLTSP_REG_QUES_PTR           (12)  // Questionable Positive Transition Filter
#define TLTSP_REG_QUES_NTR           (13)  // Questionable Negative Transition Filter
#define TLTSP_REG_MEAS_COND          (14)  // Measurement Condition Register
#define TLTSP_REG_MEAS_EVENT         (15)  // Measurement Event Register
#define TLTSP_REG_MEAS_ENAB          (16)  // Measurement Event Enable Register
#define TLTSP_REG_MEAS_PTR           (17)  // Measurement Positive Transition Filter
#define TLTSP_REG_MEAS_NTR           (18)  // Measurement Negative Transition Filter
#define TLTSP_REG_AUX_COND           (19)  // Auxiliary Condition Register
#define TLTSP_REG_AUX_EVENT          (20)  // Auxiliary Event Register
#define TLTSP_REG_AUX_ENAB           (21)  // Auxiliary Event Enable Register
#define TLTSP_REG_AUX_PTR            (22)  // Auxiliary Positive Transition Filter
#define TLTSP_REG_AUX_NTR            (23)  // Auxiliary Negative Transition Filter

// STATUS BYTE bit definitions (see IEEE488.2-1992 §11.2)
#define TLTSP_STATBIT_STB_AUX        (0x01)   // Auxiliary summary
#define TLTSP_STATBIT_STB_MEAS       (0x02)   // Device Measurement Summary
#define TLTSP_STATBIT_STB_EAV        (0x04)   // Error available
#define TLTSP_STATBIT_STB_QUES       (0x08)   // Questionable Status Summary
#define TLTSP_STATBIT_STB_MAV        (0x10)   // Message available
#define TLTSP_STATBIT_STB_ESB        (0x20)   // Event Status Bit
#define TLTSP_STATBIT_STB_MSS        (0x40)   // Master summary status
#define TLTSP_STATBIT_STB_OPER       (0x80)   // Operation Status Summary

// STANDARD EVENT STATUS REGISTER bit definitions (see IEEE488.2-1992 §11.5.1)
#define TLTSP_STATBIT_ESR_OPC        (0x01)   // Operation complete
#define TLTSP_STATBIT_ESR_RQC        (0x02)   // Request control
#define TLTSP_STATBIT_ESR_QYE        (0x04)   // Query error
#define TLTSP_STATBIT_ESR_DDE        (0x08)   // Device-Specific error
#define TLTSP_STATBIT_ESR_EXE        (0x10)   // Execution error
#define TLTSP_STATBIT_ESR_CME        (0x20)   // Command error
#define TLTSP_STATBIT_ESR_URQ        (0x40)   // User request
#define TLTSP_STATBIT_ESR_PON        (0x80)   // Power on
        
// QUESTIONABLE STATUS REGISTER bit definitions (see SCPI 99.0 §9)
#define TLTSP_STATBIT_QUES_VOLT      (0x0001) // questionable voltage measurement
#define TLTSP_STATBIT_QUES_CURR      (0x0002) // questionable current measurement
#define TLTSP_STATBIT_QUES_TIME      (0x0004) // questionable time measurement
#define TLTSP_STATBIT_QUES_POW       (0x0008) // questionable power measurement
#define TLTSP_STATBIT_QUES_TEMP      (0x0010) // questionable temperature measurement
#define TLTSP_STATBIT_QUES_FREQ      (0x0020) // questionable frequency measurement
#define TLTSP_STATBIT_QUES_PHAS      (0x0040) // questionable phase measurement
#define TLTSP_STATBIT_QUES_MOD       (0x0080) // questionable modulation measurement
#define TLTSP_STATBIT_QUES_CAL       (0x0100) // questionable calibration
#define TLTSP_STATBIT_QUES_9         (0x0200) // reserved
#define TLTSP_STATBIT_QUES_10        (0x0400) // reserved
#define TLTSP_STATBIT_QUES_11        (0x0800) // reserved
#define TLTSP_STATBIT_QUES_12        (0x1000) // reserved
#define TLTSP_STATBIT_QUES_INST      (0x2000) // instrument summary
#define TLTSP_STATBIT_QUES_WARN      (0x4000) // command warning
#define TLTSP_STATBIT_QUES_15        (0x8000) // reserved

// OPERATION STATUS REGISTER bit definitions (see SCPI 99.0 §9)
#define TLTSP_STATBIT_OPER_CAL       (0x0001) // The instrument is currently performing a calibration.
#define TLTSP_STATBIT_OPER_SETT      (0x0002) // The instrument is waiting for signals it controls to stabilize enough to begin measurements.
#define TLTSP_STATBIT_OPER_RANG      (0x0004) // The instrument is currently changing its range.
#define TLTSP_STATBIT_OPER_SWE       (0x0008) // A sweep is in progress.
#define TLTSP_STATBIT_OPER_MEAS      (0x0010) // The instrument is actively measuring.
#define TLTSP_STATBIT_OPER_TRIG      (0x0020) // The instrument is in a “wait for trigger” state of the trigger model.
#define TLTSP_STATBIT_OPER_ARM       (0x0040) // The instrument is in a “wait for arm” state of the trigger model.
#define TLTSP_STATBIT_OPER_CORR      (0x0080) // The instrument is currently performing a correction (Auto-PID tune).
#define TLTSP_STATBIT_OPER_8         (0x0100) // reserved
#define TLTSP_STATBIT_OPER_9         (0x0200) // reserved
#define TLTSP_STATBIT_OPER_10        (0x0400) // reserved
#define TLTSP_STATBIT_OPER_11        (0x0800) // reserved
#define TLTSP_STATBIT_OPER_12        (0x1000) // reserved
#define TLTSP_STATBIT_OPER_INST      (0x2000) // One of n multiple logical instruments is reporting OPERational status.
#define TLTSP_STATBIT_OPER_PROG      (0x4000) // A user-defined programming is currently in the run state.
#define TLTSP_STATBIT_OPER_15        (0x8000) // reserved

// Thorlabs defined MEASRUEMENT STATUS REGISTER bit definitions
#define TLTSP_STATBIT_MEAS_0         (0x0001) // reserved
#define TLTSP_STATBIT_MEAS_1         (0x0002) // reserved
#define TLTSP_STATBIT_MEAS_2         (0x0004) // reserved
#define TLTSP_STATBIT_MEAS_3         (0x0008) // reserved
#define TLTSP_STATBIT_MEAS_4         (0x0010) // reserved
#define TLTSP_STATBIT_MEAS_5         (0x0020) // reserved
#define TLTSP_STATBIT_MEAS_6         (0x0040) // reserved
#define TLTSP_STATBIT_MEAS_7         (0x0080) // reserved
#define TLTSP_STATBIT_MEAS_TEMP_FAIL (0x0100) // Temperature failure (the temperature is out of the specified window)
#define TLTSP_STATBIT_MEAS_NO_SENSOR (0x0200) // The selected temperature sensor is not propperly connected
#define TLTSP_STATBIT_MEAS_TEC_OPEN  (0x0400) // TEC output compliance voltage reached
#define TLTSP_STATBIT_MEAS_NO_CABLE  (0x0800) // Cable connection failure
#define TLTSP_STATBIT_MEAS_12        (0x1000) // reserved
#define TLTSP_STATBIT_MEAS_13        (0x2000) // reserved
#define TLTSP_STATBIT_MEAS_OVERTEMP  (0x4000) // The instrument's temperature is too high
#define TLTSP_STATBIT_MEAS_15        (0x8000) // reserved

// Thorlabs defined Auxiliary STATUS REGISTER bit definitions
#define TLTSP_STATBIT_AUX_GPIO1      (0x0001) // GPIO port #1
#define TLTSP_STATBIT_AUX_GPIO2      (0x0002) // GPIO port #2
#define TLTSP_STATBIT_AUX_GPIO3      (0x0004) // GPIO port #3
#define TLTSP_STATBIT_AUX_GPIO4      (0x0008) // GPIO port #4
#define TLTSP_STATBIT_AUX_4          (0x0010) // reserved
#define TLTSP_STATBIT_AUX_5          (0x0020) // reserved
#define TLTSP_STATBIT_AUX_6          (0x0040) // reserved
#define TLTSP_STATBIT_AUX_7          (0x0080) // reserved
#define TLTSP_STATBIT_AUX_8          (0x0100) // reserved
#define TLTSP_STATBIT_AUX_9          (0x0200) // reserved
#define TLTSP_STATBIT_AUX_10         (0x0400) // reserved
#define TLTSP_STATBIT_AUX_11         (0x0800) // reserved
#define TLTSP_STATBIT_AUX_12         (0x1000) // reserved
#define TLTSP_STATBIT_AUX_13         (0x2000) // reserved
#define TLTSP_STATBIT_AUX_14         (0x4000) // reserved
#define TLTSP_STATBIT_AUX_15         (0x8000) // reserved

/*===========================================================================
 Subclass: Configuration Functions - Temperature Sensor - Thermistor
===========================================================================*/
/*---------------------------------------------------------------------------
 Set/get sensor humidity offset
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_setHumSensOffset (ViSession instrHdl, ViReal64 val);
ViStatus _VI_FUNC TLTSP_getHumSensOffset (ViSession instrHdl, ViInt16 attr, ViPReal64 pVal);

/*---------------------------------------------------------------------------
 Set/get sensor temperature offset
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_setTempSensOffset (ViSession instrHdl, ViUInt16 chan, ViReal64 val);
ViStatus _VI_FUNC TLTSP_getTempSensOffset (ViSession instrHdl, ViUInt16 chan, ViInt16 attr, ViPReal64 pVal);

/*===========================================================================

 Data Functions.

===========================================================================*/

/*---------------------------------------------------------------------------
 TSP get temperature minimum/maximum value
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_getTemperatureData (ViSession instrHdl, ViUInt16 chan, ViInt16 attr, ViPReal64 pVal);

/*---------------------------------------------------------------------------
 TSP get humidity minimum/maximum value
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_getHumidityData (ViSession instrHdl, ViInt16 attr, ViPReal64 pVal);

/*---------------------------------------------------------------------------
 TSP get thermistor resistance minimum/maximum value
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_getThermRes (ViSession instrHdl, ViUInt16 chan, ViInt16 attr, ViPReal64 pVal);

/*===========================================================================

 Subclass: Low Level Measurement

===========================================================================*/
/*---------------------------------------------------------------------------
 Abort measurement
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_abort (ViSession instrHdl);

/*---------------------------------------------------------------------------
 Configure measurement
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_configure (ViSession instrHdl, ViInt16 parameter);

#define TLTSP_MEAS_TEMP        	0
#define TLTSP_MEAS_HUM         	1
#define TLTSP_MEAS_TEMP_EXT1    	2
#define TLTSP_MEAS_TEMP_EXT2		3

/*---------------------------------------------------------------------------
 Initiate measurement
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_initiate (ViSession instrHdl);

/*---------------------------------------------------------------------------
 TSP read last measured temperature value
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_fetchTemperature (ViSession instrHdl, ViUInt16 chan, ViPReal64 pVal);

/*---------------------------------------------------------------------------
 TSP read last measured humidity value
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_fetchHumidity (ViSession instrHdl, ViPReal64 pVal);

/*---------------------------------------------------------------------------
 TSP read last measured value of last configured channel
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_fetch(ViSession instrHdl, ViPReal64 pVal);

/*---------------------------------------------------------------------------
 TSP read new value (last configured channel)
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_read (ViSession instrHdl, ViPReal64 pVal);

/*---------------------------------------------------------------------------
 TSP get configuration value
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_getConfiguration (ViSession instrHdl, ViPInt16 pVal);

/*---------------------------------------------------------------------------
 TSP measure new temperature value
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_measTemperature (ViSession instrHdl, ViUInt16 chan, ViPReal64 pVal);

#define TLTSP_MEAS_TEMP1      1
#define TLTSP_MEAS_TEMP2    	2
#define TLTSP_MEAS_TEMP3		3

/*---------------------------------------------------------------------------
 TSP measure new humidity value
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_measHumidity (ViSession instrHdl, ViPReal64 pVal);

/*===========================================================================

 Utility Functions.

===========================================================================*/
/*---------------------------------------------------------------------------
 Identification query
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_identificationQuery (ViSession instrHdl, ViChar _VI_FAR manufacturerName[], ViChar _VI_FAR deviceName[], ViChar _VI_FAR serialNumber[], ViChar _VI_FAR firmwareRevision[]);

/*---------------------------------------------------------------------------
 Reset the instrument.
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_reset (ViSession instrHdl);

/*---------------------------------------------------------------------------
 Run Self-Test routine.
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_selfTest (ViSession instrHdl, ViPInt16 selfTestResult, ViChar _VI_FAR description[]);

/*---------------------------------------------------------------------------
 Error Query
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_errorQuery (ViSession instrHdl, ViPInt32 errorNumber, ViChar _VI_FAR errorMessage[]);

/*---------------------------------------------------------------------------
 Get error description. 
 This function translates the error return value from the instrument driver
 into a user-readable string.
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_errorMessage (ViSession instrHdl, ViStatus statusCode, ViChar _VI_FAR description[]);

/*---------------------------------------------------------------------------
 Read calibration message
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_calibrationMessage (ViSession instrHdl, ViChar _VI_FAR str[]);
	
/*---------------------------------------------------------------------------
 Revision Query
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_revisionQuery (ViSession instrHdl, ViChar _VI_FAR instrumentDriverRevision[], ViChar _VI_FAR firmwareRevision[]);

/*===========================================================================
 Class: Utility-Raw Instrument I/O
===========================================================================*/
/*---------------------------------------------------------------------------
 Write to Instrument
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_writeRaw (ViSession instrHdl, ViString command);

/*---------------------------------------------------------------------------
 Read from Instrument
---------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLTSP_readRaw (ViSession instrHdl, ViChar _VI_FAR buffer[], ViUInt32 size, ViPUInt32 returnCount);


#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif   /* _TLTSP_DRIVER_HEADER_ */

/****************************************************************************

  End of Header file

****************************************************************************/
  
