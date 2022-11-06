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
    sources/computeCoincidenceCountMulti.cpp \
    sources/computeHistogramCounts.cpp \
    sources/computeSingleChannelCount.cpp \
    sources/configTDC.cpp \
    sources/customizedCommands.cpp \
    sources/mergeDataFiles.cpp \
    tools/QSimpleLed.cpp \
    tools/acquisitionthread.cpp \
    tools/statisticschannel.cpp \
    tools/tsp01.cpp \
    ui/coincidencewidget.cpp \
    ui/externalapplicationswidget.cpp \
    ui/histogramwidget.cpp \
    ui/statisticswidget.cpp \
    ui/testwidget.cpp \
    ui/widget.cpp

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
    include/stdafx.h \
    tools/QSimpleLed.h \
    tools/acquisitionthread.h \
    tools/statisticschannel.h \
    tools/tsp01.h \
    ui/coincidencewidget.h \
    ui/externalapplicationswidget.h \
    ui/histogramwidget.h \
    ui/statisticswidget.h \
    ui/testwidget.h \
    ui/widget.h

FORMS += \
    ui/coincidencewidget.ui \
    ui/externalapplicationswidget.ui \
    ui/histogramwidget.ui \
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

