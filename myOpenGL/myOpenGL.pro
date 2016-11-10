QT       += core gui opengl
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets 

TARGET = myOpenGL  
TEMPLATE = app  
  
HEADERS += \                             
    myglwidget29.h
  
SOURCES += \  
    main.cpp \                             
    myglwidget29.cpp
  
LIBS += -lopengl32
LIBS += -lGlU32
