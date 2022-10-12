QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += qwt

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
RC_ICONS = resources/TDC-Counter.ico

SOURCES += \
    main.cpp \
    sources/QSimpleLed.cpp \
    sources/acquisitionthread.cpp \
    sources/computeCoincidenceCount.cpp \
    sources/computeHistogramCounts.cpp \
    sources/computeSingleChannelCount.cpp \
    sources/configTDC.cpp \
    sources/mergeDataFiles.cpp \
    sources/statisticschannel.cpp \
    ui/coincidencewidget.cpp \
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
    include/Acqiris/ivi.h \
    include/Acqiris/visa.h \
    include/Acqiris/visatype.h \
    include/Acqiris/vpptype.h \
    include/QSimpleLed.h \
    include/acquisitionthread.h \
    include/statisticschannel.h \
    include/stdafx.h \
    ui/coincidencewidget.h \
    ui/histogramwidget.h \
    ui/statisticswidget.h \
    ui/testwidget.h \
    ui/widget.h

FORMS += \
    ui/coincidencewidget.ui \
    ui/histogramwidget.ui \
    ui/statisticswidget.ui \
    ui/testwidget.ui \
    ui/widget.ui

INCLUDEPATH += $$PWD/include/
INCLUDEPATH += $$PWD/include/Acqiris/

#LIBS += -L$$PWD/lib/ -lAqDrv4
LIBS += -L$$PWD/lib/ -lAqDrv4_x64

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md
