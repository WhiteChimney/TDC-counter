Attribute VB_Name = "AcqrsInterface"
'/////////////////////////////////////////////////////////////////////////////////////////
'
'  AcqrsInterface.bas : Declaration of Acqiris device driver API for Visual Basic 6
'----------------------------------------------------------------------------------------
'  Copyright (C) 2007 Acqiris. All rights reserved.
'
'/////////////////////////////////////////////////////////////////////////////////////////
'  $Rev: 24979 $                                               $Date: 2007-02-21 09:14:29 +0100 (Wed, 21 Feb 2007) $

Option Explicit

'/////////////////////////////////////////////////////////////////////////////////////////
' General comments about the function prototypes:
'
' - All function calls require the argument 'instrumentID' in order to identify
'   the Acqiris Digitizer card to which the call is directed.
'   The only exceptions are the initializer functions 'Acqrs_getNbrInstruments',
'     'Acqrs_setSimulationOptions', 'Acqrs_init' and 'Acqrs_InitWithOptions'.
'   The last two functions actually return instrument identifiers at initialization time,
'   for subsequent use in the other function calls.
'
' - All function calls return a status value of type 'ViStatus' with information about
'   the success or failure of the call.
'   The Acqiris specific values are defined in the AcqirisErrorCodes.h file.
'   The generic VISA ones are listed in the header file 'vpptype.h'.
'
' - If important parameters supplied by the user (e.g. an invalid instrumentID) are found
'   to be invalid, most functions do not execute and return an error code of the type
'   VI_ERROR_PARAMETERi, where i = 1, 2, ... corresponds to the argument number.
'
' - If the user attempts (with a function Acqrs_configXXXX) to set a instrument
'   parameter to a value which is outside of its acceptable range, the function
'   typically adapts the parameter to the closest available value and returns
'   ACQIRIS_WARN_SETUP_ADAPTED. The instrument parameters actually set can be retrieved
'   with the 'query' functions Acqrs_getXXXX.
'
' - Data are always returned through pointers to user-allocated variables or arrays.
'
'/////////////////////////////////////////////////////////////////////////////////////////

Public Declare Function Acqrs_calibrate Lib "AqDrv4" _
Alias "_Acqrs_calibrate@4" (ByVal instrumentID As Long) As Long
' Performs an auto-calibration of the instrument
' Equivalent to Acqrs_calibrateEx with 'calType' = 0

Public Declare Function Acqrs_calibrateEx Lib "AqDrv4" _
Alias "_Acqrs_calibrateEx@16" (ByVal instrumentID As Long, _
                                ByVal calType As Long, _
                                ByVal modifier As Long, _
                                ByVal flags As Long) As Long
' Performs a (partial) auto-calibration of the instrument
'
' 'calType'=  0    calibrate the entire instrument (equivalent to 'Acqrs_calibrate')
'             1    calibrate only the current channel configuration,
'                  as set by 'Acqrs_configChannelCombination'
'             2    calibrate external clock timing. Requires operation in External Clock
'                  (Continuous), i.e. 'clockType' = 1, as set with 'Acqrs_configExtClock'
'             3    calibrate only at the current frequency (only instruments with fine resolution
'                  time bases)
'             4    calibrate fast, only at the current settings (if supported).
'                  Note: In this mode, any change on 'fullscale', 'bandwidth limit', 'channel
'                  combine', 'impedance' or 'coupling' will require a new calibration.
'
' 'modifier'
'          ['calType' = 0]    currently unused, set to zero
'          ['calType' = 1]    currently unused, set to zero
'          ['calType' = 2]    currently unused, set to zero
'          ['calType' = 3]  0    = All channels
'                          >0    = Only specified channel (for i.e. 1 = calibrate only channel 1)
'          ['calType' = 4]  0    = All channels
'                          >0    = Only specified channel (for i.e. 1 = calibrate only channel 1)
'
' 'flags'            currently unused, set to zero
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_INSTRUMENT_RUNNING   if the instrument is currently running.
' ACQIRIS_ERROR_CALIBRATION_FAILED   if the requested calibration failed.
' ACQIRIS_ERROR_NOT_SUPPORTED        if the requested calibration is not supported by the
'                                    instrument.
' ACQIRIS_ERROR_CANNOT_READ_THIS_CHANNEL if the requested channel is not available.
' ACQIRIS_ERROR_COULD_NOT_CALIBRATE  if the requested frequency is invalid ('calType' = 2 only).
' ACQIRIS_ERROR_ACQ_TIMEOUT          if an acquisition timed out during the calibration
'                                    (e.g. no clocks provided).
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function Acqrs_close Lib "AqDrv4" _
Alias "_Acqrs_close@4" (ByVal instrumentID As Long) As Long
' Close specified instrument. Once closed, this instrument is not available anymore and
' need to be reenabled using 'InitWithOptions' or 'init'.
' Note: For freeing properly all resources, 'closeAll' must still be called when
' the application close, even if 'close' was called for each instrument.
'
' Returns one of the following ViStatus values:
' VI_SUCCESS always.

Public Declare Function Acqrs_closeAll Lib "AqDrv4" _
Alias "_Acqrs_closeAll@0" () As Long
' Closes all instruments and prepares for closing of application
'
' Returns one of the following ViStatus values:
' VI_SUCCESS always.


Public Declare Function Acqrs_configLogicDevice Lib "AqDrv4" _
Alias "_Acqrs_configLogicDevice@16" (ByVal instrumentID As Long, _
                                        ByVal deviceName As String, _
                                        ByVal filePathName As String, _
                                        ByVal flags As Long) As Long
' Load/Clear/SetFilePath of the on-board logic devices. ONLY useful for a module
' with on-board programmable logic devices (SCxxx, ACxxx, APxxx, RCxxx, and 12-bit Digitizers).
'
' 'flags'= operation
'          = 0 program device, using strictly the specified path/file name
'          = 1 clear device
'          = 2 clear and program devices using appropriate files from specified path.
'          = 3 program device, but also look for file path in the 'AqDrv4.ini' file, if the file
'              was not found. It is sufficient to specify the file name (without explicit path),
'              if the file is either in the current working directory or in the directory pointed
'              to by 'FPGAPATH' in 'AqDrv4.ini'.
'
' 'deviceName'   Identifies which device to program.
'    ['flags' = 0 or 3] = device to program.
'        This string must be "Block1Devx", with 'x' = is the FPGA number to be programmed.
'        For instance, in the AP200/AP201 it must be "Block1Devx", with 'x' = 1, 2, 3 or 4.
'    ['flags' = 1] = device to clear, must be "Block1DevAll".
'    ['flags' = 2] = unused.
'
' 'filePathName'
'    ['flags' = 0 or 3] = file path and file name.
'    ['flags' = 1] = unused.
'    ['flags' = 2] = path (no file name !) where all .bit files can be found.
'
' Note: Most users do not need to call this function. Check the manual for further details.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_PARAM_STRING_INVALID    if 'deviceName' is invalid.
' ACQIRIS_ERROR_FILE_NOT_FOUND          if an FPGA file could not be found.
' ACQIRIS_ERROR_NO_DATA                 if an FPGA file did not contain expected
'                                       data.
' ACQIRIS_ERROR_FIRMWARE_NOT_AUTHORIZED if the instrument is not authorized to use the
'                                       specified 'filePathName' file.
' ACQIRIS_ERROR_EEPROM_DATA_INVALID     if the CPLD 'filePathName' is invalid.
' ACQIRIS_ERROR_FPGA_y_LOAD             if an FPGA could not be loaded,
'                                       where 'y' = FPGA nbr.
' ACQIRIS_WARN_SETUP_ADAPTED            if one of the parameters has been adapted.
' VI_SUCCESS                            otherwise.


Public Declare Function Acqrs_errorMessage Lib "AqDrv4" _
Alias "_Acqrs_errorMessage@16" (ByVal instrumentID As Long, _
                                ByVal errorCode As Long, _
                                ByVal errorMessage As String, _
                                ByVal errorMessageSize As Long) As Long
' Translates an error code into a human readable form. For file errors, the returned message
' will also contain the file name and the original 'ansi' error string.
'
' 'errorCode'          = Error code (returned by a function) to be translated
' 'errorMessage'       = Pointer to user-allocated character string (suggested size 512 bytes),
'                        into which the error message text will be copied.
' 'errorMessageSize'   = size of 'errorMessage', in bytes.
'
' NOTE: 'instrumentID' can be VI_NULL.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_BUFFER_OVERFLOW    if 'errorMessageSize' is too small.
' VI_SUCCESS                       otherwise.

Public Declare Function Acqrs_getDevType Lib "AqDrv4" _
Alias "_Acqrs_getDevType@8" (ByVal instrumentID As Long, _
                                ByRef devTypeP As Long) As Long
' Returns the device type of the specified 'instrumentID'
'
' 'devTypeP'          = The device type. See 'AqDevType'.
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function Acqrs_getDevTypeByIndex Lib "AqDrv4" _
Alias "_Acqrs_getDevTypeByIndex@8" (ByVal deviceIndex As Long, _
                                    ByRef devTypeP As Long) As Long
' Returns the device type of the instrument specified by 'deviceIndex'.
'
' 'devTypeP'          = The device type. See 'AqDevType'.
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function Acqrs_getInstrumentData Lib "AqDrv4" _
Alias "_Acqrs_getInstrumentData@20" (ByVal instrumentID As Long, _
                                        ByVal name As String, _
                                        ByRef serialNbr As Long, _
                                        ByRef busNbr As Long, _
                                        ByRef slotNbr As Long) As Long
' Returns some basic data about a specified instrument
'
' Values returned by the function:
'
' 'name'            = pointer to user-allocated string, into which the
'                     model name is returned (length < 32 characters)
' 'serialNbr'       = serial number of the instrument
' 'busNbr'          = bus number where instrument is located
' 'slotNbr'         = slot number where instrument is located
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function Acqrs_getInstrumentInfo Lib "AqDrv4" _
Alias "_Acqrs_getInstrumentInfo@12" (ByVal instrumentID As Long, _
                                        ByVal parameterString As String, _
                                        ByRef infoValue As Any) As Long
' Returns general information about a specified instrument
'
' The following value must be supplied to the function:
'
' 'parameterString'  = character string specifying the requested parameter
'                      Please refer to the manual for the accepted parameter strings
'
' Value returned by the function:
'
' 'infoValue'        = value of requested parameter
'                      The type of the returned value depends on the parameter string
'                      Please refer to the manual for the required  data type as a
'                      function of the accepted parameter strings
'                      NOTE to C/C++ programmers: 'ViAddr' resolves to 'void*'
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_PARAM_STRING_INVALID if 'parameterString' is invalid.
' ACQIRIS_ERROR_NOT_SUPPORTED        if the requested value is not supported by the
'                                    instrument.
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function Acqrs_getNbrChannels Lib "AqDrv4" _
Alias "_Acqrs_getNbrChannels@8" (ByVal instrumentID As Long, _
                                    ByRef nbrChannels As Long) As Long
' Returns the number of channels on the specified instrument.
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function Acqrs_getNbrInstruments Lib "AqDrv4" _
Alias "_Acqrs_getNbrInstruments@4" (ByRef nbrInstruments As Long) As Long
' Returns the number of supported physical Acqiris devices found on the computer.
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETER1                if 'nbrInstruments' is NULL.
' VI_SUCCESS                         otherwise.

Public Declare Function Acqrs_getVersion Lib "AqDrv4" _
Alias "_Acqrs_getVersion@12" (ByVal instrumentID As Long, _
                                ByVal versionItem As Long, _
                                ByRef version As Long) As Long
' Returns version numbers associated with a specified instrument / current device driver
'
' The following value must be supplied to the function:
'
' 'versionItem'    =   1        for version of Kernel-Mode Driver
'                      2        for version of EEPROM Common Section
'                      3        for version of EEPROM Digitizer Section
'                      4        for version of CPLD firmware
'
' Value returned by the function:
'
' 'version'        = version number.
'
' For drivers, the version number is composed of 2 parts. The upper 2 bytes represent
' the major version number, and the lower 2 bytes represent the minor version number.
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function Acqrs_init Lib "AqDrv4" _
Alias "_Acqrs_init@16" (ByVal resourceName As String, _
                        ByVal IDQuery As Boolean, _
                        ByVal resetDevice As Boolean, _
                        ByRef instrumentID As Long) As Long
' Initializes an instrument
' See remarks under 'Acqrs_InitWithOptions'

Public Declare Function Acqrs_InitWithOptions Lib "AqDrv4" _
Alias "_Acqrs_InitWithOptions@20" (ByVal resourceName As String, _
                                    ByVal IDQuery As Boolean, _
                                    ByVal resetDevice As Boolean, _
                                    ByVal optionsString As String, _
                                    ByRef instrumentID As Long) As Long
' Initializes an instrument with options
'
' The following values must be supplied to the function:
'
' 'resourceName'   = an ASCII string which identifies the instrument to be initialized
'                    See manual for a detailed description of this string.
' 'IDQuery'        = currently ignored
' 'resetDevice'    = if set to 'TRUE', resets the instrument after initialization
' 'optionsString'  = an ASCII string which specifies options. Currently, we support
'                    "CAL=False" to suppress calibration during the initialization
'                    "DMA=False" to inhibit (for diagnostics) the use of scatter-gather DMA for
'                    data transfers
'                    "Simulate=True" for the use of simulated instruments during application
'                    development.
' Values returned by the function:
'
' 'instrumentID'   = identifier for subsequent use in the other function calls.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_INIT_STRING_INVALID        if the 'resourceName' is invalid.
' ACQIRIS_ERROR_INTERNAL_DEVICENO_INVALID  if the 'resourceName' contains values that are not
'                                          within the expected ranges (e.g. wrong serial number).
' ACQIRIS_ERROR_TOO_MANY_DEVICES           if there are too many devices installed.
' ACQIRIS_ERROR_KERNEL_VERSION             if the instrument require a newer kernel driver.
' ACQIRIS_ERROR_HW_FAILURE                 if the instrument doesn't answer properly to
'                                          defined requests.
' ACQIRIS_ERROR_HW_FAILURE_CHx             if one of the channels doesn't answer properly to
'                                          defined requests, where 'x' = channel number.
' ACQIRIS_ERROR_FILE_NOT_FOUND             if a required FPGA file could not be found.
' ACQIRIS_ERROR_NO_DATA                    if a required FPGA file did not contain expected data.
' VI_ERROR_PARAMETERx                      if one of the parameters has an invalid value, where
'                                          'x' = parameter number.
' VI_SUCCESS                               otherwise.


Public Declare Function Acqrs_logicDeviceIO Lib "AqDrv4" _
Alias "_Acqrs_logicDeviceIO@28" (ByVal instrumentID As Long, _
                                    ByVal deviceName As String, _
                                    ByVal registerID As Long, _
                                    ByVal nbrValues As Long, _
                                    ByRef dataArray As Long, _
                                    ByVal readWrite As Long, _
                                    ByVal flags As Long) As Long
' Currently ONLY for APxxx, RCxxx, SCxxx and ACxxx!
'
' Reads/writes a number of 32-bit data values from/to a user-defined register in the
' logic device identified by 'deviceName[]'.
' ONLY useful for a instrument with on-board programmable logic devices.
'
' The following values must be supplied to the function:
'
' 'deviceName'       = an ASCII string which identifies the device
'                      Must be of the form "BlockMDevN", where M = 1..4 and N = 1..number
'                      of logical devices in the device block M.
'                      In the RC200, this string must be "Block1Dev1"
'                      See manual for a detailed description of this string.
' 'registerID'       = 0..to (nbrRegisters-1)
' 'nbrValues'        = number of 32-bit values to be transferred
' 'dataArray[]'      = user-supplied buffer for data values
' 'readWrite'        = 0 for reading (from logic device to application)
'                      1 for writing (from application to logic device)
' 'flags'              currently ignored, use '0'
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_PARAM_STRING_INVALID   if the 'deviceName' is not valid.
' ACQIRIS_ERROR_NO_ACCESS              if the operation is not authorized.
' ACQIRIS_ERROR_NOT_SUPPORTED          if the operation is not supported by the instrument,
'                                          or if 'registerID' is outside the expected values.
' ACQIRIS_ERROR_INSTRUMENT_RUNNING     if the instrument was not stopped beforehand.
' ACQIRIS_ERROR_DATA_ARRAY             if 'dataArray' is NULL.
' ACQIRIS_ERROR_IO_WRITE               if a 'write' verify failed.
' VI_ERROR_PARAMETERx                  if one of the parameters has an invalid value, where
'                                      'x' = parameter number.
' VI_SUCCESS                           otherwise.

Public Declare Function Acqrs_reset Lib "AqDrv4" _
Alias "_Acqrs_reset@4" (ByVal instrumentID As Long) As Long
' Resets an instrument
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETER1                     if 'instrumentID' is invalid.
' VI_SUCCESS                              otherwise.

Public Declare Function Acqrs_resetMemory Lib "AqDrv4" _
Alias "_Acqrs_resetMemory@4" (ByVal instrumentID As Long) As Long
' Resets the device memory to a known default state.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_NOT_SUPPORTED         if the instrument does not support this feature.
' ACQIRIS_ERROR_INSTRUMENT_RUNNING    if the instrument is already running.
' VI_ERROR_PARAMETER1                 if 'instrumentID' is invalid.
' VI_SUCCESS                          otherwise.

Public Declare Function Acqrs_setAttributeString Lib "AqDrv4" _
Alias "_Acqrs_setAttributeString@16" (ByVal instrumentID As Long, _
                                        ByVal channel As Long, _
                                        ByVal name As String, _
                                        ByVal value As String) As Long
' Set through 'value' the value of the attribute named 'name'.
'
' 'channel'            = 1...Nchan (as returned by 'Acqrs_getNbrChannels' ) or
'                        0 if it is an attribute related to the instrument
'                        itself (i.e. not to a channel).
' 'name'               = specify the name of the attribute to change.
'                        Please refer to the manual for the accepted names.
' 'value'              = specify the value in which the attribute will be set.
'                        Please refer to the manual for the accepted values.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_ATTR_NOT_FOUND         if not found or if a wrong 'channel' is specified.
' ACQIRIS_ERROR_ATTR_WRONG_TYPE        if found but not of the expected type.
' ACQIRIS_ERROR_ATTR_INVALID_VALUE     if 'value' is not valid.
' ACQIRIS_ERROR_ATTR_IS_READ_ONLY      if found but not writable.
' VI_SUCCESS                           otherwise.

Public Declare Function Acqrs_setLEDColor Lib "AqDrv4" _
Alias "_Acqrs_setLEDColor@8" (ByVal instrumentID As Long, ByVal color As Long)
' Sets the front-panel LED to the desired color
' 'color' = 0        OFF (returns to normal 'acquisition status' indicator)
'           1        Green
'           2        Red
'           3        Yellow
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                  if one of the parameters has an invalid value, where
'                                      'x' = parameter number.
' VI_SUCCESS                           otherwise.

Public Declare Function Acqrs_setSimulationOptions Lib "AqDrv4" _
Alias "_Acqrs_setSimulationOptions@4" (ByVal simOptionString As String) As Long
' Sets one or several options which will be used by the function 'Acqrs_InitWithOptions',
' provided that the 'optionString' supplied to 'Acqrs_InitWithOptions' contains the
' string 'simulate=TRUE' (or similar).
' Refer to the manual for the accepted form of 'simOptionString'.
' The simulation options are reset to none by setting 'simOptionString' to an empty string "".
'
' Returns one of the following ViStatus values:
' VI_SUCCESS always.

