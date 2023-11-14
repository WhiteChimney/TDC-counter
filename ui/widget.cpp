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
//    dataPtrList.clear();
//    connect(acqThread,&AcquisitionThread::acqThreadBankSwitch,this,&Widget::updateDataPtrList);

//    单道计数时钟，主时钟
    timerCount = new QTimer(this);
    connect(timerCount,&QTimer::timeout,this,&Widget::dealCountTimeOut);

    qRegisterMetaType<AqT3DataDescriptor>("AqT3DataDescriptor");

}

Widget::~Widget()
{
    on_buttonExit_released();
    delete ui;
}
