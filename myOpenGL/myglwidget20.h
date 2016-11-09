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
    
    bool m_Masking;                                 //是否使用掩模  
    bool m_Scene;                                   //控制绘制哪一层  
  
    GLfloat m_Rot;                                  //控制纹理滚动  
    QString m_FileName[5];                          //图片的路径及文件名  
    GLuint m_Texture[5];                            //储存五个纹理 
    
};  
  
#endif // MYGLWIDGET_H 
