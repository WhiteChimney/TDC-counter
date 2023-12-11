#include "hyperEBQKDwidget.h"
#include "ui_hyperEBQKDwidget.h"

hyperentanglementQKD::hyperentanglementQKD(QWidget *parent, int m_index, int m_comOffset) :
    QWidget(parent),
    ui(new Ui::hyperentanglementQKD)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);        // 在父窗口上显示独立的子窗口


    VolSPstatusIndicator = new QSimpleLed(this);
    this->setupvolSPIndicator();

    this -> refreshPorts();

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
    // 电压调节设置


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
     ui->Zperiod->setText(QString::number(zperiod/20));

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
    emit senddatapersecond(nbraxbx11, nbraxbx11error, nbraxbx22, nbraxbx22error);

    memset(&nbraxbx11,0,sizeof(nbraxbx11));
    memset(&nbraxbx11error,0,sizeof(nbraxbx11error));
    memset(&nbraxbx22,0,sizeof(nbraxbx22));
    memset(&nbraxbx22error,0,sizeof(nbraxbx22error));
    memset(&nbraxbx12,0,sizeof(nbraxbx12));
    memset(&nbrazbz11,0,sizeof(nbrazbz11));
    memset(&nbrazbz11error,0,sizeof(nbrazbz11error));
    memset(&nbrazbz22,0,sizeof(nbrazbz22));
    memset(&nbrazbz22error,0,sizeof(nbrazbz22error));
    memset(&nbrazbz12,0,sizeof(nbrazbz12));
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

/*void hyperentanglementQKD::createTempDataFile()
{
    QString fullFileName = pathName + "/" + fileName + "." + ui->comboDataFileSuffix->currentText();
    fQKD->setFileName(fullFileName);
    fStream.setDevice(fQKD);
    fQKD->open(QIODevice::WriteOnly | QIODevice::Text);
    fStream << tr("有效计数") << "\n";
    qDebug() << "创建文件";
}*/


void hyperentanglementQKD::on_buttonStartRecord_released()
{
    QKDSavable = true;
    ui->buttonStartRecord->setEnabled(false);
    fetchUiData();
    timeTic = dateTime.currentMSecsSinceEpoch();
    QString fullFileName = pathName + "/" + fileName + "." + ui->comboDataFileSuffix->currentText();
    fQKD->setFileName(fullFileName);
    fStream.setDevice(fQKD);
    fQKD->open(QIODevice::WriteOnly | QIODevice::Text);
    fStream << tr("有效计数") << "\n";
    qDebug() << "创建文件:"<<fullFileName;;

    QString fullFileName2 = pathName + "/" + "Cumulativedata.txt";
    fQKD2->setFileName(fullFileName2);
    fStream2.setDevice(fQKD2);
    fQKD2->open(QIODevice::WriteOnly | QIODevice::Text);
    fStream2 << tr("累积数据") << "\n";
//    createTempDataFile();
//    emit QKDSaveDataNeedsSync(index);

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
    ui->buttonStartRecord->setEnabled(true);
    fetchUiData();
    timeToc = dateTime.currentMSecsSinceEpoch();
    timeRelative = timeToc -timeTic;
    fStream << tr("总用时 (ms)") << timeRelative <<endl;
    fStream2 << tr("总用时 (ms)") << timeRelative <<endl;
    fQKD->close();
    fQKD2->close();
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

//  主动稳定相位部分  调节控制PZT的电压

void hyperentanglementQKD::setupvolSPIndicator()
{
    VolSPstatusIndicator->setCustomOnColor0(QSimpleLed::GREEN);
    VolSPstatusIndicator->setCustomOffColor0(QSimpleLed::RED);
    QSizePolicy p1;
   p1.setHorizontalPolicy(QSizePolicy::Minimum);
   p1.setVerticalPolicy(QSizePolicy::Preferred);
   VolSPstatusIndicator->setSizePolicy(p1);
   ui->horizontalLayout_SP->insertWidget(0,VolSPstatusIndicator);
   VolSPstatusIndicator->setStates(QSimpleLed::OFF);
}

void hyperentanglementQKD::refreshPorts()
{
    ui->comboBoxSPList->clear();
    spList.clear();
    spList = QSerialPortInfo::availablePorts();
    for (int i = 0; i < spList.size(); i++)
    ui->comboBoxSPList->addItem(spList.at(i).portName());
}


void hyperentanglementQKD::on_buttonRefresh_released()
{// 刷新串口 按钮
    this -> refreshPorts();
}


void hyperentanglementQKD::on_buttonOpenSP_released()
{// 打开串口 按钮
    if (VolSPstatusIndicator->states() == QSimpleLed::OFF)
    {
        serialsetvol.setPort(spList.at(ui->comboBoxSPList->currentIndex()));
        serialsetvol.setBaudRate(QSerialPort::Baud9600);
        serialsetvol.setDataBits(QSerialPort::Data8);
        serialsetvol.setParity(QSerialPort::NoParity);
        serialsetvol.setStopBits(QSerialPort::OneStop);
        serialsetvol.setFlowControl(QSerialPort::NoFlowControl);
        if(!serialsetvol.open(QIODevice::ReadWrite))
        {
            QMessageBox::about(NULL,"提示","无法打开串口！");
            return;
        }
        else
            VolSPstatusIndicator->setStates(QSimpleLed::ON);

    }
}


void hyperentanglementQKD::on_buttonCloseSP_released()
{// 关闭串口按钮
       if (VolSPstatusIndicator->states() == QSimpleLed::ON)
       {
           if(serialsetvol.isOpen())
           VolSPstatusIndicator->setStates(QSimpleLed::OFF);
       }
}


void hyperentanglementQKD::on_ButtonStartAdjVol_released()
{
    accumtimes = int(ui->accumulatedtime->text().toDouble());
    thresholdcalc = ui ->threshold ->text().toDouble();
    connect(this, &hyperentanglementQKD::senddatapersecond,this,&hyperentanglementQKD::dealsenddatapersecond);
}


void hyperentanglementQKD::on_ButtonStopAdjVol_released()
{
    disconnect(this, &hyperentanglementQKD::senddatapersecond,this,&hyperentanglementQKD::dealsenddatapersecond);
}


void hyperentanglementQKD::adjvol(int steptotal_m, int nbr_calc_m, int nbrerror_calc_m, bool subspacechoice)
{
  if(!subspacechoice)
  {
    switch(steptotal_m)
    {case 1:    //判断需不需要调制
        if((double)nbrerror_calc_m/(nbr_calc_m + nbrerror_calc_m) <= thresholdcalc)
        {
          nbrinstep1 = 0;
          nbrerrorinstep1 = 0;
          step1_num = 0;
        }
        else
        {
            nbrinstep1 += nbr_calc_m;
            nbrerrorinstep1 += nbrerror_calc_m;
            step1_num ++;
            if(step1_num > 1)      // 连续几次超过阈值开始调制, 为了方便先设置为0
            {steptotal = 2;
             avererrorstep1 = (double)nbrerror_calc_m/(nbr_calc_m + nbrerror_calc_m);  //累计三次的平均误码
             errorpre = avererrorstep1;
             phi = 1 - (acos(2*avererrorstep1-1))/3.1415926535897;
             if(ModulationDir)
             {    volset1 -= phi*halfwavevol1;
                 dealvolset(volset1, halfwavevol1, false);
                 ui -> volset -> setText(QString::number(volset1));
             }
             else
             {    volset1 += phi*halfwavevol1;
                  dealvolset(volset1, halfwavevol1, false);
                  ui -> volset -> setText(QString::number(volset1));
             }
                 flag_adjvol1 = true;
                 nbrinstep1 = 0;
                 nbrerrorinstep1 = 0;
                 step1_num = 0;
            }
        }
        break;

     case 2:    // 调制
        step2_num++;
        errorcurr = (double)nbrerror_calc_m/(nbr_calc_m + nbrerror_calc_m);
        flag_adjvol1 = true;
        if (errorcurr <= (thresholdcalc-0.01))
        {
            steptotal = 1;
            flag_adjvol1 = false;
            step2_num = 0;
        }
        else
        {
            phi = 1 - (acos(2*errorcurr-1))/3.1415926535897;
            if (step2_num<5)
            {
                if (errorcurr > errorpre)
                    ModulationDir = !ModulationDir;
                if(ModulationDir)
                   volset1 -= phi*halfwavevol1;
                else
                   volset1 += phi*halfwavevol1;
              dealvolset(volset1, halfwavevol1, false);
              ui -> volset -> setText(QString::number(volset1));
              errorpre = errorcurr;
            }
            else
            {
                if (errorcurr > errorpre)
                    ModulationDir = !ModulationDir;
                if(ModulationDir)
                   volset1 -= 1;
                else
                   volset1 += 1;
              dealvolset(volset1, halfwavevol1, false);
              ui -> volset -> setText(QString::number(volset1));
              errorpre = errorcurr;
            }
        }
        break;
    default:
        break;
    }
  }
  else {
      switch(steptotal_m)
      {case 1:    //判断需不需要调制
          if((double)nbrerror_calc_m/(nbr_calc_m + nbrerror_calc_m) <= thresholdcalc)
          {
            nbrinstep1_2 = 0;
            nbrerrorinstep1_2 = 0;
            step1_num_2 = 0;
          }
          else
          {
              nbrinstep1_2 += nbr_calc_m;
              nbrerrorinstep1_2 += nbrerror_calc_m;
              step1_num_2 ++;
              if(step1_num_2 > 0)      // 连续几次超过阈值开始调制, 为了方便先设置为0
              {steptotal_2 = 2;
               avererrorstep1_2 = (double)nbrerror_calc_m/(nbr_calc_m + nbrerror_calc_m);  //累计三次的平均误码
               errorpre_2 = avererrorstep1_2;
               phi_2 = 1 - (acos(2*avererrorstep1_2-1))/3.1415926535897;
               if(ModulationDir_2)
               {    volset2 -= phi_2*halfwavevol2;
                   dealvolset(volset2, halfwavevol2, true);
                   ui -> volset_2 -> setText(QString::number(volset2));
               }
               else
               {    volset2 += phi_2*halfwavevol2;
                    dealvolset(volset2, halfwavevol2, true);
                    ui -> volset_2 -> setText(QString::number(volset2));
               }
                   flag_adjvol2 = true;
                   nbrinstep1_2 = 0;
                   nbrerrorinstep1_2 = 0;
                   step1_num_2 = 0;
              }
          }
          break;

       case 2:    // 调制
          step2_num_2++;
          errorcurr_2 = (double)nbrerror_calc_m/(nbr_calc_m + nbrerror_calc_m);
          flag_adjvol2 = true;
          if (errorcurr_2 <= (thresholdcalc-0.01))
          {
              steptotal_2 = 1;
              flag_adjvol2 = false;
              step2_num_2 = 0;
          }
          else
          {
              phi_2 = 1 - (acos(2*errorcurr_2-1))/3.1415926535897;
              if (step2_num_2<5)
              {
                  if (errorcurr_2 > errorpre_2)
                      ModulationDir_2 = !ModulationDir_2;
                  if(ModulationDir_2)
                     volset2 -= phi_2*halfwavevol2;
                  else
                     volset2 += phi_2*halfwavevol2;
                dealvolset(volset2, halfwavevol2, false);
                ui -> volset_2 -> setText(QString::number(volset2));
                errorpre_2 = errorcurr_2;
              }
              else
              {
                  if (errorcurr_2 > errorpre_2)
                      ModulationDir_2 = !ModulationDir_2;
                  if(ModulationDir_2)
                     volset2 -= 1;
                  else
                     volset2 += 1;
                dealvolset(volset2, halfwavevol2, false);
                ui -> volset_2 -> setText(QString::number(volset2));
                errorpre_2 = errorcurr_2;
              }
          }
          break;
      default:
          break;
      }
  }
}

void hyperentanglementQKD::dealsenddatapersecond(int m_x11, int m_x11e, int m_x22, int m_x22e)
{
    nbraxbx11_calc += m_x11;
    nbraxbx11error_calc += m_x11e;
    nbraxbx22_calc += m_x22;
    nbraxbx22error_calc += m_x22e;
    accumnum++;
    if (accumnum > accumtimes)
    {
        adjvol(steptotal, nbraxbx11_calc, nbraxbx11error_calc, false);
        adjvol(steptotal_2, nbraxbx22_calc, nbraxbx22error_calc, true);
        if (QKDSavable)
        {
            fStream2 << nbraxbx11_calc <<"\t" << nbraxbx11error_calc <<"\t" << nbraxbx22_calc <<"\t" << nbraxbx22error_calc <<"\n" ;
        }

        nbraxbx11_calc = 0;
        nbraxbx11error_calc = 0;
        nbraxbx22_calc = 0;
        nbraxbx22error_calc = 0;
        accumnum = 1;
    }

}

QString stage(double v, int precision)
{
    return (v>=0.00 ? QString::number(v, 'f', precision) : "");
}

void hyperentanglementQKD::dealvolset(double doublevolset, double hwvol, bool subspacechoice)
{ // doublevolset 设置的电压， hwvol PZT的半波电压  subspacechoice 选择哪个PZT加电压  FALSE 表示subspace0 TRUE subspace 1
      while(doublevolset < 1)
        {
            doublevolset += 2*hwvol;
        }
       while(doublevolset > 100)
       {
          doublevolset -= 2*hwvol;
       }
       if (doublevolset >=1 and doublevolset <=100)
       {

       QString strNum = stage(doublevolset,2).remove(".");
        if (doublevolset < 10)
        strNum = strNum.prepend("0");
          if (!subspacechoice)   // PZT1
          {
              strNum = strNum.append("az");
              QByteArray V_pzt= strNum.toUtf8();
              serialsetvol.write(V_pzt);
              ui -> volset -> setText(stage(doublevolset,2));
          }
          else     // PZT2
          {
              strNum = strNum.append("bz");
              QByteArray V_pzt= strNum.toUtf8();
              serialsetvol.write(V_pzt);
              ui -> volset_2 -> setText(stage(doublevolset,2));
          }
       }
       else
       {
          qDebug() << "电压设置错误!" << "\n";
       }
    //先加上电压判断，是否超过100小于0，然后发送信号给电路
}


void hyperentanglementQKD::on_buttonsendvol_released()
{
    volset1 = ui-> volset->text().toDouble();
    volset2 = ui-> volset_2->text().toDouble();
    halfwavevol1 = ui -> hwvol ->text().toDouble();
    halfwavevol2 = ui -> hwvol_2 -> text().toDouble();

    ui -> volset -> setText(QString::number(volset1));
    ui -> volset_2 -> setText(QString::number(volset2));
    ui -> hwvol -> setText(QString::number(halfwavevol1));
    ui -> hwvol_2 ->setText(QString::number(halfwavevol2));

    dealvolset(volset1, halfwavevol1, false);
    dealvolset(volset2, halfwavevol2, true);
//    QString abs = stage(volset1,2).remove(".");
//    abs = abs.prepend("0");
//    qDebug() << abs << "\n";
//    qDebug() << stage(volset2,2).remove(".") << "\n";

}



// 调节电压按钮
void hyperentanglementQKD::on_pushButtonp1_released()
{
    volset1 += 1;
    dealvolset(volset1, halfwavevol1, false);
    ui -> volset -> setText(QString::number(volset1));
}
void hyperentanglementQKD::on_pushButtonm1_released()
{
    volset1 -= 1;
    dealvolset(volset1, halfwavevol1, false);
    ui -> volset -> setText(QString::number(volset1));
}
void hyperentanglementQKD::on_pushButtonp01_released()
{
    volset1 += 0.1;
    dealvolset(volset1, halfwavevol1, false);
    ui -> volset -> setText(QString::number(volset1));
}
void hyperentanglementQKD::on_pushButtonm01_released()
{
    volset1 -= 0.1;
    dealvolset(volset1, halfwavevol1, false);
    ui -> volset -> setText(QString::number(volset1));
}
void hyperentanglementQKD::on_pushButton001_released()
{
    volset1 += 0.01;
    dealvolset(volset1, halfwavevol1, false);
    ui -> volset -> setText(QString::number(volset1));
}
void hyperentanglementQKD::on_pushButtonm001_released()
{
    volset1 -= 0.01;
    dealvolset(volset1, halfwavevol1, false);
    ui -> volset -> setText(QString::number(volset1));
}
void hyperentanglementQKD::on_pushButtonp1_2_released()
{
    volset2 += 1;
    dealvolset(volset2, halfwavevol2, true);
    ui -> volset_2 -> setText(QString::number(volset2));
}
void hyperentanglementQKD::on_pushButtonm1_2_released()
{
    volset2 -= 1;
    dealvolset(volset2, halfwavevol2, true);
    ui -> volset_2 -> setText(QString::number(volset2));
}
void hyperentanglementQKD::on_pushButtonp01_2_released()
{
    volset2 += 0.1;
    dealvolset(volset2, halfwavevol2, true);
    ui -> volset_2 -> setText(QString::number(volset2));
}
void hyperentanglementQKD::on_pushButtonm01_2_released()
{
    volset2 -= 0.1;
    dealvolset(volset2, halfwavevol2, true);
    ui -> volset_2 -> setText(QString::number(volset2));
}
void hyperentanglementQKD::on_pushButton001_2_released()
{
    volset2 += 0.01;
    dealvolset(volset2, halfwavevol2, true);
    ui -> volset_2 -> setText(QString::number(volset2));
}
void hyperentanglementQKD::on_pushButtonm001_2_released()
{
    volset2 -= 0.01;
    dealvolset(volset2, halfwavevol2, true);
    ui -> volset_2 -> setText(QString::number(volset2));
}

