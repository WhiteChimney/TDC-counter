//////////////////////////////////////////////////////////////////////////////////////////
//
//  GetStartedTC890.cpp
//
//  This is a vey simple sample program to demonstrate the TC890 instrument
//  capabilities. It uses the internal signal ~10MHz to generate events. It
//  starts the acquisition and runs it for a number of bank switches. On each
//  readout, it checks for the common channel event counter not to miss any.
//
//----------------------------------------------------------------------------------------
//
//  Copyright Agilent Technologies Inc., 2000, 2001-2009
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <AcqirisImport.h>
#include <AcqirisT3Import.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // Initializes instrument
	ViSession idInstr;
	ViStatus status = Acqrs_InitWithOptions((ViRsrc)"PCI::INSTR0", VI_FALSE,
			VI_FALSE, "CAL=0", &idInstr);

	if (status != VI_SUCCESS)
		return printf("No instrument found.\n"), 1;

	// Configure mode continuous and enable internal test signal
	ViInt32 modeContinuous = 2;
	status = AcqrsT3_configMode(idInstr, modeContinuous, 1, 2);

    // Configure timeout to very large
    ViReal64 timeout = 8.0;
    ViInt32 flags = 0;
    status = AcqrsT3_configAcqConditions(idInstr, timeout, flags, 0);

    // Configure bank switch on full size (8MB)
    ViInt32 switchEnable = 0x04;
    ViInt32 countEvents = 200000;  // unused, for demo purpose only
    ViInt32 memorySize = 1 * 1024 * 1024;
    status = AcqrsT3_configMemorySwitch(idInstr, switchEnable, countEvents,
            memorySize, 0);

	// Configure channels, common on negative slope, other left on positive
    ViInt32 slope = 1;
    ViReal64 threshold = 0.0;
    status = AcqrsT3_configChannel(idInstr, -1, slope, threshold, 0);

    // Prepare readout structure and buffer
    size_t const arraySize = 8 * 1024 * 1024;
    char * const dataArrayP = new char[arraySize];

    AqT3ReadParameters readParam;
    ::memset(&readParam, 0, sizeof(readParam));
    readParam.dataArray = dataArrayP;
    readParam.dataSizeInBytes = arraySize;
    readParam.nbrSamples = 0;
    readParam.dataType = ReadRawData;
    readParam.readMode = AqT3ReadContinuous;

    // Calibrate instrument (as we specified not to on init)
	status = Acqrs_calibrate(idInstr);

    // Start acquisitions
	status = AcqrsT3_acquire(idInstr);

    long countLast = 0;

    long const nbrSwitch = 12;
	for (long nSwitch = 0 ; nSwitch < nbrSwitch ; ++nSwitch)
    {
        // Wait for bank switch
        status = AcqrsT3_waitForEndOfAcquisition(idInstr, 8000);

        AqT3DataDescriptor dataDesc;
        ::memset(&dataDesc, 0, sizeof(dataDesc));

        // Read acquired data (acquisition continues on other bank)
        status = AcqrsT3_readData(idInstr, 0, &readParam, &dataDesc);

        // Marks read buffer as to-be-used for acquisition (enables next switch)
        status = AcqrsT3_acquire(idInstr);

        // Simple check not to miss any common hit
        for (int n = 0 ; n < dataDesc.nbrSamples ; ++n)
        {
            long sample = ((long *)dataDesc.dataPtr)[n];
            long flag = (sample & 0x80000000) >> 31;
            long channel = (sample & 0x70000000) >> 28;
            long count = sample & 0x0FFFFFFF;

            if (flag == 0 && channel == 0)
            {
                if (count - countLast != 1)
                    printf("Error: Gap in common count between %ld and %ld\n", countLast, count);

				//printf("%d%c", count, 13);
                countLast = count;

            }

        }

        printf("Readout %ld (%d) last common %ld.\n", nSwitch, (int)dataDesc.nbrSamples, countLast);
        fflush(stdout);

	}

    // Stops the acquisition & close instruments
    status = AcqrsT3_stopAcquisition(idInstr);

	status = Acqrs_closeAll();

    // Cleanup readout structures
	delete[] dataArrayP;

	return 0;
}
