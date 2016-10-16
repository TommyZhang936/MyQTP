#include "mywidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWidget w;
    w.show();
    
    //w.setTicker(str2);
    
    return a.exec();
}
