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
    void buildFont();                               //创建字体  
    void killFont();                                //删除显示列表  
    void glPrint(const char *fmt, ...);             //输出字符串 
    
private:  
    bool fullscreen;                                //是否全屏显示  
    
    HDC m_HDC;                                      //储存当前设备的指针  
  
    GLYPHMETRICSFLOAT m_Gmf[256];                   //记录256个字符的信息  
    GLfloat m_Deep;                                 //移入屏幕的距离  
    GLuint m_Base;                                  //储存绘制字体的显示列表的开始位置  
    GLfloat m_Rot;                                  //用于旋转字体   
    
};  
  
#endif // MYGLWIDGET_H 
