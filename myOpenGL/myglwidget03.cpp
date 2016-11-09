#include "myglwidget03.h"
#include <QKeyEvent>
#include <QTimer>
#include <QTextStream>

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;  

    QTimer *timer = new QTimer(this);                   //创建一个定时器
    //将定时器的计时信号与updateGL()绑定
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(10);                                   //以10ms为一个计时周期
}  
  
MyGLWidget::~MyGLWidget()  
{
}  

//此处开始对OpenGL进行所以设置  
void MyGLWidget::initializeGL()                         
{  
    glClearColor(0.0, 0.0, 0.0, 0.0);                   //黑色背景  
    glShadeModel(GL_SMOOTH);                            //启用阴影平滑  
      
    glClearDepth(1.0);                                  //设置深度缓存  
    glEnable(GL_DEPTH_TEST);                            //启用深度测试
    glDepthFunc(GL_LEQUAL);                             //所作深度测试的类型
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正
 
} 

//重置OpenGL窗口的大小  
void MyGLWidget::resizeGL(int w, int h)                 
{  
    glViewport(0, 0, (GLint)w, (GLint)h);               //重置当前的视口  
    glMatrixMode(GL_PROJECTION);                        //选择投影矩阵  
    glLoadIdentity();                                   //重置投影矩阵  
    //设置视口的大小  
    //gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0);  
    GLfloat zNear = 0.1;
    GLfloat zFar = 100.0;
    GLfloat aspect = (GLfloat)w/(GLfloat)h;
    GLfloat fH = tan(GLfloat(90.0/360.0*3.14159))*zNear;
    GLfloat fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
    
    glMatrixMode(GL_MODELVIEW);                         //选择模型观察矩阵  
    glLoadIdentity();                                   //重置模型观察矩阵  
} 

//从这里开始进行所有的绘制  
void MyGLWidget::paintGL()                              
{  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除屏幕和深度缓存  
    glLoadIdentity();                                   //重置当前的模型观察矩阵
    
    glTranslatef(-1.5f, 0.0f, -6.0f);                   //左移1.5单位，并移入屏幕6.0单位  
    glBegin(GL_TRIANGLES);                              //开始绘制三角形 
        glColor3f(1.0f, 0.0f, 0.0f);                    //设置当前色为红色  
        glVertex3f(0.0f, 1.0f, 0.0f);                   //上顶点  
        glColor3f(0.0f, 1.0f, 0.0f);                    //设置当前色为绿色  
        glVertex3f(-1.0f, -1.0f, 0.0f);                 //左下  
        glColor3f(0.0f, 0.0f, 1.0f);                    //设置当前色为蓝色  
        glVertex3f(1.0f, -1.0f, 0.0f);                  //右下  
    glEnd();                                            //三角形绘制结束  
    
    glTranslatef(3.0f, 0.0f, 0.0f);                     //右移3.0单位  
    glColor3f(0.5f, 0.5f, 1.0f);                        //一次性将当前色设置为蓝色 
    glBegin(GL_QUADS);                                  //开始绘制四边形  
        glVertex3f(-1.0f, 1.0f, 0.0f);                  //左上  
        glVertex3f(1.0f, 1.0f, 0.0f);                   //右上  
        glVertex3f(1.0f, -1.0f, 0.0f);                  //左下  
        glVertex3f(-1.0f, -1.0f, 0.0f);                 //右下  
    glEnd();                                            //四边形绘制结束  
  
}  

//处理键盘事件
void MyGLWidget::keyPressEvent(QKeyEvent *event)  
{  
    switch (event->key())   
    {  
    //F1为全屏和普通屏的切换键  
    case Qt::Key_F1:  
        fullscreen = !fullscreen;  
        if (fullscreen)  
        {  
            showFullScreen();  
        }  
        else  
        {  
            showNormal();  
        }  
        updateGL();
        break;  
    //ESC为退出键  
    case Qt::Key_Escape:  
        close();  
        break;

    }  
}
