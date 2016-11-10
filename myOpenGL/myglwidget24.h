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
    GLuint loadTGATexture(QString filename);        //加载TGA文件并转为纹理（补充的内容）  
    void buildFont();                               //创建字体  
    void killFont();                                //删除显示列表  
    //输出字符串  
    void glPrint(GLuint x, GLuint y, int set, const char *fmt, ...);  
    
private:  
    bool fullscreen;                                //是否全屏显示  

    int m_Scroll;                                   //用来滚动屏幕  
    int m_Maxtokens;                                //记录扩展名的个数  
    int m_Swidth;                                   //剪裁宽度  
    int m_Sheight;                                  //剪裁高度  
  
    GLuint m_Base;                                  //字符显示列表的开始值  
    QString m_FileName;                             //图片的路径及文件名  
    GLuint m_Texture;                               //储存一个纹理  
    
};  
  
#endif // MYGLWIDGET_H 
