#-------------------------------------------------
#
# Project created by QtCreator 2022-04-26T08:11:59
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = MICROWAVE_UI
TEMPLATE = app

INCLUDEPATH += include
LIBS += -lcurl #-lusbgx


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    gpiotools.cpp \
    gpiobutton.cpp \
    monitoringtask.cpp \
    outputthread.cpp \
    sysparm.cpp \
    workerthread.cpp \
    mrfa_rf_table.c \
    starmedlogo.cpp \
    uart.cpp \
    frame.cpp \
    data.cpp \
    crccheck.cpp \
    sha256_software.cpp \
    securedic.cpp \
    ds28e25_ds28e22_ds28e15.cpp \
    1wire_ds2465.cpp \
    audiooutput.cpp \
    file.cpp \
    setup.cpp \
    calibration.cpp \
    usbotg.cpp \
    history.cpp \
    sysmessage.cpp

HEADERS  += mainwindow.h \
    global.h \
    gpiotools.h \
    include.h \
    gpiobutton.h \
    monitoringtask.h \
    mrfa_rf_table.h \
    outputthread.h \
    sysparm.h \
    workerthread.h \
    starmedlogo.h \
    uart.h \
    frame.h \
    data.h \
    crccheck.h \
    sha256_software.h \
    securedic.h \
    ds28e25_ds28e22_ds28e15.h \
    1wire_ds2465.h \
    audiooutput.h \
    file.h \
    setup.h \
    calibration.h \
    usbotg.h \
    history.h \
    sysmessage.h

FORMS    += mainwindow.ui \
    starmedlogo.ui \
    setup.ui \
    calibration.ui \
    history.ui \
    sysmessage.ui

RESOURCES += \
    fonts.qrc \
    sound.qrc \
    image.qrc



INCLUDEPATH += $$PWD/include/usbg
DEPENDPATH += $$PWD/include/usbg

