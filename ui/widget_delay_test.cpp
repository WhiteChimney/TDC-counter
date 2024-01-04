//#include "widget.h"
//#include "ui_widget.h"


//void Widget::on_buttonDelayStart_released()
//{
//    sFile.setFileName(ui->textDelayTextFile->text());
//    if (!sFile.open(QFile::WriteOnly | QFile::Truncate))
//        return;
//    sStream.setDevice(&sFile);
//    sStream << "Time" << "\t"
//            << "TOF[0]" << "\t"
//            << "TOF[1]" << "\t"
//            << "TOF[2]" << "\n";
//    sStream << "ms" << "\t"
//            << "50 ps" << "\t"
//            << "50 ps" << "\t"
//            << "50 ps" << "\n";

//    period = int(1e9 / freq * 20);
//    calcPeriod = int(freq/50);
//    nCOM = 0;
//    TOF[0] = 0.0;
//    TOF[1] = 0.0;
//    TOF[2] = 0.0;
//    nTOF[0] = 0;
//    nTOF[1] = 0;
//    nTOF[2] = 0;

//    timeTic = dateTime.currentMSecsSinceEpoch();
//    connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,this,&Widget::dealDelayTest);
//}

//void Widget::dealDelayTest(AqT3DataDescriptor* dataDescPtr)
//{
//    long nbrSamples = dataDescPtr->nbrSamples;
//    for (long n = 0 ; n < nbrSamples ; ++n)
//    {
//        int sample = ((long *)dataDescPtr->dataPtr)[n];  //dataPtr指向time value data buffer
//        int channel = (sample & 0x70000000) >> 28;   //右移28位为channel位
//        int TimeOfFlight = sample & 0x0FFFFFFF;

//        // flag 为 0 表示 COM 信号，channel 为 7 表示内存切换
//        if (channel == 0 || channel == 7)                        // Channel=1-6 are the physical channels;
//                                                // Data = an integer giving the time value in units of 50 ps
//                                                // Channel=7 is for marker data.
//        {
//            nCOM++;
//            if (nCOM >= calcPeriod)
//            {
//                timeToc = dateTime.currentMSecsSinceEpoch();
//                timeRelative = timeToc - timeTic;
//                sStream << timeRelative << "\t"
//                        << TOF[0] << "\t"
//                        << TOF[1] << "\t"
//                        << TOF[2] << "\n";
//                TOF[0] = 0.0;
//                TOF[1] = 0.0;
//                TOF[2] = 0.0;
//                nTOF[0] = 0;
//                nTOF[1] = 0;
//                nTOF[2] = 0;
//                nCOM = 0;
//                qDebug() << currentPeriod++;
//            }
//        }
//        else
//        {
//            nTOF[channel-1]++;
//            TimeOfFlight %= period;
//            // 计算均值
//            TOF[channel-1] = (1-1.0/nTOF[channel-1])*TOF[channel-1] + double(TimeOfFlight)/nTOF[channel-1];
//    //            qDebug() << channel << TOF[channel-1];
//        }
//    }
//}

//void Widget::on_buttonDelayStop_released()
//{
//    disconnect(acqThread,&AcquisitionThread::acqThreadBankSwitch,this,&Widget::dealDelayTest);
//    sFile.close();
//}
