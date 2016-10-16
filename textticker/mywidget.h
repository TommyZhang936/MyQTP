#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include "textticker.h"

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();
    
private slots:
    void on_pushButton_clicked();
    
    void on_pushButton_2_clicked();

    void on_pushButton_up_clicked();
    
    void on_pushButton_down_clicked();
    
    void on_pushButton_3_clicked();
    
private:
    Ui::MyWidget *ui;
    
    QString m_strTicker;
    int m_timeTicker;
    TextTicker *text_Ticker;
    bool tickerB;
};

#endif // MYWIDGET_H
