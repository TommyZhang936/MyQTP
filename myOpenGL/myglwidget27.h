#ifndef MYGLWIDGET_H  
#define MYGLWIDGET_H  
  
#include <QWidget>  
#include <QGLWidget>

typedef float GLmatrix16f[16];                      //把float[16]重新命名  
typedef float GLvector4f[4];                        //把float[4]重新命名  
  
class GLUquadric;  
class glObject;  

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
    void VMatMult(GLmatrix16f M, GLvector4f v);     //完成矩阵乘法v=M*v  
    void drawRoom();                                //绘制房间  
  
private:  
    bool fullscreen;                                //是否全屏显示  
   
    GLfloat m_xRot;                                 //x旋转角度  
    GLfloat m_yRot;                                 //y旋转角度  
    GLfloat m_xSpeed;                               //x旋转速度  
    GLfloat m_ySpeed;                               //y旋转速度  
  
    glObject *obj;                                  //指向模型的指针  
    GLUquadric *m_Quadratic;                        //二次几何体 
    
};  
  
#endif // MYGLWIDGET_H 
