QT       += core gui opengl
  
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets 

TARGET = myOpenGL  
TEMPLATE = app  
  
HEADERS += \  
    myglwidget.h   
  
SOURCES += \  
    main.cpp \  
    myglwidget.cpp
  
LIBS += -lopengl32
