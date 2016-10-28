#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "moneytohz.h"
#include <QBasicTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
protected:
    // 绘制文本
    void paintEvent(QPaintEvent *event);
    // 定时刷新
    void timerEvent(QTimerEvent *event);

private:
    QBasicTimer m_timer;
    QString m_strText;
    int m_nStep;
    
private slots:    
    void on_lineEdit_textChanged(const QString &arg1);
    
private:
    Ui::Widget *ui;
    
    MoneyToHZ *newMHZ1;

};

#endif // WIDGET_H
