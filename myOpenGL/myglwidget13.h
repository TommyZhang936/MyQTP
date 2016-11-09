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
  
    int m_FontSize;                                 //控制字体的大小  
    GLuint m_Base;                                  //储存绘制字体的显示列表的开始位置  
    GLfloat m_Cnt1;                                 //字体移动计数器1  
    GLfloat m_Cnt2;                                 //字体移动计数器2 
    
};  
  
#endif // MYGLWIDGET_H 
