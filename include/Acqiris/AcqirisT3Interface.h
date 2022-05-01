//////////////////////////////////////////////////////////////////////////////////////////
//
//  AcqirisT3Interface.h:    Time Counter Driver Function Declarations
//
//----------------------------------------------------------------------------------------
//  Copyright (C) 2006-2009 Agilent_Technologies, Inc.
//
//  Purpose:    Declaration of AcqrsT3 Time Counter device driver API
//
//////////////////////////////////////////////////////////////////////////////////////////

#include "vpptype.h"
#include "AcqirisDataTypes.h"

// The following prefixes should be set outside of this file, typically by the
// AcqirisT3Import.h file, to specify the calling convention used for the function
// and whether it is exported or imported.
#if !defined(ACQ_DLL) || !defined(ACQ_CC)
#error AcqirisT3Interface.h should not be included directly, please use AcqirisT3Import.h instead.
#endif


//////////////////////////////////////////////////////////////////////////////////////////
// General comments about the function prototypes:
//
// - All function calls require the argument 'instrumentID' in order to identify
//   the Agilent_Technologies Time Counter to which the call is directed.
//
// - All function calls return a status value of type 'ViStatus' with information about
//   the success or failure of the call.
//   The Acqiris specific values are declared in the file "AcqirisErrorCodes.h".
//   The generic ones, defined by the VXIplug&play Systems Alliance, are listed in the
//   file 'vpptype.h'.
//
// - If important parameters supplied by the user (e.g. an instrumentID) are found
//   to be invalid, most functions do not execute and return an error code of the type
//   VI_ERROR_PARAMETERi, where i = 1, 2, ... corresponds to the argument number.
//
// - Data are always returned through pointers to user-allocated variables or arrays.
//
//////////////////////////////////////////////////////////////////////////////////////////



//! Checks if the acquisition has terminated.
/*!
 *  'doneP' = Indicator of termination.
 *             VI_TRUE: the acquisition has finished
 *            VI_FALSE: the acquisition is still running
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_acqDone(ViSession instrumentID, ViBoolean* doneP);



//! Starts an acquisition.
/*!
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_acquire(ViSession instrumentID);



//! Configures the acquisition conditions of the Time Counter.
/*!
 *  'timeout'  = Value of timeout in seconds. The value 0.0 means no timeout,
 *                it is not acceptable in some modes.
 *
 *  'flags'    = unused, must be 0
 *
 *  'reserved' = unused, must be 0
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_configAcqConditions(ViSession instrumentID, ViReal64 timeout,
    ViInt32 flags, ViInt32 reserved);



//! Configures the channel source control parameters of the Time Counter.
/*!
 *  'channel'  = 1...nbrChannels (as returned by 'Acqrs_getNbrChannels' )
 *             = -1 for the Common Channel
 *             = -2 for the Veto Input
 *
 *  'mode'     = Bit field
 *       bit0  = 0: positive slope / 1: negative slope
 *       bit31 = 0: active channel / 1: inactive channel
 *
 *  'level'    = Threshold value in Volts
 *
 *  'reserved' = unused, must be 0
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_configChannel(ViSession instrumentID, ViInt32 channel,
    ViInt32 mode, ViReal64 level, ViInt32 reserved);



//! Configures Control-IO connectors of the Time Counter.
/*!
 *  'connector'   =  1: Front Panel I/O Aux 1
 *                =  2: Front Panel I/O Aux 2
 *                = 13: Front Panel Veto Input
 *
 *  'signal'      = value depends on 'connector', refer to manual for definitions.
 *
 *  'qualifier1'  = Bit field
 *           bit0 =  1: use 50 Ohm termination / 0: don't
 *
 *  'qualifier2'  = currently unused (set to zero!)
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_configControlIO(ViSession instrumentID, ViInt32 connector,
    ViInt32 signal, ViInt32 qualifier1, ViReal64 qualifier2);



//! Configures the memory switch conditions.
/*!
 *  'switchEnable' = Bit field that identifies the event that switches the bank
 *                    1: switch on auxiliary I/O control.
 *                    2: switch on count of common events.
 *                    4: switch on memory size reached.
 *
 *  'countEvent'   = Number of events on common channel before switching.
 *
 *  'sizeMemory'   = Size of memory to fill before switching.
 *
 *  'reserved'     = unused, must be 0
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_configMemorySwitch(ViSession instrumentID,
    ViInt32 switchEnable, ViInt32 countEvent, ViInt32 sizeMemory, ViInt32 reserved);



//! Configures the operational mode of the Time Counter.
/*!
 *  'mode' = Operation mode.
 *            1: Standard acquisition
 *            2: Time Of Flight acquisition
 *
 *  'modifier'
 *      ['mode' = {1}]
 *          bit0 =    0: single hit / 1: multiple hits
 *
 *  'flags'
 *      ['mode' = {1,2}]
 *          bit0 =    0: use internal clock reference / 1: use external clock reference
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_configMode(ViSession instrumentID, ViInt32 mode,
    ViInt32 modifier, ViInt32 flags);



//! Generate a COMMON hit by software. Currently only supported on TC890.
/*!
 *    The function returns immediately after initiating a trigger.
 *    All parameters except 'instrumentID' are currently unused and must be set to 0.
*/
ACQ_DLL ViStatus ACQ_CC AcqrsT3_forceTrig(ViSession instrumentID, ViInt32 forceTrigType,
    ViInt32 modifier, ViInt32 flags);



//! Gets the acquisition conditions of the Time Counter.
/*!
 *  'timeoutP', 'flagsP' and 'reservedP' : can be NULL, nothing will be returned.
 *
 *  For the meaning of the returned values, refer to AcqrsT3_configAcqConditions
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_getAcqConditions(ViSession instrumentID, ViReal64 *timeoutP,
    ViInt32 *flagsP, ViInt32 *reservedP);



//! Gets the channel source control parameters of the Time Counter.
/*!
 *  'channel' = 1...nbrChannels: (as returned by 'Acqrs_getNbrChannels' )
 *            = -1: for the common channel
 *            = -2: for the Veto Input
 *
 *  'modeP', 'levelP' and 'reservedP' can be NULL, nothing will be returned
 *
 *  For the meaning of the returned values, refer to AcqrsT3_configChannel
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_getChannel(ViSession instrumentID, ViInt32 channel,
    ViInt32 *modeP, ViReal64 *levelP, ViInt32 *reservedP);



//! Gets the Control-IO connectors configuration of the Time Counter.
/*!
 *  'connector'  = Gives connector to get parameters from
 *                  1: Front Panel I/O Aux 1
 *                  2: Front Panel I/O Aux 2
 *                 13: Front Panel Veto Input
 *
 *  'signalP', 'qualifier1P' and 'qualifier2P' can be NULL, nothing will be returned
 *
 *  For the meaning of the returned values, refer to AcqrsT3_configControlIO
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_getControlIO(ViSession instrumentID, ViInt32 connector,
    ViInt32 *signalP, ViInt32 *qualifier1P, ViReal64 *qualifier2P);



//! Gets the memory switch conditions of the Time Counter.
/*!
 *  'switchEnableP', 'countEventP', 'sizeMemory2P' and 'reservedP' can be NULL,
 *  nothing will be returned
 *
 *  For the meaning of the returned values, refer to AcqrsT3_configMemorySwitch
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_getMemorySwitch(ViSession instrumentID,
    ViInt32 *switchEnableP, ViInt32 *countEventP, ViInt32 *sizeMemoryP, ViInt32 *reservedP);



//! Gets the operational mode of the Time Counter.
/*!
 *  'modeP', 'modifierP' and 'flagsP' can be NULL, nothing will be returned.
 *
 *  For the meaning of the returned values, refer to AcqrsT3_configMode
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_getMode(ViSession instrumentID, ViInt32* modeP,
    ViInt32* modifierP, ViInt32* flagsP);



//! Returns the data acquired by the Time Counter.
/*!
 *  'channel'  = unused, must be 0.
 *
 *  'readPar'  = pointer to a user-supplied structure that specifies what and how to read.
 *
 *  'dataDesc' = user-allocated structure for returned data and data descriptors.
 *
 *  Deprecated: Prefer the 'typed' functions 'AcqrsT3_readDataInt32' or 'AcqrsT3_readDataReal64'.
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_readData(ViSession instrumentID, ViInt32 channel,
       AqT3ReadParameters* readPar, AqT3DataDescriptor* dataDesc);



//! Returns the data acquired by the Time Counter as ViInt32 values.
/*!
 *  'channel'    = unused, must be 0.
 *
 *  'readParP'   = pointer to a user-supplied structure that specifies what and how to read.
 *                 'readParP->dataType' must be either ReadInt32 or ReadRawData.
 *                 'readParP->dataArray' is not used by this function and should be NULL.
 *
 *  'dataArrayP' = pointer to the memory location to write the data to. The buffer must
 *                 be 32 bits aligned, i. e. its address must be a multiple of 4.
 *
 *  'dataDescP'  = user-allocated structure for returned data and data descriptors.
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_readDataInt32(ViSession instrumentID, ViInt32 channel,
           AqT3ReadParameters* readParP, ViInt32* dataArrayP, AqT3DataDescriptor* dataDescP);



//! Returns the data acquired by the Time Counter as ViReal64 values.
/*!
 *  'channel'    = unused, must be 0.
 *
 *  'readParP'   = pointer to a user-supplied structure that specifies what and how to read.
 *                 'readParP->dataType' must be  ReadInt64.
 *                 'readParP->dataArray' is not used by this function and should be NULL.
 *
 *  'dataArrayP' = pointer to the memory location to write the data to. The buffer must
 *                 be 32 bits aligned, i. e. its address must be a multiple of 4.
 *
 *  'dataDescP'  = user-allocated structure for returned data and data descriptors.
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_readDataReal64(ViSession instrumentID, ViInt32 channel,
          AqT3ReadParameters* readParP,  ViReal64* dataArrayP, AqT3DataDescriptor* dataDescP);



//! Stops the acquisition immediately.
/*!
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_stopAcquisition(ViSession instrumentID);



//! Waits until the end of the acquisition.
/*!
 *  This function returns after the acquisition has terminated or after timeout,
 *  whichever comes first.
 *
 *  This function puts the calling thread into 'idle' until it returns, permitting optimal
 *  use of the CPU by other threads.
 *
 *  'timeout' = Timeout duration in milliseconds. For protection, 'timeout' is
 *              internally clipped to a range of [0, 10000] milliseconds.
 *
 *  Returns the following status values:
 *    ACQIRIS_ERROR_ACQ_TIMEOUT:         if the timeout duration has expired.
 *    ACQIRIS_ERROR_INSTRUMENT_STOPPED:  if the acquisition was not started beforehand
 *    VI_SUCCESS:                        otherwise, meaning the acquisition has ended.
 *
 */
ACQ_DLL ViStatus ACQ_CC AcqrsT3_waitForEndOfAcquisition(ViSession instrumentID, ViInt32 timeout);


