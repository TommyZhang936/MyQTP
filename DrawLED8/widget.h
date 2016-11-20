#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
protected:
    void keyPressEvent(QKeyEvent *event);           //处理键盘按下事件  
           
private:
    bool fullscreen;                                //是否全屏显示  

};

#endif // WIDGET_H
