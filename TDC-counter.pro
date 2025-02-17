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
    sources/computeSingleChannelCount.cpp \
    sources/configTDC.cpp \
    sources/vectorProcessing.cpp \
    tools/QSimpleLed.cpp \
    tools/acquisitionthread.cpp \
    ui/coincidencewidget.cpp \
    ui/coincidencewidget_ui.cpp \
    ui/testwidget.cpp \
    ui/widget.cpp \
    ui/widget_acquisition.cpp \
    ui/widget_coincidence.cpp \
    ui/widget_config.cpp \
    ui/widget_singleCount.cpp \
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
    include/projectInfo.h \
    include/stdafx.h \
    tools/QSimpleLed.h \
    tools/acquisitionthread.h \
    ui/coincidencewidget.h \
    ui/testwidget.h \
    ui/widget.h

FORMS += \
    ui/coincidencewidget.ui \
    ui/testwidget.ui \
    ui/widget.ui

INCLUDEPATH += $$PWD/include/
INCLUDEPATH += $$PWD/include/Acqiris/
INCLUDEPATH += $$PWD/include/QwtPlot/
INCLUDEPATH += $$PWD/include/VISA/
INCLUDEPATH += $$PWD/include/Thorlabs/
INCLUDEPATH += $$PWD/ui/
INCLUDEPATH += $$PWD/tools/

LIBS += -L$$PWD/lib/ -lAqDrv4_x64
LIBS += -L$$PWD/lib/ -lqwt
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

