#-------------------------------------------------
#
# Project created by QtCreator 2011-10-24T15:05:30
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SideBar
TEMPLATE = app

DESTDIR = bin
CONFIG -= app_bundle
UI_DIR = temp
MOC_DIR = temp
OBJECTS_DIR = temp

SOURCES += main.cpp\
        SideBarDemo.cpp

HEADERS  += SideBarDemo.h

FORMS    += SideBarDemo.ui

RESOURCES += \
    resources.qrc

