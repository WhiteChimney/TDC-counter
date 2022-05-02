#include "histogramwidget.h"
#include "ui_histogramwidget.h"

HistogramWidget::HistogramWidget(QWidget *parent, int index0) :
    QWidget(parent),
    ui(new Ui::HistogramWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);        // 在父窗口上显示独立的子窗口
    index = index0;

    qwtHistPlot = new QwtPlotHistogram();
    qwtHistPlot->attach(ui->qwtPlot);
    setupPlotStyle();
//    testPlot();

    timerHist = new QTimer(this);
    connect(timerHist,&QTimer::timeout,this,&HistogramWidget::dealTimeOut);

    iniPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    iniName = iniPath + "/AcqirisTDC_qt/histogramConfigurations" + QString::number(index) +".ini";
    QFileInfo iniInfo(iniName);
    if (iniInfo.isFile())
        loadFromIni();
    else
        saveToIni();
}

HistogramWidget::~HistogramWidget()
{
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
    emit askDealAcqBankSwitchHist(index);
    timerHist->start(1000.0*accumulateTime);
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

void HistogramWidget::dealAcqThreadBankSwitchHist(AqT3DataDescriptor* dataDescPtr)
{
    // prepare samples to plot
    algorithmHistogramCount(dataDescPtr, channel1, channel2, delay, timeStart, binWidth, nbrIntervals, binHeight);
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
