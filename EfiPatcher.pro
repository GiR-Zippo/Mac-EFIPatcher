#-------------------------------------------------
#
# Project created by QtCreator 2018-09-22T14:03:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EfiPatcher
TEMPLATE = app


SOURCES += main.cpp\
        efipatcher.cpp \
    hexview/QHexView.cpp \
    OpRom/oprom.cpp \
    toolbox.cpp \
    automate.cpp

HEADERS  += efipatcher.h \
    hexview/QHexView.h \
    OpRom/oprom.h \
    toolbox.h \
    automate.h \
    radeon_bios_decode/atombios.h \
    radeon_bios_decode/radeon.h \
    radeon_bios_decode/radeon_bios_decode.h

FORMS    += efipatcher.ui \
    toolbox.ui
