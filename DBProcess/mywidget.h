#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include "DBProcess.h"

class MyWidget : public QWidget
{
    Q_OBJECT
    
public:
    MyWidget(QWidget *parent = 0);
    ~MyWidget();
    
private:
    CDBProcess *newDB;
    QMap<int, QString> mapSql;
};

#endif // MYWIDGET_H
