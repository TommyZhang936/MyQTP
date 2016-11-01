#ifndef ROTARYBUTTON_H
#define ROTARYBUTTON_H

#include <QObject>
#include <QWidget>
 
#include <QGraphicsView>  
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QTimeLine>  
#include <QDebug>    

#include <QVariant>
#include <QPropertyAnimation>

class RotaryButton : public QWidget
{
    Q_OBJECT
    
public:
    explicit RotaryButton(QWidget *parent = 0);
    ~RotaryButton();
    
protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);
    
private:
    bool enter;                         //是否进入
    bool leave;                         //是否离开
    
    int pixWidth;                       //图片显示宽度
    int pixHeight;                      //图片显示高度
    
    QString text;                       //显示文字
    QString image;                      //图像路径
    
    int m_angle;   //rotation angle  
    int m_delta; 
    int m_current; //store rotation direction 
    
    QPropertyAnimation *enterAnimation; //进入动画
    QPropertyAnimation *leaveAnimation; //离开动画
    
signals:
    
public slots:
    //设置显示的文字
    void setText(QString text);
    //设置显示的图像
    void setImage(QString image);
   
private slots:
    void enterImageChanged(QVariant index);
    void leaveImageChanged(QVariant index);
    
};


#endif // ROTARYBUTTON_H
