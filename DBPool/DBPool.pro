#-------------------------------------------------
#
# Project created by QtCreator 2016-10-09T15:15:53
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DBPool
TEMPLATE = app


SOURCES += main.cpp\
    dbconnectionpool.cpp \
    connectiontestthread.cpp

HEADERS  += \
    dbconnectionpool.h \
    connectiontestthread.h
