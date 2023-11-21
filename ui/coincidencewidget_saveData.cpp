#include "coincidencewidget.h"
#include "ui_coincidencewidget.h"

void CoincidenceWidget::createTempDataFile()
{
    // 将配置写入临时文件
    fCoin->open(QIODevice::WriteOnly | QIODevice::Text);

    // 双通道
    if (ui->stackCoin->currentIndex()==0)
    {
        fStream << tr("符合计数") << index+1 << "\n"
                << tr("当前时间：") << QDateTime::currentDateTime().toString() << "\n"
                << tr("符合通道：Channel ") << 6*device1 + channel1 << tr(" & ") << 6*device2 + channel2 << "\n"
                << tr("符合门宽：") << tolerance/20.0 << tr("ns") << "\n"
                << tr("延时：") << delay/20.0 << tr("ns") << "\n"
                << tr("偶然符合额外延时：") << delayAcc/20.0 << tr("ns") << "\n";
        if (enableAccumulateTime)
        {
            fStream << tr("计时与单道计数同步") << "\n"
                    << "\n"
                    << "\n"
                    << tr("符合计数") << index+1 << "\n";
        }
        else
        {
            fStream << tr("计时未与单道计数同步") << "\n"
                    << tr("累计时间：") << accumulateTime << tr("s") << "\n"
                    << "\n"
                    << tr("时间/ms\t计数") << "\n";
        }
    }
    else
    {
        QString strChannel = "符合通道：Channel ";
        QString strDelay = "延时：";
        for (int i = 0; i < 6; i++)
        {
            if (channelMark[i])
            {
                strChannel.append(QString::number(i+1));
                strChannel.append(" & ");
                strDelay.append(QString::number(delayMulti[i]/20.0));
                strDelay.append("ns, ");
            }
            if (channelMark_2[i])
            {
                strChannel.append(QString::number(i+1+6));
                strChannel.append(" & ");
                strDelay.append(QString::number(delayMulti_2[i]/20.0));
                strDelay.append("ns, ");
            }
        }
        strChannel.remove(strChannel.length()-3,3);
        strDelay.remove(strDelay.length()-2,2);

        fStream << tr("符合计数") << index+1 << "\n"
                << tr("当前时间：") << QDateTime::currentDateTime().toString() << "\n"
                << tr("符合门宽：") << toleranceMulti/20.0 << tr("ns") << "\n"
                << strChannel << "\n"
                << strDelay << "\n";
        if (enableAccumulateTimeMulti)
        {
            fStream << tr("计时与单道计数同步") << "\n"
                    << "\n"
                    << "\n"
                    << "\n"
                    << tr("符合计数") << index+1 << "\n";
        }
        else
        {
            fStream << tr("计时未与单道计数同步") << "\n"
                    << tr("累计时间：") << accumulateTimeMulti << tr("s") << "\n"
                    << "\n"
                    << "\n"
                    << tr("时间/ms\t计数") << "\n";
        }
    }
    fCoin->close();
}

void CoincidenceWidget::dealSaveCoinData()
{
    if (coinSavable)
    {
        fCoin->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
        if ((ui->stackCoin->currentIndex() == 0 and (!enableAccumulateTime))
                or (ui->stackCoin->currentIndex() == 1 and (!enableAccumulateTimeMulti)))
        {// 如果不与单道同步，则记录时间；否则不记录，之后合并
            timeToc = QDateTime::currentMSecsSinceEpoch();
            fStream << timeToc - timeTic << "\t";
        }
        if (ui->stackCoin->currentIndex()==0) // 双通道模式
            fStream << nbrCoin << "\n";
        else
            fStream << nbrCoinMulti << "\n";
        fCoin->close();
    }
}

void CoincidenceWidget::startRecordCoinLocal()
{
    disconnect(timerCoin,&QTimer::timeout,this,&CoincidenceWidget::dealTimeOut);
    connect(timerCoin,&QTimer::timeout,this,&CoincidenceWidget::dealSaveCoinData);
    connect(timerCoin,&QTimer::timeout,this,&CoincidenceWidget::dealTimeOut);
}

void CoincidenceWidget::stopRecordCoinLocal()
{
    disconnect(timerCoin,&QTimer::timeout,this,&CoincidenceWidget::dealSaveCoinData);
}
