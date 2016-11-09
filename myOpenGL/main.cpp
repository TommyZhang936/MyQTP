#include <QApplication>  
#include "myglwidget23.h"
  
int main(int argc, char *argv[])  
{  
    QApplication app(argc, argv);  
      
    //glutInit( &argv, argc );
    
    MyGLWidget w;  
    w.resize(400, 300);  
    w.show();  
      
    return app.exec();  
}  
