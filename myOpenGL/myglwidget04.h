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
    
    GLfloat m_rtri;                                 //控制三角形的角度  
    GLfloat m_rquad;                                //控制四边形的角度 
    
};  
  
#endif // MYGLWIDGET_H 
