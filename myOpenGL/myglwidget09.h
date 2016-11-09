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
    bool m_Twinkle;                                 //星星是否闪烁  
  
    static const int num = 50;                      //星星的数目  
    struct star                                            //为星星创建的结构体  
    {                                    
        int r, g, b;                                //星星的颜色  
        GLfloat dist;                               //星星距离中心的距离  
        GLfloat angle;                              //当前星星所处的角度  
    } m_stars[num];  
  
    GLfloat m_Deep;                                 //星星离观察者的距离  
    GLfloat m_Tilt;                                 //星星的倾角  
    GLfloat m_Spin;                                 //星星的自转 
    
};  
  
#endif // MYGLWIDGET_H 
