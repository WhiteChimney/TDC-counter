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
                tr("默认为 COM 频率的 1/100"));
    ui->checkBoxNbrCOMbuffer->setToolTip(
                tr("缓存中保留的 TDC 内存切换的数据指针个数，\n") +
                tr("通道间延时差较大且需要计算符合时需要适当调大，\n") +
                tr("但太大可能导致程序内存占用较高"));

//     初始化设备
    status = Acqrs_InitWithOptions((ViRsrc)"PCI::INSTR0", VI_FALSE,
            VI_FALSE, "CAL=0", &idInstr);

    if (status != VI_SUCCESS)
    {
        QMessageBox::critical(this,
                              QString("警告"),
                              QString("未发现可操控设备"),
                              QMessageBox::Ok);
    }

//    数据采集线程
    acqThread = new AcquisitionThread();
    connect(acqThread,&AcquisitionThread::acqThreadStarted,this,&Widget::dealAcqThreadStarted);
    connect(this,&Widget::acqParamReady,acqThread,&AcquisitionThread::dealAcqParamReady);
    connect(acqThread,&AcquisitionThread::acqThreadFinished,this,&Widget::dealAcqThreadFinished);
//    connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,this,&Widget::dealAcqThreadBankSwitch);
    dataPtrList.clear();
    connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,this,&Widget::updateDataPtrList);

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
<<<<<<< HEAD
