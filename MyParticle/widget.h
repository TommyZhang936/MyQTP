#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *event);           //处理键盘按下事件  
    
private:
    Ui::Widget *ui;
    
private slots:
    void changeParticle();
    
private:
    void writeDebugMsg(QPainter &p);
    void drawParticle(QPainter &p);
    void debug_msg(const QString &msg);
    
    
private:
    bool fullscreen;                                //是否全屏显示  
    
    QStringList m_messages;
    
    QPoint m_mousePos;
    
    QString m_FileName;                             //图片的路径及文件名 
    QPixmap m_Pix;
    QImage m_Img;
    QRectF m_Rect;
    int m_Color;                                 //当前的颜色  
    float m_Slowdown;                               //减速粒子，数值越大减速得越慢  
    float m_xSpeed;                                 //x方向的速度  
    float m_ySpeed;                                 //y方向的速度  
    
    static const int MAX_PARTICLES = 1000;          //最大粒子数 
    static const float COLORS[12][3];             //彩虹的颜色 
    
    struct Particle                                 //创建粒子结构体  
    {  
        bool active;                                //是否激活  
        float life;                                 //粒子生命  
        float fade;                                 //衰减速度  
  
        float r, g, b;                              //粒子颜色  
        float x, y;                              //位置坐标  
        float xi, yi;                           //各方向速度  
        float xg, yg;                           //各方向加速度  
    } m_Particles[MAX_PARTICLES];                   //存放1000个粒子的数组
};

#endif // WIDGET_H
