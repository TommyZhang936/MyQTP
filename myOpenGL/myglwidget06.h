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
    
    GLfloat m_xRot;                                 //绕x轴旋转的角度  
    GLfloat m_yRot;                                 //绕y轴旋转的角度  
    GLfloat m_zRot;                                 //绕z轴旋转的角度  
      
    QString m_FileName;                             //图片的路径及文件名  
    GLuint m_Texture;                               //储存一个纹理 
    
};  
  
#endif // MYGLWIDGET_H 
