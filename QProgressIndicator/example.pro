qt += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = Example

DEPENDPATH += .
INCLUDEPATH += . intermediate

# Input
HEADERS += QProgressIndicator.h
SOURCES += main.cpp QProgressIndicator.cpp

OBJECTS_DIR = intermediate
MOC_DIR = intermediate
UI_HEADERS_DIR = intermediate
UI_SOURCES_DIR = intermediate
RCC_DIR = intermediate
