#include "AcqirisImport.h"
#include "AcqirisT3Import.h"
#include <cmath>
#include <QVector>
#include <QDebug>

//bool channelToBeCalculated(int channel, int *channels, int nbrChannels);

int findInsertPosition1(QVector<int> timeSeq, int TimeOfFlight)
{
    if (timeSeq.size() == 0)
        return 0;
    int index = 0;
    bool bingo = false;
    while (!bingo and index < timeSeq.size())
    {
        if (timeSeq.at(index) >= TimeOfFlight)
            bingo = true;
        else
            index++;
    }
    return index;
}


int checkoutcome(int channelnum, int time1, int period)
{  //将AliceBob的数据存到一个长16的数组中，前8个是Alice，前四个为Alicex基的四个探测器结果，第5,6，是z基的两个探测器的前脉冲，7，8为两个探测器的后一个脉冲结果
    if((channelnum >0 and channelnum<5) or (channelnum >10 and channelnum<15))
    {
        if(channelnum<10)
        {
        return channelnum-1;
        }
        else{
            return channelnum-3;
        }
    }
    else{
        if(((channelnum >4 and channelnum<7) or (channelnum >14 and channelnum<17)) and (time1/period) % 2==0)
        {
            if(channelnum<10)
            {return channelnum-1;}
            else{
                return channelnum-3;
            }
        }
        else{
            if(channelnum<10)
            {
                return channelnum+1;
            }
            else{
                return channelnum-1;
            }
        }
    }
}

int checkkey(int ax[], int az[], int bx[], int bz[], int axn, int azn, int bxn, int bzn){
    // 子空间1x基没有误码，
    int axkey, azkey, bxkey, bzkey;
    if((axn>0 and bxn>0) and !(azn>0 and bzn>0) )
    {
        axkey = ax[rand()%axn];
        bxkey = bx[rand()%bxn];
        if(axkey%2==0 and bxkey%2==0)
        {
          if(axkey==bxkey)
          return 1;     //space1 x
          else
          return 2;      // space1 x  error
        }
        else{ if(axkey%2 == 0 and bxkey%2 ==1){
                  return 5;  // errror   a space1   b space 2
            }
            else{
                if(axkey%2==1 and bxkey%2 ==0 )
                {
                      return 5; // error     a space2   b space1
                }
                else{
                    if (axkey==bxkey)
                        return 3;         //space2 x
                    else return 4;           // space2 x  error
                }
            }
        }
    }
    else if ((azn>0 and bzn>0) and !(axn>0 and bxn>0))
    {
        azkey = az[rand()%azn];
        bzkey = bz[rand()%bzn];
        if(azkey%2==0 and bzkey%2==0)
        {
          if(azkey==bzkey)
          return 6;     //space1 x
          else
          return 7;      // space1 x  error
        }
        else{ if(azkey%2 == 0 and bzkey%2 ==1){
                  return 10;  // errror   a space1   b space 2
            }
            else{
                if(azkey%2==1 and bzkey%2 ==0 )
                {
                      return 10; // error     a space2   b space1
                }
                else{
                    if (azkey==bzkey)
                        return 8;         //space2 x
                    else return 9;           // space2 x  error
                }
            }
        }
    }
    else{
        return 11;
    }
}


void computeQKDAcrossDevices_HOLD
            (AqT3DataDescriptor* dataDescPtr,
             QList<QVector<int>>& timeSeq,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
             QList<QVector<int>>& channelSeq,    // 升序排列后的时间，与通道编号一一对应
             int *nbrCOMdelay,
             int *delayInCOM,
             int timeCOMunit,
             int *COM_HEAD)
{
//    读取时间数据
    long nbrSamples = dataDescPtr->nbrSamples;

    int index = 0; // 用于标记元素插入位置
    for (long n = 0 ; n < nbrSamples ; ++n)
    {
        int sample = ((long *)dataDescPtr->dataPtr)[n];  //dataPtr指向time value data buffer
//        int flag = (sample & 0x80000000) >> 31;      //“sample”值右移31位为flag位
        int channel = (sample & 0x70000000) >> 28;   //右移28位为channel位
        int TimeOfFlight = sample & 0x0FFFFFFF;             //最右侧28位为计数值

        // flag 为 0 表示 COM 信号，channel 为 7 表示内存切换
        if (channel != 0 and channel != 7)                        // Channel=1-6 are the physical channels;
                                                // Data = an integer giving the time value in units of 50 ps
                                                // Channel=7 is for marker data.
        {

            int indexCOM;
            TimeOfFlight += delayInCOM[channel-1];
            // channel-1 通道所插入的序列编号应为 nbrCOMdelay[channel-1]
            if (TimeOfFlight > timeCOMunit)
            {
                TimeOfFlight -= timeCOMunit;
                indexCOM = (nbrCOMdelay[channel-1] + 1 + *COM_HEAD) % timeSeq.size();
            }
            else
                indexCOM = (nbrCOMdelay[channel-1] + *COM_HEAD) % timeSeq.size();
            index = findInsertPosition1(timeSeq[indexCOM], TimeOfFlight);        // 按时间升序排列
            timeSeq[indexCOM].insert(index, TimeOfFlight);
            channelSeq[indexCOM].insert(index, channel);

        }
        else
            *COM_HEAD = ((*COM_HEAD)+1) % timeSeq.size();
    }
}


void computeQKDAcrossDevices_COMPUTE(
                 QList<QVector<int>> &timeSeqX1,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
                 QList<QVector<int>> &channelSeqX1,    // 升序排列后的时间，与通道编号一一对应
                 QList<QVector<int>> &timeSeqX2,       // 用于存储按时间顺序排列后的通道编号（0-5 对应实际的 1-6）
                 QList<QVector<int>> &channelSeqX2,    // 升序排列后的时间，与通道编号一一对应
                 int nbrChannelsX1, int nbrChannelsX2,
                 int* channelsX1, int *channelsX2,
                 int* axbx11, int* axbx12, int* axbx22, int* axbx11error, int* axbx22error,
                 int* azbz11, int* azbz12, int* azbz22, int* azbz11error, int* azbz22error,
                 int* abandon,
                 int period,
                 int toleranceMulti,
                 int *COM_HEAD_X1, int *COM_HEAD_X2, int *COM_HEAD_compute)
{
    int vectorSize = timeSeqX1.size();
    int computeLength = *COM_HEAD_X1 - *COM_HEAD_compute;

    if (*COM_HEAD_X1 < *COM_HEAD_compute and *COM_HEAD_X2 < *COM_HEAD_compute)
    {
        if (*COM_HEAD_X1 > *COM_HEAD_X2)        // 2 < 1 < X
            computeLength = vectorSize - *COM_HEAD_compute + *COM_HEAD_X2;
        else                                // 1 < 2 < X
            computeLength = vectorSize - *COM_HEAD_compute + *COM_HEAD_X1;

    }
    else
    {
        if (*COM_HEAD_X1 < *COM_HEAD_compute)        // 1 < X < 2
            computeLength = *COM_HEAD_X2 - *COM_HEAD_compute;
        else
        {
            if (*COM_HEAD_X1 > *COM_HEAD_X2 and *COM_HEAD_X2 > *COM_HEAD_compute)
                                            // X < 2 < 1
                computeLength = *COM_HEAD_X2 - *COM_HEAD_compute;
        }
    }
    int nbrChannels = nbrChannelsX1 + nbrChannelsX2;
    int *channels = new int[nbrChannels]();
    for (int i = 0; i < nbrChannelsX1; i++)
        channels[i] = channelsX1[i];
    for (int i = 0; i < nbrChannelsX2; i++)
        channels[i+nbrChannelsX1] = channelsX2[i] + 6;

    int index;

    for (int m = *COM_HEAD_compute; m < *COM_HEAD_compute + computeLength; m++)
    {
        int mm = m % vectorSize;

        if (timeSeqX1[mm].size() + timeSeqX2[mm].size() >= 2)                            // 计算上一轮符合
        {
            // 先将 X2 的时间及通道数据合并到 X1 向量中
            for (int j = 0; j < timeSeqX2[mm].size(); j++)
            {
                index = findInsertPosition1(timeSeqX1[mm], timeSeqX2[mm][j]);
                timeSeqX1[mm].insert(index, timeSeqX2[mm][j]);
                channelSeqX1[mm].insert(index, channelSeqX2[mm][j] + 10);
            }

           //先将Alice和Bob的响应分类存到数组aliceX[4], aliceZ[4], bobX[4], bobZ[4]中，然后判断是否有符合;
            for(int i=0; i < timeSeqX1[mm].size()-1; i++)
            {
                int spacing_qkd=0, channelrecord = 0;
                bool channelbool[16];
                int aliceX[4], aliceZ[4], bobX[4], bobZ[4];
                memset(channelbool,false,sizeof(channelbool));
                memset(aliceX,0,sizeof(aliceX));
                memset(bobX,0,sizeof(bobX));
                memset(aliceZ,0,sizeof(aliceZ));
                memset(bobZ,0,sizeof(bobZ));
                int indexcheck;
                int aliceXnum=0, bobXnum=0, aliceZnum=0, bobZnum=0;
                indexcheck=checkoutcome(channelSeqX1[mm][i], timeSeqX1[mm][i], period);
                if(indexcheck<4)
                {
                    aliceX[0] = indexcheck;
                    aliceXnum++;
                }
                else{
                    if (indexcheck<8){
                        aliceZ[0] = indexcheck-4;
                        aliceZnum++;
                    }
                    else if(indexcheck<12){
                        bobX[0] = indexcheck-8;
                        bobXnum++;
                    }
                    else{
                        bobZ[0] = indexcheck-12;
                        bobZnum++;
                    }
                }
                channelrecord = 10*channelrecord+channelSeqX1[mm][i];
                if((timeSeqX1[mm][i]/period)%2==0)
                {
                    while (timeSeqX1[mm].at(i+spacing_qkd+1)-timeSeqX1[mm].at(i)<=toleranceMulti and i+spacing_qkd+1 < timeSeqX1[mm].size())
                    { //
                    indexcheck=checkoutcome(channelSeqX1[mm][i+spacing_qkd+1],timeSeqX1[mm][i+spacing_qkd+1], period);
                    if(indexcheck<4)
                    {aliceX[aliceXnum] = indexcheck;
                        aliceXnum++;
                    }
                    else{
                        if (indexcheck<8){
                            aliceZ[aliceZnum] = indexcheck-4;
                            aliceZnum++;
                        }
                        else if(indexcheck<12){
                            bobX[bobXnum] = indexcheck-8;
                            bobXnum++;
                        }
                        else{
                            bobZ[bobZnum] = indexcheck-12;
                            bobZnum++;    //每个基中的数量
                        }
                    }
                    channelrecord = 10*channelrecord+channelSeqX1[mm][i+spacing_qkd+1];
                    spacing_qkd++;
                    }

                   if((timeSeqX1[mm].at(i+spacing_qkd+1)-timeSeqX1[mm].at(i)>=period-1/3*toleranceMulti) and ((timeSeqX1[mm].at(i+spacing_qkd+1)-timeSeqX1[mm].at(i)<=period+toleranceMulti))
                           and (bobZnum>0 or aliceZnum>0) and i+spacing_qkd+1 < timeSeqX1[mm].size())
                   {
                       indexcheck=checkoutcome(channelSeqX1[mm][i+spacing_qkd+1], timeSeqX1[mm][i+spacing_qkd+1], period);
                       if(indexcheck<4)
                       {
                          qDebug()<<"checkoutcome error";
                       }
                       else{
                           if (indexcheck<8){
                               aliceZ[aliceZnum] = indexcheck-4;
                               aliceZnum++;
                           }
                           else if(indexcheck<12){
                               qDebug()<<"checkoutcome error 2";
                           }
                           else{
                               bobZ[bobZnum] = indexcheck-12;
                               bobZnum++;
                           }
                       }
                       channelrecord = 10*channelrecord+channelSeqX1[mm][i+spacing_qkd+1];
                       int spacing_qkdz=1;
                        while ((timeSeqX1[mm].at(i+spacing_qkd+1+spacing_qkdz)-timeSeqX1[mm].at(i+spacing_qkd+1)<=toleranceMulti) and (i+spacing_qkd+1 < timeSeqX1[mm].size()))
                       {
                         indexcheck=checkoutcome(channelSeqX1[mm][i+spacing_qkd+1+spacing_qkdz], timeSeqX1[mm][i+spacing_qkd+1+spacing_qkdz], period);
                            if(indexcheck<4)
                            {
                               qDebug()<<"checkoutcome error";
                            }
                            else{
                                if (indexcheck<8){
                                    aliceZ[aliceZnum] = indexcheck-4;
                                    aliceZnum++;
                                }
                                else if(indexcheck<12){
                                    qDebug()<<"checkoutcome error 2";
                                }
                                else{
                                    bobZ[bobZnum] = indexcheck-12;
                                    bobZnum++;
                                }
                            }
                            channelrecord = 10*channelrecord+channelSeqX1[mm][i+spacing_qkd+1+spacing_qkdz];
                            spacing_qkdz++;
                       }
                      spacing_qkd += spacing_qkdz;
                   }
                }
                else{
                    while (timeSeqX1[mm].at(i+spacing_qkd+1)-timeSeqX1[mm].at(i)<=toleranceMulti and i+spacing_qkd+1 < timeSeqX1[mm].size())
                    { //
                        indexcheck=checkoutcome(channelSeqX1[mm][i+spacing_qkd+1],timeSeqX1[mm][i+spacing_qkd+1], period);
                        if(indexcheck<4)
                        {aliceX[aliceXnum] = indexcheck;
                            aliceXnum++;
                        }
                        else{
                            if (indexcheck<8){
                                aliceZ[aliceZnum] = indexcheck-4;
                                aliceZnum++;
                            }
                            else if(indexcheck<12){
                                bobX[bobXnum] = indexcheck-8;
                                bobXnum++;
                            }
                            else{
                                bobZ[bobZnum] = indexcheck-12;
                                bobZnum++;    //每个基中的数量
                            }
                        }
                        spacing_qkd++;
                    }
                }
                switch(checkkey(aliceX, aliceZ,bobX,bobZ,aliceXnum,aliceZnum,bobXnum,bobZnum)){
                case 1 :
                    (*axbx11)++;
                    break;
                case 2 :
                    (*axbx11error)++;
                    break;
                case 3 :
                    (*axbx22)++;
                    break;
                case 4 :
                    (*axbx22error)++;
                    break;
                case 5 :
                    (*axbx12)++;
                    break;
                case 6 :
                    (*azbz11)++;
                    break;
                case 7 :
                    (*azbz11error)++;
                    break;
                case 8 :
                    (*azbz22)++;
                    break;
                case 9 :
                    (*azbz22error)++;
                    break;
                case 10 :
                    (*azbz12)++;
                    break;
                case 11 :
                    (*abandon)++;
                    break;
                default:
                    break;
                }
                i=i+spacing_qkd;
           }


        timeSeqX1[mm].clear();
        channelSeqX1[mm].clear();
        timeSeqX2[mm].clear();
        channelSeqX2[mm].clear();
     }
   }
    *COM_HEAD_compute = (*COM_HEAD_compute + computeLength) % vectorSize;
    delete[] channels;
}
