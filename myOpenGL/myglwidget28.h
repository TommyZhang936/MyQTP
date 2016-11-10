#ifndef MYGLWIDGET_H  
#define MYGLWIDGET_H  
  
#include <QWidget>  
#include <QGLWidget>
#include "point_3d.h"  

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
    POINT_3D bernstein(float u, POINT_3D *p);       //计算贝塞尔方程的值  
    GLuint genBezier();                             //生成贝塞尔曲面的显示列表  
    void initBezier();                              //初始化贝塞尔曲面 
    
private:  
    bool fullscreen;                                //是否全屏显示  
   
    bool m_ShowCPoints;                             //是否显示控制点  
    GLfloat m_Rot;                                  //旋转的角度  
    int m_Divs;                                     //细分数  
  
    struct BEZIER_PATCH                             //贝塞尔曲面结构体  
    {  
        POINT_3D anchors[4][4];                     //控制点坐标  
        GLuint dlBPatch;                            //储存显示列表地址  
        GLuint texture;                             //储存绘制的纹理  
    } m_Mybezier;                                   //储存要绘制的贝塞尔曲面数据  
    
};  
  
#endif // MYGLWIDGET_H 
