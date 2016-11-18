#-------------------------------------------------
#
# Project created by QtCreator 2016-11-18T17:49:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DelegateDemo
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    track.cpp \
    trackdelegate.cpp

HEADERS  += widget.h \
    track.h \
    trackdelegate.h
