#ifndef ROTARYBUTTON_H
#define ROTARYBUTTON_H

#include <QWidget>
#include <QPropertyAnimation>

class RotaryButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int angle READ getAngle WRITE setAngle)

public:
    explicit RotaryButton(QWidget *parent = 0);
    ~RotaryButton();

    //设置显示的文字
    void setText(QString text);
    //设置显示的图像
    void setImage(QString image);

    int getAngle() {return angle;}
    void setAngle(int ang1)
        {
            if(ang1 == angle)
            {
                return;
            }
            angle = ang1;
            update();
        }
    
protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);
    
private:    
    int angle;                              //rotation angle
    
    int pixWidth;                       //图片显示宽度
    int pixHeight;                      //图片显示高度
    
    QString text;                       //显示文字
    QString image;                      //图像路径
    
    QRect rectPix;                        //绘图框
        
    QPropertyAnimation *enterAnimation; //进入动画
    QPropertyAnimation *leaveAnimation; //离开动画
    
};

#endif // ROTARYBUTTON_H
