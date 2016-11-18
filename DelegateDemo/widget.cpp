#include "widget.h"
#include "track.h"
#include "trackdelegate.h"
#include <QVBoxLayout>

Widget::Widget(QList<Track> *tracks, QWidget *parent)
    : QWidget(parent)
{
    this->tracks = tracks; 

    tableWidget = new QTableWidget(tracks->count(), 2); 
    tableWidget->setItemDelegate(new TrackDelegate(1)); 
    tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Track") << tr("Duration")); 

    for (int row = 0; row < tracks->count(); ++row) 
    { 
            Track track = tracks->at(row); 

            QTableWidgetItem *item0 = new QTableWidgetItem(track.title); 
            tableWidget->setItem(row, 0, item0); 

            QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(track.duration)); 
            item1->setTextAlignment(Qt::AlignRight); 
            tableWidget->setItem(row, 1, item1); 
    } 

    QVBoxLayout *mainLayout = new QVBoxLayout; 
    mainLayout->addWidget(tableWidget); 
    this->setLayout(mainLayout); 
}

