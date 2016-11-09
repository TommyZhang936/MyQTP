#ifndef MYGLWIDGET_H  
#define MYGLWIDGET_H  
  
#include <QWidget>  
#include <QGLWidget>

class MyGLWidget : public QGLWidget
{  
    Q_OBJECT  
    
public:  
    explicit MyGLWidget(QWidget *parent = 0);  
    ~MyGLWidget();  
      
protected:  
    //对3个纯虚函数的重定义  
    void initializeGL();  
    void resizeGL(int w, int h);  
    void paintGL();  
      
    void keyPressEvent(QKeyEvent *event);           //处理键盘按下事件  
      
private:  
    bool fullscreen;                                //是否全屏显示  
      
    QString m_FileName;                             //图片的路径及文件名  
    GLuint m_Texture;                               //储存一个纹理 
    
    static const int MAX_PARTICLES = 1000;          //最大粒子数  
    static const GLfloat COLORS[12][3];             //彩虹的颜色  
    bool m_Rainbow;                                 //是否为彩虹模式  
    GLuint m_Color;                                 //当前的颜色  
  
    float m_Slowdown;                               //减速粒子  
    float m_xSpeed;                                 //x方向的速度  
    float m_ySpeed;                                 //y方向的速度  
    float m_Deep;                                   //移入屏幕的距离  
  
    struct Particle                                 //创建粒子结构体  
    {  
        bool active;                                //是否激活  
        float life;                                 //粒子生命  
        float fade;                                 //衰减速度  
  
        float r, g, b;                              //粒子颜色  
        float x, y, z;                              //位置坐标  
        float xi, yi, zi;                           //各方向速度  
        float xg, yg, zg;                           //各方向加速度  
    } m_Particles[MAX_PARTICLES];                   //存放1000个粒子的数组  
    
};  
  
#endif // MYGLWIDGET_H 
