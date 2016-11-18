#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[]) 
{ 
        QApplication a(argc, argv); 
        QList<Track> tracks; 
        Track t1("Song 1", 200); 
        Track t2("Song 2", 150); 
        Track t3("Song 3", 120); 
        Track t4("Song 4", 210); 
        tracks << t1 << t2 << t3 << t4; 
        Widget te(&tracks, NULL); 
        te.show(); 
        return a.exec(); 
} 
