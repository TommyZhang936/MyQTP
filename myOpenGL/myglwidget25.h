#ifndef MYGLWIDGET_H  
#define MYGLWIDGET_H  
  
#include <QWidget>  
#include <QGLWidget>

struct VERTEX                                       //顶点结构体  
{  
    float x, y, z;  
};  
  
struct OBJECT                                       //物体结构体  
{  
    int verts;                                      //物体中顶点的个数  
    QVector<VERTEX> vPoints;                        //包含顶点数据的向量  
};  
  

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
    void loadObject(QString filename, OBJECT *k);   //从文件加载一个模型  
    VERTEX calculate(int i);                        //计算第i个顶点变形过程每一步的位移  
    
private:  
    bool fullscreen;                                //是否全屏显示  

    GLfloat m_xRot;                                 //x轴旋转角度  
    GLfloat m_yRot;                                 //y轴旋转角度  
    GLfloat m_zRot;                                 //z轴旋转角度  
    GLfloat m_xSpeed;                               //x轴旋转速度  
    GLfloat m_ySpeed;                               //y轴旋转速度  
    GLfloat m_zSpeed;                               //z轴旋转速度  
    GLfloat m_xPos;                                 //x轴坐标  
    GLfloat m_yPos;                                 //y轴坐标  
    GLfloat m_zPos;                                 //z轴坐标  
  
    int m_Key;                                      //物体的标示符  
    int m_Step;                                     //当前变形步数  
    int m_Steps;                                    //变形的总步数  
    bool m_MorphOrNot;                              //是否在变形过程  
  
    OBJECT m_Morph1;                                //要绘制的4个物体  
    OBJECT m_Morph2;  
    OBJECT m_Morph3;  
    OBJECT m_Morph4;  
    OBJECT m_Helper;                                //协助绘制变形过程的物体(中间模型)  
    OBJECT *m_Src;                                  //变形的源物体  
    OBJECT *m_Dest;                                 //变形的目标物体 
    
};  
  
#endif // MYGLWIDGET_H 
