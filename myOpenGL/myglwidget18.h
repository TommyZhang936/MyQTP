#ifndef MYGLWIDGET_H  
#define MYGLWIDGET_H  
  
#include <QWidget>  
#include <QGLWidget>
class GLUquadric;  

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
    void glDrawCube();                              //绘制立方体 
    
private:  
    bool fullscreen;                                //是否全屏显示  
      
    QString m_FileName;                             //图片的路径及文件名  
    GLuint m_Texture;                               //储存一个纹理 
    
    bool m_Light;                                   //光源的开/关  
    
    GLfloat m_xRot;                                 //x旋转角度  
    GLfloat m_yRot;                                 //y旋转角度  
    GLfloat m_xSpeed;                               //x旋转速度  
    GLfloat m_ySpeed;                               //y旋转速度  
    GLfloat m_Deep;                                 //深入屏幕的距离  
    
    int m_Part1;                                    //圆盘的起始角度  
    int m_Part2;                                    //圆盘的结束角度  
    int m_P1;                                       //增量1  
    int m_P2;                                       //增量2  
    GLUquadric *m_Quadratic;                        //二次几何体  
    GLuint m_Object;                                //绘制对象标示符  
    
    //二次几何体GLUquadric（NeHe教程用的是GLUquadricObj，源代码中GLUquadricObj是GLUquadric的别名），其实它本质上是一个二次方程，
    //即a1x^2 + a2y^2 + a3z^2 + a4xy + a5yz + a6zx + a7x + a8y + a9z + a10 = 0。
    //要知道，任何一个空间规则曲面（包括平面）都是可以用二次方程表示出来的，因此OpenGL利用二次几何体来实现一些函数，帮助用户更简单的绘画出常用的空间曲面。
};  
  
#endif // MYGLWIDGET_H 
