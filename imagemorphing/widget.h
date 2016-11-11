#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

// 图片颜色结构体 
struct DOTCOLOR                                       
{  
    bool eq;                                   // 颜色值是否一致
    int r, g, b, a;                             // 颜色值变化因子 0 1 -1
    int rf, gf, bf, af;                        // From 颜色值
    int rt, gt, bt, at;                        // To 颜色值
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    virtual void paintEvent(QPaintEvent *);

private slots:
    void on_pushButton_clicked();
    void imageMorph();

private:
    void readPic();
    
private:
    Ui::Widget *ui;

    int imageLenth;                       // 图片宽度（默认长宽一致）
    bool morphing;                       // 变化方向，true正 false负
    int mStep;                              // 当前步长

    QTimer *timer;

    QImage picFrom;                     // From图片
    QImage picTo;                         // To图片
    QImage picGoto;                     // 中间图片
    
    QVector<DOTCOLOR> yzColors;
};

#endif // WIDGET_H
