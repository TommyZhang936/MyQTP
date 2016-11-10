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
    void drawObject();                              //绘制球体  
    void drawFloor();                               //绘制地面 
    
private:  
    bool fullscreen;                                //是否全屏显示  
      
    QString m_FileName[3];                          //图片的路径及文件名  
    GLuint m_Texture[3];                            //储存一个纹理  
  
    GLfloat m_xRot;                                 //x旋转角度  
    GLfloat m_yRot;                                 //y旋转角度  
    GLfloat m_xSpeed;                               //x旋转速度  
    GLfloat m_ySpeed;                               //y旋转速度  
    GLfloat m_Deep;                                 //深入屏幕的距离  
    GLfloat m_Height;                               //球离开地面的高度  
  
    GLUquadric *m_Quadratic;                        //二次几何体  
    
    bool m_Light;
    
};  
  
#endif // MYGLWIDGET_H 
