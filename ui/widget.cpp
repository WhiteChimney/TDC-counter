#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Acqiris TDC 计数程序 V")
                           + tr(PROJECT_VERSION_0) + tr(".")
                           + tr(PROJECT_VERSION_1) + tr(".")
                           + tr(PROJECT_VERSION_2));
    this->setupAcqIndicator();
    ui->tabWidget->setCurrentWidget(ui->pageSettings);

//    清空临时文件
    QDir tempDataPath(iniPath + "/Data");
    if (tempDataPath.isReadable())
    {
        if (! tempDataPath.isEmpty())
        {
            tempDataPath.removeRecursively();
            QThread::msleep(100);
            tempDataPath.mkdir(iniPath + "/Data");
        }
    }
    else
        tempDataPath.mkdir(iniPath + "/Data");

//    检查是否存在配置文件（ini）
    QFileInfo iniInfo(iniName);
    if (iniInfo.isFile())
        loadFromIni();   // 存在配置则加载
    else
    {
        ui->textDataDirectory->setCurrentText(
                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
        saveToIni();     // 否则保存当前配置
    }
    fSingleCount->setFileName(tempFileName_SCC);
    fStream.setDevice(fSingleCount);

    ui->checkboxEnableCountEvents->setToolTip(
                tr("设置 TDC 的 COM 通道累计多少信号个数后进行内存切换，\n") +
                tr("默认为 COM 频率的 1/100\n") +
                tr("提升该频率可以降低每秒计数抖动，\n") +
                tr("但可能会增加 CPU 占用"));

//     初始化设备
    tdc = new Acqiris_TDC("PCI::INSTR0",this);
    connect(tdc,&Acqiris_TDC::acquisitionStarted,this,&Widget::dealAcqThreadStarted);
    connect(tdc,&Acqiris_TDC::acquisitionFinished,this,&Widget::dealAcqThreadFinished);
    tdc->initialize();

/*    tdc_2 = new Acqiris_TDC("PCI::INSTR1",this);
    connect(tdc_2,&Acqiris_TDC::acquisitionStarted,this,&Widget::dealAcqThreadStarted_2);
    connect(tdc_2,&Acqiris_TDC::acquisitionFinished,this,&Widget::dealAcqThreadFinished_2);
//    connect(tdc_2,&Acqiris_TDC::readyToAcquireData,this,&Widget::startAcquisitionSync);
    tdc_2->initialize();
*/
/*    QString errorMsg = "未发现可操控设备";
    if (tdc->getStatus() != VI_SUCCESS)
        errorMsg.append(" TDC 1");
   if (tdc_2->getStatus() != VI_SUCCESS)
        errorMsg.append(" TDC 2");
    if (tdc->getStatus() != VI_SUCCESS or tdc_2->getStatus() != VI_SUCCESS)
        QMessageBox::critical(this,
                          QString("警告"),
                          QString(errorMsg),
                          QMessageBox::Ok); */

//    单道计数时钟，主时钟
    timerCount = new QTimer(this);
    connect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);

//    数据保存时的倒计时时钟
    timerFile = new QTimer(this);
    connect(timerFile,&QTimer::timeout,this,&Widget::dealRecordTimeOut);

    qRegisterMetaType<AqT3DataDescriptor>("AqT3DataDescriptor");

    this->setupExtAppWidget();
}

Widget::~Widget()
{
    on_buttonExit_released();
    saveToIni();
    QDir tempDataPath(iniPath + "/Data");
    tempDataPath.removeRecursively();
    QThread::msleep(100);
    tempDataPath.mkdir(iniPath + "/Data");
    delete ui;
}















