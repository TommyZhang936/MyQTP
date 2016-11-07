#include "oventimer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OvenTimer w;
    w.show();
    
    return a.exec();
}
