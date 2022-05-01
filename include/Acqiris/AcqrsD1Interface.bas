Attribute VB_Name = "AcqrsD1Interface"
'/////////////////////////////////////////////////////////////////////////////////////////
'
'  AcqrsD1Interface.bas : Declaration of Acqiris device driver API for Visual Basic 6
'----------------------------------------------------------------------------------------
'  Copyright (C) 1999 - 2007 Acqiris. All rights reserved.
'
'/////////////////////////////////////////////////////////////////////////////////////////
'  $Rev: 27894 $                    $Date: 2007-11-23 13:14:17 +0100 (Fri, 23 Nov 2007) $

Option Explicit

'/////////////////////////////////////////////////////////////////////////////////////////
' Define Instrument Specific Error Codes
'/////////////////////////////////////////////////////////////////////////////////////////

Public Const VI_SUCCESS As Long = 0
Public Const VI_ERROR As Long = &H80000000

Public Const VI_ERROR_PARAMETER1 As Long = VI_ERROR + &H3FFC0001
Public Const VI_ERROR_PARAMETER2 As Long = VI_ERROR + &H3FFC0002
Public Const VI_ERROR_PARAMETER3 As Long = VI_ERROR + &H3FFC0003
Public Const VI_ERROR_PARAMETER4 As Long = VI_ERROR + &H3FFC0004
Public Const VI_ERROR_PARAMETER5 As Long = VI_ERROR + &H3FFC0005
Public Const VI_ERROR_PARAMETER6 As Long = VI_ERROR + &H3FFC0006
Public Const VI_ERROR_PARAMETER7 As Long = VI_ERROR + &H3FFC0007
Public Const VI_ERROR_PARAMETER8 As Long = VI_ERROR + &H3FFC0008

Public Const ACQIRIS_ERROR As Long = &HBFFA4000
Public Const ACQIRIS_WARNING As Long = &H3FFA4000

Public Const ACQIRIS_ERROR_FILE_NOT_FOUND As Long = ACQIRIS_ERROR + &H800
Public Const ACQIRIS_ERROR_PATH_NOT_FOUND As Long = ACQIRIS_ERROR + &H801
Public Const ACQIRIS_ERROR_INVALID_HANDLE As Long = ACQIRIS_ERROR + &H803
Public Const ACQIRIS_ERROR_NOT_SUPPORTED As Long = ACQIRIS_ERROR + &H805
Public Const ACQIRIS_ERROR_INVALID_WINDOWS_PARAM As Long = ACQIRIS_ERROR + &H806
Public Const ACQIRIS_ERROR_NO_DATA As Long = ACQIRIS_ERROR + &H807
Public Const ACQIRIS_ERROR_NO_ACCESS As Long = ACQIRIS_ERROR + &H808
Public Const ACQIRIS_ERROR_BUFFER_OVERFLOW As Long = ACQIRIS_ERROR + &H809
Public Const ACQIRIS_ERROR_BUFFER_NOT_64BITS_ALIGNED As Long = ACQIRIS_ERROR + &H80A
public const ACQIRIS_ERROR_BUFFER_NOT_32BITS_ALIGNED as long = ACQIRIS_ERROR + &H80A

Public Const ACQIRIS_ERROR_ALREADY_OPEN As Long = ACQIRIS_ERROR + &H840
Public Const ACQIRIS_ERROR_SETUP_NOT_AVAILABLE As Long = ACQIRIS_ERROR + &H880

Public Const ACQIRIS_ERROR_IO_WRITE As Long = ACQIRIS_ERROR + &H8A0
Public Const ACQIRIS_ERROR_IO_READ As Long = ACQIRIS_ERROR + &H8A1
public const ACQIRIS_ERROR_IO_DEVICE_OFF as long = ACQIRIS_ERROR + &H8A2

Public Const ACQIRIS_ERROR_INTERNAL_DEVICENO_INVALID As Long = ACQIRIS_ERROR + &H8C0
Public Const ACQIRIS_ERROR_TOO_MANY_DEVICES As Long = ACQIRIS_ERROR + &H8C1
Public Const ACQIRIS_ERROR_EEPROM_DATA_INVALID As Long = ACQIRIS_ERROR + &H8C2
Public Const ACQIRIS_ERROR_INIT_STRING_INVALID As Long = ACQIRIS_ERROR + &H8C3
Public Const ACQIRIS_ERROR_INSTRUMENT_NOT_FOUND As Long = ACQIRIS_ERROR + &H8C4
Public Const ACQIRIS_ERROR_INSTRUMENT_RUNNING As Long = ACQIRIS_ERROR + &H8C5
Public Const ACQIRIS_ERROR_INSTRUMENT_STOPPED As Long = ACQIRIS_ERROR + &H8C6
Public Const ACQIRIS_ERROR_MODULES_NOT_ON_SAME_BUS As Long = ACQIRIS_ERROR + &H8C7
Public Const ACQIRIS_ERROR_NOT_ENOUGH_DEVICES As Long = ACQIRIS_ERROR + &H8C8
Public Const ACQIRIS_ERROR_NO_MASTER_DEVICE As Long = ACQIRIS_ERROR + &H8C9
Public Const ACQIRIS_ERROR_PARAM_STRING_INVALID As Long = ACQIRIS_ERROR + &H8CA
Public Const ACQIRIS_ERROR_COULD_NOT_CALIBRATE As Long = ACQIRIS_ERROR + &H8CB
Public Const ACQIRIS_ERROR_CANNOT_READ_THIS_CHANNEL As Long = ACQIRIS_ERROR + &H8CC
Public Const ACQIRIS_ERROR_PRETRIGGER_STILL_RUNNING As Long = ACQIRIS_ERROR + &H8CD
Public Const ACQIRIS_ERROR_CALIBRATION_FAILED As Long = ACQIRIS_ERROR + &H8CE
Public Const ACQIRIS_ERROR_MODULES_NOT_CONTIGUOUS As Long = ACQIRIS_ERROR + &H8CF
Public Const ACQIRIS_ERROR_INSTRUMENT_ACQ_LOCKED As Long = ACQIRIS_ERROR + &H8D0
Public Const ACQIRIS_ERROR_INSTRUMENT_ACQ_NOT_LOCKED As Long = ACQIRIS_ERROR + &H8D1
Public Const ACQIRIS_ERROR_EEPROM2_DATA_INVALID As Long = ACQIRIS_ERROR + &H8D2

Public Const ACQIRIS_ERROR_INVALID_GEOMAP_FILE As Long = ACQIRIS_ERROR + &H8E0

Public Const ACQIRIS_ERROR_ACQ_TIMEOUT As Long = ACQIRIS_ERROR + &H900
Public Const ACQIRIS_ERROR_TIMEOUT As Long = ACQIRIS_ERROR_ACQ_TIMEOUT      ' for backwards compatibility
Public Const ACQIRIS_ERROR_OVERLOAD As Long = ACQIRIS_ERROR + &H901
Public Const ACQIRIS_ERROR_PROC_TIMEOUT As Long = ACQIRIS_ERROR + &H902
Public Const ACQIRIS_ERROR_LOAD_TIMEOUT As Long = ACQIRIS_ERROR + &H903
Public Const ACQIRIS_ERROR_READ_TIMEOUT As Long = ACQIRIS_ERROR + &H904
Public Const ACQIRIS_ERROR_INTERRUPTED As Long = ACQIRIS_ERROR + &H905
Public Const ACQIRIS_ERROR_WAIT_TIMEOUT As Long = ACQIRIS_ERROR + &H906

Public Const ACQIRIS_ERROR_FIRMWARE_NOT_AUTHORIZED As Long = ACQIRIS_ERROR + &HA00
Public Const ACQIRIS_ERROR_FPGA_1_LOAD As Long = ACQIRIS_ERROR + &HA01
Public Const ACQIRIS_ERROR_FPGA_2_LOAD As Long = ACQIRIS_ERROR + &HA02
Public Const ACQIRIS_ERROR_FPGA_3_LOAD As Long = ACQIRIS_ERROR + &HA03
Public Const ACQIRIS_ERROR_FPGA_4_LOAD As Long = ACQIRIS_ERROR + &HA04
Public Const ACQIRIS_ERROR_FPGA_5_LOAD As Long = ACQIRIS_ERROR + &HA05
Public Const ACQIRIS_ERROR_FPGA_6_LOAD As Long = ACQIRIS_ERROR + &HA06
Public Const ACQIRIS_ERROR_FPGA_7_LOAD As Long = ACQIRIS_ERROR + &HA07
Public Const ACQIRIS_ERROR_FPGA_8_LOAD As Long = ACQIRIS_ERROR + &HA08
Public Const ACQIRIS_ERROR_FIRMWARE_NOT_SUPPORTED As Long = ACQIRIS_ERROR + &HA09

Public Const ACQIRIS_ERROR_FPGA_1_FLASHLOAD_NO_INIT As Long = ACQIRIS_ERROR + &HA10
Public Const ACQIRIS_ERROR_FPGA_1_FLASHLOAD_NO_DONE As Long = ACQIRIS_ERROR + &HA11
Public Const ACQIRIS_ERROR_FPGA_2_FLASHLOAD_NO_INIT As Long = ACQIRIS_ERROR + &HA12
Public Const ACQIRIS_ERROR_FPGA_2_FLASHLOAD_NO_DONE As Long = ACQIRIS_ERROR + &HA13

Public Const ACQIRIS_ERROR_SELFCHECK_MEMORY As Long = ACQIRIS_ERROR + &HA20
Public Const ACQIRIS_ERROR_SELFCHECK_DAC As Long = ACQIRIS_ERROR + &HA21

Public Const ACQIRIS_ERROR_FLASH_ACCESS_TIMEOUT As Long = ACQIRIS_ERROR + &HA30
Public Const ACQIRIS_ERROR_FLASH_FAILURE As Long = ACQIRIS_ERROR + &HA31
Public Const ACQIRIS_ERROR_FLASH_READ As Long = ACQIRIS_ERROR + &HA32

Public Const ACQIRIS_ERROR_ATTR_NOT_FOUND As Long = ACQIRIS_ERROR + &HB00
Public Const ACQIRIS_ERROR_ATTR_WRONG_TYPE As Long = ACQIRIS_ERROR + &HB01
Public Const ACQIRIS_ERROR_ATTR_IS_READ_ONLY As Long = ACQIRIS_ERROR + &HB02
Public Const ACQIRIS_ERROR_ATTR_IS_WRITE_ONLY As Long = ACQIRIS_ERROR + &HB03
Public Const ACQIRIS_ERROR_ATTR_ALREADY_DEFINED As Long = ACQIRIS_ERROR + &HB04
Public Const ACQIRIS_ERROR_ATTR_IS_LOCKED As Long = ACQIRIS_ERROR + &HB05
Public Const ACQIRIS_ERROR_ATTR_INVALID_VALUE As Long = ACQIRIS_ERROR + &HB06

Public Const ACQIRIS_ERROR_KERNEL_VERSION As Long = ACQIRIS_ERROR + &HC00
Public Const ACQIRIS_ERROR_UNKNOWN_ERROR As Long = ACQIRIS_ERROR + &HC01
Public Const ACQIRIS_ERROR_OTHER_WINDOWS_ERROR As Long = ACQIRIS_ERROR + &HC02
Public Const ACQIRIS_ERROR_VISA_DLL_NOT_FOUND As Long = ACQIRIS_ERROR + &HC03
Public Const ACQIRIS_ERROR_OUT_OF_MEMORY As Long = ACQIRIS_ERROR + &HC04
Public Const ACQIRIS_ERROR_UNSUPPORTED_DEVICE As Long = ACQIRIS_ERROR + &HC05

Public Const ACQIRIS_ERROR_PARAMETER9 As Long = ACQIRIS_ERROR + &HD09
Public Const ACQIRIS_ERROR_PARAMETER10 As Long = ACQIRIS_ERROR + &HD0A
Public Const ACQIRIS_ERROR_PARAMETER11 As Long = ACQIRIS_ERROR + &HD0B
Public Const ACQIRIS_ERROR_PARAMETER12 As Long = ACQIRIS_ERROR + &HD0C
Public Const ACQIRIS_ERROR_PARAMETER13 As Long = ACQIRIS_ERROR + &HD0D
Public Const ACQIRIS_ERROR_PARAMETER14 As Long = ACQIRIS_ERROR + &HD0E
Public Const ACQIRIS_ERROR_PARAMETER15 As Long = ACQIRIS_ERROR + &HD0F

Public Const ACQIRIS_ERROR_NBR_SEG As Long = ACQIRIS_ERROR + &HD10
Public Const ACQIRIS_ERROR_NBR_SAMPLE As Long = ACQIRIS_ERROR + &HD11
Public Const ACQIRIS_ERROR_DATA_ARRAY As Long = ACQIRIS_ERROR + &HD12
Public Const ACQIRIS_ERROR_SEG_DESC_ARRAY As Long = ACQIRIS_ERROR + &HD13
Public Const ACQIRIS_ERROR_FIRST_SEG As Long = ACQIRIS_ERROR + &HD14
Public Const ACQIRIS_ERROR_SEG_OFF As Long = ACQIRIS_ERROR + &HD15
Public Const ACQIRIS_ERROR_FIRST_SAMPLE As Long = ACQIRIS_ERROR + &HD16
Public Const ACQIRIS_ERROR_DATATYPE As Long = ACQIRIS_ERROR + &HD17
Public Const ACQIRIS_ERROR_READMODE As Long = ACQIRIS_ERROR + &HD18

Public Const ACQIRIS_ERROR_HW_FAILURE As Long = ACQIRIS_ERROR + &HD80
Public Const ACQIRIS_ERROR_HW_FAILURE_CH1 As Long = ACQIRIS_ERROR + &HD81
Public Const ACQIRIS_ERROR_HW_FAILURE_CH2 As Long = ACQIRIS_ERROR + &HD82
Public Const ACQIRIS_ERROR_HW_FAILURE_CH3 As Long = ACQIRIS_ERROR + &HD83
Public Const ACQIRIS_ERROR_HW_FAILURE_CH4 As Long = ACQIRIS_ERROR + &HD84
Public Const ACQIRIS_ERROR_HW_FAILURE_CH5 As Long = ACQIRIS_ERROR + &HD85
Public Const ACQIRIS_ERROR_HW_FAILURE_CH6 As Long = ACQIRIS_ERROR + &HD86
Public Const ACQIRIS_ERROR_HW_FAILURE_CH7 As Long = ACQIRIS_ERROR + &HD87
Public Const ACQIRIS_ERROR_HW_FAILURE_CH8 As Long = ACQIRIS_ERROR + &HD88
Public Const ACQIRIS_ERROR_HW_FAILURE_EXT1 As Long = ACQIRIS_ERROR + &HDA0

Public Const ACQIRIS_WARN_SETUP_ADAPTED As Long = ACQIRIS_WARNING + &HE00
Public Const ACQIRIS_WARN_READPARA_NBRSEG_ADAPTED As Long = ACQIRIS_WARNING + &HE10
Public Const ACQIRIS_WARN_READPARA_NBRSAMP_ADAPTED As Long = ACQIRIS_WARNING + &HE11
Public Const ACQIRIS_WARN_EEPROM_AND_DLL_MISMATCH As Long = ACQIRIS_WARNING + &HE12
Public Const ACQIRIS_WARN_ACTUAL_DATASIZE_ADAPTED As Long = ACQIRIS_WARNING + &HE13
Public Const ACQIRIS_WARN_UNEXPECTED_TRIGGER As Long = ACQIRIS_WARNING + &HE14
Public Const ACQIRIS_WARN_READPARA_FLAGS_ADAPTER As Long = ACQIRIS_WARNING + &HE15
Public Const ACQIRIS_WARN_SIMOPTION_STRING_UNKNOWN As Long = ACQIRIS_WARNING + &HE16

Public Const ACQIRIS_WARN_HARDWARE_TIMEOUT As Long = ACQIRIS_WARNING + &HE60

Public Const ACQIRIS_WARN_SELFCHECK_MEMORY As Long = ACQIRIS_WARNING + &HF00

'/////////////////////////////////////////////////////////////////////////////////////////
' General comments about the function prototypes:
'
' - All function calls require the argument 'instrumentID' in order to identify
'   the Acqiris Digitizer card to which the call is directed
'   The only exceptions are the initializer functions 'GetNbrPhysicalInstruments',
'   'SetSimulationOptions', 'Init' and 'InitWithOptions'.
'   The last two functions actually return instrument identifiers at initialization time,
'   for subsequent use in the other function calls.
'
' - All function calls return a status value of type 'Long' with information about
'   the success or failure of the call.
'   The values are shown in the list above. They can be translated into a human readable string
'   with a call to the function 'errorMessage'.
'
' - If important parameters supplied by the user (e.g. an invalid instrumentID) are found
'   to be invalid, most functions do not execute and return an error code of the type
'   VI_ERROR_PARAMETERi, where i = 1, 2, ... corresponds to the argument number.
'
' - If the user attempts (with a function ConfigXXXX) to set a digitizer
'   parameter to a value which is outside of its acceptable range, the function
'   typically adapts the parameter to the closest available value and returns
'   ACQIRIS_WARN_SETUP_ADAPTED. The digitizer parameters actually set can be retrieved
'   with the 'query' functions GetXXXX.
'
' - Data are always returned to user-allocated variables or arrays.
'
'/////////////////////////////////////////////////////////////////////////////////////////


'/////////////////////////////////////////////////////////////////////////////////////////
' Data Structures for 'ReadData' function

Type AqReadParameters
    dataType As Long            ' 0 = 8-bit (Byte)  1 = 16-bit (Word) 2 = 32-bit (Long)  3 = 64-bit (Double)
    readMode As Long
    firstSegment As Long
    nbrSegments As Long
    firstSampleInSeg As Long
    nbrSamplesInSeg As Long
    segmentOffset As Long
    dataArraySize As Long       ' in bytes
    segDescArraySize As Long    ' in bytes
    reserved1 As Double
    reserved2 As Double
    reserved3 As Double
End Type

Type AqSegmentDescriptor
    horPos As Double
    timeStampLo As Long         ' Timestamp
    timeStampHi As Long
End Type

Type AqSegmentDescriptorAvg
    horPos As Double
    timeStampLo As Long         ' Timestamp
    timeStampHi As Long
    actualTriggersInSeg As Long ' Number of actual triggers acquired in this segment
    avgOvfl As Long             ' Acquisition overflow (avg)
    avgStatus As Long           ' Average status (avg)
    avgMax As Long              ' Max value in the sequence (avg)
    reserved1 As Double
End Type

Type AqDataDescriptor
    returnedSamplesPerSeg As Long
    indexFirstPoint As Long     ' "waveArray(wfDesc.indexFirstPoint)" is the first valid point of "waveArray"
    sampTime As Double
    vGain As Double
    vOffset As Double
    returnedSegments As Long    ' When reading multiple segments in one waveform
    nbrAvgWforms As Long
    actualTriggersInAcqLo As Long
    actualTriggersInAcqHi As Long
    actualDataSize As Long
    missingValue As Long
    reserved3 As Double
End Type

Type AqGateParameters
    gatePos As Long
    gateLength As Long
End Type


'/////////////////////////////////////////////////////////////////////////////////////////
' Function Prototypes

Public Declare Function AccumulateData Lib "AqDrv4" _
Alias "_AcqrsD1_accumulateData@28" (ByVal instrumentID As Long, _
                                    ByVal channel As Long, _
                                    readPar As AqReadParameters, _
                                    dataArray As Any, _
                                    sumArray As Long, _
                                    dataDesc As AqDataDescriptor, _
                                    segDescArray As Any) As Long
' Returns a waveform segment as an array and accumulates it in a client array.
'
' NOTE: Due to the destination 'sumArray' type, this routine should be called at most
'       16777216 times for 'readPar.dataType = 0' (char) or
'       65536 times for 'readPar.dataType = 1' (short).
'       Otherwise, an overflow may occur and the summed values will wrap around 0. Note that the sumArray
'       can be interpreted as an unsigned integer. Alternatively, negative values have to be
'       increased by 2**32.
'
' The following values must be supplied to the function:
'
' 'channel'        = 1...Nchan (as returned by 'GetNbrChannels' )
' 'readPar'        = pointer to a user-supplied structure that specifies what and how to read,
'                    similar to function 'ReadData'.
'                    However, only 'readPar.nbrSegments = 1' and 'readPar.readMode = 0' (ReadModeStdW) are supported.
'                    'readPar.dataType = 3' (real) and 'readPar.dataType = 2' (long) are NOT supported.
'
' Values returned by the function:
'
' 'dataArray'      = user-allocated data destination array of type defined by 'readPar.dataType'
'                    Its size in dataType units MUST be at least (nbrSamples + 32),
'                    for reasons of data alignment. Please refer to the manual for additional details.
' 'sumArray'       = user-allocated waveform accumulation array of type Long (32 bits).
'                    Its size MUST be at least 'nbrSamples'. Note that the sumArray can be interpreted
'                    as an unsigned integer. Alternatively. negative values have to be increased by 2**32.
' 'dataDesc'       = user-allocated structure for returned data descriptor values
' 'segDescArray'   = NULL pointer or pointer to a user allocated structure for returned AqSegmentDescriptor
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_DATA_ARRAY                 if 'dataArray' is NULL.
' ACQIRIS_ERROR_NOT_SUPPORTED              if this function is not supported by the instrument.
' ACQIRIS_ERROR_INSTRUMENT_RUNNING         if an acquisition is already running.
' ACQIRIS_ERROR_CANNOT_READ_THIS_CHANNEL   if the requested channel is not available.
' VI_ERROR_PARAMETERx                      if one of the parameters has an invalid value, where 
'                                          x = parameter number.
' VI_SUCCESS                               otherwise.
 
Public Declare Function AccumulateWform Lib "AqDrv4" _
Alias "_AcqrsD1_accumulateWform@44" (ByVal instrumentID As Long, _
                                     ByVal channel As Long, _
                                     ByVal segmentNumber As Long, _
                                     ByVal firstSample As Long, _
                                     ByVal nbrSamples As Long, _
                                     waveformArray As Byte, _
                                     sumArray As Long, _
                                     returnedSamples As Long, _
                                     sampTime As Double, _
                                     vGain As Double, _
                                     vOffset As Double) As Long
' Returns a waveform as a byte array and accumulate it in a client array.
'
' The following values must be supplied to the function:
'
' 'channel'        = 1...Nchan (as returned by 'GetNbrChannels' )
' 'segmentNumber'  = requested segment number, may assume 0 to 'nbrSegments - 1',
'                    as set with the function 'ConfigMemory'
' 'firstSample'    = requested position of first sample to read, typically 0
' 'nbrSamples'     = requested number of samples, may assume 1 to 'nbrSamples', as
'                    set with the function 'ConfigMemory'
'
' Values returned by the function:
'
' 'waveformArray'  = user-allocated waveform destination array for returned data
'                    Its size MUST be at least 'nbrSamples' + 32, for reasons of
'                    data alignment.
' 'sumArray'       = user-allocated waveform accumulation array for returned data
'                    Its size MUST be at least 'nbrSamples'.
' 'returnedSamples'= number of data samples actually returned
' 'sampTime'       = sampling interval in seconds
' 'vGain'          = vertical gain in Volts/LSB
' 'vOffset'        = vertical offset in Volts
'                    The value in Volts of a data point 'data' in the returned
'                    'waveformArray' array is computed with the formula
'                    V = vGain * data - vOffset
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_NOT_SUPPORTED              if this function is not supported by the instrument.
' ACQIRIS_ERROR_INSTRUMENT_RUNNING         if an acquisition is already running.
' ACQIRIS_ERROR_CANNOT_READ_THIS_CHANNEL   if the requested channel is not available.
' VI_ERROR_PARAMETERx                      if one of the parameters has an invalid value, where 
'                                          x = parameter number.
' VI_SUCCESS                               otherwise.

Public Declare Function AcqDone Lib "AqDrv4" _
Alias "_AcqrsD1_acqDone@8" (ByVal instrumentID As Long, done As Boolean) As Long
' Checks if the acquisition has terminated
' Returns 'done' = TRUE if the acquisition is terminated
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_IO_READ    if a link error has been detected (e.g. PCI link lost).
' VI_SUCCESS otherwise.

Public Declare Function Acquire Lib "AqDrv4" _
Alias "_AcqrsD1_acquire@4" (ByVal instrumentID As Long) As Long
' Starts an acquisition. This function is equivalent to 'AcquireEx' with 'acquireMode = 0, 
' acquireFlags = 0'
'
' Common return values:
' ACQIRIS_ERROR_INSTRUMENT_RUNNING if the instrument is already running. 
' VI_SUCCESS otherwise.

Public Declare Function AcquireEx Lib "AqDrv4" _
Alias "_AcqrsD1_acquireEx@20" (ByVal instrumentID As Long, _
                               ByVal acquireMode As Long, _
                               ByVal acquireFlags As Long, _
                               ByVal acquireParams As Long, _
                               ByVal reserved As Long) As Long
' Starts an acquisition.
'
' The following values must be supplied to the function:
'
' 'acquireMode'   = 0       Normal, start an acquisition and return immediatly (equivalent to function 'Acquire').
'                 = 2       Averagers only! Sets continuous accumulation and starts an acquisition.
'
' 'acquireFlags'  = 0   No flags.
'                 = 4   Reset timestamps (if supported).
' 'acquireParams'       Unused, must be set to 0.
' 'reserved'            Unused, must be set to 0.
'
' Common return values:
'
' ACQIRIS_ERROR_INSTRUMENT_RUNNING if the instrument is already running.
' ACQIRIS_ERROR_NOT_SUPPORTED if the requested mode or flag is not supported by current instrument.
' VI_SUCCESS otherwise.

Public Declare Function AveragedData Lib "AqDrv4" _
Alias "_AcqrsD1_averagedData@44" (ByVal instrumentID As Long, _
                                  ByVal channel As Long, _
                                  readPar As AqReadParameters, _
                                  ByVal nbrAcq As Long, _
                                  ByVal calculateMean As Boolean, _
                                  ByVal timeout As Double, _
                                  dataArray As Any, _
                                  sumArray As Long, _
                                  dataDesc As AqDataDescriptor, _
                                  segDescArray As Any) As Long
'Perform a series of acquisitions and get the resulting averaged waveform for one segment.
'
'NOTE: Because the acquisition control loop is done inside this function, it is
'suitable ONLY for single instrument, single channel, single segment operation.
'
'NOTE2: This function is for DIGITIZERS ONLY !! For getting the average of an averager module (AP-XXX)
'       you must use 'readData', and have set the instrument through 'configMode' with 'mode = 2'.
'
'The following values must be supplied to the function:
'
' 'channel'        = 1...Nchan (as returned by 'GetNbrChannels' )
''readPar'         = pointer to a user-supplied structure that specifies what and how to read,
'                    similar to function 'ReadData'.
'                    However, only 'readPar.nbrSegments = 1' and 'readPar.readMode = 0' (ReadModeStdW) are supported.
'                    'readPar.dataType = 3' (real) and 'readPar.dataType = 2' (long) are NOT supported.
''nbrAcq'          = number of acquisitions to be performed. This value can be at most
'                    16777216 for 'readPar.dataType = 0' (char) or
'                    65536 for 'readPar.dataType = 1' (short)
'                    Otherwise, an overflow may occur and the summed values will wrap around 0.
''calculateMean'   = if '1', each sample of the 'sumArray' is divided by the 'nbrAcq', in order
'                    to calculate the mean. If '0', the 'sumArray' will contain the
'                    sample-by-sample sums, and must be divided by 'nbrAcq' afterward to get
'                    the averaged values
''timeout'         = acquisition timeout in seconds. The function will return
'                    ACQIRIS_ERROR_ACQ_TIMEOUT if there is no trigger within the specified timeout interval
'                    after the start of each acquisition.
'
'Values returned by the function:
'
''dataArray'       = user-allocated data destination array of type defined by 'readPar.dataType'
'                    Its size in dataType units MUST be at least (nbrSamples + 32),
'                    for reasons of data alignment. Please refer to the manual for additional details.
''sumArray'        = user-allocated waveform accumulation array of type Long (32 bits).
'                    Its size MUST be at least 'nbrSamples'. Note that the sumArray can be interpreted
'                    as an unsigned integer. Alternatively, negative values have to be increased by 2**32.
''dataDesc'        = user-allocated structure for returned data descriptor values, from last acquisition.
''segDescArray'    = NULL pointer or pointer to a user allocated structure for returned AqSegmentDescriptor.
'                    Returned value in 'segDescArray' is from the last acquisition.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_DATA_ARRAY                 if 'dataArray' is NULL.
' ACQIRIS_ERROR_DATATYPE                   if 'readPar->dataType' is invalid.
' ACQIRIS_ERROR_INSTRUMENT_RUNNING         if an acquisition is already running.
' ACQIRIS_ERROR_CANNOT_READ_THIS_CHANNEL   if the requested channel is not available.
' ACQIRIS_ERROR_ACQ_TIMEOUT                if 'timeout' has elapsed without getting valid 
'                                          triggers.
' VI_ERROR_PARAMETERx                      if one of the parameters has an invalid value, where 
'                                          x = parameter number.
' VI_SUCCESS                               otherwise.
' For other errors, please see 'ReadData'.

Public Declare Function AveragedWform Lib "AqDrv4" _
Alias "_AcqrsD1_averagedWform@56" (ByVal instrumentID As Long, _
                                   ByVal channel As Long, _
                                   ByVal segmentNumber As Long, _
                                   ByVal firstSample As Long, _
                                   ByVal nbrSamples As Long, _
                                   ByVal nbrAcq As Long, _
                                   ByVal timeout As Double, _
                                   waveformArray As Byte, _
                                   sumArray As Long, _
                                   returnedSamples As Long, _
                                   sampTime As Double, _
                                   vGain As Double, _
                                   vOffset As Double) As Long
' Perform a series of acquisitions and get the resulting averaged waveform.
'
' NOTE: The 'sumArray' contains the sample-by-sample sums. To get the average values,
' the array elements must be divided by 'nbrAcq'.
'
' NOTE: Because the acquisition control loop is done inside this function, it is
' suitable ONLY for single instrument, single channel operation.
'
' The following values must be supplied to the function:
'
' 'channel'        = 1...Nchan (as returned by 'GetNbrChannels' )
' 'segmentNumber'  = requested segment number, may assume 0 to 'nbrSegments - 1',
'                    as set with the function 'ConfigMemory'
' 'firstSample'    = requested position of first sample to read, typically 0. May assume 0 to
'                    (the number of samples - 1) as set with the function 'ConfigMemory'
' 'nbrSamples'     = requested number of samples, may assume 1 to 'nbrSamples', as
'                    set with the function 'ConfigMemory'
' 'nbrAcq'         = number of acquisitions to be performed
' 'timeout'        = acquisition timeout in seconds. The function will return an error
'                    if there are no triggers within the specified timeout
'
' Values returned by the function:
'
' 'waveformArray'  = user-allocated waveform destination array for returned data
'                    Its size MUST be at least 'nbrSamples' + 32, for reasons of
'                    data alignment.
' 'sumArray'       = user-allocated waveform accumulation array for returned data
'                    Its size MUST be at least 'nbrSamples'.
' 'returnedSamples'= number of data samples actually returned
' 'sampTime'       = sampling interval in seconds
' 'vGain'          = vertical gain in Volts/LSB
' 'vOffset'        = vertical offset in Volts
'                    The value in Volts of a data point 'data' in the returned
'                    'waveformArray' array is computed with the formula
'                    V = vGain * data - vOffset
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_INSTRUMENT_RUNNING         if an acquisition is already running.
' ACQIRIS_ERROR_CANNOT_READ_THIS_CHANNEL   if the requested channel is not available.
' ACQIRIS_ERROR_ACQ_TIMEOUT                if 'timeout' has elapsed without getting valid 
'                                          triggers.
' VI_ERROR_PARAMETERx                      if one of the parameters has an invalid value, where 
'                                          x = parameter number.
' VI_SUCCESS                               otherwise.

Public Declare Function BestNominalSamples Lib "AqDrv4" _
Alias "_AcqrsD1_bestNominalSamples@8" (ByVal instrumentID As Long, _
                                       nomSamples As Long) As Long
' Helper function to ease the instrument configuration.
' Returns maximum nominal number of samples which fits into the available memory.
'
' Values returned by the function:
'
' 'nomSamples'     = maximum number of data samples available
'
' NOTE: When using this method, make sure to use 'ConfigHorizontal' and
'       'ConfigMemory' beforehand to set the sampling rate and the number of
'       segments to the desired values ('nbrSamples' in 'ConfigMemory' may be
'       any number!). 'BestNominalSamples' depends on these variables.
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                   if one of the parameters has an invalid value, where 
'                                       x = parameter number.
' VI_SUCCESS                            when a good solution has been found.

Public Declare Function BestSampInterval Lib "AqDrv4" _
Alias "_AcqrsD1_bestSampInterval@24" (ByVal instrumentID As Long, _
                                      ByVal maxSamples As Long, _
                                      ByVal timeWindow As Double, _
                                      sampInterval As Double, _
                                      nomSamples As Long) As Long
' Helper function to ease the instrument configuration.
' Returns the best possible sampling rate for an acquisition which covers the 'timeWindow'
' with no more than 'maxSamples', taking into account the current state of the instrument,
' in particular the requested channel combination and the number of segments.
' In addition, this routine returns the 'real' nominal number of samples which can
' be accommodated (it is computed as timeWindow/sampInterval !).
'
' The following values must be supplied to the function:
'
' 'maxSamples'     = maximum number of samples to be used
' 'timeWindow'     = time window in seconds to be covered
'
' Values returned by the function:
'
' 'sampInterval'   = recommended sampling interval in seconds
' 'nomSamples'     = recommended number of data samples
'
' Return code   = ACQIRIS_ERROR_SETUP_NOT_AVAILABLE
'                   when the available memory is too short, and the longest available
'                   sampling interval too short. The returned sampling interval is the
'                   longest one possible.
'               = VI_SUCCESS
'                   when a good solution has been found.
'
' NOTE: This function DOES NOT modify the state of the digitizer at all. It simply returns
'       a recommendation which the user is free to override.
' NOTE: When using this method, make sure to use 'ConfigMemory' beforehand to set
'       the number of segments to the desired value. ('nbrSamples' may be any
'       number!). 'BestSampInterval' depends on this variable.
' NOTE: The returned 'recommended' values for the 'sampInterval' and the nominal number
'       of samples 'nomSamples' are expected to be used for configuring the instrument
'       with calls to 'ConfigHorizontal' and 'ConfigMemory'. Make sure
'       to use the same number of segments in this second call to 'ConfigMemory'
'       as in the first one.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_SETUP_NOT_AVAILABLE when the available memory is too short, and the longest
'                                   available sampling interval too short. The returned 
'                                   sampling interval is the longest one possible.
' VI_ERROR_PARAMETERx               if one of the parameters has an invalid value, where 
'                                   x = parameter number.
' VI_SUCCESS                        when a good solution has been found.
                                      
Public Declare Function Calibrate Lib "AqDrv4" _
Alias "_AcqrsD1_calibrate@4" (ByVal instrumentID As Long) As Long
' Performs an auto-calibration of the instrument
' Equivalent to CalibrateEx with 'calType' = 0

Public Declare Function CalibrateEx Lib "AqDrv4" _
Alias "_AcqrsD1_calibrateEx@16" (ByVal instrumentID As Long, _
                                 ByVal calType As Long, _
                                 ByVal modifier As Long, _
                                 ByVal flags As Long) As Long
' Performs a (partial) auto-calibration of the instrument
'
' 'calType'=  0    calibrate the entire instrument (equivalent to 'Calibrate')
'             1    calibrate only the current channel configuration, 
'                  as set by 'ConfigChannelCombination'
'             2    calibrate external clock timing. Requires operation in External Clock 
'                  (Continuous), i.e. 'clockType' = 1, as set with 'ConfigExtClock'
'             3    calibrate only at the current frequency (only instruments with fine resolution 
'                  time bases)
'             4    calibrate fast, only at the current settings (if supported). 
'                  Note: In this mode, any change on 'fullscale', 'bandwidth limit', 'channel 
'                  combine', 'impedance' or 'coupling' will require a new calibration.
'
' 'modifier'
'          ['calType' = 0] currently unused, set to zero
'          ['calType' = 1] currently unused, set to zero
'          ['calType' = 2] currently unused, set to zero
'          ['calType' = 3]      0  = All channels
'                              >0  = Only specified channel (for i.e. 1 = calibrate only channel 1)
'
' 'flags'          currently unused, set to zero
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

Public Declare Function CloseInstr Lib "AqDrv4" _
Alias "_AcqrsD1_close@4" (ByVal instrumentID As Long) As Long
' Close specified instrument. Once closed, this instrument is not available anymore and
' need to be reenabled using 'InitWithOptions' or 'init'.
' Note: For freeing properly all resources, 'closeAll' must still be called when
' the application close, even if 'close' was called for each instrument.
'
' Returns one of the following ViStatus values:
' VI_SUCCESS always.

Public Declare Function CloseAll Lib "AqDrv4" _
Alias "_AcqrsD1_closeAll@0" () As Long
' Closes all instruments and prepares for closing of application
'
' Returns one of the following ViStatus values:
' VI_SUCCESS always.

Public Declare Function ConfigAvgConfig Lib "AqDrv4" _
Alias "_AcqrsD1_configAvgConfig@16" (ByVal instrumentID As Long, _
                                     ByVal channel As Long, _
                                     ByVal parameterString As String, _
                                     value As Any) As Long
' Configures Averagers and Analyzer (APxxx)!
'
' Configures parameter in the channel dependent averager/analyzer configuration 'channel'
' 'channel'        = 1...Nchan
'                  = 0 (selecting channel 1) is supported for backwards compatibility
'
' 'parameterString' = character string specifying the requested parameter
'                     Please refer to the manual for the accepted parameter strings
' 'value'           = value to set
'                     The type of the value depends on 'parameterString'
'                     Please refer to the manual for the required  data type as a
'                     function of the accepted parameters.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_NOT_SUPPORTED        if this function is not supported by the instrument.
' ACQIRIS_ERROR_PARAM_STRING_INVALID if 'parameterString' is invalid.
' ACQIRIS_WARN_SETUP_ADAPTED         if 'value' has been adapted.
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigAvgConfigInt32 Lib "AqDrv4" _
Alias "_AcqrsD1_configAvgConfigInt32@16" (ByVal instrumentID As Long, _
                                     ByVal channel As Long, _
                                     ByVal parameterString As String, _
                                     byval value As long) As Long
' Configures Averagers and Analyzer (APxxx)!
'
' Configures parameter in the channel dependent averager/analyzer configuration 'channel'
' 'channel'        = 1...Nchan
'                  = 0 (selecting channel 1) is supported for backwards compatibility
'
' 'parameterString'= character string specifying the requested parameter
'                    Please refer to the manual for the accepted parameter strings
' 'value'          = value to set
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_NOT_SUPPORTED        if this function is not supported by the instrument.
' ACQIRIS_ERROR_PARAM_STRING_INVALID if 'parameterString' is invalid.
' ACQIRIS_WARN_SETUP_ADAPTED         if 'value' has been adapted.
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigAvgConfigReal64 Lib "AqDrv4" _
Alias "_AcqrsD1_configAvgConfigReal64@20" (ByVal instrumentID As Long, _
                                     ByVal channel As Long, _
                                     ByVal parameterString As String, _
                                     byval value As double) As Long
' Configures Averagers and Analyzer (APxxx)!
'
' Configures parameter in the channel dependent averager/analyzer configuration 'channel'
' 'channel'        = 1...Nchan
'                  = 0 (selecting channel 1) is supported for backwards compatibility
'
' 'parameterString'= character string specifying the requested parameter
'                    Please refer to the manual for the accepted parameter strings
' 'value'          = value to set
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_NOT_SUPPORTED        if this function is not supported by the instrument.
' ACQIRIS_ERROR_PARAM_STRING_INVALID if 'parameterString' is invalid.
' ACQIRIS_WARN_SETUP_ADAPTED         if 'value' has been adapted.
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigChannelCombination Lib "AqDrv4" _
Alias "_AcqrsD1_configChannelCombination@12" (ByVal instrumentID As Long, _
                                     ByVal nbrConvertersPerChannel As Long, _
                                     ByVal usedChannels As Long) As Long
' Configures combined operation of multiple channels
'
' 'nbrConvertersPerChannel'    = 1     all channels use 1 converter each (default)
'                              = 2     half of the channels use 2 converters each
'                              = 4     1/4  of the channels use 4 converters each
'
' 'usedChannels'       bit-field indicating which channels are used (LSbit = channel 1)
' The acceptable values for 'usedChannels' depend on 'nbrConvertersPerChannel'
' and on the number of available channels in the digitizer:
' A) If 'nbrConvertersPerChannel' = 1, 'usedChannels' must reflect the fact that
' ALL channels are available for use. It accepts a single value for a given digitizer:
' 'usedChannels'       = 0x00000001    if the digitizer has 1 channel
'                      = 0x00000003    if the digitizer has 2 channels
'                      = 0x0000000f    if the digitizer has 4 channels
' B) If 'nbrConvertersPerChannel' = 2, 'usedChannels' must reflect the fact that
' only half of the channels may be used:
' 'usedChannels'       = 0x00000001    use channel 1 on a 2-channel digitizer
'                        0x00000002    use channel 2 on a 2-channel digitizer
'                        0x00000003    use channels 1+2 on a 4-channel digitizer
'                        0x00000005    use channels 1+3 on a 4-channel digitizer
'                        0x00000009    use channels 1+4 on a 4-channel digitizer
'                        0x00000006    use channels 2+3 on a 4-channel digitizer
'                        0x0000000a    use channels 2+4 on a 4-channel digitizer
'                        0x0000000c    use channels 3+4 on a 4-channel digitizer
' C) If 'nbrConvertersPerChannel' = 4, 'usedChannels' must reflect the fact that
' only 1 of the channels may be used:
' 'usedChannels'       = 0x00000001    use channel 1 on a 4-channel digitizer
'                        0x00000002    use channel 2 on a 4-channel digitizer
'                        0x00000004    use channel 3 on a 4-channel digitizer
'                        0x00000008    use channel 4 on a 4-channel digitizer
' NOTE: Digitizers which don't support channel combination, always use the default
' 'nbrConvertersPerChannel' = 1, and the single possible value of 'usedChannels'
' NOTE: If digitizers are combined with ASBus, the channel combination applies equally to
' all participating digitizers.
'
' Returns one of the following ViStatus values:
' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigControlIO Lib "AqDrv4" _
Alias "_AcqrsD1_configControlIO@24" (ByVal instrumentID As Long, _
                                     ByVal connector As Long, _
                                     ByVal signal As Long, _
                                     ByVal qualifier1 As Long, _
                                     ByVal qualifier2 As Double) As Long
' Configures Control-IO connectors
' Typically, only a few (or no) IO connectors are present on a single digitizer
'
' 'connector'      = 1     Front Panel I/O A (MMCX connector)
'                  = 2     Front Panel I/O B (MMCX connector)
'                  = 9     Front Panel Trigger Out (MMCX connector)
' 'signal'         = value depends on 'connector', refer to manual for definitions.
' 'qualifier1', 'qualifier2'   currently unused (set to zero!)
'
' Returns one of the following ViStatus values:
' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigExtClock Lib "AqDrv4" _
Alias "_AcqrsD1_configExtClock@36" (ByVal instrumentID As Long, _
                                    ByVal clockType As Long, _
                                    ByVal inputThreshold As Double, _
                                    ByVal delayNbrSamples As Long, _
                                    ByVal refFrequency As Double, _
                                    ByVal sampFrequency As Double) As Long
' Configures the external clock of the digitizer
'
' 'clockType'      = 0     Internal Clock (default at start-up)
'                  = 1     External Clock (continuous operation)
'                  = 2     External Reference (10 MHz)
'                  = 4     External Clock (start/stop operation)
' 'inputThreshold' = input threshold for external clock or reference in mV
' 'delayNbrSamples'= number of samples to acquire after trigger (for 'clockType' = 1 ONLY!)
' 'refFrequency'   = frequency, in Hz, of external reference
' 'sampFrequency'  = frequency, in Hz, of requested sampling
'
' NOTE: When 'clockType' is set to 1 or 4, the values 'sampInterval' and 'delayTime'
'       in the function 'ConfigHorizontal' are ignored.
'
' Returns one of the following ViStatus values:
' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigFCounter Lib "AqDrv4" _
Alias "_AcqrsD1_configFCounter@40" (ByVal instrumentID As Long, _
                                    ByVal signalChannel As Long, _
                                    ByVal typeMeas As Long, _
                                    ByVal targetValue As Double, _
                                    ByVal apertureTime As Double, _
                                    ByVal reserved As Double, _
                                    ByVal flags As Long) As Long
' Configures the frequency counter
'
' 'signalChannel'  = 1...Nchan for signal input channel
' 'typeMeas'       = 0     Frequency
'                  = 1     Period
'                  = 2     Totalize, counts input pulses during interval defined by 'apertureTime'
' 'targetValue'    = estimate of expected result (set to 0.0, if no estimate available)
' 'apertureTime'   = minimum measurement time for Frequency and Period modes
'                  = time gate for Totalize mode
' 'reserved', 'flags'  currently unused (set to zero!)
'
' Returns one of the following ViStatus values:
' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigHorizontal Lib "AqDrv4" _
Alias "_AcqrsD1_configHorizontal@20" (ByVal instrumentID As Long, _
                                      ByVal sampInterval As Double, _
                                      ByVal delayTime As Double) As Long
' Configures the horizontal control parameters of the digitizer.
'
' 'sampInterval'   = sampling interval in seconds
' 'delayTime'      = trigger delay time in seconds, with respect to the
'                    beginning of the record.
'                    A positive number corresponds to trigger BEFORE the beginning
'                    of the record (post-trigger recording).
'                    A negative number corresponds to pre-trigger recording. It
'                    cannot be smaller than (- sampInterval * nbrSamples), which
'                    corresponds to 100% pre-trigger.
'
' Returns one of the following ViStatus values:
' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigLogicDevice Lib "AqDrv4" _
Alias "_AcqrsD1_configLogicDevice@16" (ByVal instrumentID As Long, _
                                       ByVal deviceName As String, _
                                       ByVal filePathName As String, _
                                       ByVal modifier As Long) As Long
' Load/Clear/SetFilePath of the on-board logic devices. ONLY useful for a module
' with on-board programmable logic devices  (SCxxx, ACxxx, APxxx, and 12-bit Digitizers).
'
' 'flags'    = operation
'            = 0 program device, using strictly the specified path/file name
'            = 1 clear device
'            = 2 clear and program devices using appropriate files from specified path.
'            = 3 program device, but also look for file path in the 'AqDrv4.ini' file, if the file 
'                was not found. It is sufficient to specify the file name (without explicit path), 
'                if the file is either in the current working directory or in the directory pointed 
'                to by 'FPGAPATH' in 'AqDrv4.ini'.
'
' 'deviceName'   Identifies which device to program.
'    ['flags' = 0 or 3] = device to program.
'        This string must be "Block1Devx", with x = is the FPGA number to be programmed.
'        For instance, in the SC240, it must be "Block1Dev1".
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
' ACQIRIS_ERROR_NO_DATA                 if an FPGA file did not contain expected data.
' ACQIRIS_ERROR_FIRMWARE_NOT_AUTHORIZED if the instrument is not authorized to use the 
'                                       specified 'filePathName' file.
' ACQIRIS_ERROR_EEPROM_DATA_INVALID     if the CPLD 'filePathName' is invalid.
' ACQIRIS_ERROR_FPGA_y_LOAD             if an FPGA could not be loaded, 
'                                       where 'y' = FPGA nbr. 
' ACQIRIS_WARN_SETUP_ADAPTED            if one of the parameters has been adapted.
' VI_SUCCESS                            otherwise.


Public Declare Function ConfigMemory Lib "AqDrv4" _
Alias "_AcqrsD1_configMemory@12" (ByVal instrumentID As Long, _
                                  ByVal nbrSamples As Long, _
                                  ByVal nbrSegments As Long) As Long
' Configures the memory control parameters of the digitizer.
'
' 'nbrSamples'     = nominal number of samples to record (per segment!)
' 'nbrSegments'    = number of segments to acquire
'                    1 corresponds to the normal single-trace acquisition mode.
'
' Returns one of the following ViStatus values:
' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigMemoryEx Lib "AqDrv4" _
Alias "_AcqrsD1_configMemoryEx@24" (ByVal instrumentID As Long, _
                                    ByVal nbrSamplesHi As Long, _
                                    ByVal nbrSamplesLo As Long, _
                                    ByVal nbrSegments As Long, _
                                    ByVal nbrBanks As Long, _
                                    ByVal flags As Long) As Long
' Configures the memory control parameters of the digitizer.
'
' 'nbrSamplesHi'      = reserved for future use, must be set to 0.
' 'nbrSamplesLo'      = nominal number of samples to record (per segment!).
' 'nbrSegments'       = number of segments to acquire per bank
'                       1 corresponds to the normal single-trace acquisition mode.
' 'nbrBanks'          = number of banks in which the memory will be split, 
'                       for buffered reading (SAR).
'                       1 corresponds to the normal acquisition mode.
' 'flags'             = 0 no flags. 
'                     = 1 force use of internal memory (for 10-bit digitizers with extended 
'                       memory options only).
'
' Returns one of the following ViStatus values:
' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigMode Lib "AqDrv4" _
Alias "_AcqrsD1_configMode@16" (ByVal instrumentID As Long, _
                                ByVal mode As Long, _
                                ByVal modifier As Long, _
                                ByVal flags As Long) As Long
' Configures the operational mode of the digitizer.
'
' 'mode'           = operational mode
'                  = 0     normal acquisition
'                  = 1     stream data to DPU (only in SC2x0/AC2x0 Data Streamers)
'                  = 2     averaging mode (only in Real-Time Averagers APx00 and AP240)
'                  = 3     dual-memory mode (only in AP101, AP201)
'                  = 6     frequency counter mode
'                  = 7     SSR mode (for AP240)
'
' 'modifier'
'          ['mode' = 0]    = not used, set to 0
'          ['mode' = 1]    = not used, set to 0
'          ['mode' = 2]    = not used, set to 0
'          ['mode' = 3]    = not used, set to 0
'          ['mode' = 6]    = not used, set to 0
'          ['mode' = 7]    = not used, set to 0
'
' 'flags'  ['mode' = 0]    = 0     normal
'                          = 1     'Start-on-Trigger' mode (if available)
'                          = 2     'Sequence-wrap' mode (use 'nbrSegments' > 1)
'                          = 10    'SAR' mode (use 'nbrBanks' > 1) 
'          ['mode' = 1]    = 0     normal
'                          = 1     autoRestart
'          ['mode' = 2]    = 0     unused, set to 0
'          ['mode' = 3]    = 0     acquire into memory bank 0
'                          = 1     acquire into memory bank 1
'          ['mode' = 6]    = 0     not used, set to 0
'          ['mode' = 7]    = 0     not used, set to 0
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_FILE_NOT_FOUND       if an FPGA file could not be found.
' ACQIRIS_ERROR_NO_DATA              if an FPGA file did not contain expected data.
' ACQIRIS_ERROR_FPGA_x_LOAD          if an FPGA could not be loaded, 
'                                    where 'x' = FPGA nbr. 
' ACQIRIS_ERROR_INSTRUMENT_RUNNING   if the instrument is currently running.
' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigMultiInput Lib "AqDrv4" _
Alias "_AcqrsD1_configMultiInput@12" (ByVal instrumentID As Long, _
                                      ByVal channel As Long, _
                                      ByVal inputs As Long) As Long
' Configures the multiple input on a channel
'
'  'connection'    = 0     set to input connection A
'                  = 1     set to input connection B
'
' NOTE: This function is only of use for instruments with an input-multiplexer (i.e. more
'       than 1 input per digitizer, e.g. DP211). On the 'normal' instruments with a single
'       input per digitizer, this function may be ignored.
'
' Returns one of the following ViStatus values:
' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigSetupArray Lib "AqDrv4" _
Alias "_AcqrsD1_configSetupArray@20" (ByVal instrumentID As Long, _
                                      ByVal channel As Long, _
                                      ByVal setupType As Long, _
                                      ByVal nbrSetupObj As Long, _
                                            setupData As Any) As Long
' Configures an array of setup data (typically for on-board processing)
'
' 'setupType'       = indicates the object type (data structure) of which the setup data is composed
'                     Some objects might be simple elements, e.g. Long
'
' 'nbrSetupObj'     = number of configuration objects contained in configSetupData
'
' 'setupData'       = setup data array
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_NOT_SUPPORTED        if this function is not supported by the instrument.
' ACQIRIS_ERROR_BUFFER_OVERFLOW      if 'nbrSetupObj' exceeds the maximum allowed value.
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigTrigClass Lib "AqDrv4" _
Alias "_AcqrsD1_configTrigClass@36" (ByVal instrumentID As Long, _
                                     ByVal trigClass As Long, _
                                     ByVal sourcePattern As Long, _
                                     ByVal validatePattern As Long, _
                                     ByVal holdType As Long, _
                                     ByVal holdValue1 As Double, _
                                     ByVal holdValue2 As Double) As Long
' Configures the trigger class control parameters of the digitizer.
'
' 'trigClass'          = 0             edge trigger
'                      = 1             TV trigger (12-bit-FAMILY External only)
'                      = 3             OR (if available)
'                      = 4             NOR (if available)
'                      = 5             AND (if available)
'                      = 6             NAND (if available)
' 'sourcePattern'      = 0x000n0001    Enable Channel 1
'                      = 0x000n0002    Enable Channel 2
'                      = 0x000n0004    Enable Channel 3
'                      = 0x000n0008    Enable Channel 4    etc.
'                      = 0x800n0000    Enable External Trigger 1
'                      = 0x400n0000    Enable External Trigger 2 (if available) etc.
'                        where n is 0 for single instruments, or the module number for
'                        MultiInstruments (ASBus operation). When 'trigClass' = 3,4,5 or 6,
'                        the 'sourcePattern' can be a combination of different sources.
'                        See manual for a detailed description of 'sourcePattern'.
' 'validatePattern'      Unused, set to 0.
' 'holdoffType'        = 0             Holdoff by time (if available)
' 'holdoffTime'        Holdoff time, in units of seconds.
' 'reserved'           Unused, set to 0.0.
'
' Note: The detailed TV trigger configuration is set with the function 'ConfigTrigTV'
' Note2: trigClass = 3,4,5 or 6 features are only supported within a single instrument, or
'   within a single module in an ASBUS configuration (if available).
'
' Returns one of the following ViStatus values:
' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigTrigSource Lib "AqDrv4" _
Alias "_AcqrsD1_configTrigSource@32" (ByVal instrumentID As Long, _
                                      ByVal channel As Long, _
                                      ByVal trigCoupling As Long, _
                                      ByVal trigSlope As Long, _
                                      ByVal trigLevel1 As Double, _
                                      ByVal trigLevel2 As Double) As Long
' Configures the trigger source control parameters for a specified channel in the digitizer.
'
' 'channel'        = 1... (Number of IntTrigSources) for internal trigger sources
'                  = -1..-(Number of ExtTrigSources) for external trigger sources
' 'trigCoupling'   = 0     DC
'                  = 1     AC
'                  = 2     HFreject (if available)
'                  = 3     DC, 50 Ohms (external trigger only, if available)
'                  = 4     AC, 50 Ohms (external trigger only, if available)
' 'trigSlope'      = 0     Positive
'                  = 1     Negative
'                  = 2     Window, transition out of window
'                  = 3     Window, transition into window
'                  = 4     HFdivide (by factor 4)
' 'trigLevel1' (internal)  in % of Vertical Full Scale of the channel settings
'              (external)  in mV
' 'trigLevel2' (internal)  in % of Vertical Full Scale of the channel settings
'              (external)  in mV
'                          'trigLevel2' is only used when Window Trigger is selected
' NOTE: Some of the possible states may unavailable in some digitizers.
'
' Returns one of the following ViStatus values:
' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigTrigTV Lib "AqDrv4" _
Alias "_AcqrsD1_configTrigTV@20" (ByVal instrumentID As Long, _
                                  ByVal channel As Long, _
                                  ByVal standard As Long, _
                                  ByVal field As Long, _
                                  ByVal line As Long) As Long
' Configures the TV trigger control parameters for a specified channel in the digitizer.
'
' 'channel'        = -1..-(Number of ExtTrigSources) for external trigger sources
'                    NOTE: the TV trigger option is only available on the External Trigger input
' 'standard'       = 0     625 lines per frame / 50 Hz
'                  = 2     525 lines per frame / 60 Hz
' 'field'          = 1     field 1 (odd)
'                  = 2     field 2 (even)
' 'line'           = line number, depends on the 'standard' and 'field' selection:
'                    1 to 263  for 'standard' = 525/60Hz and 'field' = 1
'                    1 to 262  for 'standard' = 525/60Hz and 'field' = 2
'                    1 to 313  for 'standard' = 625/50Hz and 'field' = 1
'                  314 to 625  for 'standard' = 625/50Hz and 'field' = 2
'
' Returns one of the following ViStatus values:
' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
' ACQIRIS_ERROR_NOT_SUPPORTED        if this 'channel' does not support the TV trigger.
' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
' VI_ERROR_PARAMETER2                if 'channel' is invalid.
' VI_SUCCESS                         otherwise.

Public Declare Function ConfigVertical Lib "AqDrv4" _
Alias "_AcqrsD1_configVertical@32" (ByVal instrumentID As Long, _
                                    ByVal channel As Long, _
                                    ByVal fullScale As Double, _
                                    ByVal offset As Double, _
                                    ByVal coupling As Long, _
                                    ByVal bandwidth As Long) As Long
' Configures the vertical control parameters for a specified channel in the digitizer.
'
' 'channel'        = 1...Nchan
'                  = -1  for Ext. Trigger Input of digitizers with programmable Trigger Full Scale
' 'fullScale'      = in Volts
' 'offset'         = in Volts
' 'coupling'       = 0     Ground (Averagers ONLY)
'                  = 1     DC, 1 MOhm
'                  = 2     AC, 1 MOhm
'                  = 3     DC, 50 Ohms
'                  = 4     AC, 50 Ohms
' 'bandwidth'      = 0     no bandwidth limit (default)
'                  = 1     bandwidth limit =  25 MHz
'                  = 2     bandwidth limit = 700 MHz
'                  = 3     bandwidth limit = 200 MHz
'                  = 4     bandwidth limit =  20 MHz
'                  = 5     bandwidth limit =  35 MHz
' NOTE: Not all bandwidth limits are available on a single instrument. In some, there is no
'       bandwidth limiting capability at all. In this case, use 'bandwidth' = 0.
'
' Returns one of the following ViStatus values:
' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
' VI_SUCCESS                         otherwise.

Public Declare Function errorMessage Lib "AqDrv4" _
Alias "_AcqrsD1_errorMessage@12" (ByVal instrumentID As Long, _
                                  ByVal errorCode As Long, _
                                  ByVal errorMessage As String) As Long
' Translates an error code into a human readable form
'
' 'errorCode'      = Error code (returned by a function) to be translated
' 'errorMessage'   = User-allocated character string (minimum size 256),
'                    into which the error-message text is returned
' The string 'errorMessage' must be pre-allocated as "dim errorMessage as String * 256"
'
' Returns one of the following ViStatus values:
' VI_SUCCESS always.

Public Declare Function errorMessageEx Lib "AqDrv4" _
Alias "_AcqrsD1_errorMessageEx@16" (ByVal instrumentID As Long, _
                                    ByVal errorCode As Long, _
                                    ByVal errorMessage As String, _
                                    errorMessageSize As Long) As Long
' Translates an error code into a human readable form. For file errors, the returned message 
' will also contain the file name and the original 'ansi' error string.
'
' 'errorCode'          = Error code (returned by a function) to be translated
' 'errorMessage'       = Pointer to user-allocated character string (suggested size 512 bytes),
'                        into which the error message text will be copied.
'                        The string 'errorMessage' must be pre-allocated as 
'                        "Dim errorMessage As String * 512"
' 'errorMessageSize'   = size of 'errorMessage', in bytes.
'
' NOTE: 'instrumentID' can be 0.
'
' Returns the following status values:
' ACQIRIS_ERROR_BUFFER_OVERFLOW    if 'errorMessageSize' is too small.
' VI_SUCCESS                       otherwise.

Public Declare Function ForceTrig Lib "AqDrv4" _
Alias "_AcqrsD1_forceTrig@4" (ByVal instrumentID As Long) As Long
' Forces a 'manual' trigger. The function returns immediately after initiating
' an acquisition. One must therefore wait until this acquisition has terminated
' before reading the data, by checking the status with the 'AcqDone'
' function. Equivalent to 'ForceTrigEx' with 'forceTrigType' = 0

Public Declare Function ForceTrigEx Lib "AqDrv4" _
Alias "_AcqrsD1_forceTrigEx@16" (ByVal instrumentID As Long, _
                                 ByVal forceTrigType As Long, _
                                 ByVal modifier As Long, _
                                 ByVal flags As Long) As Long
' Forces a 'manual' trigger. The function returns immediately after initiating
' a trigger. One must therefore wait until this acquisition has terminated
' before reading the data, by checking the status with the 'AcqrsD1_acqDone'
' or the 'WaitForEndOfAcquisition' functions.
'
' 'forceTrigType'=      0   Sends a software trigger to end the (entire) acquisition.
'                           In multisegment mode, the current segment is acquired, the acquisition is terminated
'                           and the data and timestamps of subsequent segments are invalid.
'                           The 'trigOut' Control IO will NOT generate a trigger output.
'                           Equivalent to 'ForceTrig'.
'                       1   Send a software trigger similar to a hardware trigger.
'                           In multisegment mode, the acquisition advances to the next segment and then waits again
'                           for a trigger. If no valid triggers are provided to the device, the application must call
'                           'ForceTrigEx' as many times as there are segments. In this mode, 'trigOut' Control IO will
'                           generate a trigger output on each successful call. Every acquired segment will be valid.
'                           This mode is only supported for single (i.e. non-ASBus-connected) instruments.
' 'modifier'                currently unused, must be zero
' 'flags'                   currently unused, must be zero
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_INSTRUMENT_STOPPED          if the instrument is already stopped. 
' ACQIRIS_ERROR_PRETRIGGER_STILL_RUNNING    if the requested data before trigger is being 
'                                           acquired.
' ACQIRIS_ERROR_NOT_SUPPORTED               if this function is not supported by the current 
'                                           mode (e.g. mode Average on APxxx).
' VI_ERROR_PARAMETERx                       if one of the parameters has an invalid value, where 
'                                           x = parameter number.
' VI_SUCCESS                                otherwise.

Public Declare Function FreeBank Lib "AqDrv4" _
Alias "AcqrsD1_freeBank@8" (ByVal instrumentID As Long, _
                            ByVal reserved As Long) As Long
' Free current bank during SAR acquisitions. Calling this function indicates to the driver that
' the current SAR bank has been read and can be reused for a new acquisition. 
' 
' 'reserved'         Unused, must be set to 0.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_NO_DATA                if there is no data available, no bank to unlock. 
' ACQIRIS_ERROR_SETUP_NOT_AVAILABLE    if the SAR mode is not available, or not activated.
' ACQIRIS_ERROR_INSTRUMENT_STOPPED     was not started using 'acquire' beforehand, or was stopped.
' VI_SUCCESS                           otherwise.

Public Declare Function GetAvgConfig Lib "AqDrv4" _
Alias "_AcqrsD1_getAvgConfig@16" (ByVal instrumentID As Long, _
                                  ByVal channel As Long, _
                                  ByVal parameterString As String, _
                                  value As Any) As Long
' Returns a parameter from the averager/analyzer configuration
' See remarks under 'ConfigAvgConfig'
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_PARAM_STRING_INVALID if 'parameterString' is invalid.
' ACQIRIS_ERROR_NOT_SUPPORTED        if this function is not supported by the  
'                                    instrument.
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetAvgConfigInt32 Lib "AqDrv4" _
Alias "_AcqrsD1_getAvgConfigInt32@16" (ByVal instrumentID As Long, _
                                       ByVal channel As Long, _
                                       ByVal parameterString As String, _
                                       value As Long) As Long
' Returns a parameter from the averager/analyzer configuration
' See remarks under 'AcqrsD1_configAvgConfigInt32'
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_PARAM_STRING_INVALID if 'parameterString' is invalid.
' ACQIRIS_ERROR_NOT_SUPPORTED        if this function is not supported by the  
'                                    instrument.
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetAvgConfigReal64 Lib "AqDrv4" _
Alias "_AcqrsD1_getAvgConfigReal64@16" (ByVal instrumentID As Long, _
                                        ByVal channel As Long, _
                                        ByVal parameterString As String, _
                                        value As Double) As Long
' Returns a parameter from the averager/analyzer configuration
' See remarks under 'AcqrsD1_configAvgConfigReal64'
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_PARAM_STRING_INVALID if 'parameterString' is invalid.
' ACQIRIS_ERROR_NOT_SUPPORTED        if this function is not supported by the  
'                                    instrument.
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetChannelCombination Lib "AqDrv4" _
Alias "_AcqrsD1_getChannelCombination@12" (ByVal instrumentID As Long, _
                                  nbrConvertersPerChannel As Long, _
                                  usedChannels As Long) As Long
' Returns parameters of combined operation of multiple channels
' See remarks under 'ConfigChannelCombination'
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetControlIO Lib "AqDrv4" _
Alias "_AcqrsD1_getControlIO@20" (ByVal instrumentID As Long, _
                                  ByVal connector As Long, _
                                  signal As Long, _
                                  qualifier1 As Long, _
                                  qualifier2 As Double) As Long
' Returns the state of Control-IO connectors
' See remarks under 'ConfigControlIO'
'
' SPECIAL CASE: If 'connector' = 0 is specified, the returned value of 'signal'
' is the bit-coded list of the 'connectors' which are available in the digitizer.
' E.g. If the connectors 1 (I/O A) and 9 (TrigOut) are present, the bits 1 and 9 of
' 'signal' are set, where bit 0 is the LSbit and 31 is the MSbit.
' Thus, 'signal' would be equal to 0x202.
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetExtClock Lib "AqDrv4" _
Alias "_AcqrsD1_getExtClock@24" (ByVal instrumentID As Long, _
                                 clockType As Long, _
                                 inputThreshold As Double, _
                                 delayNbrSamples As Long, _
                                 refFrequency As Double, _
                                 sampFrequency As Double) As Long
' Returns the (external) clock parameters of the digitizer
' See remarks under 'ConfigExtClock'
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetFCounter Lib "AqDrv4" _
Alias "_AcqrsD1_getFCounter@28" (ByVal instrumentID As Long, _
                                 signalChannel As Long, _
                                 typeMeas As Long, _
                                 targetValue As Double, _
                                 apertureTime As Double, _
                                 reserved As Double, _
                                 flags As Long) As Long
' Returns the current settings of the frequency counter
' See remarks under 'ConfigFCounter'
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetHorizontal Lib "AqDrv4" _
Alias "_AcqrsD1_getHorizontal@12" (ByVal instrumentID As Long, _
                                   sampInterval As Double, _
                                   delayTime As Double) As Long
' Returns the current horizontal control parameters of the digitizer.
' See remarks under 'ConfigHorizontal'
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetInstrumentData Lib "AqDrv4" _
Alias "_AcqrsD1_getInstrumentData@20" (ByVal instrumentID As Long, _
                                   ByVal name As String, _
                                   serialNbr As Long, _
                                   busNbr As Long, _
                                   slotNbr As Long) As Long
' Returns some basic data about a specified digitizer
'
' Values returned by the function:
'
' 'name'           = model name (length < 32 characters)
' 'serialNbr'      = serial number
' 'busNbr'         = bus number where digitizer is located
' 'slotNbr'        = slot number where digitizer is located
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetInstrumentInfo Lib "AqDrv4" _
Alias "_AcqrsD1_getInstrumentInfo@12" (ByVal instrumentID As Long, _
                                   ByVal parameterString As String, _
                                   infoValue As Any) As Long
' Returns general information about a specified digitizer
'
' The following value must be supplied to the function:
'
' 'parameterString'= character string specifying the requested parameter
'                    Please refer to the manual for the accepted parameter strings
'
' Value returned by the function:
'
' 'infoValue'      = value of requested parameter
'                    The type of the returned value depends on the parameter string
'                    Please refer to the manual for the required data type as a
'                    function of the accepted parameter strings:
'                    Data types marked as 'Vi32' in the manual should be 'Long'
'                    Data types marked as 'ViReal64' should be 'Double'
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_PARAM_STRING_INVALID if 'parameterString' is invalid.
' ACQIRIS_ERROR_NOT_SUPPORTED        if the requested value is not supported by the 
'                                    instrument.
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetMemory Lib "AqDrv4" _
Alias "_AcqrsD1_getMemory@12" (ByVal instrumentID As Long, _
                               nbrSamples As Long, _
                               nbrSegments As Long) As Long
' Returns the current memory control parameters of the digitizer.
' See remarks under 'ConfigMemory'
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetMemoryEx Lib "AqDrv4" _
Alias "_AcqrsD1_getMemoryEx@24" (ByVal instrumentID As Long, _
                                 nbrSamplesHi As Long, _
                                 nbrSamplesLo As Long, _
                                 nbrSegments As Long, _
                                 nbrBanks As Long, _
                                 flags As Long) As Long
' Returns the current memory control parameters of the digitizer.
' See remarks under 'AcqrsD1_configMemoryEx'
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetMode Lib "AqDrv4" _
Alias "_AcqrsD1_getMode@16" (ByVal instrumentID As Long, _
                             mode As Long, _
                             modifier As Long, _
                             flags As Long) As Long
' Returns the current operational mode of the digitizer.
' See remarks under 'ConfigMode'
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.


Public Declare Function GetMultiInput Lib "AqDrv4" _
Alias "_AcqrsD1_getMultiInput@12" (ByVal instrumentID As Long, _
                                   ByVal channel As Long, _
                                   inputs As Long) As Long
' Returns the multiple input configuration on a channel
' See remarks under 'ConfigMultiInput'
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetNbrChannels Lib "AqDrv4" _
Alias "_AcqrsD1_getNbrChannels@8" (ByVal instrumentID As Long, _
                                   nbrChannels As Long) As Long
' Returns the number of channels on the specified digitizer.
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetNbrPhysicalInstruments Lib "AqDrv4" _
Alias "_AcqrsD1_getNbrPhysicalInstruments@4" (nbrInstruments As Long) As Long
' Returns the number of supported physical Acqiris digitizers found on the computer.
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETER1                if 'nbrInstruments' is NULL.
' VI_SUCCESS                         otherwise.

Public Declare Function GetSetupArray Lib "AqDrv4" _
Alias "_AcqrsD1_getSetupArray@24" (ByVal instrumentID As Long, _
                                   ByVal channel As Long, _
                                   ByVal setupType As Long, _
                                   ByVal nbrSetupObj As Long, _
                                   setupData As Any, _
                                   nbrSetupObjReturned As Long) As Long
' Returns setup data array (typically used for on-board processing)
' See remarks under 'ConfigSetupArray'
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_NOT_SUPPORTED        if this function is not supported by the instrument.
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetTrigClass Lib "AqDrv4" _
Alias "_AcqrsD1_getTrigClass@28" (ByVal instrumentID As Long, _
                                  trigClass As Long, _
                                  sourcePattern As Long, _
                                  validatePattern As Long, _
                                  holdType As Long, _
                                  holdValue1 As Double, _
                                  holdValue2 As Double) As Long
' Returns the current trigger class control parameters of the digitizer.
' See remarks under 'ConfigTrigClass'
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetTrigSource Lib "AqDrv4" _
Alias "_AcqrsD1_getTrigSource@24" (ByVal instrumentID As Long, _
                                   ByVal channel As Long, _
                                   trigCoupling As Long, _
                                   trigSlope As Long, _
                                   trigLevel1 As Double, _
                                   trigLevel2 As Double) As Long
' Returns the current trigger source control parameters for a specified channel.
' See remarks under 'ConfigTrigSource'
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetTrigTV Lib "AqDrv4" _
Alias "_AcqrsD1_getTrigTV@20" (ByVal instrumentID As Long, _
                               ByVal channel As Long, _
                               standard As Long, _
                               field As Long, _
                               line As Long) As Long
' Returns the current TV trigger control parameters of the digitizer.
' See remarks under 'ConfigTrigTV'
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function GetVersion Lib "AqDrv4" _
Alias "_AcqrsD1_getVersion@12" (ByVal instrumentID As Long, _
                                ByVal versionItem As Long, _
                                version As Long) As Long
' Returns version numbers associated with a specified digitizer / current device driver
'
' The following value must be supplied to the function:
'
' 'versionItem'    =   1       for version of Kernel-Mode Driver
'                      2       for version of EEPROM Common Section
'                      3       for version of EEPROM Digitizer Section
'                      4       for version of CPLD firmware
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

Public Declare Function GetVertical Lib "AqDrv4" _
Alias "_AcqrsD1_getVertical@24" (ByVal instrumentID As Long, _
                                 ByVal channel As Long, _
                                 fullScale As Double, _
                                 offset As Double, _
                                 coupling As Long, _
                                 bandwidth As Long) As Long
' Returns the current vertical control parameters for a specified channel.
' See remarks under 'ConfigVertical'
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    x = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function Init Lib "AqDrv4" _
Alias "_AcqrsD1_init@16" (ByVal resourceName As String, _
                          ByVal IDQuery As Boolean, _
                          ByVal resetDevice As Boolean, _
                          instrumentID As Long) As Long
' Initializes an instrument
' See remarks under 'InitWithOptions'

Public Declare Function InitWithOptions Lib "AqDrv4" _
Alias "_AcqrsD1_InitWithOptions@20" (ByVal resourceName As String, _
                                     ByVal IDQuery As Boolean, _
                                     ByVal resetDevice As Boolean, _
                                     ByVal optionsString As String, _
                                     instrumentID As Long) As Long
' Initializes an instrument with options
'
' The following values must be supplied to the function:
' 'resourceName'   = an ASCII string which identifies the digitizer to be initialized
'                    See manual for a detailed description of this string.
' 'IDQuery'        = currently ignored
' 'resetDevice'    = if set to 'TRUE', resets the digitizer after initialization
' 'optionsString'  = an ASCII string which specifies options. Currently, we support
'                    "CAL=False" to suppress calibration during the initialization
'                    "DMA=False" to inhibit (for diagnostics) the use of scatter-gather DMA for data transfers
'                    "Simulate=True" for the use of simulated digitizers during application development.
'
' Values returned by the function:
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

Public Declare Function LogicDeviceIO Lib "AqDrv4" _
Alias "_AcqrsD1_logicDeviceIO@28" (ByVal instrumentID As Long, _
                                   ByVal deviceName As String, _
                                   ByVal registerID As Long, _
                                   ByVal nbrValues As Long, _
                                   dataArray As Long, _
                                   ByVal readWrite As Long, _
                                   ByVal modifier As Long) As Long
' Currently ONLY for APxxx, SCxxx and ACxxx!
'
' Reads/writes a number of 32-bit data values from/to a user-defined register in the
' logic device identified by 'deviceName[]'.
' ONLY useful for a digitizer with on-board programmable logic devices.
'
' The following values must be supplied to the function:
'
' 'deviceName'     = an ASCII string which identifies the device
'                    Must be of the form "BlockMDevN", where M = 1..4 and N = 1..number
'                    of logical devices in the device block M.
'                    In the RC200, this string must be "Block1Dev1"
'                    See manual for a detailed description of this string.
' 'registerID'     = 0..to (nbrRegisters-1)
' 'nbrValues'      = number of 32-bit values to be transferred
' 'dataArray'      = user-supplied buffer for data values
' 'readWrite'      = 0 for reading (from logic device to application)
'                    1 for writing (from application to logic device)
' 'flags'            currently ignored, use '0'
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

Public Declare Function MultiInstrAutoDefine Lib "AqDrv4" _
Alias "_AcqrsD1_multiInstrAutoDefine@8" (ByVal optionsString As String, _
                                         nbrInstruments As Long) As Long
' Automatically combines as many digitizers as possible to "MultiInstrument"s.
' Digitizers are only combined if they are physically connected via ASBus.
' This call must be followed by 'nbrInstruments' calls to 'Init' or
' 'InitWithOptions' to retrieve the 'instrumentID's of the (multi)digitizers.
'
' The following value must be supplied to the function:
'
' 'optionsString'  = an ASCII string which specifies options.
'                    Currently, no options are supported
'
' Values returned by the function:
'
' 'nbrInstruments' = number of user-accessible instruments. This number includes
'                    also single instruments that don't participate on the ASBus.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_EEPROM_DATA_INVALID  if one of the instruments being initialized has invalid 
'                                    data in EEPROM.
' ACQIRIS_ERROR_UNSUPPORTED_DEVICE   if one of the instruments being initialized is not 
'                                    supported by this driver version.
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    'x' = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function MultiInstrDefine Lib "AqDrv4" _
Alias "_AcqrsD1_multiInstrDefine@16" (instrumentList As Long, _
                                      ByVal nbrInstruments As Long, _
                                      ByVal masterID As Long, _
                                      instrumentID As Long) As Long
' 'Manually' combines a number of digitizers into a single "MultiInstrument"
'
' The following values must be supplied to the function:
'
' 'instrumentList' = array of 'instrumentID' of already initialized single digitizers
' 'nbrInstruments' = number of digitizers in the 'instrumentList'
' 'masterID'       = 'instrumentID' of master digitizer
'
' Values returned by the function:
'
' 'instrumentID'   = identifier of the new "MultiInstrument", for subsequent use
' in the other function calls.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_NOT_SUPPORTED        if this function is not supported by the instrument(s).
' ACQIRIS_ERROR_NOT_ENOUGH_DEVICES   if 'nbrInstruments' is < 2.
' ACQIRIS_ERROR_TOO_MANY_DEVICES     if 'nbrInstruments' exceeds the maximum number 
'                                    of ASBUS instruments.
' ACQIRIS_ERROR_INSTRUMENT_NOT_FOUND if one of the 'instrumentList[]' entries is invalid.
' ACQIRIS_ERROR_NO_MASTER_DEVICE     if 'masterID' is invalid.
' ACQIRIS_ERROR_SETUP_NOT_AVAILABLE  if one of the 'instrumentList[]' entries is not ASBUS 
'                                    compatible.
' ACQIRIS_ERROR_UNSUPPORTED_DEVICE   if one of the 'instrumentList[]' entries is not supported 
'                                    by this driver version.
' ACQIRIS_ERROR_INTERNAL_DEVICENO_INVALID if one of the 'instrumentList[]' entries is duplicated. 
' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
'                                    'x' = parameter number.
' VI_SUCCESS                         otherwise.

Public Declare Function MultiInstrUndefineAll Lib "AqDrv4" _
Alias "_AcqrsD1_multiInstrUndefineAll@4" (ByVal optionsString As String) As Long
' Undefines all "MultiInstruments".
'
' The following value must be supplied to the function:
'
' 'optionsString'  = an ASCII string which specifies options.
'                    Currently, no options are supported
'
' Please refer to the User's manual for a detailed description of the steps required
' to reestablish the identifiers of the the existing individual digitizers, in order
' to continue using them.
'
' Returns one of the following ViStatus values:
' VI_SUCCESS always.

Public Declare Function ProcessData Lib "AqDrv4" _
Alias "_AcqrsD1_processData@12" (ByVal instrumentID As Long, _
                                 ByVal processType As Long, _
                                 ByVal flags As Long) As Long
' Starts data processing on acquired data (only in instruments with on-board data processing)
'
' The following values must be supplied to the function:
'
' 'processType'    =   0       for no processing
'                      1       for extrema mode
'                      2       for hysteresis mode
'                      3       for interpolated extrema mode
'                      4       for interpolated hysteresis mode
'
' defines how switching for the dual-bank memory is done
'
' 'flags'          =   0       switching is done by software
'                      1       switching is automatic, auto switch turned on
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_NOT_SUPPORTED      if this function is not supported by the instrument.
' VI_SUCCESS                       otherwise.

Public Declare Function ProcDone Lib "AqDrv4" _
Alias "_AcqrsD1_procDone@8" (ByVal instrumentID As Long, done As Boolean) As Long
' Checks if the on-board processing has terminated
'
' Returns 'done' = TRUE if the processing is terminated
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_NOT_SUPPORTED      if this function is not supported by the instrument.
' ACQIRIS_ERROR_IO_READ            if a link error has been detected (e.g. PCI link lost).
' VI_ERROR_PARAMETERx              if one of the parameters has an invalid value, where 
'                                  'x' = parameter number.
' VI_SUCCESS                       otherwise.

Public Declare Function ReadCharSequence Lib "AqDrv4" _
Alias "_AcqrsD1_readCharSequence@60" (ByVal instrumentID As Long, _
                                      ByVal channel As Long, _
                                      ByVal firstSegment As Long, _
                                      ByVal nbrSegments As Long, _
                                      ByVal firstSampleInSeg As Long, _
                                      ByVal nbrSamplesInSeg As Long, _
                                      ByVal segmentOffset As Long, _
                                      ByVal arraySize As Long, _
                                      waveformArray As Byte, _
                                      horPos As Double, _
                                      sampTime As Double, _
                                      vGain As Double, _
                                      vOffset As Double, _
                                      timeStampLo As Long, _
                                      timeStampHi As Long) As Long
' Returns a sequence waveform as an array of byte arrays.
' This function is faster than a loop over 'ReadCharWform' if many
' short segments (< 10'000 points/segment) are read. See Manual for details.
'
' The following values must be supplied to the function:
'
' 'channel'        = 1...Nchan (as returned by 'GetNbrChannels' )
' 'firstSegment'   = requested first segment number, may assume 0 to 'nbrSegments - 1',
'                    as set with the function 'ConfigMemory'
' 'nbrSegments'    = requested number of segments, may assume 1 to the number of
'                    segments set with the function 'ConfigMemory'
' 'firstSampleInSeg' = requested position of first sample in segment to read,
'                    typically 0. May assume 0 to 'nbrSamples - 1', as set with the
'                    function 'ConfigMemory'
' 'nbrSamplesInSeg'  = requested number of samples per segment, may assume 1 to the
'                    number of samples set with the function 'ConfigMemory'
' 'segmentOffset'  = requested offset (in samples!) between adjacent segments in the
'                    destination buffer 'waveformArray'. Must be >= 'nbrSamplesInSeg'.
' 'arraySize'      = number of data elements in the user-allocated 'waveformArray'
'                    Refer to function description in the manual for the required
'                    array size!
'
' Values returned by the function:
'
' 'waveformArray'  = user-allocated waveform destination array of type Byte.
'                    Refer to function description in the manual for the required
'                    array size!
' 'horPos'         = user-allocated array of horizontal positions, one per segment,
'                    of first (nominal) data point with respect to the origin of the 'nominal
'                    trigger delay' in seconds. Since the first data point is BEFORE the
'                    origin, these numbers will be in the range [-sampTime, 0].
'                    The number of elements in the 'horPos' array MUST be >= the number
'                    of segments set with the function 'ConfigMemory'.
' 'sampTime'       = sampling interval in seconds
' 'vGain'          = vertical gain in Volts/LSB
' 'vOffset'        = vertical offset in Volts
'                    The value in Volts of a data point 'data' in the returned
'                    'waveformArray' array is computed with the formula
'                    V = vGain * data - vOffset
' 'timeStampLo' +
' 'timeStampHi'    = user-allocated arrays of low and high part of the 64-bit trigger time stamps,
'                    one per segment, in units of picoseconds. These values are the trigger
'                    times with respect to an arbitrary time origin (typically the start-time
'                    of the acquisition).
'                    The number of elements in the 'timeStampXx' arrays MUST be >= the
'                    number of segments set with the function 'ConfigMemory'.
'                    Intended for the computation of time differences between two waveform
'                    acquisitions. See manual for detailed explanations.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_NOT_SUPPORTED          if this function is not supported by the instrument.
' ACQIRIS_ERROR_INSTRUMENT_RUNNING     if the instrument is running.
' ACQIRIS_ERROR_NO_DATA                if nothing was acquired beforehand.
' VI_ERROR_PARAMETERx                  if one of the parameters has an invalid value, where 
'                                      'x' = parameter number.
' VI_SUCCESS                           otherwise.
' For other errors, please see 'ReadData'.

Public Declare Function ReadCharWform Lib "AqDrv4" _
Alias "_AcqrsD1_readCharWform@56" (ByVal instrumentID As Long, _
                              ByVal channel As Long, _
                              ByVal segmentNumber As Long, _
                              ByVal firstSample As Long, _
                              ByVal nbrSamples As Long, _
                              waveformArray As Byte, _
                              returnedSamples As Long, _
                              addrFirstPoint As Long, _
                              horPos As Double, _
                              sampTime As Double, _
                              vGain As Double, _
                              vOffset As Double, _
                              timeStampLo As Long, _
                              timeStampHi As Long) As Long
' Returns a waveform as a byte array.
'
' The following values must be supplied to the function:
'
' 'channel'        = 1...Nchan (as returned by 'GetNbrChannels' )
' 'segmentNumber'  = requested segment number, may assume 0 to 'nbrSegments - 1',
'                    as set with the function 'ConfigMemory'
' 'firstSample'    = requested position of first sample to read, typically 0
' 'nbrSamples'     = requested number of samples, may assume 1 to 'nbrSamples', as
'                    set with the function 'ConfigMemory'
'
' Values returned by the function:
'
' 'waveformArray'  = user-allocated waveform destination array for returned data
'                    Its size MUST be at least 'nbrSamples' + 32, for reasons of
'                    data alignment.
' 'returnedSamples'= number of data samples actually returned
' 'addrFirstPoint' = the offset of the first valid data point in the destination array.
'                    It should always be in the range [0...31].
' 'horPos'         = horizontal position of first (nominal) data point with respect
'                    to the origin of the 'nominal trigger delay' in seconds. Since
'                    the first data point is BEFORE the origin, this number will be
'                    in the range [-sampTime, 0].
' 'sampTime'       = sampling interval in seconds
' 'vGain'          = vertical gain in Volts/LSB
' 'vOffset'        = vertical offset in Volts
'                    The value in Volts of a data point 'data' in the returned
'                    'waveformArray' array is computed with the formula
'                    V = vGain * data - vOffset
' 'timeStampLo' +
' 'timeStampHi'    = low and high part of the 64-bit trigger time stamp, in units of
'                    picoseconds. This value is the trigger time with respect to an
'                    arbitrary time origin (typically the start-up time of the device).
'                    Intended for the computation of time differences between two waveform
'                    acquisitions. See manual for detailed explanations.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_NOT_SUPPORTED          if this function is not supported by the instrument.
' ACQIRIS_ERROR_INSTRUMENT_RUNNING     if the instrument is running.
' ACQIRIS_ERROR_NO_DATA                if nothing was acquired beforehand.
' VI_ERROR_PARAMETERx                  if one of the parameters has an invalid value, where 
'                                      'x' = parameter number.
' VI_SUCCESS                           otherwise.
' For other errors, please see 'readData'.

Public Declare Function ReadData Lib "AqDrv4" _
Alias "_AcqrsD1_readData@24" (ByVal instrumentID As Long, _
                              ByVal channel As Long, _
                              readPar As AqReadParameters, _
                              dataArray As Any, _
                              dataDesc As AqDataDescriptor, _
                              segDescArray As Any) As Long
' Returns a waveform and associated descriptors, in one of several possible formats
' This function is the preferred way of reading waveforms, since it includes the capabilities
' of all the other read functions, and more!
'
' The following values must be supplied to the function:
'
' 'channel'        = 1...Nchan (as returned by 'GetNbrChannels' )
' 'readPar'        = a user-supplied structure that specifies what and how to read
'
' Values returned by the function:
'
' 'dataArray'      = user-allocated waveform destination array of type defined by 'readPar.dataType'
'                    When reading a single segment of raw data, its size MUST be at least
'                    'nbrSamples' + 32, for reasons of data alignment. Please refer to the manual
'                    for additional details.
' 'dataDesc'       = user-allocated structure for returned data descriptor values
' 'segDescArray'   = user allocated array of structures for returned segment descriptor values
'                    This array must contain at least 'readPar.nbrSegments' elements of the
'                    appropriate type (typically 'AqSegmentDescriptor' or 'AqSegmentDescriptorAvg')
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_INSTRUMENT_RUNNING       if the instrument is running.
' ACQIRIS_ERROR_READMODE                 if 'readPar.readMode' is not valid.
' ACQIRIS_ERROR_NOT_SUPPORTED            if 'readPar.readMode' is not supported by the instrument.
' ACQIRIS_ERROR_DATATYPE                 if 'readPar.dataType' is not valid for the 
'                                        chosen 'readPar.readMode' or for this instrument.
' ACQIRIS_ERROR_FIRST_SEG                if 'readPar.firstSegment' is invalid.
' ACQIRIS_ERROR_NBR_SEG                  if 'readPar.nbrSegments' is invalid.
' ACQIRIS_ERROR_DATA_ARRAY or
' ACQIRIS_ERROR_NBR_SAMPLE               if 'readPar.dataArraySize' is invalid.
' ACQIRIS_ERROR_SEG_DESC_ARRAY           if 'readPar.segDescArraySize' is invalid.
' ACQIRIS_ERROR_SEG_OFF                  if 'readPar.segmentOffset' is invalid.
' ACQIRIS_ERROR_NBR_SEG                  if 'readPar.nbrSegments' is invalid.
' ACQIRIS_ERROR_BUFFER_OVERFLOW          if 'readPar.dataArraySize' is too small.
' ACQIRIS_ERROR_NO_DATA                  if nothing was acquired beforehand.
' ACQIRIS_ERROR_CANNOT_READ_THIS_CHANNEL if the requested channel is not available.
' ACQIRIS_ERROR_READ_TIMEOUT             if the reading encountered a problem.
' ACQIRIS_WARN_READPARA_NBRSEG_ADAPTED   if 'readPar.nbrSegments' has been adapted.
' ACQIRIS_WARN_ACTUAL_DATASIZE_ADAPTED   if 'readPar.dataArraySize' has been adapted.
' ACQIRIS_WARN_READPARA_FLAGS_ADAPTED    if 'readPar.flags' has been adapted.
' VI_ERROR_PARAMETERx                    if one of the parameters has an invalid value, where 
'                                        'x' = parameter number.
' VI_SUCCESS                             otherwise.

Public Declare Function ReadFCounter Lib "AqDrv4" _
Alias "_AcqrsD1_readFCounter@8" (ByVal instrumentID As Long, _
                                 result As Double) As Long
' Reads the frequency counter
' 'result'      = result of measurement, whose units depend on the measurement 'type':
'                 Hz     for type = 0 (Frequency)
'                 sec    for type = 1 (Period)
'                 counts for type = 2 (Totalize)
'
' Common return values:
' ACQIRIS_ERROR_NOT_SUPPORTED if this function is not supported by the instrument.
' ACQIRIS_ERROR_NO_DATA       if the instrument has not first acquired data in the 'frequency 
'                             counter' mode (mode = 6).
' VI_ERROR_PARAMETERx         if one of the parameters has an invalid value, where 
'                             'x' = parameter number.
' VI_SUCCESS                  otherwise.

Public Declare Function ReadRealSequence Lib "AqDrv4" _
Alias "_AcqrsD1_readRealSequence@52" (ByVal instrumentID As Long, _
                                      ByVal channel As Long, _
                                      ByVal firstSegment As Long, _
                                      ByVal nbrSegments As Long, _
                                      ByVal firstSampleInSeg As Long, _
                                      ByVal nbrSamplesInSeg As Long, _
                                      ByVal segmentOffset As Long, _
                                      ByVal arraySize As Long, _
                                      waveformArray As Double, _
                                      horPos As Double, _
                                      sampTime As Double, _
                                      timeStampLo As Long, _
                                      timeStampHi As Long) As Long
' Returns a sequence waveform as an array of floating point (double) arrays, with the
' measured data values in Volts.
' This function is faster than a loop over 'ReadRealWform' if many
' short segments (< 10'000 points/segment) are read.
'
' See remarks under 'ReadCharSequence'

Public Declare Function ReadRealWform Lib "AqDrv4" _
Alias "_AcqrsD1_readRealWform@44" (ByVal instrumentID As Long, _
                                   ByVal channel As Long, _
                                   ByVal segmentNumber As Long, _
                                   ByVal firstSample As Long, _
                                   ByVal nbrSamples As Long, _
                                   waveformArray As Double, _
                                   returnedSamples As Long, _
                                   horPos As Double, _
                                   sampTime As Double, _
                                   timeStampLo As Long, _
                                   timeStampHi As Long) As Long
' Returns a waveform as a floating point (double) array, with the measured data values in Volts.
' See remarks under 'ReadCharWform'

Public Declare Function ReportNbrAcquiredSegments Lib "AqDrv4" _
Alias "_AcqrsD1_reportNbrAcquiredSegments@8" (ByVal instrumentID As Long, _
                                              nbrSegments As Long) As Long
' Returns the number of segments already acquired.
' Can be called during the acquisition period, in order to follow the progress of a
' Sequence acquisition.
' Can be called after an acquisition, in order to obtain the number of segments actually
' acquired (until 'StopAcquisition' was called).
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                  if one of the parameters has an invalid value, where 
'                                      'x' = parameter number.
' VI_SUCCESS                           otherwise.

Public Declare Function Reset Lib "AqDrv4" _
Alias "_AcqrsD1_reset@4" (ByVal instrumentID As Long) As Long
' Resets an instrument
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETER1                     if 'instrumentID' is invalid.
' VI_SUCCESS                              otherwise.

Public Declare Function ResetDigitizerMemory Lib "AqDrv4" _
Alias "_AcqrsD1_resetDigitizerMemory@4" (ByVal instrumentID As Long) As Long
' Resets the digitizer memory to a known default state, ONLY useful
' for a digitizer with the battery back-up option.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_INSTRUMENT_RUNNING    if the instrument is already running. 
' VI_ERROR_PARAMETER1                 if 'instrumentID' is invalid.
' VI_SUCCESS                          otherwise.

Public Declare Function RestoreInternalRegisters Lib "AqDrv4" _
Alias "_AcqrsD1_restoreInternalRegisters@20" (ByVal instrumentID As Long, _
                                              ByVal delayOffset As Double, _
                                              ByVal delayScale As Double) As Long
' Restores some internal registers of an instrument, needed ONLY after power-up
' of a digitizer with the battery back-up option.
' Please refer to the User's manual for a detailed description of the steps required
' to read battery backed-up waveforms.
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                  if one of the parameters has an invalid value, where 
'                                      'x' = parameter number.
' VI_SUCCESS                           otherwise.

Public Declare Function SetAttributeString Lib "AqDrv4" _
Alias "AcqrsD1_setAttributeString@16" (ByVal instrumentID As Long, _
                                       ByVal channel As Long, _
                                       ByVal name As String, _
                                       ByVal value As String) As Long
' Set through 'value' the value of the attribute named 'name'.
'
' 'channel'            = 1...Nchan (as returned by 'AcqrsD1_getNbrChannels' ) or
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

Public Declare Function SetLEDColor Lib "AqDrv4" _
Alias "_AcqrsD1_setLEDColor@8" (ByVal instrumentID As Long, _
                                ByVal color As Long) As Long
' Sets the front-panel LED to the desired color
' 'color' = 0      OFF (returns to normal 'acquisition status' indicator)
'           1      Green
'           2      Red
'           3      Yellow
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETERx                  if one of the parameters has an invalid value, where 
'                                      'x' = parameter number.
' VI_SUCCESS                           otherwise.

Public Declare Function SetSimulationOptions Lib "AqDrv4" _
Alias "_AcqrsD1_setSimulationOptions@4" (ByVal simOptionString As String) As Long
' Sets one or several options which will be used by the function 'InitWithOptions',
' provided that the 'optionString' supplied to 'InitWithOptions' contains the
' string 'simulate=TRUE' (or similar).
' Refer to the manual for the accepted form of 'simOptionString'.
' The simulation options are reset to none by setting 'simOptionString' to an empty string "".
'
' Returns one of the following ViStatus values:
' VI_SUCCESS always.

Public Declare Function StopAcquisition Lib "AqDrv4" _
Alias "_AcqrsD1_stopAcquisition@4" (ByVal instrumentID As Long) As Long
' Stops the acquisition immediately
'
' Returns one of the following ViStatus values:
' VI_ERROR_PARAMETER1                  if 'instrumentID' is invalid.
' VI_SUCCESS                           otherwise.

Public Declare Function StopProcessing Lib "AqDrv4" _
Alias "_AcqrsD1_stopProcessing@4" (ByVal instrumentID As Long) As Long
' Stops the on-board processing immediately (only in instruments with on-board data processing)
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_NOT_SUPPORTED      if this function is not supported by the instrument.
' VI_ERROR_PARAMETERx              if one of the parameters has an invalid value, where 
'                                  'x' = parameter number.
' VI_SUCCESS                       otherwise.

Public Declare Function WaitForEndOfAcquisition Lib "AqDrv4" _
Alias "_AcqrsD1_waitForEndOfAcquisition@8" (ByVal instrumentID As Long, _
                                            ByVal timeout As Long) As Long
' Returns after acquisition has terminated or after timeout, whichever is shorter.
' 'timeout' is in milliseconds. For protection, 'timeout' is internally clipped to a
' range of [0, 10000] milliseconds.
'
' This function puts the calling thread into 'idle' until it returns, permitting optimal
' use of the CPU by other threads.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_OVERLOAD           if a channel/trigger overload was detected.
' ACQIRIS_ERROR_ACQ_TIMEOUT        if the acquisition timed out (and there was no overload). 
'                                  In this case, you should use either 
'                                  'AcqrsD1_stopAcquisition()' or 'AcqrsD1_forceTrig()' to 
'                                  stop the acquisition.
' ACQIRIS_ERROR_IO_READ            if a link error has been detected (e.g. PCI link lost).
' ACQIRIS_ERROR_INSTRUMENT_STOPPED if the acquisition was not started beforehand
' VI_ERROR_PARAMETERx              if one of the parameters has an invalid value, where 
'                                  'x' = parameter number.
' VI_SUCCESS                       otherwise.

Public Declare Function WaitForEndOfProcessing Lib "AqDrv4" _
Alias "_AcqrsD1_waitForEndOfProcessing@8" (ByVal instrumentID As Long, _
                                           ByVal timeout As Long) As Long
' Returns after on-board processing has terminated or after timeout, whichever is shorter.
' 'timeout' is in milliseconds. For protection, 'timeout' is internally clipped to a
' range of [0, 10000] milliseconds. (only in instruments with on-board data processing)
'
' This function puts the calling thread into 'idle' until it returns, permitting optimal
' use of the CPU by other threads.
'
' Returns one of the following ViStatus values:
' ACQIRIS_ERROR_PROC_TIMEOUT        if the processing timed out. In this case, you should use 
'                                   'AcqrsD1_stopProcessing()' to stop the processing.
' ACQIRIS_ERROR_IO_READ             if a link error has been detected (e.g. PCI link lost).
' VI_ERROR_PARAMETERx               if one of the parameters has an invalid value, where 
'                                   'x' = parameter number.
' VI_SUCCESS                        otherwise.

