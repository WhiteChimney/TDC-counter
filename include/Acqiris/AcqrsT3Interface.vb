Imports System.Runtime.InteropServices

Module AcqrsT3Interface
    '/////////////////////////////////////////////////////////////////////////////////////////
    '
    '  AcqrsT3Interface.vb : Declaration of Acqiris device driver API 
    '  for Visual Basic Dot Net
    '----------------------------------------------------------------------------------------
    '  Copyright (C) 2007, 2007-2010 Agilent. All rights reserved.
    '
    '/////////////////////////////////////////////////////////////////////////////////////////
    '  $Rev: 38649 $                  $Date: 2010-03-22 10:05:29 +0100 (Mon, 22 Mar 2010) $

    '//////////////////////////////////////////////////////////////////////////////////////////      
    '// General comments about the function prototypes:
    '//
    '// - All function calls require the argument 'instrumentID' in order to identify
    '//   the Acqiris Timer card to which the call is directed.
    '//
    '// - All function calls return a status value of type 'ViStatus' with information about
    '//   the success or failure of the call.
    '//   The Acqiris specific values are declared in the file "AcqirisErrorCodes.h".
    '//   The generic ones, defined by the VXIplug&play Systems Alliance, are listed in the
    '//   file 'vpptype.h'.
    '//
    '// - If important parameters supplied by the user (e.g. an instrumentID) are found
    '//   to be invalid, most functions do not execute and return an error code of the type
    '//   VI_ERROR_PARAMETERi, where i = 1, 2, ... corresponds to the argument number.
    '//
    '// - Data are always returned through pointers to user-allocated variables or arrays.
    '//
    '//////////////////////////////////////////////////////////////////////////////////////////      

    Public Structure AqT3ReadParametersInt32
        Public dataArray As Int32          ' Pointer to user allocated memory
        Public dataSizeInBytes As UInt32   ' Size of user allocated memory in bytes
        Public nbrSamples As Int32         ' Number of samples requested
        Public dataType As Int32           ' Use 'enum AqReadType' defined above
        Public readMode As Int32           ' Use 'enum AqT3ReadModes' defined below
        Public reserved3 As Int32          ' Reserved, must be 0
        Public reserved2 As Int32          ' Reserved, must be 0
        Public reserved1 As Int32          ' Reserved, must be 0
    End Structure

    Public Structure AqT3ReadParametersDbl
        Public dataArray As Double         ' Pointer to user allocated memory
        Public dataSizeInBytes As UInt32   ' Size of user allocated memory in bytes
        Public nbrSamples As Int32         ' Number of samples requested
        Public dataType As Int32           ' Use 'enum AqReadType' defined above
        Public readMode As Int32           ' Use 'enum AqT3ReadModes' defined below
        Public reserved3 As Int32          ' Reserved, must be 0
        Public reserved2 As Int32          ' Reserved, must be 0
        Public reserved1 As Int32          ' Reserved, must be 0
    End Structure

    Public Structure AqT3DataDescriptorInt32
        Public dataPtr As Int32            ' Pointer to returned data (same as user buffer if aligned)
        Public nbrSamples As Int32         ' Number of samples returned
        Public sampleSize As Int32         ' Size in bytes of one data sample
        Public sampleType As Int32         ' Type of the returned samples, see AqT3SampleType below
        Public flags As Int32              ' Readout flags
        Public reserved3 As Int32          ' Reserved, will be 0
        Public reserved2 As Int32          ' Reserved, will be 0
        Public reserved1 As Int32          ' Reserved, will be 0
    End Structure

    Enum AqT3ReadModes
        AqT3ReadStandard    ' Standard read mode
        AqT3ReadContinuous  ' Continuous read mode
        nbrAqT3ReadModes
    End Enum

    Enum AqT3SampleType
        AqT3SecondReal64    ' Double value in seconds
        AqT3Count50psInt32  ' Count of 50 ps
        AqT3Count5psInt32   ' Count of 5 ps
        AqT3Struct50ps6ch   ' Struct on 32 bits with (LSB to MSB) 27 bits count of 50 ps, 3 bits channel number, 1 bit overflow
        nbrAqT3SampleType
    End Enum

    ' Checks if the acquisition has terminated.
    ' 
    '  'doneP' = Indicator of termination.
    '            VI_TRUE: the acquisition has finished
    '            VI_FALSE: the acquisition is still running
    Public Declare Function AcqrsT3_acqDone Lib "AqDrv4" _
    Alias "_AcqrsT3_acqDone@8" (ByVal instrumentID As Int32, _
                                ByRef doneP As Boolean) As Int32


    ' Starts an acquisition.
    Public Declare Function AcqrsT3_acquire Lib "AqDrv4" _
    Alias "_AcqrsT3_acquire@4" (ByVal instrumentID As Int32) As Int32


    ' Configures the acquisiton conditions of the Time Counter.
    '
    '  'timeout'  = Value of timeout in seconds. The value 0.0 means no timeout,
    '                it is not acceptable in some modes.
    '  
    '  'flags'    = unused, must be 0
    '    
    '  'reserved' = unused, must be 0
    '
    Public Declare Function AcqrsT3_configAcqConditions Lib "AqDrv4" _
    Alias "_AcqrsT3_configAcqConditions@20" (ByVal instrumentID As Int32, _
                                             ByVal timeout As Double, _
                                             ByVal flags As Int32, _
                                             ByVal reserved As Int32) As Int32


    ' Configures the channel source control parameters of the Time Counter.
    ' 
    '  'channel'  = 1...nbrChannels (as returned by 'Acqrs_getNbrChannels' )
    '             = -1 for the Common Channel
    '             = -2 for the Veto Input
    '    
    '  'mode'     = Bitfield
    '       bit0  = 0: positive slope / 1: negative slope
    '       bit31 = 0: active channel / 1: inactive channel
    '    
    '  'level'    = Threshold value in Volts
    '  
    '  'reserved' = unused, must be 0
    '    
    Public Declare Function AcqrsT3_configChannel Lib "AqDrv4" _
    Alias "_AcqrsT3_configChannel@24" (ByVal instrumentID As Int32, _
                                       ByVal channel As Int32, _
                                       ByVal mode As Int32, _
                                       ByVal level As Double, _
                                       ByVal reserved As Int32) As Int32


    ' Configures Control-IO connectors of the Time Counter.
    ' 
    '  'connector'   =  1: Front Panel I/O Aux 1
    '                =  2: Front Panel I/O Aux 2
    '                = 13: Front Panel Veto Input
    '
    '  'signal'      = value depends on 'connector', refer to manual for definitions.
    ' 
    '  'qualifier1'  = Bitfield
    '           bit0 =  1: use 50 Ohm termination / 0: don't
    '
    '  'qualifier2'  = currently unused (set to zero!)
    '
    Public Declare Function AcqrsT3_configControlIO Lib "AqDrv4" _
    Alias "_AcqrsT3_configControlIO@24" (ByVal instrumentID As Int32, _
                                         ByVal connector As Int32, _
                                         ByVal signal As Int32, _
                                         ByVal qualifier1 As Int32, _
                                         ByVal qualifier2 As Double) As Int32


    ' Configures the memory switch conditions.
    ' 
    '  'switchEnable' = Bitfield that identifies the event that switches the bank
    '                    1: switch on auxiliary I/O control.
    '                    2: switch on count of common events.
    '                    4: switch on memory size reached.
    ' 
    '  'countEvent'   = Number of events on common channel before switching.
    '    
    '  'sizeMemory'   = Size of memory to fill before switching.
    '    
    '  'reserved'     = unused, must be 0
    '    
    Public Declare Function AcqrsT3_configMemorySwitch Lib "AqDrv4" _
    Alias "_AcqrsT3_configMemorySwitch@20" (ByVal instrumentID As Int32, _
                                            ByVal switchEnable As Int32, _
                                            ByVal countEvent As Int32, _
                                            ByVal sizeMemory As Int32, _
                                            ByVal reserved As Int32) As Int32


    ' Configures the operational mode of the Time Counter.
    ' 
    '  'mode' = Operation mode.
    '            1: Standard acquisition
    '            2: Time Of Flight acquisition
    '                
    '  'modifier'
    '      ['mode' = {1}]
    '          bit0 =    0: single hit / 1: multiple hits
    '    
    '  'flags'
    '      ['mode' = {1,2}]
    '          bit0 =    0: use internal clock reference / 1: use external clock reference
    '    
    Public Declare Function AcqrsT3_configMode Lib "AqDrv4" _
    Alias "_AcqrsT3_configMode@16" (ByVal instrumentID As Int32, _
                                    ByVal mode As Int32, _
                                    ByVal modifier As Int32, _
                                    ByVal flags As Int32) As Int32

    ' Generate a COMMON hit by software. Currently only supported on TC890.
    '
    ' The function returns immediately after initiating a trigger.
    ' All parameters except 'instrumentID' are currently unused and should be set to 0
    '
    Public Declare Function AcqrsT3_forceTrig Lib "AqDrv4" _
    Alias "_AcqrsT3_forceTrig@16" (ByVal instrumentID As Int32, _
                                    ByVal forceTrigType As Int32, _
                                    ByVal modifier As Int32, _
                                    ByVal flags As Int32) As Int32

    ' Gets the acquisition conditions of the Time Counter.
    '
    '  'timeoutP', 'flagsP' and 'reservedP' : can be NULL, nothing will be returned.
    '    
    '  For the meaning of the returned values, refer to AcqrsT3_configAcqConditions
    '    
    Public Declare Function AcqrsT3_getAcqConditions Lib "AqDrv4" _
    Alias "_AcqrsT3_getAcqConditions@16" (ByVal instrumentID As Int32, _
                                          ByRef timeoutP As Double, _
                                          ByRef flagsP As Int32, _
                                          ByRef reservedP As Int32) As Int32


    ' Gets the channel source control parameters of the Time Counter.
    ' 
    '  'channel' = 1...nbrChannels: (as returned by 'Acqrs_getNbrChannels' )
    '            = -1: for the common channel
    '            = -2 for the Veto Input
    '    
    '  'modeP', 'levelP' and 'reservedP' can be NULL, nothing will be returned
    '    
    '  For the meaning of the returned values, refer to AcqrsT3_configChannel
    '    
    Public Declare Function AcqrsT3_getChannel Lib "AqDrv4" _
    Alias "_AcqrsT3_getChannel@20" (ByVal instrumentID As Int32, _
                                    ByVal channel As Int32, _
                                    ByRef modeP As Int32, _
                                    ByRef levelP As Double, _
                                    ByRef reservedP As Int32) As Int32


    ' Gets the Control-IO connectors configuration of the Time Counter.
    ' 
    '  'connector'  = Gives connector to get parameters from
    '                  1: Front Panel I/O Aux 1
    '                  2: Front Panel I/O Aux 2
    '                 13: Front Panel Veto Input
    '
    '  'signalP', 'qualifier1P' and 'qualifier2P' can be NULL, nothing will be returned
    '    
    '  For the meaning of the returned values, refer to AcqrsT3_configControlIO
    '
    Public Declare Function AcqrsT3_getControlIO Lib "AqDrv4" _
    Alias "_AcqrsT3_getControlIO@20" (ByVal instrumentID As Int32, _
                                      ByVal connector As Int32, _
                                      ByRef signalP As Int32, _
                                      ByRef qualifier1P As Int32, _
                                      ByRef qualifier2P As Double) As Int32


    ' Gets the memory switch conditions of the Time Counter.
    ' 
    '  'switchEnableP', 'contuEventP', 'sizeMemory2P' and 'reservedP' can be NULL,
    '  nothing will be returned
    '    
    '  For the meaning of the returned values, refer to AcqrsT3_configMemorySwitch
    '    
    Public Declare Function AcqrsT3_getMemorySwitch Lib "AqDrv4" _
    Alias "_AcqrsT3_getMemorySwitch@20" (ByVal instrumentID As Int32, _
                                         ByRef switchEnableP As Int32, _
                                         ByRef countEventP As Int32, _
                                         ByRef sizeMemoryP As Int32, _
                                         ByRef reservedP As Int32) As Int32


    ' Gets the operational mode of the Time Counter.
    ' 
    '  'modeP', 'modifierP' and 'flagsP' can be NULL, nothing will be returned.
    '    
    '  For the meaning of the returned values, refer to AcqrsT3_configMode
    '    
    Public Declare Function AcqrsT3_getMode Lib "AqDrv4" _
    Alias "_AcqrsT3_getMode@16" (ByVal instrumentID As Int32, _
                                 ByRef modeP As Int32, _
                                 ByRef modifierP As Int32, _
                                 ByRef flagsP As Int32) As Int32

    ' Returns the data acquired by the Time Counter as Int32 values.
    '
    '  'channel'    = unused, must be 0.
    '
    '  'readParP'   = pointer to a user-supplied structure that specifies what and how to read.
    '    
    '  'dataArrayP' = pointer to the memory location to write the data to
    '
    '  'dataDescP'  = user-allocated structure for returned data and data descriptors.
    '    
    Public Declare Function AcqrsT3_readDataInt32 Lib "AqDrv4" _
    Alias "_AcqrsT3_readDataInt32@20" (ByVal instrumentID As Int32, _
                                    ByVal channel As Int32, _
                                    ByRef readPar As AqT3ReadParametersInt32, _
                                    ByVal datArrayP As System.IntPtr, _
                                    ByRef dataDesc As AqT3DataDescriptorInt32) As Int32

    ' Returns the data acquired by the Time Counter as Real64 values.
    '
    '  'channel'    = unused, must be 0.
    '
    '  'readParP'   = pointer to a user-supplied structure that specifies what and how to read.
    '    
    '  'dataArrayP' = pointer to the memory location to write the data to
    '
    '  'dataDescP'  = user-allocated structure for returned data and data descriptors.
    '    
    '
    Public Declare Function AcqrsT3_readDataReal64 Lib "AqDrv4" _
    Alias "_AcqrsT3_readDataReal64@20" (ByVal instrumentID As Int32, _
                                    ByVal channel As Int32, _
                                    ByRef readPar As AqT3ReadParametersInt32, _
                                    ByVal datArrayP As System.IntPtr, _
                                    ByRef dataDesc As AqT3DataDescriptorInt32) As Int32

    ' Stops the acquisition immediately
    '
    Public Declare Function AcqrsT3_stopAcquisition Lib "AqDrv4" _
    Alias "_AcqrsT3_stopAcquisition@4" (ByVal instrumentID As Int32) As Int32


    ' Waits until the end of the acquisition.
    '
    '  This function returns after the acquisition has terminated or after timeout,
    '  whichever is shorter.
    ' 
    '  This function puts the calling thread into 'idle' until it returns, permitting optimal 
    '  use of the CPU by other threads.
    '    
    '  'timeout' = Timeout duration in milliseconds. For protection, 'timeout' is
    '              internally clipped to a range of [0, 10000] milliseconds.
    '    
    '  Returns the following status values:
    '    ACQIRIS_ERROR_ACQ_TIMEOUT:         if the timeout duration has expired.
    '    ACQIRIS_ERROR_INSTRUMENT_STOPPED:  if the acquisition was not started beforehand
    '    VI_SUCCESS:                        otherwise, meaning the acquisition has ended.
    Public Declare Function AcqrsT3_waitForEndOfAcquisition Lib "AqDrv4" _
    Alias "_AcqrsT3_waitForEndOfAcquisition@8" (ByVal instrumentID As Int32, _
                                                ByVal timeout As Int32) As Int32

End Module
