#-------------------------------------------------
#
# Project created by QtCreator 2016-10-26T10:11:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SwitchPushButton
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    switchbutton.cpp \
    imagebutton.cpp \
    masklabel.cpp \
    hilightbutton.cpp \
    switchbuttont.cpp

HEADERS  += widget.h \
    switchbutton.h \
    imagebutton.h \
    masklabel.h \
    hilightbutton.h \
    switchbuttont.h

FORMS    += widget.ui

RESOURCES += \
    images.qrc
