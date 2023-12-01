#include "testwidget.h"
#include "ui_testwidget.h"
#include <QDebug>
#include <QList>
#include<QVector>
#include "stdafx.h"

// 测试子窗口

TestWidget::TestWidget(QWidget *parent, int index0) :
    QWidget(parent),
    ui(new Ui::TestWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);        // 在父窗口上显示独立的子窗口
    index = index0;
}

TestWidget::~TestWidget()
{
    delete ui;
}

void TestWidget::on_buttonStart_released()
{
    ui->textResult->setText("start test");
    ui->textResult->setText(this->testTC890());
    ui->textResult->adjustSize();
}

void TestWidget::on_buttonReturn_released()
{
    emit returnSignal(index);
}

QString TestWidget::testTC890()
{
/*    QString text = "";
    QString textResult = "";

    // Initializes instrument
    ViSession idInstr;
    Acqrs_InitWithOptions((ViRsrc)"PCI::INSTR0", VI_FALSE,
            VI_FALSE, "CAL=0", &idInstr);

    // Configure mode continuous and enable internal test signal
    ViInt32 modeContinuous = 2;
    AcqrsT3_configMode(idInstr, modeContinuous, 1, 2);

    // Configure timeout to very large
    ViReal64 timeout = 8.0;
    ViInt32 flags = 0;
    AcqrsT3_configAcqConditions(idInstr, timeout, flags, 0);

    // Configure bank switch on full size (8MB)
    ViInt32 switchEnable = 0x04;
    ViInt32 countEvents = 200000;  // unused, for demo purpose only
    ViInt32 memorySize = 1 * 1024 * 1024;
    AcqrsT3_configMemorySwitch(idInstr, switchEnable, countEvents,
            memorySize, 0);

    // Configure channels, common on negative slope, other left on positive
    ViInt32 slope = 1;
    ViReal64 threshold = 0.0;
    AcqrsT3_configChannel(idInstr, -1, slope, threshold, 0);

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
    Acqrs_calibrate(idInstr);

    // Start acquisitions
    AcqrsT3_acquire(idInstr);

    long countLast = 0;

    long const nbrSwitch = 12;
    for (long nSwitch = 0 ; nSwitch < nbrSwitch ; ++nSwitch)
    {
        // Wait for bank switch
        AcqrsT3_waitForEndOfAcquisition(idInstr, 8000);

        AqT3DataDescriptor dataDesc;
        ::memset(&dataDesc, 0, sizeof(dataDesc));

        // Read acquired data (acquisition continues on other bank)
        AcqrsT3_readData(idInstr, 0, &readParam, &dataDesc);

        // Marks read buffer as to-be-used for acquisition (enables next switch)
        AcqrsT3_acquire(idInstr);

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
                {
                    textResult.append(text.sprintf("Error: Gap in common count between %ld and %ld\n", countLast, count));
                }
                //printf("%d%c", count, 13);
                countLast = count;

            }

        }
        textResult.append(text.sprintf("Readout %ld (%d) last common %ld.\n", nSwitch, (int)dataDesc.nbrSamples, countLast));
        fflush(stdout);

    }

    // Stops the acquisition & close instruments
    AcqrsT3_stopAcquisition(idInstr);

//    Acqrs_closeAll();

    // Cleanup readout structures
    delete[] dataArrayP;

    return textResult; */

}
