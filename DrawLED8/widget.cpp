#include "widget.h"
#include "led8.h"
#include <QKeyEvent>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    fullscreen = false;
    resize(800, 600);
    LEDO *led8 = new LEDO(this);
    led8->show();

}

Widget::~Widget()
{
}

//处理键盘事件
void Widget::keyPressEvent(QKeyEvent *event)  
{  
    switch (event->key())   
    {  
    //F1为全屏和普通屏的切换键  
    case Qt::Key_F1:  
        fullscreen = !fullscreen;  
        if (fullscreen)  
        {  
            showFullScreen();  
        }  
        else  
        {  
            showNormal();  
        }  
        update();

        break;  
    //ESC为退出键  
    case Qt::Key_Escape:  
        close();  
        break;
    }
}
