Option Explicit On 

Public Module AcqrsD1Interface_x64
    '/////////////////////////////////////////////////////////////////////////////////////////
    '
    '  AcqrsD1Interface_x64.vb : Declaration of Acqiris device driver API 
    '                            for Visual Basic Dot Net x64
    '----------------------------------------------------------------------------------------
    '  Copyright (C) 2009, Agilent. All rights reserved.
    '
    '/////////////////////////////////////////////////////////////////////////////////////////
    '  $Rev: 37088 $                  $Date: 2009-11-09 15:24:53 +0100 (Mon, 09 Nov 2009) $

    '/////////////////////////////////////////////////////////////////////////////////////////
    ' Define Instrument Specific Error Codes
    '/////////////////////////////////////////////////////////////////////////////////////////

    Public Const VI_SUCCESS As Int32 = 0
    Public Const VI_ERROR As Int32 = &H80000000

    Public Const VI_ERROR_PARAMETER1 As Int32 = VI_ERROR + &H3FFC0001
    Public Const VI_ERROR_PARAMETER2 As Int32 = VI_ERROR + &H3FFC0002
    Public Const VI_ERROR_PARAMETER3 As Int32 = VI_ERROR + &H3FFC0003
    Public Const VI_ERROR_PARAMETER4 As Int32 = VI_ERROR + &H3FFC0004
    Public Const VI_ERROR_PARAMETER5 As Int32 = VI_ERROR + &H3FFC0005
    Public Const VI_ERROR_PARAMETER6 As Int32 = VI_ERROR + &H3FFC0006
    Public Const VI_ERROR_PARAMETER7 As Int32 = VI_ERROR + &H3FFC0007
    Public Const VI_ERROR_PARAMETER8 As Int32 = VI_ERROR + &H3FFC0008

    Public Const ACQIRIS_ERROR As Int32 = &HBFFA4000
    Public Const ACQIRIS_WARNING As Int32 = &H3FFA4000

    Public Const ACQIRIS_MAX_SPECIFIC_ERROR_CODE As Int32 = ACQIRIS_ERROR + &HFFF
    Public Const ACQIRIS_MAX_SPECIFIC_WARN_CODE As int32 = ACQIRIS_WARNING + &HFFF

    Public Const ACQIRIS_ERROR_FILE_NOT_FOUND As Int32 = ACQIRIS_ERROR + &H800
    Public Const ACQIRIS_ERROR_PATH_NOT_FOUND As Int32 = ACQIRIS_ERROR + &H801
    Public Const ACQIRIS_ERROR_INVALID_CHARS_IN_PATH As Int32 = ACQIRIS_ERROR + &H802
    Public Const ACQIRIS_ERROR_INVALID_HANDLE As Int32 = ACQIRIS_ERROR + &H803
    Public Const ACQIRIS_ERROR_NOT_SUPPORTED As Int32 = ACQIRIS_ERROR + &H805
    Public Const ACQIRIS_ERROR_INVALID_WINDOWS_PARAM As Int32 = ACQIRIS_ERROR + &H806
    Public Const ACQIRIS_ERROR_NO_DATA As Int32 = ACQIRIS_ERROR + &H807
    Public Const ACQIRIS_ERROR_NO_ACCESS As Int32 = ACQIRIS_ERROR + &H808
    Public Const ACQIRIS_ERROR_BUFFER_OVERFLOW As Int32 = ACQIRIS_ERROR + &H809
    Public Const ACQIRIS_ERROR_BUFFER_NOT_64BITS_ALIGNED As Int32 = ACQIRIS_ERROR + &H80A
    Public Const ACQIRIS_ERROR_BUFFER_NOT_32BITS_ALIGNED As Int32 = ACQIRIS_ERROR + &H80B
    Public Const ACQIRIS_ERROR_CAL_FILE_CORRUPTED As Int32 = ACQIRIS_ERROR + &H80C
    Public Const ACQIRIS_ERROR_CAL_FILE_VERSION As Int32 = ACQIRIS_ERROR + &H80D
    Public Const ACQIRIS_ERROR_CAL_FILE_SERIAL As Int32 = ACQIRIS_ERROR + &H80E

    Public Const ACQIRIS_ERROR_ALREADY_OPEN As Int32 = ACQIRIS_ERROR + &H840
    Public Const ACQIRIS_ERROR_SETUP_NOT_AVAILABLE As Int32 = ACQIRIS_ERROR + &H880

    Public Const ACQIRIS_ERROR_IO_WRITE As Int32 = ACQIRIS_ERROR + &H8A0
    Public Const ACQIRIS_ERROR_IO_READ As Int32 = ACQIRIS_ERROR + &H8A1
    Public Const ACQIRIS_ERROR_IO_DEVICE_OFF As Int32 = ACQIRIS_ERROR + &H8A2
    Public Const ACQIRIS_ERROR_IO_VME_CONFIG As Int32 = ACQIRIS_ERROR + &H8A3
    Public Const ACQIRIS_ERROR_IO_VME_ACCESS As Int32 = ACQIRIS_ERROR + &H8A4

    Public Const ACQIRIS_ERROR_INTERNAL_DEVICENO_INVALID As Int32 = ACQIRIS_ERROR + &H8C0
    Public Const ACQIRIS_ERROR_TOO_MANY_DEVICES As Int32 = ACQIRIS_ERROR + &H8C1
    Public Const ACQIRIS_ERROR_EEPROM_DATA_INVALID As Int32 = ACQIRIS_ERROR + &H8C2
    Public Const ACQIRIS_ERROR_INIT_STRING_INVALID As Int32 = ACQIRIS_ERROR + &H8C3
    Public Const ACQIRIS_ERROR_INSTRUMENT_NOT_FOUND As Int32 = ACQIRIS_ERROR + &H8C4
    Public Const ACQIRIS_ERROR_INSTRUMENT_RUNNING As Int32 = ACQIRIS_ERROR + &H8C5
    Public Const ACQIRIS_ERROR_INSTRUMENT_STOPPED As Int32 = ACQIRIS_ERROR + &H8C6
    Public Const ACQIRIS_ERROR_MODULES_NOT_ON_SAME_BUS As Int32 = ACQIRIS_ERROR + &H8C7
    Public Const ACQIRIS_ERROR_NOT_ENOUGH_DEVICES As Int32 = ACQIRIS_ERROR + &H8C8
    Public Const ACQIRIS_ERROR_NO_MASTER_DEVICE As Int32 = ACQIRIS_ERROR + &H8C9
    Public Const ACQIRIS_ERROR_PARAM_STRING_INVALID As Int32 = ACQIRIS_ERROR + &H8CA
    Public Const ACQIRIS_ERROR_COULD_NOT_CALIBRATE As Int32 = ACQIRIS_ERROR + &H8CB
    Public Const ACQIRIS_ERROR_CANNOT_READ_THIS_CHANNEL As Int32 = ACQIRIS_ERROR + &H8CC
    Public Const ACQIRIS_ERROR_PRETRIGGER_STILL_RUNNING As Int32 = ACQIRIS_ERROR + &H8CD
    Public Const ACQIRIS_ERROR_CALIBRATION_FAILED As Int32 = ACQIRIS_ERROR + &H8CE
    Public Const ACQIRIS_ERROR_MODULES_NOT_CONTIGUOUS As Int32 = ACQIRIS_ERROR + &H8CF
    Public Const ACQIRIS_ERROR_INSTRUMENT_ACQ_LOCKED As Int32 = ACQIRIS_ERROR + &H8D0
    Public Const ACQIRIS_ERROR_INSTRUMENT_ACQ_NOT_LOCKED As Int32 = ACQIRIS_ERROR + &H8D1
    Public Const ACQIRIS_ERROR_EEPROM2_DATA_INVALID As Int32 = ACQIRIS_ERROR + &H8D2
    Public Const ACQIRIS_ERROR_INSTRUMENT_IN_USE As Int32 = ACQIRIS_ERROR + &H8D3
    Public Const ACQIRIS_ERROR_MEZZIO_IN_USE As Int32 = ACQIRIS_ERROR + &H8D4
    Public Const ACQIRIS_ERROR_MEZZIO_ACQ_TIMEOUT As Int32 = ACQIRIS_ERROR + &H8D5
    Public Const ACQIRIS_ERROR_DEVICE_ALREADY_OPEN As Int32 = ACQIRIS_ERROR + &H8D6
    Public Const ACQIRIS_ERROR_EEPROM_CRC_FAILED As Int32 = ACQIRIS_ERROR + &H8D7

    Public Const ACQIRIS_ERROR_INVALID_GEOMAP_FILE As Int32 = ACQIRIS_ERROR + &H8E0

    Public Const ACQIRIS_ERROR_ACQ_TIMEOUT As Int32 = ACQIRIS_ERROR + &H900
    Public Const ACQIRIS_ERROR_TIMEOUT As Int32 = ACQIRIS_ERROR_ACQ_TIMEOUT      ' for backwards compatibility
    Public Const ACQIRIS_ERROR_OVERLOAD As Int32 = ACQIRIS_ERROR + &H901
    Public Const ACQIRIS_ERROR_PROC_TIMEOUT As Int32 = ACQIRIS_ERROR + &H902
    Public Const ACQIRIS_ERROR_LOAD_TIMEOUT As Int32 = ACQIRIS_ERROR + &H903
    Public Const ACQIRIS_ERROR_READ_TIMEOUT As Int32 = ACQIRIS_ERROR + &H904
    Public Const ACQIRIS_ERROR_INTERRUPTED As Int32 = ACQIRIS_ERROR + &H905
    Public Const ACQIRIS_ERROR_WAIT_TIMEOUT As Int32 = ACQIRIS_ERROR + &H906
    Public Const ACQIRIS_ERROR_CLOCK_SOURCE As Int32 = ACQIRIS_ERROR + &H907
    Public Const ACQIRIS_ERROR_OPERATION_CANCELLED As Int32 = ACQIRIS_ERROR + &H908

    Public Const ACQIRIS_ERROR_FIRMWARE_NOT_AUTHORIZED As Int32 = ACQIRIS_ERROR + &HA00
    Public Const ACQIRIS_ERROR_FPGA_1_LOAD As Int32 = ACQIRIS_ERROR + &HA01
    Public Const ACQIRIS_ERROR_FPGA_2_LOAD As Int32 = ACQIRIS_ERROR + &HA02
    Public Const ACQIRIS_ERROR_FPGA_3_LOAD As Int32 = ACQIRIS_ERROR + &HA03
    Public Const ACQIRIS_ERROR_FPGA_4_LOAD As Int32 = ACQIRIS_ERROR + &HA04
    Public Const ACQIRIS_ERROR_FPGA_5_LOAD As Int32 = ACQIRIS_ERROR + &HA05
    Public Const ACQIRIS_ERROR_FPGA_6_LOAD As Int32 = ACQIRIS_ERROR + &HA06
    Public Const ACQIRIS_ERROR_FPGA_7_LOAD As Int32 = ACQIRIS_ERROR + &HA07
    Public Const ACQIRIS_ERROR_FPGA_8_LOAD As Int32 = ACQIRIS_ERROR + &HA08
    Public Const ACQIRIS_ERROR_FIRMWARE_NOT_SUPPORTED As Int32 = ACQIRIS_ERROR + &HA09

    Public Const ACQIRIS_ERROR_FPGA_1_AUTOLOAD_NO_INIT As Int32 = ACQIRIS_ERROR + &HA10
    Public Const ACQIRIS_ERROR_FPGA_1_AUTOLOAD_NO_DONE As Int32 = ACQIRIS_ERROR + &HA11
    Public Const ACQIRIS_ERROR_FPGA_2_AUTOLOAD_NO_INIT As Int32 = ACQIRIS_ERROR + &HA12
    Public Const ACQIRIS_ERROR_FPGA_2_AUTOLOAD_NO_DONE As Int32 = ACQIRIS_ERROR + &HA13

    Public Const ACQIRIS_ERROR_SELFCHECK_MEMORY As Int32 = ACQIRIS_ERROR + &HA20
    Public Const ACQIRIS_ERROR_SELFCHECK_DAC As Int32 = ACQIRIS_ERROR + &HA21
    Public Const ACQIRIS_ERROR_SELFCHECK_RAMP As Int32 = ACQIRIS_ERROR + &HA22
    Public Const ACQIRIS_ERROR_SELFCHECK_PCIE_LINK As Int32 = ACQIRIS_ERROR + &HA23
    Public Const ACQIRIS_ERROR_SELFCHECK_PCIE_DEVICE As Int32 = ACQIRIS_ERROR + &HA24

    Public Const ACQIRIS_ERROR_FLASH_ACCESS_TIMEOUT As Int32 = ACQIRIS_ERROR + &HA30
    Public Const ACQIRIS_ERROR_FLASH_FAILURE As Int32 = ACQIRIS_ERROR + &HA31
    Public Const ACQIRIS_ERROR_FLASH_READ As Int32 = ACQIRIS_ERROR + &HA32
    Public Const ACQIRIS_ERROR_FLASH_WRITE As Int32 = ACQIRIS_ERROR + &HA33
    Public Const ACQIRIS_ERROR_FLASH_EMPTY As Int32 = ACQIRIS_ERROR + &HA34

    Public Const ACQIRIS_ERROR_ATTR_NOT_FOUND As Int32 = ACQIRIS_ERROR + &HB00
    Public Const ACQIRIS_ERROR_ATTR_WRONG_TYPE As Int32 = ACQIRIS_ERROR + &HB01
    Public Const ACQIRIS_ERROR_ATTR_IS_READ_ONLY As Int32 = ACQIRIS_ERROR + &HB02
    Public Const ACQIRIS_ERROR_ATTR_IS_WRITE_ONLY As Int32 = ACQIRIS_ERROR + &HB03
    Public Const ACQIRIS_ERROR_ATTR_ALREADY_DEFINED As Int32 = ACQIRIS_ERROR + &HB04
    Public Const ACQIRIS_ERROR_ATTR_IS_LOCKED As Int32 = ACQIRIS_ERROR + &HB05
    Public Const ACQIRIS_ERROR_ATTR_INVALID_VALUE As Int32 = ACQIRIS_ERROR + &HB06
    Public Const ACQIRIS_ERROR_ATTR_CALLBACK_STATUS As Int32 = ACQIRIS_ERROR + &HB07
    Public Const ACQIRIS_ERROR_ATTR_CALLBACK_EXCEPTION As Int32 = ACQIRIS_ERROR + &HB08

    Public Const ACQIRIS_ERROR_KERNEL_VERSION As Int32 = ACQIRIS_ERROR + &HC00
    Public Const ACQIRIS_ERROR_UNKNOWN_ERROR As Int32 = ACQIRIS_ERROR + &HC01
    Public Const ACQIRIS_ERROR_OTHER_WINDOWS_ERROR As Int32 = ACQIRIS_ERROR + &HC02
    Public Const ACQIRIS_ERROR_VISA_DLL_NOT_FOUND As Int32 = ACQIRIS_ERROR + &HC03
    Public Const ACQIRIS_ERROR_OUT_OF_MEMORY As Int32 = ACQIRIS_ERROR + &HC04
    Public Const ACQIRIS_ERROR_UNSUPPORTED_DEVICE As Int32 = ACQIRIS_ERROR + &HC05

    Public Const ACQIRIS_ERROR_PARAMETER9 As Int32 = ACQIRIS_ERROR + &HD09
    Public Const ACQIRIS_ERROR_PARAMETER10 As Int32 = ACQIRIS_ERROR + &HD0A
    Public Const ACQIRIS_ERROR_PARAMETER11 As Int32 = ACQIRIS_ERROR + &HD0B
    Public Const ACQIRIS_ERROR_PARAMETER12 As Int32 = ACQIRIS_ERROR + &HD0C
    Public Const ACQIRIS_ERROR_PARAMETER13 As Int32 = ACQIRIS_ERROR + &HD0D
    Public Const ACQIRIS_ERROR_PARAMETER14 As Int32 = ACQIRIS_ERROR + &HD0E
    Public Const ACQIRIS_ERROR_PARAMETER15 As Int32 = ACQIRIS_ERROR + &HD0F

    Public Const ACQIRIS_ERROR_NBR_SEG As Int32 = ACQIRIS_ERROR + &HD10
    Public Const ACQIRIS_ERROR_NBR_SAMPLE As Int32 = ACQIRIS_ERROR + &HD11
    Public Const ACQIRIS_ERROR_DATA_ARRAY As Int32 = ACQIRIS_ERROR + &HD12
    Public Const ACQIRIS_ERROR_SEG_DESC_ARRAY As Int32 = ACQIRIS_ERROR + &HD13
    Public Const ACQIRIS_ERROR_FIRST_SEG As Int32 = ACQIRIS_ERROR + &HD14
    Public Const ACQIRIS_ERROR_SEG_OFF As Int32 = ACQIRIS_ERROR + &HD15
    Public Const ACQIRIS_ERROR_FIRST_SAMPLE As Int32 = ACQIRIS_ERROR + &HD16
    Public Const ACQIRIS_ERROR_DATATYPE As Int32 = ACQIRIS_ERROR + &HD17
    Public Const ACQIRIS_ERROR_READMODE As Int32 = ACQIRIS_ERROR + &HD18

    Public Const ACQIRIS_ERROR_VM_FILE_EXTENSION As Int32 = ACQIRIS_ERROR + &HD50
    Public Const ACQIRIS_ERROR_VM_FILE_VERSION As Int32 = ACQIRIS_ERROR + &HD51
    Public Const ACQIRIS_ERROR_VM_FILE_READ As Int32 = ACQIRIS_ERROR + &HD52
    Public Const ACQIRIS_ERROR_VM_FILE_INVALID As Int32 = ACQIRIS_ERROR + &HD53
    Public Const ACQIRIS_ERROR_VM_VERIFICATION As Int32 = ACQIRIS_ERROR + &HD54
    Public Const ACQIRIS_ERROR_VM_CRC As Int32 = ACQIRIS_ERROR + &HD55

    Public Const ACQIRIS_ERROR_HW_FAILURE As Int32 = ACQIRIS_ERROR + &HD80
    Public Const ACQIRIS_ERROR_HW_FAILURE_CH1 As Int32 = ACQIRIS_ERROR + &HD81
    Public Const ACQIRIS_ERROR_HW_FAILURE_CH2 As Int32 = ACQIRIS_ERROR + &HD82
    Public Const ACQIRIS_ERROR_HW_FAILURE_CH3 As Int32 = ACQIRIS_ERROR + &HD83
    Public Const ACQIRIS_ERROR_HW_FAILURE_CH4 As Int32 = ACQIRIS_ERROR + &HD84
    Public Const ACQIRIS_ERROR_HW_FAILURE_CH5 As Int32 = ACQIRIS_ERROR + &HD85
    Public Const ACQIRIS_ERROR_HW_FAILURE_CH6 As Int32 = ACQIRIS_ERROR + &HD86
    Public Const ACQIRIS_ERROR_HW_FAILURE_CH7 As Int32 = ACQIRIS_ERROR + &HD87
    Public Const ACQIRIS_ERROR_HW_FAILURE_CH8 As Int32 = ACQIRIS_ERROR + &HD88
    Public Const ACQIRIS_ERROR_HW_FAILURE_EXT1 As Int32 = ACQIRIS_ERROR + &HDA0

    Public Const ACQIRIS_ERROR_MAC_T0_ADJUSTMENT As Int32 = ACQIRIS_ERROR + &HDC0
    Public Const ACQIRIS_ERROR_MAC_ADC_ADJUSTMENT As Int32 = ACQIRIS_ERROR + &HDC1
    Public Const ACQIRIS_ERROR_MAC_RESYNC_ADJUSTMENT As Int32 = ACQIRIS_ERROR + &HDC2

    Public Const ACQIRIS_WARN_SETUP_ADAPTED As Int32 = ACQIRIS_WARNING + &HE00
    Public Const ACQIRIS_WARN_READPARA_NBRSEG_ADAPTED As Int32 = ACQIRIS_WARNING + &HE10
    Public Const ACQIRIS_WARN_READPARA_NBRSAMP_ADAPTED As Int32 = ACQIRIS_WARNING + &HE11
    Public Const ACQIRIS_WARN_EEPROM_AND_DLL_MISMATCH As Int32 = ACQIRIS_WARNING + &HE12
    Public Const ACQIRIS_WARN_ACTUAL_DATASIZE_ADAPTED As Int32 = ACQIRIS_WARNING + &HE13
    Public Const ACQIRIS_WARN_UNEXPECTED_TRIGGER As Int32 = ACQIRIS_WARNING + &HE14
    Public Const ACQIRIS_WARN_READPARA_FLAGS_ADAPTER As Int32 = ACQIRIS_WARNING + &HE15
    Public Const ACQIRIS_WARN_SIMOPTION_STRING_UNKNOWN As Int32 = ACQIRIS_WARNING + &HE16
    Public Const ACQIRIS_WARN_INSTRUMENT_IN_USE As Int32 = ACQIRIS_WARNING + &HE17

    Public Const ACQIRIS_WARN_HARDWARE_TIMEOUT As Int32 = ACQIRIS_WARNING + &HE60
    Public Const ACQIRIS_WARN_RESET_IGNORED As Int32 = ACQIRIS_WARNING + &HE61

    Public Const ACQIRIS_WARN_SELFCHECK_MEMORY As Int32 = ACQIRIS_WARNING + &HF00
    Public Const ACQIRIS_WARN_CLOCK_SOURCE As Int32 = ACQIRIS_WARNING + &HF01

    Public Const ACQIRIS_WARN_NUMERIC_OVERFLOW As Int32 = ACQIRIS_WARNING + &HF20

    '/////////////////////////////////////////////////////////////////////////////////////////
    ' General comments about the function prototypes:
    '
    ' - All function calls require the argument 'instrumentID' in order to identify
    '   the Acqiris Digitizer card to which the call is directed
    '   The only exceptions are the initializer functions 'AcqrsD1_getNbrPhysicalInstruments',
    '   'AcqrsD1_setSimulationOptions', 'AcqrsD1_init' and 'AcqrsD1_InitWithOptions'.
    '   The last two functions actually return instrument identifiers at initialization time,
    '   for subsequent use in the other function calls.
    '
    ' - All function calls return a status value of type 'Int32' with information about
    '   the success or failure of the call.
    '   The values are shown in the list above. They can be translated into a human readable string
    '   with a call to the function 'AcqrsD1_errorMessage'.
    '
    ' - If important parameters supplied by the user (e.g. an invalid instrumentID) are found
    '   to be invalid, most functions do not execute and return an error code of the type
    '   VI_ERROR_PARAMETERi, where i = 1, 2, ... corresponds to the argument number.
    '
    ' - If the user attempts (with a function AcqrsD1_configXXXX) to set a digitizer
    '   parameter to a value which is outside of its acceptable range, the function
    '   typically adapts the parameter to the closest available value and returns
    '   ACQIRIS_WARN_SETUP_ADAPTED. The digitizer parameters actually set can be retrieved
    '   with the 'query' functions AcqrsD1_getXXXX.
    '
    ' - Data are always returned to user-allocated variables or arrays.
    '
    '/////////////////////////////////////////////////////////////////////////////////////////


    '/////////////////////////////////////////////////////////////////////////////////////////
    ' Data Structures for '...readData' and '...writeData' functions
    Public Structure AqReadParameters
        Public dataType As Int32            ' 0 = 8-bit (Byte)  1 = 16-bit (Word) 2 = 32-bit (Int32)  3 = 64-bit (Double)
        Public readMode As Int32
        Public firstSegment As Int32
        Public nbrSegments As Int32
        Public firstSampleInSeg As Int32
        Public nbrSamplesInSeg As Int32
        Public segmentOffset As Int32
        Public dataArraySize As Int32       ' in bytes
        Public segDescArraySize As Int32    ' in bytes
        Public flags As Int32
        Public reserved As Int32
        Public reserved2 As Double
        Public reserved3 As Double
    End Structure

    Public Structure AqWriteParameters
        Public dataType As Int32            ' 0 = 8-bit (Byte)  1 = 16-bit (Word) 2 = 32-bit (Int32)  3 = 64-bit (Double)
        Public readMode As Int32
        Public firstSegment As Int32
        Public nbrSegments As Int32
        Public firstSampleInSeg As Int32
        Public nbrSamplesInSeg As Int32
        Public segmentOffset As Int32
        Public dataArraySize As Int32       ' in bytes
        Public segDescArraySize As Int32    ' in bytes
        Public flags As Int32
        Public reserved As Int32
        Public reserved2 As Double
        Public reserved3 As Double
    End Structure

    Public Structure AqSegmentDescriptor
        Public horPos As Double
        Public timeStampLo As UInt32         ' Timestamp
        Public timeStampHi As UInt32
    End Structure

    ' For 'readMode = ReadModeSeqRawW' only.
    Public Structure AqSegmentDescriptorSeqRaw
        Public horPos As Double
        Public timeStampLo As UInt32       ' Timestamp 
        Public timeStampHi As UInt32
        Public indexFirstPoint As UInt32   ' Pointer to the first sample of this segment, 
        ' into the array passed to 'readData'.
        Public actualSegmentSize As UInt32 ' In samples, for the circular buffer wrapping.
        Public reserved As Int32
    End Structure

    Public Structure AqSegmentDescriptorAvg
        Public horPos As Double
        Public timeStampLo As UInt32         ' Timestamp
        Public timeStampHi As UInt32
        Public actualTriggersInSeg As UInt32 ' Number of actual triggers acquired in this segment
        Public avgOvfl As Int32              ' Acquisition overflow (avg)
        Public avgStatus As Int32            ' Average status (avg)
        Public avgMax As Int32               ' Max value in the sequence (avg)
        Public reserved1 As Double
    End Structure

    Public Structure AqDataDescriptor
        Public returnedSamplesPerSeg As Int32
        Public indexFirstPoint As Int32     ' "waveArray(wfDesc.indexFirstPoint)" is the first valid point of "waveArray"
        Public sampTime As Double
        Public vGain As Double
        Public vOffset As Double
        Public returnedSegments As Int32    ' When reading multiple segments in one waveform
        Public nbrAvgWforms As Int32
        Public actualTriggersInAcqLo As UInt32
        Public actualTriggersInAcqHi As UInt32
        Public actualDataSize As UInt32      ' new, data size header included
        Public reserved2 As Int32
        Public reserved3 As Double
    End Structure

    Public Structure AqGateParameters
        Public gatePos As Int32
        Public gateLength As Int32
    End Structure


    '/////////////////////////////////////////////////////////////////////////////////////////
    ' Function Prototypes

    Public Declare Function AcqrsD1_accumulateData Lib "AqDrv4_x64" _
    Alias "AcqrsD1_accumulateData" (ByVal instrumentID As Int32, _
                                        ByVal channel As Int32, _
                                        ByRef readPar As AqReadParameters, _
                                        ByVal dataArray As System.IntPtr, _
                                        ByRef sumArray As Int32, _
                                        ByRef dataDesc As AqDataDescriptor, _
                                        ByRef segDescArray As AqSegmentDescriptor) As Int32
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
    ' 'channel'        = 1...Nchan (as returned by 'AcqrsD1_getNbrChannels' )
    ' 'readPar'        = pointer to a user-supplied structure that specifies what and how to read,
    '                    similar to function 'AcqrsD1_readData'.
    '                    However, only 'readPar.nbrSegments = 1' and 'readPar.readMode = 0' (ReadModeStdW) are supported.
    '                    'readPar.dataType = 3' (real) and 'readPar.dataType = 2' (Int32) are NOT supported.
    '
    ' Values returned by the function:
    '
    ' 'dataArray'      = user-allocated data destination array of type defined by 'readPar.dataType'
    '                    Its size in dataType units MUST be at least (nbrSamples + 32),
    '                    for reasons of data alignment. Please refer to the manual for additional details.
    ' 'sumArray'       = user-allocated waveform accumulation array of type Int32 (32 bits).
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

    Public Declare Function AcqrsD1_accumulateWform Lib "AqDrv4_x64" _
    Alias "AcqrsD1_accumulateWform" (ByVal instrumentID As Int32, _
                                         ByVal channel As Int32, _
                                         ByVal segmentNumber As Int32, _
                                         ByVal firstSample As Int32, _
                                         ByVal nbrSamples As Int32, _
                                         ByVal dataArray As System.IntPtr, _
                                         ByVal sumArray As System.IntPtr, _
                                         ByRef returnedSamples As Int32, _
                                         ByRef sampTime As Double, _
                                         ByRef vGain As Double, _
                                         ByRef vOffset As Double) As Int32
    ' Returns a waveform as a byte array and accumulate it in a client array.
    '
    ' The following values must be supplied to the function:
    '
    ' 'channel'        = 1...Nchan (as returned by 'AcqrsD1_getNbrChannels' )
    ' 'segmentNumber'  = requested segment number, may assume 0 to 'nbrSegments - 1',
    '                    as set with the function 'AcqrsD1_configMemory'
    ' 'firstSample'    = requested position of first sample to read, typically 0
    ' 'nbrSamples'     = requested number of samples, may assume 1 to 'nbrSamples', as
    '                    set with the function 'AcqrsD1_configMemory'
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

    Public Declare Function AcqrsD1_acqDone Lib "AqDrv4_x64" _
    Alias "AcqrsD1_acqDone" (ByVal instrumentID As Int32, ByRef done As Boolean) As Int32
    ' Checks if the acquisition has terminated
    ' Returns 'done' = TRUE if the acquisition is terminated
    '
    ' Returns one of the following ViStatus values:
    ' ACQIRIS_ERROR_IO_READ    if a link error has been detected (e.g. PCI link lost).
    ' VI_SUCCESS otherwise.

    Public Declare Function AcqrsD1_acquire Lib "AqDrv4_x64" _
    Alias "AcqrsD1_acquire" (ByVal instrumentID As Int32) As Int32
    ' Starts an acquisition. This function is equivalent to 'acquireEx' with 'acquireMode = 0, 
    ' acquireFlags = 0'
    '
    ' Common return values:
    ' ACQIRIS_ERROR_INSTRUMENT_RUNNING if the instrument is already running. 
    ' VI_SUCCESS otherwise.

    Public Declare Function AcqrsD1_acquireEx Lib "AqDrv4_x64" _
    Alias "AcqrsD1_acquireEx" (ByVal instrumentID As Int32, _
                                 ByVal acquireMode As Int32, _
                                 ByVal acquireFlags As Int32, _
                                 ByVal acquireParams As Int32, _
                                 ByVal reserved As Int32) As Int32
    ' Starts an acquisition.
    '
    ' The following values must be supplied to the function:
    '
    ' 'acquireMode'	  = 0       Normal, start an acquisition and return immediatly (equivalent to function 'acquire').
    '				  = 2		Averagers only! Sets continuous accumulation and starts an acquisition.
    '            
    ' 'acquireFlags'  = 0   No flags.
    '                 = 4   Reset timestamps (if supported).
    ' 'acquireParams'		Unused, must be set to 0.
    ' 'reserved'		  	Unused, must be set to 0.
    '
    ' Returns one of the following ViStatus values:
    ' ACQIRIS_ERROR_INSTRUMENT_RUNNING if the instrument is already running. 
    ' ACQIRIS_ERROR_NOT_SUPPORTED      if the requested mode or flag is not supported by the  
    '                                  instrument.
    ' VI_SUCCESS                       otherwise.

    Public Declare Function AcqrsD1_averagedData Lib "AqDrv4_x64" _
    Alias "AcqrsD1_averagedData" (ByVal instrumentID As Int32, _
                                      ByVal channel As Int32, _
                                      ByRef readPar As AqReadParameters, _
                                      ByVal nbrAcq As Int32, _
                                      ByVal calculateMean As Boolean, _
                                      ByVal timeout As Double, _
                                      ByVal dataArray As System.IntPtr, _
                                      ByVal sumArray As System.IntPtr, _
                                      ByRef dataDesc As AqDataDescriptor, _
                                      ByRef segDescArray As AqSegmentDescriptor) As Int32
    ' Perform a series of acquisitions and get the resulting averaged waveform for one segment.
    '
    ' NOTE: Because the acquisition control loop is done inside this function, it is
    ' suitable ONLY for single instrument, single channel, single segment operation.
    '
    ' NOTE2: This function is for DIGITIZERS ONLY !! For getting the average of an averager module (AP-XXX)
    '        you must use 'readData', and have set the instrument through 'AcqrsD1_configMode' with 'mode = 2'.
    '
    ' The following values must be supplied to the function:
    '
    ' 'channel'         = 1...Nchan (as returned by 'AcqrsD1_getNbrChannels' )
    ' 'readPar'         = pointer to a user-supplied structure that specifies what and how to read,
    '                     similar to function 'AcqrsD1_readData'.
    '                     However, only 'readPar.nbrSegments = 1' and 'readPar.readMode = 0' (ReadModeStdW) are supported.
    '                     'readPar.dataType = 3' (real) and 'readPar.dataType = 2' (Int32) are NOT supported.
    ' 'nbrAcq'          = number of acquisitions to be performed. This value can be at most
    '                     16777216 for 'readPar.dataType = 0' (char) or
    '                     65536 for 'readPar.dataType = 1' (short)
    '                     Otherwise, an overflow may occur and the summed values will wrap around 0.
    ' 'calculateMean'   = if '1', each sample of the 'sumArray' is divided by the 'nbrAcq', in order
    '                     to calculate the mean. If '0', the 'sumArray' will contain the
    '                     sample-by-sample sums, and must be divided by 'nbrAcq' afterward to get
    '                     the averaged values
    ' 'timeout'         = acquisition timeout in seconds. The function will return
    '                     ACQIRIS_ERROR_ACQ_TIMEOUT if there is no trigger within the specified timeout interval
    '                     after the start of each acquisition.
    '
    ' Values returned by the function:
    '
    ' 'dataArray'       = user-allocated data destination array of type defined by 'readPar.dataType'
    '                     Its size in dataType units MUST be at least (nbrSamples + 32),
    '                     for reasons of data alignment. Please refer to the manual for additional details.
    ' 'sumArray'        = user-allocated waveform accumulation array of type Int32 (32 bits).
    '                     Its size MUST be at least 'nbrSamples'. Note that the sumArray can be interpreted
    '                     as an unsigned integer. Alternatively, negative values have to be increased by 2**32.
    ' 'dataDesc'        = user-allocated structure for returned data descriptor values, from last acquisition.
    ' 'segDescArray'    = NULL pointer or pointer to a user allocated structure for returned AqSegmentDescriptor.
    '                     Returned value in 'segDescArray' is from the last acquisition.
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
    ' For other errors, please see 'readData'.

    Public Declare Function AcqrsD1_averagedWform Lib "AqDrv4_x64" _
    Alias "AcqrsD1_averagedWform" (ByVal instrumentID As Int32, _
                                       ByVal channel As Int32, _
                                       ByVal segmentNumber As Int32, _
                                       ByVal firstSample As Int32, _
                                       ByVal nbrSamples As Int32, _
                                       ByVal nbrAcq As Int32, _
                                       ByVal timeout As Double, _
                                       ByVal waveformArray As System.IntPtr, _
                                       ByVal sumArray As System.IntPtr, _
                                       ByRef returnedSamples As Int32, _
                                       ByRef sampTime As Double, _
                                       ByRef vGain As Double, _
                                       ByRef vOffset As Double) As Int32
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
    ' 'channel'        = 1...Nchan (as returned by 'AcqrsD1_getNbrChannels' )
    ' 'segmentNumber'  = requested segment number, may assume 0 to 'nbrSegments - 1',
    '                    as set with the function 'AcqrsD1_configMemory'
    ' 'firstSample'    = requested position of first sample to read, typically 0. May assume 0 to
    '                    (the number of samples - 1) as set with the function 'AcqrsD1_configMemory'
    ' 'nbrSamples'     = requested number of samples, may assume 1 to 'nbrSamples', as
    '                    set with the function 'AcqrsD1_configMemory'
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

    Public Declare Function AcqrsD1_bestNominalSamples Lib "AqDrv4_x64" _
    Alias "AcqrsD1_bestNominalSamples" (ByVal instrumentID As Int32, _
                                           ByRef nomSamples As Int32) As Int32
    ' Helper function to ease the instrument configuration.
    ' Returns maximum nominal number of samples which fits into the available memory.
    '
    ' Values returned by the function:
    '
    ' 'nomSamples'     = maximum number of data samples available
    '
    ' NOTE: When using this method, make sure to use 'AcqrsD1_configHorizontal' and
    '       'AcqrsD1_configMemory' beforehand to set the sampling rate and the number of
    '       segments to the desired values ('nbrSamples' in 'AcqrsD1_configMemory' may be
    '       any number!). 'AcqrsD1_bestNominalSamples' depends on these variables.
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                   if one of the parameters has an invalid value, where 
    '                                       x = parameter number.
    ' VI_SUCCESS                            when a good solution has been found.

    Public Declare Function AcqrsD1_bestSampInterval Lib "AqDrv4_x64" _
    Alias "AcqrsD1_bestSampInterval" (ByVal instrumentID As Int32, _
                                          ByVal maxSamples As Int32, _
                                          ByVal timeWindow As Double, _
                                          ByRef sampInterval As Double, _
                                          ByRef nomSamples As Int32) As Int32
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
    '                   when the available memory is too short, and the Int32est available
    '                   sampling interval too short. The returned sampling interval is the
    '                   Int32est one possible.
    '               = VI_SUCCESS
    '                   when a good solution has been found.
    '
    ' NOTE: This function DOES NOT modify the state of the digitizer at all. It simply returns
    '       a recommendation which the user is free to override.
    ' NOTE: When using this method, make sure to use 'ConfigMemory' beforehand to set
    '       the number of segments to the desired value. ('nbrSamples' may be any
    '       number!). 'AcqrsD1_bestSampInterval' depends on this variable.
    ' NOTE: The returned 'recommended' values for the 'sampInterval' and the nominal number
    '       of samples 'nomSamples' are expected to be used for configuring the instrument
    '       with calls to 'AcqrsD1_configHorizontal' and 'AcqrsD1_configMemory'. Make sure
    '       to use the same number of segments in this second call to 'AcqrsD1_configMemory'
    '       as in the first one.
    '
    ' Returns one of the following ViStatus values:
    ' ACQIRIS_ERROR_SETUP_NOT_AVAILABLE when the available memory is too short, and the longest
    '                                   available sampling interval too short. The returned 
    '                                   sampling interval is the longest one possible.
    ' VI_ERROR_PARAMETERx               if one of the parameters has an invalid value, where 
    '                                   x = parameter number.
    ' VI_SUCCESS                        when a good solution has been found.

    Public Declare Function AcqrsD1_calibrate Lib "AqDrv4_x64" _
    Alias "AcqrsD1_calibrate" (ByVal instrumentID As Int32) As Int32
    ' Performs an auto-calibration of the instrument
    ' Equivalent to AcqrsD1_calibrateEx with 'calType' = 0

    Public Declare Function AcqrsD1_calibrateEx Lib "AqDrv4_x64" _
    Alias "AcqrsD1_calibrateEx" (ByVal instrumentID As Int32, _
                                     ByVal calType As Int32, _
                                     ByVal modifier As Int32, _
                                     ByVal flags As Int32) As Int32
    ' Performs a (partial) auto-calibration of the instrument
    '
    ' 'calType'=  0    calibrate the entire instrument (equivalent to 'AcqrsD1_calibrate')
    '             1    calibrate only the current channel configuration, 
    '                  as set by 'AcqrsD1_configChannelCombination'
    '             2    calibrate external clock timing. Requires operation in External Clock 
    '                  (Continuous), i.e. 'clockType' = 1, as set with 'AcqrsD1_configExtClock'
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

    Public Declare Function AcqrsD1_close Lib "AqDrv4_x64" _
    Alias "AcqrsD1_close" (ByVal instrumentID As Int32) As Int32
    ' Close specified instrument. Once closed, this instrument is not available anymore and
    ' need to be reenabled using 'InitWithOptions' or 'init'.
    ' Note: For freeing properly all resources, 'closeAll' must still be called when
    ' the application close, even if 'close' was called for each instrument.
    '
    ' Returns one of the following ViStatus values:
    ' VI_SUCCESS always.

    Public Declare Function AcqrsD1_closeAll Lib "AqDrv4_x64" _
    Alias "AcqrsD1_closeAll" () As Int32
    ' Closes all instruments and prepares for closing of application
    '
    ' Returns one of the following ViStatus values:
    ' VI_SUCCESS always.

    Public Declare Function AcqrsD1_configAvgConfig Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configAvgConfig" (ByVal instrumentID As Int32, _
                                         ByVal channel As Int32, _
                                         ByVal parameterString As String, _
                                         ByRef value As Int32) As Int32
    Public Declare Function AcqrsD1_configAvgConfig Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configAvgConfig" (ByVal instrumentID As Int32, _
                                         ByVal channel As Int32, _
                                         ByVal parameterString As String, _
                                         ByRef value As Double) As Int32
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

    Public Declare Function AcqrsD1_configAvgConfigInt32 Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configAvgConfigInt32" (ByVal instrumentID As Int32, _
                                         ByVal channel As Int32, _
                                         ByVal parameterString As String, _
                                         ByRef value As Int32) As Int32
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

    Public Declare Function AcqrsD1_configAvgConfigReal64 Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configAvgConfigReal64" (ByVal instrumentID As Int32, _
                                         ByVal channel As Int32, _
                                         ByVal parameterString As String, _
                                         ByRef value As Double) As Int32
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

    Public Declare Function AcqrsD1_configChannelCombination Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configChannelCombination" (ByVal instrumentID As Int32, _
                                                  ByVal nbrConvertersPerChannel As Int32, _
                                                  ByVal usedChannels As Int32) As Int32
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

    Public Declare Function AcqrsD1_configControlIO Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configControlIO" (ByVal instrumentID As Int32, _
                                         ByVal connector As Int32, _
                                         ByVal signal As Int32, _
                                         ByVal qualifier1 As Int32, _
                                         ByVal qualifier2 As Double) As Int32
    ' Configures Control-IO connectors
    ' Typically, only a few (or no) IO connectors are present on a single digitizer
    ' 
    ' 'connector'        = 1        Front Panel I/O A (MMCX connector)
    '                    = 2        Front Panel I/O B (MMCX connector)
    '                    = 3        Front Panel I/O C (MMCX connector, if available)
    '                    = 9        Front Panel Trigger Out (MMCX connector)
    '                    = 11       PXI Bus 10 MHz (if available)
    '                    = 12       PXI Bus Star Trigger (if available)
    ' 'signal'           = value depends on 'connector', refer to manual for definitions.
    ' 'qualifier1',      = value depends on 'connector', refer to manual for definitions.
    ' 'qualifier2'       = If trigger veto functionality is available (if available), 
    '                      accepts values between 30 ns and 1.0 sec. 
    '                      The trigger veto values given will be rounded off to steps of 33 ns. 
    '                      A value of 0.0 means that no holdoff is required and no Prepare for 
    '                      Trigger signal will be needed.
    '
    ' Returns one of the following ViStatus values:
    ' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
    ' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_configExtClock Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configExtClock" (ByVal instrumentID As Int32, _
                                        ByVal clockType As Int32, _
                                        ByVal inputThreshold As Double, _
                                        ByVal delayNbrSamples As Int32, _
                                        ByVal refFrequency As Double, _
                                        ByVal sampFrequency As Double) As Int32
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
    '       in the function 'AcqrsD1_configHorizontal' are ignored.
    '
    ' Returns one of the following ViStatus values:
    ' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
    ' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_configFCounter Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configFCounter" (ByVal instrumentID As Int32, _
                                        ByVal signalChannel As Int32, _
                                        ByVal typeMeas As Int32, _
                                        ByVal targetValue As Double, _
                                        ByVal apertureTime As Double, _
                                        ByVal reserved As Double, _
                                        ByVal flags As Int32) As Int32
    ' Configures the frequency counter
    '
    ' 'signalChannel'	= 1...Nchan for signal input channel
    ' 'typeMeas'		= 0		Frequency
    '					= 1		Period
    '					= 2		Totalize, counts input pulses during interval defined by 'apertureTime'
    ' 'targetValue'	    = estimate of expected result (set to 0.0, if no estimate available)
    ' 'apertureTime'	= minimum measurement time for Frequency and Period modes
    '					= time gate for Totalize mode
    ' 'reserved', 'flags'	currently unused (set to zero!)
    '
    ' Returns one of the following ViStatus values:
    ' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
    ' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_configHorizontal Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configHorizontal" (ByVal instrumentID As Int32, _
                                          ByVal sampInterval As Double, _
                                          ByVal delayTime As Double) As Int32
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
    ' Returns one of the following ViStatus values:
    ' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
    ' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_configLogicDevice Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configLogicDevice" (ByVal instrumentID As Int32, _
                                           ByVal deviceName As String, _
                                           ByVal filePathName As String, _
                                           ByVal modifier As Int32) As Int32
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
    '        For instance, in the AP200/AP201 it must be "Block1Devx", with x = 1, 2, 3 or 4.
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


    Public Declare Function AcqrsD1_configMemory Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configMemory" (ByVal instrumentID As Int32, _
                                      ByVal nbrSamples As Int32, _
                                      ByVal nbrSegments As Int32) As Int32
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

    Public Declare Function AcqrsD1_configMemoryEx Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configMemoryEx" (ByVal instrumentID As Int32, _
                                        ByVal nbrSamplesHi As UInt32, _
                                        ByVal nbrSamplesLo As UInt32, _
                                        ByVal nbrSegments As Int32, _
                                        ByVal nbrBanks As Int32, _
                                        ByVal flags As Int32) As Int32
    ' Configures the memory control parameters of the digitizer.
    '
    ' 'nbrSamplesHi'      = reserved for future use, must be set to 0.
    ' 'nbrSamplesLo'      = nominal number of samples to record (per segment!).
    ' 'nbrSegments'       = number of segments to acquire per bank
    '                       1 corresponds to the normal single-trace acquisition mode.
    ' 'nbrBanks'          = number of banks in which the memory will be split, 
    '                       for buffered reading (DigSSR).
    '                       1 corresponds to the normal acquisition mode.
    ' 'flags'             = 0 no flags. 
    '                     = 1 force use of internal memory (for 10-bit digitizers with extended 
    '                       memory options only).
    ' Returns one of the following ViStatus values:
    ' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
    ' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
    ' VI_SUCCESS                         otherwise.


    Public Declare Function AcqrsD1_configMode Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configMode" (ByVal instrumentID As Int32, _
                                    ByVal mode As Int32, _
                                    ByVal modifier As Int32, _
                                    ByVal flags As Int32) As Int32
    ' Configures the operational mode of the digitizer.
    '
    ' 'mode'            = operational mode
    '                   = 0        normal acquisition
    '                   = 1        stream data to DPU (only in SC2x0/AC2x0 Data Streamers)
    '                   = 2        averaging mode (only in Real-Time Averagers APx00 and AP240)
    '                   = 3        dual-memory mode (only in AP101, AP201)
    '                   = 6        frequency counter mode
    '                   = 7        SSR mode (for AP235/AP240)
    '
    ' 'modifier' = not used, set to 0
    '
    ' 'flags'    ['mode' = 0]    = 0        normal
    '                            = 1        'Start-on-Trigger' mode (if available)
    '                            = 2        'Sequence-wrap' mode (use 'nbrSegments' > 1) 
    '                            = 10       'SAR' mode (use 'nbrBanks' > 1) 
    '            ['mode' = 1]    = 0        normal
    '                            = 1        autoRestart
    '            ['mode' = 3]    = 0        acquire into memory bank 0
    '                            = 1        acquire into memory bank 1
    '            otherwise  unused, set to 0
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

    Public Declare Function AcqrsD1_configMultiInput Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configMultiInput" (ByVal instrumentID As Int32, _
                                          ByVal channel As Int32, _
                                          ByVal input As Int32) As Int32
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

    Public Declare Function AcqrsD1_configSetupArray Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configSetupArray" (ByVal instrumentID As Int32, _
                                          ByVal channel As Int32, _
                                          ByVal setupType As Int32, _
                                          ByVal nbrSetupObj As Int32, _
                                          ByRef setupData As Int32) As Int32
    ' Configures an array of setup data (typically for on-board processing)
    '
    ' 'setupType'       = indicates the object type (data structure) of which the setup data is composed
    '                     Some objects might be simple elements, e.g. Int32
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

    Public Declare Function AcqrsD1_configTrigClass Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configTrigClass" (ByVal instrumentID As Int32, _
                                         ByVal trigClass As Int32, _
                                         ByVal sourcePattern As Int32, _
                                         ByVal validatePattern As Int32, _
                                         ByVal holdType As Int32, _
                                         ByVal holdValue1 As Double, _
                                         ByVal holdValue2 As Double) As Int32
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
    ' Note: The detailed TV trigger configuration is set with the function 'AcqrsD1_configTrigTV'
    ' Note2: trigClass = 3,4,5 or 6 features are only supported within a single instrument, or
    '   within a single module in an ASBUS configuration (if available).
    '
    ' Returns one of the following ViStatus values:
    ' ACQIRIS_WARN_SETUP_ADAPTED         if one of the parameters has been adapted.
    ' VI_ERROR_PARAMETER1                if 'instrumentId' is invalid.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_configTrigSource Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configTrigSource" (ByVal instrumentID As Int32, _
                                          ByVal channel As Int32, _
                                          ByVal trigCoupling As Int32, _
                                          ByVal trigSlope As Int32, _
                                          ByVal trigLevel1 As Double, _
                                          ByVal trigLevel2 As Double) As Int32
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

    Public Declare Function AcqrsD1_configTrigTV Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configTrigTV" (ByVal instrumentID As Int32, _
                                      ByVal channel As Int32, _
                                      ByVal standard As Int32, _
                                      ByVal field As Int32, _
                                      ByVal line As Int32) As Int32
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

    Public Declare Function AcqrsD1_configVertical Lib "AqDrv4_x64" _
    Alias "AcqrsD1_configVertical" (ByVal instrumentID As Int32, _
                                        ByVal channel As Int32, _
                                        ByVal fullScale As Double, _
                                        ByVal offset As Double, _
                                        ByVal coupling As Int32, _
                                        ByVal bandwidth As Int32) As Int32
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

    Public Declare Function AcqrsD1_errorMessage Lib "AqDrv4_x64" _
    Alias "AcqrsD1_errorMessage" (ByVal instrumentID As Int32, _
                                      ByVal errorCode As Int32, _
                                      ByVal errorMessage As String) As Int32
    ' Translates an error code into a human readable form 
    '
    ' 'errorCode'        = Error code (returned by a function) to be translated
    ' 'errorMessage'     = Pointer to user-allocated character string (minimum size 256),
    '                      into which the error-message text is returned
    '                      The string 'errorMessage' must be pre-allocated as 
    '                      "Dim errorMessage As String = Space(256)"
    '
    ' NOTE: 'instrumentID' can be VI_NULL.
    '
    ' Returns one of the following ViStatus values:
    ' VI_SUCCESS always.

    Public Declare Function AcqrsD1_errorMessageEx Lib "AqDrv4_x64" _
    Alias "AcqrsD1_errorMessageEx" (ByVal instrumentID As Int32, _
                                        ByVal errorCode As Int32, _
                                        ByVal errorMessage As String, _
                                        ByRef errorMessageSize As Int32) As Int32
    ' Translates an error code into a human readable form. For file errors, the returned message 
    ' will also contain the file name and the original 'ansi' error string.
    '
    ' 'errorCode'          = Error code (returned by a function) to be translated
    ' 'errorMessage'       = Pointer to user-allocated character string (suggested size 512 bytes),
    '                        into which the error message text will be copied.
    '                        The string 'errorMessage' must be pre-allocated as 
    '                        "Dim errorMessage As String = Space(512)"
    ' 'errorMessageSize'   = size of 'errorMessage', in bytes.
    '
    ' NOTE: 'instrumentID' can be 0.
    '
    ' Returns the following status values:
    '
    ' ACQIRIS_ERROR_BUFFER_OVERFLOW    if 'errorMessageSize' is too small.
    ' VI_SUCCESS                       otherwise.

    Public Declare Function AcqrsD1_forceTrig Lib "AqDrv4_x64" _
    Alias "AcqrsD1_forceTrig" (ByVal instrumentID As Int32) As Int32
    ' Forces a 'manual' trigger. The function returns immediately after initiating
    ' an acquisition. One must therefore wait until this acquisition has terminated
    ' before reading the data, by checking the status with the 'AcqrsD1_acqDone'
    ' function. Equivalent to 'AcqrsD1_forceTrigEx' with 'forceTrigType' = 0

    Public Declare Function AcqrsD1_forceTrigEx Lib "AqDrv4_x64" _
    Alias "AcqrsD1_forceTrigEx" (ByVal instrumentID As Int32, _
                                     ByVal forceTrigType As Int32, _
                                     ByVal modifier As Int32, _
                                     ByVal flags As Int32) As Int32
    ' Forces a 'manual' trigger. The function returns immediately after initiating
    ' a trigger. One must therefore wait until this acquisition has terminated
    ' before reading the data, by checking the status with the 'AcqrsD1_acqDone'
    ' or the 'AcqrsD1_waitForEndOfAcquisition' functions.
    '
    ' 'forceTrigType'=		0	Sends a software trigger to end the (entire) acquisition. 
    '							In multisegment mode, the current segment is acquired, the acquisition is terminated 
    '							and	the data and timestamps of subsequent segments are invalid.
    '							The 'trigOut' Control IO will NOT generate a trigger output.
    '							Equivalent to 'AcqrsD1_forceTrig'.
    '						1	Send a software trigger similar to a hardware trigger. 
    '							In multisegment mode, the acquisition advances to the next segment and then waits again 
    '							for a trigger. If no valid triggers are	provided to the device, the application must call
    '							'AcqrsD1_forceTrigEx' as many times as there are segments. In this mode, 'trigOut' Control IO will
    '							generate a trigger output on each successful call. Every acquired segment will be valid. 
    '							This mode is only supported for single (i.e. non-ASBus-connected) instruments.
    ' 'modifier'				currently unused, must be zero
    ' 'flags'					currently unused, must be zero 
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

    Public Declare Function AcqrsD1_freeBank Lib "AqDrv4_x64" _
    Alias "AcqrsD1_freeBank" (ByVal instrumentID As Int32, _
                                 ByVal reserved As Int32) As Int32
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

    Public Declare Function AcqrsD1_getAvgConfig Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getAvgConfig" (ByVal instrumentID As Int32, _
                                      ByVal channel As Int32, _
                                      ByVal parameterString As String, _
                                      ByRef value As Int32) As Int32
    Public Declare Function AcqrsD1_getAvgConfig Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getAvgConfig" (ByVal instrumentID As Int32, _
                                      ByVal channel As Int32, _
                                      ByVal parameterString As String, _
                                      ByRef value As Double) As Int32
    ' Returns a parameter from the averager/analyzer configuration
    ' See remarks under 'AcqrsD1_configAvgConfig'
    '
    ' Returns one of the following ViStatus values:
    ' ACQIRIS_ERROR_PARAM_STRING_INVALID if 'parameterString' is invalid.
    ' ACQIRIS_ERROR_NOT_SUPPORTED        if this function is not supported by the  
    '                                    instrument.
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getAvgConfigInt32 Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getAvgConfigInt32" (ByVal instrumentID As Int32, _
                                           ByVal channel As Int32, _
                                           ByVal parameterString As String, _
                                           ByRef value As Int32) As Int32
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

    Public Declare Function AcqrsD1_getAvgConfigReal64 Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getAvgConfigReal64" (ByVal instrumentID As Int32, _
                                            ByVal channel As Int32, _
                                            ByVal parameterString As String, _
                                            ByRef value As Double) As Int32
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

    Public Declare Function AcqrsD1_getChannelCombination Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getChannelCombination" (ByVal instrumentID As Int32, _
                                               ByRef nbrConvertersPerChannel As Int32, _
                                               ByRef usedChannels As Int32) As Int32
    ' Returns parameters of combined operation of multiple channels
    ' See remarks under 'AcqrsD1_configChannelCombination'
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getControlIO Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getControlIO" (ByVal instrumentID As Int32, _
                                      ByVal connector As Int32, _
                                      ByRef signal As Int32, _
                                      ByRef qualifier1 As Int32, _
                                      ByRef qualifier2 As Double) As Int32
    ' Returns the state of Control-IO connectors
    ' See remarks under 'AcqrsD1_configControlIO'
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

    Public Declare Function AcqrsD1_getExtClock Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getExtClock" (ByVal instrumentID As Int32, _
                                     ByRef clockType As Int32, _
                                     ByRef inputThreshold As Double, _
                                     ByRef delayNbrSamples As Int32, _
                                     ByRef refFrequency As Double, _
                                     ByRef sampFrequency As Double) As Int32
    ' Returns the (external) clock parameters of the digitizer
    ' See remarks under 'AcqrsD1_configExtClock'
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getFCounter Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getFCounter" (ByVal instrumentID As Int32, _
                                     ByRef signalChannel As Int32, _
                                     ByRef typeMeas As Int32, _
                                     ByRef targetValue As Double, _
                                     ByRef apertureTime As Double, _
                                     ByRef reserved As Double, _
                                     ByRef flags As Int32) As Int32
    ' Returns the current settings of the frequency counter
    ' See remarks under 'AcqrsD1_configFCounter'
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getHorizontal Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getHorizontal" (ByVal instrumentID As Int32, _
                                       ByRef sampInterval As Double, _
                                       ByRef delayTime As Double) As Int32
    ' Returns the current horizontal control parameters of the digitizer.
    ' See remarks under 'AcqrsD1_configHorizontal'
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getInstrumentData Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getInstrumentData" (ByVal instrumentID As Int32, _
                                           ByVal name As String, _
                                           ByRef serialNbr As Int32, _
                                           ByRef busNbr As Int32, _
                                           ByRef slotNbr As Int32) As Int32
    ' Returns some basic data about a specified digitizer
    '
    ' Values returned by the function:
    '
    ' 'name'           = model name (length < 32 characters)
    '                    The string 'name' must be pre-allocated as 
    '                    "Dim name As String = Space(32)"
    ' 'serialNbr'      = serial number
    ' 'busNbr'         = bus number where digitizer is located
    ' 'slotNbr'        = slot number where digitizer is located
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getInstrumentInfo Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getInstrumentInfo" (ByVal instrumentID As Int32, _
                                           ByVal parameterString As String, _
                                           ByRef infoValue As Int32) As Int32
    Public Declare Function AcqrsD1_getInstrumentInfo Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getInstrumentInfo" (ByVal instrumentID As Int32, _
                                           ByVal parameterString As String, _
                                           ByRef infoValue As Double) As Int32
    Public Declare Function AcqrsD1_getInstrumentInfo Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getInstrumentInfo" (ByVal instrumentID As Int32, _
                                           ByVal parameterString As String, _
                                           ByVal infoValue As String) As Int32
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
    '                    Data types marked as 'Vi32' in the manual should be 'Int32'
    '                    Data types marked as 'ViReal64' should be 'Double'
    '                    Data types markes as string must be pre-allocated as 
    '                    "Dim infoValue As String = Space(32)"
    '
    ' Returns one of the following ViStatus values:
    ' ACQIRIS_ERROR_PARAM_STRING_INVALID if 'parameterString' is invalid.
    ' ACQIRIS_ERROR_NOT_SUPPORTED        if the requested value is not supported by the 
    '                                    instrument.
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getMemory Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getMemory" (ByVal instrumentID As Int32, _
                                   ByRef nbrSamples As Int32, _
                                   ByRef nbrSegments As Int32) As Int32
    ' Returns the current memory control parameters of the digitizer.
    ' See remarks under 'AcqrsD1_configMemory'
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getMemoryEx Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getMemoryEx" (ByVal instrumentID As Int32, _
                                     ByRef nbrSamplesHi As UInt32, _
                                     ByRef nbrSamplesLo As UInt32, _
                                     ByRef nbrSegments As Int32, _
                                     ByRef nbrBanks As Int32, _
                                     ByRef flags As Int32) As Int32
    ' Returns the current memory control parameters of the digitizer.
    ' See remarks under 'AcqrsD1_configMemoryEx'
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getMode Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getMode" (ByVal instrumentID As Int32, _
                                 ByRef mode As Int32, _
                                 ByRef modifier As Int32, _
                                 ByRef flags As Int32) As Int32
    ' Returns the current operational mode of the digitizer.
    ' See remarks under 'AcqrsD1_configMode'
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getMultiInput Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getMultiInput" (ByVal instrumentID As Int32, _
                                       ByVal channel As Int32, _
                                       ByRef input As Int32) As Int32
    ' Returns the multiple input configuration on a channel
    ' See remarks under 'AcqrsD1_configMultiInput'
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getNbrChannels Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getNbrChannels" (ByVal instrumentID As Int32, _
                                       ByRef nbrChannels As Int32) As Int32
    ' Returns the number of channels on the specified digitizer.
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getNbrPhysicalInstruments Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getNbrPhysicalInstruments" (ByRef nbrInstruments As Int32) As Int32
    ' Returns the number of supported physical Acqiris digitizers found on the computer.
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETER1                if 'nbrInstruments' is NULL.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getSetupArray Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getSetupArray" (ByVal instrumentID As Int32, _
                                       ByVal channel As Int32, _
                                       ByVal setupType As Int32, _
                                       ByVal nbrSetupObj As Int32, _
                                       ByRef setupData As Int32, _
                                       ByRef nbrSetupObjReturned As Int32) As Int32
    ' Returns setup data array (typically used for on-board processing)
    ' See remarks under 'AcqrsD1_configSetupArray'
    '
    ' Returns one of the following ViStatus values:
    ' ACQIRIS_ERROR_NOT_SUPPORTED        if this function is not supported by the instrument.
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getTrigClass Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getTrigClass" (ByVal instrumentID As Int32, _
                                      ByRef trigClass As Int32, _
                                      ByRef sourcePattern As Int32, _
                                      ByRef validatePattern As Int32, _
                                      ByRef holdType As Int32, _
                                      ByRef holdValue1 As Double, _
                                      ByRef holdValue2 As Double) As Int32
    ' Returns the current trigger class control parameters of the digitizer.
    ' See remarks under 'AcqrsD1_configTrigClass'
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getTrigSource Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getTrigSource" (ByVal instrumentID As Int32, _
                                       ByVal channel As Int32, _
                                       ByRef trigCoupling As Int32, _
                                       ByRef trigSlope As Int32, _
                                       ByRef trigLevel1 As Double, _
                                       ByRef trigLevel2 As Double) As Int32
    ' Returns the current trigger source control parameters for a specified channel.
    ' See remarks under 'AcqrsD1_configTrigSource'
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_getTrigTV Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getTrigTV" (ByVal instrumentID As Int32, _
                                   ByVal channel As Int32, _
                                   ByRef standard As Int32, _
                                   ByRef field As Int32, _
                                   ByRef line As Int32) As Int32
    ' Returns the current TV trigger control parameters of the digitizer.
    ' See remarks under 'AcqrsD1_AcqrsD1_configTrigTV'
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.


    Public Declare Function AcqrsD1_getVersion Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getVersion" (ByVal instrumentID As Int32, _
                                    ByVal versionItem As Int32, _
                                    ByRef version As Int32) As Int32
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

    Public Declare Function AcqrsD1_getVertical Lib "AqDrv4_x64" _
    Alias "AcqrsD1_getVertical" (ByVal instrumentID As Int32, _
                                     ByVal channel As Int32, _
                                     ByRef fullScale As Double, _
                                     ByRef offset As Double, _
                                     ByRef coupling As Int32, _
                                     ByRef bandwidth As Int32) As Int32
    ' Returns the current vertical control parameters for a specified channel.
    ' See remarks under 'AcqrsD1_configVertical'
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                if one of the parameters has an invalid value, where 
    '                                    x = parameter number.
    ' VI_SUCCESS                         otherwise.

    Public Declare Function AcqrsD1_init Lib "AqDrv4_x64" _
    Alias "AcqrsD1_init" (ByVal resourceName As String, _
                              ByVal IDQuery As Boolean, _
                              ByVal resetDevice As Boolean, _
                              ByRef instrumentID As Int32) As Int32
    ' Initializes an instrument
    ' See remarks under 'AcqrsD1_initWithOptions'

    Public Declare Function AcqrsD1_InitWithOptions Lib "AqDrv4_x64" _
    Alias "AcqrsD1_InitWithOptions" (ByVal resourceName As String, _
                                         ByVal IDQuery As Boolean, _
                                         ByVal resetDevice As Boolean, _
                                         ByVal optionsString As String, _
                                         ByRef instrumentID As Int32) As Int32
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

    Public Declare Function AcqrsD1_logicDeviceIO Lib "AqDrv4_x64" _
    Alias "AcqrsD1_logicDeviceIO" (ByVal instrumentID As Int32, _
                                       ByVal deviceName As String, _
                                       ByVal registerID As Int32, _
                                       ByVal nbrValues As Int32, _
                                       ByVal dataArray As System.IntPtr, _
                                       ByVal readWrite As Int32, _
                                       ByVal modifier As Int32) As Int32
    ' Currently ONLY for APxxx, SCxxx and ACxxx!
    '
    ' Reads/writes a number of 32-bit data values from/to a user-defined register in the
    ' logic device identified by 'deviceName[]'. 
    ' ONLY useful for a digitizer with on-board programmable logic devices.
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
    ' 'flags'            : bit31=1 forces not to use DMA transfer to FPGA
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

    Public Declare Function AcqrsD1_multiInstrAutoDefine Lib "AqDrv4_x64" _
    Alias "AcqrsD1_multiInstrAutoDefine" (ByVal optionsString As String, _
                                             ByRef nbrInstruments As Int32) As Int32
    ' Automatically combines as many digitizers as possible to "MultiInstrument"s.
    ' Digitizers are only combined if they are physically connected via ASBus.
    ' This call must be followed by 'nbrInstruments' calls to 'AcqrsD1_init' or
    ' 'AcqrsD1_InitWithOptions' to retrieve the 'instrumentID's of the (multi)digitizers.
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

    Public Declare Function AcqrsD1_multiInstrDefine Lib "AqDrv4_x64" _
    Alias "AcqrsD1_multiInstrDefine" (ByRef instrumentList As Int32, _
                                          ByVal nbrInstruments As Int32, _
                                          ByVal masterID As Int32, _
                                          ByRef instrumentID As Int32) As Int32
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

    Public Declare Function AcqrsD1_multiInstrUndefineAll Lib "AqDrv4_x64" _
    Alias "AcqrsD1_multiInstrUndefineAll" (ByVal optionsString As String) As Int32
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

    Public Declare Function AcqrsD1_processData Lib "AqDrv4_x64" _
    Alias "AcqrsD1_processData" (ByVal instrumentID As Int32, _
                                     ByVal processType As Int32, _
                                     ByVal flags As Int32) As Int32
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

    Public Declare Function AcqrsD1_procDone Lib "AqDrv4_x64" _
    Alias "AcqrsD1_procDone" (ByVal instrumentID As Int32, ByRef done As Boolean) As Int32
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

    Public Declare Function AcqrsD1_readCharSequence Lib "AqDrv4_x64" _
    Alias "AcqrsD1_readCharSequence" (ByVal instrumentID As Int32, _
                                          ByVal channel As Int32, _
                                          ByVal firstSegment As Int32, _
                                          ByVal nbrSegments As Int32, _
                                          ByVal firstSampleInSeg As Int32, _
                                          ByVal nbrSamplesInSeg As Int32, _
                                          ByVal segmentOffset As Int32, _
                                          ByVal arraySize As Int32, _
                                          ByVal waveformArray As System.IntPtr, _
                                          ByRef horPos As Double, _
                                          ByRef sampTime As Double, _
                                          ByRef vGain As Double, _
                                          ByRef vOffset As Double, _
                                          ByVal timeStampLo As System.IntPtr, _
                                          ByVal timeStampHi As System.IntPtr) As Int32
    ' Returns a sequence waveform as an array of byte arrays.
    ' This function is faster than a loop over 'AcqrsD1_readCharWform' if many
    ' short segments (< 10'000 points/segment) are read. See Manual for details.
    '
    ' The following values must be supplied to the function:
    '
    ' 'channel'        = 1...Nchan (as returned by 'AcqrsD1_getNbrChannels' )
    ' 'firstSegment'   = requested first segment number, may assume 0 to 'nbrSegments - 1',
    '                    as set with the function 'AcqrsD1_configMemory'
    ' 'nbrSegments'    = requested number of segments, may assume 1 to the number of
    '                    segments set with the function 'AcqrsD1_configMemory'
    ' 'firstSampleInSeg' = requested position of first sample in segment to read,
    '                    typically 0. May assume 0 to 'nbrSamples - 1', as set with the
    '                    function 'AcqrsD1_configMemory'
    ' 'nbrSamplesInSeg'  = requested number of samples per segment, may assume 1 to the
    '                    number of samples set with the function 'AcqrsD1_configMemory'
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
    '                    of segments set with the function 'AcqrsD1_configMemory'.
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
    ' For other errors, please see 'readData'.

    Public Declare Function AcqrsD1_readCharWform Lib "AqDrv4_x64" _
    Alias "AcqrsD1_readCharWform" (ByVal instrumentID As Int32, _
                                  ByVal channel As Int32, _
                                  ByVal segmentNumber As Int32, _
                                  ByVal firstSample As Int32, _
                                  ByVal nbrSamples As Int32, _
                                  ByVal waveformArray As System.IntPtr, _
                                  ByRef returnedSamples As Int32, _
                                  ByRef addrFirstPoint As Int32, _
                                  ByRef horPos As Double, _
                                  ByRef sampTime As Double, _
                                  ByRef vGain As Double, _
                                  ByRef vOffset As Double, _
                                  ByVal timeStampLo As System.IntPtr, _
                                  ByVal timeStampHi As System.IntPtr) As Int32
    ' Returns a waveform as a byte array.
    '
    ' The following values must be supplied to the function:
    '
    ' 'channel'        = 1...Nchan (as returned by 'AcqrsD1_getNbrChannels' )
    ' 'segmentNumber'  = requested segment number, may assume 0 to 'nbrSegments - 1',
    '                    as set with the function 'AcqrsD1_configMemory'
    ' 'firstSample'    = requested position of first sample to read, typically 0
    ' 'nbrSamples'     = requested number of samples, may assume 1 to 'nbrSamples', as
    '                    set with the function 'AcqrsD1_configMemory'
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

    Public Declare Function AcqrsD1_readData Lib "AqDrv4_x64" _
    Alias "AcqrsD1_readData" (ByVal instrumentID As Int32, _
                                  ByVal channel As Int32, _
                                  ByRef readPar As AqReadParameters, _
                                  ByVal dataArray As System.IntPtr, _
                                  ByRef dataDesc As AqDataDescriptor, _
                                  ByVal segDescArray As System.IntPtr) As Int32
    ' Returns a waveform and associated descriptors, in one of several possible formats
    ' This function is the preferred way of reading waveforms, since it includes the capabilities
    ' of all the other read functions, and more!
    '
    ' The following values must be supplied to the function:
    '
    ' 'channel'        = 1...Nchan (as returned by 'AcqrsD1_getNbrChannels' )
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
    ' ACQIRIS_ERROR_READMODE                 if 'readPar->readMode' is not valid.
    ' ACQIRIS_ERROR_NOT_SUPPORTED            if 'readPar->readMode' is not supported by the instrument.
    ' ACQIRIS_ERROR_DATATYPE                 if 'readPar->dataType' is not valid for the 
    '                                        chosen 'readPar->readMode' or for this instrument.
    ' ACQIRIS_ERROR_FIRST_SEG                if 'readPar->firstSegment' is invalid.
    ' ACQIRIS_ERROR_NBR_SEG                  if 'readPar->nbrSegments' is invalid.
    ' ACQIRIS_ERROR_DATA_ARRAY or
    ' ACQIRIS_ERROR_NBR_SAMPLE               if 'readPar->dataArraySize' is invalid.
    ' ACQIRIS_ERROR_SEG_DESC_ARRAY           if 'readPar->segDescArraySize' is invalid.
    ' ACQIRIS_ERROR_SEG_OFF                  if 'readPar->segmentOffset' is invalid.
    ' ACQIRIS_ERROR_NBR_SEG                  if 'readPar->nbrSegments' is invalid.
    ' ACQIRIS_ERROR_BUFFER_OVERFLOW          if 'readPar->dataArraySize' is too small.
    ' ACQIRIS_ERROR_NO_DATA                  if nothing was acquired beforehand.
    ' ACQIRIS_ERROR_CANNOT_READ_THIS_CHANNEL if the requested channel is not available.
    ' ACQIRIS_ERROR_READ_TIMEOUT             if the reading encountered a problem.
    ' ACQIRIS_WARN_READPARA_NBRSEG_ADAPTED   if 'readPar->nbrSegments' has been adapted.
    ' ACQIRIS_WARN_ACTUAL_DATASIZE_ADAPTED   if 'readPar->dataArraySize' has been adapted.
    ' ACQIRIS_WARN_READPARA_FLAGS_ADAPTED    if 'readPar->flags' has been adapted.
    ' VI_ERROR_PARAMETERx                    if one of the parameters has an invalid value, where 
    '                                        'x' = parameter number.
    ' VI_SUCCESS                             otherwise.

    Public Declare Function AcqrsD1_readFCounter Lib "AqDrv4_x64" _
    Alias "AcqrsD1_readFCounter" (ByVal instrumentID As Int32, _
                                     ByRef result As Double) As Int32
    ' Reads the frequency counter
    ' 'result'		= result of measurement, whose units depend on the measurement 'type':
    '				  Hz     for type = 0 (Frequency)
    '				  sec    for type = 1 (Period)
    '				  counts for type = 2 (Totalize)
    '
    ' Common return values:
    ' ACQIRIS_ERROR_NOT_SUPPORTED if this function is not supported by the instrument.
    ' ACQIRIS_ERROR_NO_DATA       if the instrument has not first acquired data in the 'frequency 
    '                             counter' mode (mode = 6).
    ' VI_ERROR_PARAMETERx         if one of the parameters has an invalid value, where 
    '                             'x' = parameter number.
    ' VI_SUCCESS                  otherwise.

    Public Declare Function AcqrsD1_readRealSequence Lib "AqDrv4_x64" _
    Alias "AcqrsD1_readRealSequence" (ByVal instrumentID As Int32, _
                                          ByVal channel As Int32, _
                                          ByVal firstSegment As Int32, _
                                          ByVal nbrSegments As Int32, _
                                          ByVal firstSampleInSeg As Int32, _
                                          ByVal nbrSamplesInSeg As Int32, _
                                          ByVal segmentOffset As Int32, _
                                          ByVal arraySize As Int32, _
                                          ByVal waveformArray As System.IntPtr, _
                                          ByRef horPos As Double, _
                                          ByRef sampTime As Double, _
                                          ByVal timeStampLo As System.IntPtr, _
                                          ByVal timeStampHi As System.IntPtr) As Int32
    ' Returns a sequence waveform as an array of floating point (double) arrays, with the
    ' measured data values in Volts.
    ' This function is faster than a loop over 'AcqrsD1_readRealWform' if many
    ' short segments (< 10'000 points/segment) are read.
    '
    ' See remarks under 'AcqrsD1_readCharSequence'

    Public Declare Function AcqrsD1_readRealWform Lib "AqDrv4_x64" _
    Alias "AcqrsD1_readRealWform" (ByVal instrumentID As Int32, _
                                       ByVal channel As Int32, _
                                       ByVal segmentNumber As Int32, _
                                       ByVal firstSample As Int32, _
                                       ByVal nbrSamples As Int32, _
                                       ByVal waveformArray As System.IntPtr, _
                                       ByRef returnedSamples As Int32, _
                                       ByRef horPos As Double, _
                                       ByRef sampTime As Double, _
                                       ByVal timeStampLo As System.IntPtr, _
                                       ByVal timeStampHi As System.IntPtr) As Int32
    ' Returns a waveform as a floating point (double) array, with the measured data values in Volts.
    ' See remarks under 'AcqrsD1_readCharWform'

    Public Declare Function AcqrsD1_reportNbrAcquiredSegments Lib "AqDrv4_x64" _
    Alias "AcqrsD1_reportNbrAcquiredSegments" (ByVal instrumentID As Int32, _
                                                  ByRef nbrSegments As Int32) As Int32
    ' Returns the number of segments already acquired.
    ' Can be called during the acquisition period, in order to follow the progress of a
    ' Sequence acquisition.
    ' Can be called after an acquisition, in order to obtain the number of segments actually
    ' acquired (until 'AcqrsD1_stopAcquisition' was called).
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                  if one of the parameters has an invalid value, where 
    '                                      'x' = parameter number.
    ' VI_SUCCESS                           otherwise.

    Public Declare Function AcqrsD1_reset Lib "AqDrv4_x64" _
    Alias "AcqrsD1_reset" (ByVal instrumentID As Int32) As Int32
    ' Resets an instrument
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETER1                     if 'instrumentID' is invalid.
    ' VI_SUCCESS                              otherwise.

    Public Declare Function AcqrsD1_resetDigitizerMemory Lib "AqDrv4_x64" _
    Alias "AcqrsD1_resetDigitizerMemory" (ByVal instrumentID As Int32) As Int32
    ' Resets the digitizer memory to a known default state, ONLY useful
    ' for a digitizer with the battery back-up option.
    '
    ' Returns one of the following ViStatus values:
    ' ACQIRIS_ERROR_INSTRUMENT_RUNNING    if the instrument is already running. 
    ' VI_ERROR_PARAMETER1                 if 'instrumentID' is invalid.
    ' VI_SUCCESS                          otherwise.

    Public Declare Function AcqrsD1_restoreInternalRegisters Lib "AqDrv4_x64" _
    Alias "AcqrsD1_restoreInternalRegisters" (ByVal instrumentID As Int32, _
                                                  ByVal delayOffset As Double, _
                                                  ByVal delayScale As Double) As Int32
    ' Restores some internal registers of an instrument, needed ONLY after power-up
    ' of a digitizer with the battery back-up option.
    ' Please refer to the User's manual for a detailed description of the steps required
    ' to read battery backed-up waveforms.
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETERx                  if one of the parameters has an invalid value, where 
    '                                      'x' = parameter number.
    ' VI_SUCCESS                           otherwise.

    Public Declare Function AcqrsD1_setAttributeString Lib "AqDrv4_x64" _
    Alias "AcqrsD1_setAttributeString" (ByVal instrumentID As Int32, _
                                            ByVal channel As int32, _
                                            ByVal name As String, _
                                            ByVal value As String) As int32
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

    Public Declare Function AcqrsD1_setLEDColor Lib "AqDrv4_x64" _
    Alias "AcqrsD1_setLEDColor" (ByVal instrumentID As Int32, _
                                    ByVal color As Int32) As Int32
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

    Public Declare Function AcqrsD1_setSimulationOptions Lib "AqDrv4_x64" _
    Alias "AcqrsD1_setSimulationOptions" (ByVal simOptionString As String) As Int32
    ' Sets one or several options which will be used by the function 'AcqrsD1_InitWithOptions',
    ' provided that the 'optionString' supplied to 'AcqrsD1_InitWithOptions' contains the
    ' string 'simulate=TRUE' (or similar).
    ' Refer to the manual for the accepted form of 'simOptionString'.
    ' The simulation options are reset to none by setting 'simOptionString' to an empty string "".
    '
    ' Returns one of the following ViStatus values:
    ' VI_SUCCESS always.

    Public Declare Function AcqrsD1_stopAcquisition Lib "AqDrv4_x64" _
    Alias "AcqrsD1_stopAcquisition" (ByVal instrumentID As Int32) As Int32
    ' Stops the acquisition immediately
    '
    ' Returns one of the following ViStatus values:
    ' VI_ERROR_PARAMETER1                  if 'instrumentID' is invalid.
    ' VI_SUCCESS                           otherwise.

    Public Declare Function AcqrsD1_stopProcessing Lib "AqDrv4_x64" _
    Alias "AcqrsD1_stopProcessing" (ByVal instrumentID As Int32) As Int32
    ' Stops the on-board processing immediately (only in instruments with on-board data processing)
    '
    ' Returns one of the following ViStatus values:
    ' ACQIRIS_ERROR_NOT_SUPPORTED      if this function is not supported by the instrument.
    ' VI_ERROR_PARAMETERx              if one of the parameters has an invalid value, where 
    '                                  'x' = parameter number.
    ' VI_SUCCESS                       otherwise.

    Public Declare Function AcqrsD1_waitForEndOfAcquisition Lib "AqDrv4_x64" _
    Alias "AcqrsD1_waitForEndOfAcquisition" (ByVal instrumentID As Int32, _
                                                ByVal timeout As Int32) As Int32
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

    Public Declare Function AcqrsD1_waitForEndOfProcessing Lib "AqDrv4_x64" _
    Alias "AcqrsD1_waitForEndOfProcessing" (ByVal instrumentID As Int32, _
                                               ByVal timeout As Int32) As Int32
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

End Module
