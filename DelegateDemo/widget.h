#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QList>
#include "track.h"

class Widget : public QWidget
{
    Q_OBJECT 

public: 
    Widget(QList<Track> *tracks, QWidget *parent); 

private: 
    QList<Track> *tracks; 
    QTableWidget *tableWidget; 
};

#endif // WIDGET_H
