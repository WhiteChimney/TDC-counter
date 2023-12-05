#include "hyperEBQKDwidget.h"
#include "ui_hyperEBQKDwidget.h"

hyperentanglementQKD::hyperentanglementQKD(QWidget *parent, int m_index, int m_comOffset) :
    QWidget(parent),
    ui(new Ui::hyperentanglementQKD)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);        // 在父窗口上显示独立的子窗口
    index = m_index;

    COM_offset = m_comOffset;

}

hyperentanglementQKD::~hyperentanglementQKD()
{
    on_ButtonStopQKD_released();
    on_ButtonRetornQKD_released();
    delete ui;
}

void hyperentanglementQKD::fetchUiData()
{
    channelMark[0] = true; delayMulti[0] = int(20*ui->TDC1delay1->text().toDouble());
    channelMark[1] = true; delayMulti[1] = int(20*ui->TDC1delay2->text().toDouble());
    channelMark[2] = true; delayMulti[2] = int(20*ui->TDC1delay3->text().toDouble());
    channelMark[3] = true; delayMulti[3] = int(20*ui->TDC1delay4->text().toDouble());
    channelMark[4] = true; delayMulti[4] = int(20*ui->TDC1delay5->text().toDouble());
    channelMark[5] = true; delayMulti[5] = int(20*ui->TDC1delay6->text().toDouble());
    channelMark_2[0] = true; delayMulti_2[0] = int(20*ui->TDC2delay1->text().toDouble());
    channelMark_2[1] = true; delayMulti_2[1] = int(20*ui->TDC2delay2->text().toDouble());
    channelMark_2[2] = true; delayMulti_2[2] = int(20*ui->TDC2delay3->text().toDouble());
    channelMark_2[3] = true; delayMulti_2[3] = int(20*ui->TDC2delay4->text().toDouble());
    channelMark_2[4] = true; delayMulti_2[4] = int(20*ui->TDC2delay5->text().toDouble());
    channelMark_2[5] = true; delayMulti_2[5] = int(20*ui->TDC2delay6->text().toDouble());

    toleranceMulti = 30;    //符合窗口选择1.5ns
    zperiod = int(20*ui->Zperiod->text().toDouble());
    // 数据保存设置
    pathName = ui->textDataDirectory->currentText();
    fileName = ui->textDataFileName->text();
//    metaData = ui->textMetaData->toPlainText();
//    enableRecordTime = ui->checkboxRecordTime->isChecked();
//    recordTime = ui->textRecordTime->text().toDouble();

    pushUiData();
}

void hyperentanglementQKD::pushUiData()
{

//    多通道的数据
     ui->TDC1delay1->setText(QString::number(delayMulti[0]/20.0));
     ui->TDC1delay2->setText(QString::number(delayMulti[1]/20.0));
     ui->TDC1delay3->setText(QString::number(delayMulti[2]/20.0));
     ui->TDC1delay4->setText(QString::number(delayMulti[3]/20.0));
     ui->TDC1delay5->setText(QString::number(delayMulti[4]/20.0));
     ui->TDC1delay6->setText(QString::number(delayMulti[5]/20.0));
     ui->TDC2delay1->setText(QString::number(delayMulti_2[0]/20.0));
     ui->TDC2delay2->setText(QString::number(delayMulti_2[1]/20.0));
     ui->TDC2delay3->setText(QString::number(delayMulti_2[2]/20.0));
     ui->TDC2delay4->setText(QString::number(delayMulti_2[3]/20.0));
     ui->TDC2delay5->setText(QString::number(delayMulti_2[4]/20.0));
     ui->TDC2delay6->setText(QString::number(delayMulti_2[5]/20.0));
     ui->Zperiod->setText(QString::number(toleranceMulti/20.0));

}

void hyperentanglementQKD::on_ButtonRetornQKD_released()
{
    on_ButtonStopQKD_released();
    emit returnSignal(index);
}


void hyperentanglementQKD::on_ButtonStartQKD_released()
{
    on_ButtonStopQKD_released();
    fetchUiData();
    emit QKDTimerNeedsSync(index);
//    QKDSavable = true; // 此时可保存数据
    emit requestQKDParam(index);
}


void hyperentanglementQKD::on_ButtonStopQKD_released()
{
//   QKDSavable= false;
   emit askStopDealAcqBankSwitchQKD(index);
   emit QKDTimerStopsSync(index);

   clear2DintVector(&timeSeq);
   clear2DintVector(&timeSeqAcc);
   clear2DintVector(&channelSeq);
   clear2DintVector(&channelSeqAcc);
   clear2DintVector(&timeSeq_2);
   clear2DintVector(&timeSeqAcc_2);
   clear2DintVector(&channelSeq_2);
   clear2DintVector(&channelSeqAcc_2);
   clear2DintVector(&timeSeqX1);
   clear2DintVector(&timeSeqAccX1);
   clear2DintVector(&channelSeqX1);
   clear2DintVector(&channelSeqAccX1);
   clear2DintVector(&timeSeqX2);
   clear2DintVector(&timeSeqAccX2);
   clear2DintVector(&channelSeqX2);
   clear2DintVector(&channelSeqAccX2);
}

void hyperentanglementQKD::dealRequestQKDParam(int m_index,
                                               double *m_delayCN,
                                               double *m_delayCN_2,
                                               double m_freqCOM,
                                               int m_countEvents)
{
    if (index==m_index)
    {
        delayCN = m_delayCN;
        delayCN_2 = m_delayCN_2;
        freqCOM = m_freqCOM;
        countEvents = m_countEvents;

        nbrChannels = 0;
        nbrChannels_2 = 0;

        for (int i = 0; i < 6; i++)
        {
            if (channelMark[i])
            {
                channels[nbrChannels] = i;
                nbrChannels++;
            }
            if (channelMark_2[i])
            {
                channels_2[nbrChannels_2] = i;
                nbrChannels_2++;
            }
        }
        axbx11calc = &nbraxbx11;
        axbx12calc = &nbraxbx12;
        axbx22calc = &nbraxbx22;
        axbx11errorcalc = &nbraxbx11error;
        axbx22errorcalc = &nbraxbx22error;
        azbz11calc = &nbrazbz11;
        azbz12calc = &nbrazbz12;
        azbz22calc = &nbrazbz22;
        azbz11errorcalc = &nbrazbz11error;
        azbz22errorcalc = &nbrazbz22error;
        abandoncalc = &nbrnbrabandon;
        toleranceCalc = toleranceMulti;
        delayCalc = delayMulti;
        delayCalc_2 = delayMulti_2;


        //    预处理 TDC 参数
        double timeCOM = 1000000.0/freqCOM;           // 单位为 us
        timeCOMunit = int(20*1000.0*timeCOM);         // TDC 内部单位，50 ps
        double delayTotal[6] = {0.0};
        double delayTotal_2[6] = {0.0};

        double minDelay = delayCN[0] + delayCalc[0]/20.0/1000.0;
        for (int i = 0; i < 6; i++)
        {
            delayTotal[i] = delayCN[i] + delayCalc[i]/20.0/1000.0;
            delayTotal_2[i] = delayCN_2[i] + delayCalc_2[i]/20.0/1000.0;
            if (delayTotal[i] < minDelay)
                minDelay = delayTotal[i];
            if (delayTotal_2[i] < minDelay)
                minDelay = delayTotal_2[i];
        }

        maxNbrCOMdelay = 0;
        for (int i = 0; i < 6; i++)
        {
            delayTotal[i] -= minDelay;             // 保证所有延时均为非负
            delayTotal_2[i] -= minDelay;             // 保证所有延时均为非负
            nbrCOMdelay[i] = floor(delayTotal[i]/timeCOM);
            nbrCOMdelay_2[i] = floor(delayTotal_2[i]/timeCOM);
            if (nbrCOMdelay[i] > maxNbrCOMdelay)
                maxNbrCOMdelay = nbrCOMdelay[i];
            if (nbrCOMdelay_2[i] > maxNbrCOMdelay)
                maxNbrCOMdelay = nbrCOMdelay_2[i];
            delayInCOM[i] = int(20*1000.0*(delayTotal[i] - timeCOM*nbrCOMdelay[i]));
            delayInCOM_2[i] = int(20*1000.0*(delayTotal_2[i] - timeCOM*nbrCOMdelay_2[i]));

        }
        //    时间序列所需要保存的 COM 周期数量为 nbrCOMdelay 中的最大值 +2
        resizeSeqLength(&timeSeqX1, 3*(countEvents + maxNbrCOMdelay+2));
        resizeSeqLength(&channelSeqX1, 3*(countEvents + maxNbrCOMdelay+2));
        resizeSeqLength(&timeSeqX2, 3*(countEvents + maxNbrCOMdelay+2));
        resizeSeqLength(&channelSeqX2, 3*(countEvents + maxNbrCOMdelay+2));
        nbrChannels_X = nbrChannels + nbrChannels_2;

        if (COM_offset < 0)
        {
            COM_HEAD_X1 = (-COM_offset) % timeSeqX1.size();
            COM_HEAD_X2 = 0;
        }
        else
        {
            COM_HEAD_X1 = 0;
            COM_HEAD_X2 = COM_offset % timeSeqX2.size();
        }
        COM_HEAD_compute = 0;

        emit askDealAcqBankSwitchQKD(index);

    }
}

void hyperentanglementQKD::dealAcqThreadBankSwitchQKD(AqT3DataDescriptor* dataDescPtr)
{
    // TDC 1 先存数据
    computeQKDAcrossDevices_HOLD
            (dataDescPtr,
             timeSeqX1,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
             channelSeqX1,    // 升序排列后的时间，与通道编号一一对应
             nbrCOMdelay,
             delayInCOM,
             timeCOMunit,
             &COM_HEAD_X1);
}

void hyperentanglementQKD::dealAcqThreadBankSwitchQKD_2(AqT3DataDescriptor* dataDescPtr_2)
{
    computeQKDAcrossDevices_HOLD
            (dataDescPtr_2,
             timeSeqX2,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
             channelSeqX2,    // 升序排列后的时间，与通道编号一一对应
             nbrCOMdelay_2,
             delayInCOM_2,
             timeCOMunit,
             &COM_HEAD_X2);

    computeQKDAcrossDevices_COMPUTE(
                     timeSeqX1,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
                     channelSeqX1,    // 升序排列后的时间，与通道编号一一对应
                     timeSeqX2,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
                     channelSeqX2,    // 升序排列后的时间，与通道编号一一对应
                     nbrChannels, nbrChannels_2,
                     channels, channels_2,
                     axbx11calc, axbx12calc, axbx22calc, axbx11errorcalc, axbx22errorcalc,
                     azbz11calc, azbz12calc, azbz22calc, azbz11errorcalc, azbz22errorcalc,
                     abandoncalc,
                     zperiod,
                     toleranceMulti,
                     &COM_HEAD_X1, &COM_HEAD_X2, &COM_HEAD_compute);
}

void hyperentanglementQKD::dealTimeOut()
{
    //subspace encoding
    ui->SubSpace1XCount->display(nbraxbx11+nbraxbx11error);
    ui->SubSpace1XQBER->display((double)nbraxbx11error/(nbraxbx11+nbraxbx11error));
    ui->SubSpace1ZCount->display(nbrazbz11+nbrazbz11error);
    ui->SubSpace1ZQBER-> display((double)nbrazbz11error/(nbrazbz11+nbrazbz11error));
    ui->SubSpace2XCount-> display(nbraxbx22+nbraxbx22error);
    ui->SubSpace2XQBER -> display((double)nbraxbx22error/(nbraxbx22+nbraxbx22error));
    ui->SubSpace2ZCount-> display(nbrazbz22+nbrazbz22error);
    ui->SubSpace2ZQBER-> display((double)nbrazbz22error/(nbrazbz22+nbrazbz22error));
    // 4D coding
    ui -> FullSpaceXCount ->display(nbraxbx11+nbraxbx11error+nbraxbx22+nbraxbx22error+nbraxbx12);
    ui->FullSpaceXQBER -> display(1-((double)nbraxbx11+nbraxbx22)/(nbraxbx11+nbraxbx11error+nbraxbx22+nbraxbx22error+nbraxbx12));
    ui->FullSpaceZCount -> display(nbrazbz11+nbrazbz11error+nbrazbz22+nbrazbz22error+nbrazbz12);
    ui->FullSpaceZQBER -> display(1-((double)nbrazbz11+nbrazbz22)/(nbrazbz11+nbrazbz11error+nbrazbz22+nbrazbz22error+nbrazbz12));

}

void hyperentanglementQKD::changeComOffset(int newOffset)
{
    int offsetChange = newOffset - COM_offset;
    COM_offset = newOffset;
    if (offsetChange < 0)
    {
        if (timeSeqX1.size() == 0) return;
        COM_HEAD_X1 = (COM_HEAD_X1 - offsetChange) % timeSeqX1.size();
    }
    else
    {
        if (timeSeqX2.size() == 0) return;
        COM_HEAD_X2 = (COM_HEAD_X2 + offsetChange) % timeSeqX2.size();
    }
}

//数据记录

void hyperentanglementQKD::createTempDataFile()
{
    QString fullFileName = pathName + "/" + fileName + "." + ui->comboDataFileSuffix->currentText();
    fQKD->setFileName(fullFileName);
    fStream.setDevice(fQKD);
    fQKD->open(QIODevice::WriteOnly | QIODevice::Text);
    fStream << tr("有效计数") << "\n";
}


void hyperentanglementQKD::on_buttonStartRecord_released()
{
    QKDSavable = true;
    ui->buttonStartRecord->setEnabled(false);
    fetchUiData();
    timeTic = dateTime.currentMSecsSinceEpoch();
    if (windowState()!=Qt::WindowActive)
   {
    createTempDataFile();
//    emit QKDSaveDataNeedsSync(index);
    }
}

/*void hyperentanglementQKD::dealSaveDataTimeOut()
{
    timeToc = dateTime.currentMSecsSinceEpoch();
    timeRelative = timeToc -timeTic;
    fStream << "\t" <<timeRelative <<"\n";
} */


void hyperentanglementQKD::on_buttonStopRecord_released()
{
    QKDSavable = false;
    fetchUiData();
    timeToc = dateTime.currentMSecsSinceEpoch();
    timeRelative = timeToc -timeTic;
    fStream << tr("总用时 (ms)") << timeRelative <<endl;
    fQKD->close();
//    emit QKDSaveDataStopsSync(index);
}




void hyperentanglementQKD::on_buttonDataDirectory_released()
{
    pathName = QFileDialog::getExistingDirectory
                        (this,tr("Open Directory"),
                         QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                         QFileDialog::ShowDirsOnly
                         | QFileDialog::DontResolveSymlinks);
    ui->textDataDirectory->setCurrentText(pathName);
    ui->textDataDirectory->addItem(pathName);
}




void hyperentanglementQKD::on_buttonOpenDataDir_released()
{
    fetchUiData();
    QDesktopServices::openUrl(QUrl::fromLocalFile(pathName));
}



