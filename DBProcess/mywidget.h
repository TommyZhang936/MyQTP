#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include "DBProcess.h"

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT
    
public:
    MyWidget(QWidget *parent = 0);
    ~MyWidget();
    
private slots:
    void on_pushButton_F_clicked();
    
    void on_pushButton_P_clicked();
    
    void on_pushButton_N_clicked();
    
    void on_pushButton_L_clicked();
    
    void on_lineEdit_Seek_textEdited(const QString &arg1);
    
private:
    void showValue(int idx = 0);
    
private:
    CDBProcess *newDB;
    QMap<int, QString> mapSql;
    
private:
    Ui::MyWidget *ui;
    
    long intRCount;
    long currentRec;
    
    bool readOK;
    int intRkey;
    double dig1num;
    QString strGuid;
    QString strOrders;
};

#endif // MYWIDGET_H
