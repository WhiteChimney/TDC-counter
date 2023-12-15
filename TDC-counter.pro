QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += qwt

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
RC_ICONS = resources/TDC-Counter.ico

SOURCES += \
    main.cpp \
    sources/computeCoincidenceCount.cpp \
    sources/computeHistogramCounts.cpp \
    sources/computeSingleChannelCount.cpp \
    sources/configTDC.cpp \
    sources/customizedCommands.cpp \
    sources/hyperEBQKDgeneratekey.cpp \
    sources/mergeDataFiles.cpp \
    sources/vectorProcessing.cpp \
    tools/QSimpleLed.cpp \
    tools/acqiris_tdc.cpp \
    tools/acqiris_acquisitionthread.cpp \
    tools/cliplcdnumber.cpp \
    tools/dp832a_serial.cpp \
    tools/dp832a_usb.cpp \
    tools/statisticschannel.cpp \
    tools/tsp01.cpp \
    ui/coincidencewidget.cpp \
    ui/coincidencewidget_saveData.cpp \
    ui/coincidencewidget_ui.cpp \
    ui/extAppWidget_dp832a_serial.cpp \
    ui/extAppWidget_dp832a_usb.cpp \
    ui/extAppWidget_tsp01.cpp \
    ui/externalapplicationswidget.cpp \
    ui/histogramwidget.cpp \
    ui/hyperEBQKDwidget.cpp \
    ui/statisticswidget.cpp \
    ui/testwidget.cpp \
    ui/widget.cpp \
    ui/widget_QKD.cpp \
    ui/widget_acquisition.cpp \
    ui/widget_coincidence.cpp \
    ui/widget_extApp.cpp \
    ui/widget_histogram.cpp \
    ui/widget_saveData.cpp \
    ui/widget_singleCount.cpp \
    ui/widget_statistics.cpp \
    ui/widget_test.cpp \
    ui/widget_ui.cpp

HEADERS += \
    include/Acqiris/AcqirisD1Import.h \
    include/Acqiris/AcqirisD1Interface.h \
    include/Acqiris/AcqirisDataTypes.h \
    include/Acqiris/AcqirisErrorCodes.h \
    include/Acqiris/AcqirisImport.h \
    include/Acqiris/AcqirisInterface.h \
    include/Acqiris/AcqirisT3Import.h \
    include/Acqiris/AcqirisT3Interface.h \
    include/VISA/ivi.h \
    include/VISA/visa.h \
    include/VISA/visatype.h \
    include/VISA/vpptype.h \
    include/projectInfo.h \
    include/stdafx.h \
    tools/QSimpleLed.h \
    tools/acqiris_tdc.h \
    tools/acqiris_acquisitionthread.h \
    tools/cliplcdnumber.h \
    tools/customizedfunctions.hpp \
    tools/dp832a_serial.h \
    tools/dp832a_usb.h \
    tools/statisticschannel.h \
    tools/tsp01.h \
    ui/coincidencewidget.h \
    ui/externalapplicationswidget.h \
    ui/histogramwidget.h \
    ui/hyperEBQKDwidget.h \
    ui/statisticswidget.h \
    ui/testwidget.h \
    ui/widget.h

FORMS += \
    ui/coincidencewidget.ui \
    ui/externalapplicationswidget.ui \
    ui/histogramwidget.ui \
    ui/hyperEBQKDwidget.ui \
    ui/statisticswidget.ui \
    ui/testwidget.ui \
    ui/widget.ui

INCLUDEPATH += $$PWD/include/
INCLUDEPATH += $$PWD/include/Acqiris/
INCLUDEPATH += $$PWD/include/VISA/
INCLUDEPATH += $$PWD/include/Thorlabs/
INCLUDEPATH += $$PWD/ui/
INCLUDEPATH += $$PWD/tools/

LIBS += -L$$PWD/lib/ -lAqDrv4_x64
LIBS += -L$$PWD/lib/ -lTLTSP_64
LIBS += -L$$PWD/lib/ -lvisa64
#LIBS += -L$$PWD/lib/ -lAqDrv4
#LIBS += -L$$PWD/lib/ -lTLTSP_32
#LIBS += -L$$PWD/lib/ -lvisa32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md

