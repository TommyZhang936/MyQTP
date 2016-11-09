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
    
    bool m_Light;                                   //光源的开/关  
    bool m_Blend;                                   //是否混合  
    
    GLfloat m_xRot;                                 //x旋转角度  
    GLfloat m_yRot;                                 //y旋转角度  
    GLfloat m_xSpeed;                               //x旋转速度  
    GLfloat m_ySpeed;                               //y旋转速度  
    GLfloat m_Deep;                                 //深入屏幕的距离  
    
};  
  
#endif // MYGLWIDGET_H 
