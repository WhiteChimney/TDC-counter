#include "stdafx.h"
#include "widget.h"
#include "coincidencewidget.h"

// 用于合成最终数据文件

void Widget::mergeDataFiles
    (RecordMode modeRecord,
     QString fileName,
     QString metaData,
     QVector<CoincidenceWidget *> vCoinWidget)
{
    QString line;
    QFile fMerge(fileName);
    QString iniPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/AcqirisTDC_qt";
    QString tempFileName_SCC = iniPath + "/Data/SingleCount.txt";
    QFile fSingle(tempFileName_SCC);
    switch (modeRecord)
    {
    case SingleAndCoinCount:
    {
        fMerge.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream fStream(&fMerge);
        fStream << tr(metaData.toStdString().data()) << endl
                << endl;
        fSingle.open(QIODevice::ReadOnly | QIODevice::Text);
        for (int i = 0; i < 4; i++)
        {
            line = QString::fromLocal8Bit(fSingle.readLine());
            fStream << line;
        }
        QVector<QFile*> vfCoin;
        for (int i = 0; i < vCoinWidget.count(); i++)
        {
            if (vCoinWidget.at(i)->windowState() == Qt::WindowActive)
            {
                QString tempFileName_C = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                       + "/AcqirisTDC_qt/Data/Coincidence" + QString::number(i) +".txt";
                QFile *fCoin = new QFile();
                fCoin->setFileName(tempFileName_C);
                fCoin->open(QIODevice::ReadOnly | QIODevice::Text);
                for (int i = 0; i < 9; i++)
                {
                    line = QString::fromLocal8Bit(fCoin->readLine());
                    fStream << line;
                }
                vfCoin.append(fCoin);
            }
        }
        line = QString::fromLocal8Bit(fSingle.readLine().trimmed());
        while (! line.isNull())
        {
            fStream << line;
            for (int i = 0; i < vfCoin.count(); i++)
            {
                line = QString::fromLocal8Bit(vfCoin.at(i)->readLine().trimmed());
                fStream << "\t" << line;
            }
            fStream << endl;
            line = QString::fromLocal8Bit(fSingle.readLine().trimmed());
        }
        fMerge.close();
        fSingle.close();
        for (int i = 0; i < vfCoin.count(); i++)
            vfCoin.at(i)->close();
        break;
    }

    case SingleCountOnly:
    {
        fMerge.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream fStream(&fMerge);
        fStream << tr(metaData.toStdString().data()) << endl
                << endl;
        fSingle.open(QIODevice::ReadOnly | QIODevice::Text);
        line = QString::fromLocal8Bit(fSingle.readAll());
        fStream << line;
        fSingle.close();
        fMerge.close();
        break;
    }

    case CoinCountOnly:
    {
        for (int i = 0; i < vCoinWidget.count(); i++)
        {
            QFileInfo fileInfo(fileName);
            QDir coinPath;
            coinPath.mkdir(fileInfo.path() + "/Coincidences");
            if (vCoinWidget.at(i)->windowState() == Qt::WindowActive)
            {
                QString copiedFileName = fileInfo.path() + "/Coincidences/Coincidence" + QString::number(i) +".txt";
                QFile fCopiedFile(copiedFileName);
                fCopiedFile.open(QIODevice::WriteOnly | QIODevice::Text);
                QTextStream fStream(&fCopiedFile);
                fStream << tr(metaData.toStdString().data()) << endl
                        << endl;

                QString tempFileName_C = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                       + "/AcqirisTDC_qt/Data/Coincidence" + QString::number(i) +".txt";
                QFile *fCoin = new QFile();
                fCoin->setFileName(tempFileName_C);
                fCoin->open(QIODevice::ReadOnly | QIODevice::Text);
                line = QString::fromLocal8Bit(fCoin->readAll());
                fStream << line;

                fCoin->close();
                fCopiedFile.close();
            }
        }
        break;
    }

    default:
        return;
        break;

    }
}
