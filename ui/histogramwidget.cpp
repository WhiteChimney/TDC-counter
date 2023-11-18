#include "histogramwidget.h"
#include "ui_histogramwidget.h"

HistogramWidget::HistogramWidget(QWidget *parent, int index0) :
    QWidget(parent),
    ui(new Ui::HistogramWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);        // 在父窗口上显示独立的子窗口
//    this->setAttribute(Qt::WA_DeleteOnClose);   // 退出时执行析构函数，有点问题，会导致程序崩溃
    index = index0;

    qwtHistPlot = new QwtPlotHistogram();
    qwtHistPlot->attach(ui->qwtPlot);
    setupPlotStyle();
//    testPlot();                         // 便于查看画图样式

    timerHist = new QTimer(this);
    connect(timerHist,&QTimer::timeout,this,&HistogramWidget::dealTimeOut);

    QString appVersion = "V" + tr(PROJECT_VERSION_0) + tr(".")
                             + tr(PROJECT_VERSION_1) + tr(".")
                             + tr(PROJECT_VERSION_2);
    iniPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    iniPath = iniPath + "/" + appVersion;
    iniName = iniPath + "/Configurations/histogram" + QString::number(index) +".ini";

    QFileInfo iniInfo(iniName);
    if (iniInfo.isFile())
        loadFromIni();
    else
        saveToIni();
}

HistogramWidget::~HistogramWidget()
{
    on_buttonReturn_released();
    saveToIni();
    delete ui;
}

void HistogramWidget::setupPlotStyle()
{
    qwtHistPlot->setStyle(QwtPlotHistogram::Outline);          // 直方图样式，Outline, Lines, Columns
    qwtHistPlot->setBrush(Qt::cyan);                           // 柱填充色
    qwtHistPlot->setPen(Qt::darkBlue, 1.0, Qt::SolidLine);     // 柱线条色
    ui->qwtPlot->setCanvasBackground(Qt::white);               // 画布背景色
//    ui->qwtPlot->setTitle("累计值");                          // 图标题
//    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom,"时间（ns）"); // 横坐标轴标题
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,"计数");           // 纵坐标轴标题
}

void HistogramWidget::testPlot()
{
    int nbrIntervals = 10;
    int intervalStart = 0, binWidth = 1;
    binHeight = new int[nbrIntervals]();
    histIntervals = new QwtInterval[nbrIntervals]();
    QVector<QwtIntervalSample> histSamples(nbrIntervals);
    for (int i = 0; i < nbrIntervals; i++)
    {
        binHeight[i] = 2*i*i;
        histIntervals[i] = QwtInterval(intervalStart,intervalStart+binWidth);
        histSamples[i] = QwtIntervalSample(binHeight[i],histIntervals[i]);
        intervalStart = intervalStart+binWidth;
    }
    qwtHistPlot->setSamples(histSamples);
    ui->qwtPlot->replot();
}

void HistogramWidget::fetchUiData()
{
    device1 = ui->comboDevice1->currentIndex();
    device2 = ui->comboDevice2->currentIndex();
    channel1 = ui->comboChannel1->currentText().toInt();
    channel2 = ui->comboChannel2->currentText().toInt();
    accumulateTime = ui->textAccumlateTime->text().toDouble();
    delay = ui->textDelay->text().toDouble();
    timeStart = ui->textTimeStart->text().toDouble();
    timeStop = ui->textTimeStop->text().toDouble();
    if (timeStart > timeStop)
    {
        double timeTemp = timeStop;
        timeStop = timeStart;
        timeStart = timeTemp;
    }
    binWidth = abs(ui->textBinWidth->text().toDouble());
    if (binWidth < 0.05) binWidth = 0.05;
    nbrIntervals = ceil(abs(timeStop-timeStart)/binWidth);
    timeStop = timeStart + nbrIntervals*binWidth;

    pushUiData();
}

void HistogramWidget::pushUiData()
{
    ui->comboDevice1->setCurrentIndex(device1);
    ui->comboDevice2->setCurrentIndex(device2);
    ui->comboChannel1->setCurrentText(QString::number(channel1));
    ui->comboChannel2->setCurrentText(QString::number(channel2));
    ui->textAccumlateTime->setText(QString::number(accumulateTime));
    ui->textDelay->setText(QString::number(delay));
    ui->textTimeStart->setText(QString::number(timeStart));
    ui->textTimeStop->setText(QString::number(timeStop));
    ui->textBinWidth->setText(QString::number(binWidth));
}

void HistogramWidget::on_buttonReturn_released()
{
    on_buttonStop_released();
    emit returnSignal(index);
}

void HistogramWidget::on_buttonStart_released()
{
    fetchUiData();
    binHeight = new int[nbrIntervals]();
    histIntervals = new QwtInterval[nbrIntervals]();
    for (int i = 0; i < nbrIntervals; i++)
    {
        double intervalStart = timeStart + i*binWidth;
        histIntervals[i] = QwtInterval(intervalStart,intervalStart+binWidth);
    }
    timerHist->start(1000.0*accumulateTime);
    emit requestHistParam(index);
}

void HistogramWidget::dealTimeOut()
{
    QVector<QwtIntervalSample> histSamples(nbrIntervals);
    for (int i = 0; i < nbrIntervals; i++)
    {
        histSamples[i] = QwtIntervalSample(binHeight[i],histIntervals[i]);
    }
    qwtHistPlot->setSamples(histSamples);
    ui->qwtPlot->replot();
    memset(binHeight,0,nbrIntervals*sizeof(binHeight[0]));
}

void HistogramWidget::dealRequestHistParam(int index0, double *delayCN0, double freqCOM0)
{

    if (index == index0)
    {
        delayCN = delayCN0;
        freqCOM = freqCOM0;

        //    预处理 TDC 参数
        double timeCOM = 1000000.0/freqCOM;           // 单位为 us
        timeCOMunit = int(20*1000.0*timeCOM);         // TDC 内部单位，50 ps
        double delayTotal[6] = {0.0};
        double minDelay = delayCN[0];
        for (int i = 0; i < 6; i++)
        {
            delayTotal[i] = delayCN[i];
            if (delayTotal[i] < minDelay)
                minDelay = delayTotal[i];
        }
        int maxNbrCOMdelay = 0;
        for (int i = 0; i < 6; i++)
        {
            delayTotal[i] -= minDelay;             // 保证所有延时均为非负
            nbrCOMdelay[i] = floor(delayTotal[i]/timeCOM);
            if (nbrCOMdelay[i] > maxNbrCOMdelay)
                maxNbrCOMdelay = nbrCOMdelay[i];
            delayInCOM[i] = int(20*1000.0*delayTotal[i] - timeCOM*nbrCOMdelay[i]);
        }

    //    时间序列所需要保存的 COM 周期数量为 nbrCOMdelay 中的最大值 +2
        resizeSeqLength(&timeSeq1, maxNbrCOMdelay+2);
        resizeSeqLength(&timeSeq2, maxNbrCOMdelay+2);
        COM_HEAD = 0;

        emit askDealAcqBankSwitchHist(index);
    }
}

void HistogramWidget::dealAcqThreadBankSwitchHist(AqT3DataDescriptor* dataDescPtr)
{
//    AqT3DataDescriptor *dataDescPtr = dataPtrList.last();
    // prepare samples to plot
    computeHistogramCount(dataDescPtr,
                          timeSeq1, timeSeq2,
                          channel1, channel2, delay,
                          timeStart, binWidth, nbrIntervals, binHeight,
                          nbrCOMdelay,
                          delayInCOM,
                          timeCOMunit,
                          &COM_HEAD);
//    computeHistogramCount(dataDescPtr, channel1, channel2, delay, timeStart, binWidth, nbrIntervals, binHeight);
}

void HistogramWidget::on_buttonStop_released()
{
    emit askStopDealAcqBankSwitchHist(index);
    timerHist->stop();
}

void HistogramWidget::saveToIni()
{
    fetchUiData();

    QSettings *configIni = new QSettings(iniName, QSettings::IniFormat);
    configIni->setValue("直方图配置/device1",device1);
    configIni->setValue("直方图配置/device2",device2);
    configIni->setValue("直方图配置/channel1",channel1);
    configIni->setValue("直方图配置/channel2",channel2);
    configIni->setValue("直方图配置/accumulateTime",accumulateTime);
    configIni->setValue("直方图配置/delay",delay);
    configIni->setValue("直方图配置/timeStart",timeStart);
    configIni->setValue("直方图配置/timeStop",timeStop);
    configIni->setValue("直方图配置/binWidth",binWidth);
    delete configIni;
}

void HistogramWidget::loadFromIni()
{
    QSettings *configIni = new QSettings(iniName, QSettings::IniFormat);
    device1 = configIni->value("直方图配置/device1").toInt();
    device2 = configIni->value("直方图配置/device2").toInt();
    channel1 = configIni->value("直方图配置/channel1").toInt();
    channel2 = configIni->value("直方图配置/channel2").toInt();
    accumulateTime = configIni->value("直方图配置/accumulateTime").toDouble();
    delay = configIni->value("直方图配置/delay").toDouble();
    timeStart = configIni->value("直方图配置/timeStart").toDouble();
    timeStop = configIni->value("直方图配置/timeStop").toDouble();
    binWidth = configIni->value("直方图配置/binWidth").toDouble();
    delete configIni;

    pushUiData();
}
