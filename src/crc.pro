#-------------------------------------------------
#
# Project created by QtCreator 2015-06-15T13:45:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = crc
TEMPLATE = app

DEFINES += _MAPPDEBUG

SOURCES += main.cpp\
        dialog.cpp \
    crcbase.cpp \
    crc7.cpp \
    crc16.cpp \
    crc8.cpp \
    crc12.cpp \
    crc32.cpp \
    crc4.cpp \
    crc24.cpp

HEADERS  += dialog.h \
    mtypesdef.h \
    crcbase.h \
    crc7.h \
    crc16.h \
    crc8.h \
    crc12.h \
    crc32.h \
    crc4.h \
    crc24.h

FORMS    += dialog.ui

RESOURCES += \
    res.qrc

RC_FILE += res/version.rc
