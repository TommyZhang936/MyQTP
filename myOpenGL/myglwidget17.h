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
    //输出字符串  
    void glPrint(GLuint x, GLuint y, const char *string, int set); 
      
private:  
    bool fullscreen;                                //是否全屏显示  
    GLuint m_Base;                                  //储存绘制字体的显示列表的开始位置  
    GLfloat m_Cnt1;                                 //字体移动计数器1  
    GLfloat m_Cnt2;                                 //字体移动计数器2  
  
    QString m_FileName[2];                          //图片的路径及文件名  
    GLuint m_Texture[2];                            //储存两个纹理 
    
};  
  
#endif // MYGLWIDGET_H 
