#include "myglwidget12.h"
#include <QKeyEvent>
#include <QTimer>
#include <QTextStream>

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;  
    
    m_xRot = 0.0f;  
    m_yRot = 0.0f; 
    m_FileName = "E:/QTP/MyOpenGL/QtImage/Cube.bmp";         //应根据实际存放图片的路径进行修改  

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
    m_Texture = bindTexture(QPixmap(m_FileName));       //载入位图并转换成纹理  
    glEnable(GL_TEXTURE_2D);                            //启用纹理映射  
    buildLists();                                       //创建显示列表 
  
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
    static const GLfloat boxColor[5][3] =               //盒子的颜色数组  
    {  
        //亮：红、橙、黄、绿、蓝  
        {1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f},  
        {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}  
    };  
    static const GLfloat topColor[5][3] =               //顶部的颜色数组  
    {  
        //暗：红、橙、黄、绿、蓝  
        {0.5f, 0.0f, 0.0f}, {0.5f, 0.25f, 0.0f}, {0.5f, 0.5f, 0.0f},  
        {0.0f, 0.5f, 0.0f}, {0.0f, 0.5f, 0.5f}  
    };  
  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除屏幕和深度缓存  
    glBindTexture(GL_TEXTURE_2D, m_Texture);            //选择纹理  
  
    for (int y=1; y<6; y++)                             //循环来控制画盒子  
    {  
        for (int x=0; x<y; x++)  
        {  
            glLoadIdentity();  
            //设置盒子的位置  
            glTranslatef(1.4f+(float(x)*2.8f)-(float(y)*1.4f),  
                         ((6.0f-float(y))*2.4f)-7.0f, -20.0f);  
            glRotatef(45.0f+m_xRot, 1.0f, 0.0f, 0.0f);  
            glRotatef(45.0f+m_yRot, 0.0f, 1.0f, 0.0f);  
            glColor3fv(boxColor[y-1]);                  //选择盒子颜色  
            glCallList(m_Box);                          //绘制盒子  
            glColor3fv(topColor[y-1]);                  //选择顶部颜色  
            glCallList(m_Top);                          //绘制顶部  
        }  
    }  
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
    case Qt::Key_Left:                                  //Left按下向左旋转  
        m_yRot -= 1.0f;  
        break;  
    case Qt::Key_Right:                                 //Right按下向右旋转  
        m_yRot += 1.0f;  
        break;  
    case Qt::Key_Up:                                    //Up按下向上旋转  
        m_xRot -= 1.0f;  
        break;  
    case Qt::Key_Down:                                  //Down按下向下旋转  
        m_xRot += 1.0f;  
        break;      
    }  
}

void MyGLWidget::buildLists()                           //创建盒子的显示列表  
{  
    m_Box = glGenLists(2);                              //创建两个显示列表的空间  
    glNewList(m_Box, GL_COMPILE);                       //开始创建第一个显示列表  
        glBegin(GL_QUADS);  
            glTexCoord2f(0.0f, 0.0f);  
            glVertex3f(1.0f, -1.0f, 1.0f);              //右上(底面)  
            glTexCoord2f(1.0f, 0.0f);  
            glVertex3f(-1.0f, -1.0f, 1.0f);             //左上(底面)  
            glTexCoord2f(1.0f, 1.0f);  
            glVertex3f(-1.0f, -1.0f, -1.0f);            //左下(底面)  
            glTexCoord2f(0.0f, 1.0f);  
            glVertex3f(1.0f, -1.0f, -1.0f);             //右下(底面)  
  
            glTexCoord2f(1.0f, 1.0f);  
            glVertex3f(1.0f, 1.0f, 1.0f);               //右上(前面)  
            glTexCoord2f(0.0f, 1.0f);  
            glVertex3f(-1.0f, 1.0f, 1.0f);              //左上(前面)  
            glTexCoord2f(0.0f, 0.0f);  
            glVertex3f(-1.0f, -1.0f, 1.0f);             //左下(前面)  
            glTexCoord2f(1.0f, 0.0f);  
            glVertex3f(1.0f, -1.0f, 1.0f);              //右下(前面)  
  
            glTexCoord2f(0.0f, 0.0f);  
            glVertex3f(1.0f, -1.0f, -1.0f);             //右上(后面)  
            glTexCoord2f(1.0f, 0.0f);  
            glVertex3f(-1.0f, -1.0f, -1.0f);            //左上(后面)  
            glTexCoord2f(1.0f, 1.0f);  
            glVertex3f(-1.0f, 1.0f, -1.0f);             //左下(后面)  
            glTexCoord2f(0.0f, 1.0f);  
            glVertex3f(1.0f, 1.0f, -1.0f);              //右下(后面)  
  
            glTexCoord2f(1.0f, 1.0f);  
            glVertex3f(-1.0f, 1.0f, 1.0f);              //右上(左面)  
            glTexCoord2f(0.0f, 1.0f);  
            glVertex3f(-1.0f, 1.0f, -1.0f);             //左上(左面)  
            glTexCoord2f(0.0f, 0.0f);  
            glVertex3f(-1.0f, -1.0f, -1.0f);            //左下(左面)  
            glTexCoord2f(1.0f, 0.0f);  
            glVertex3f(-1.0f, -1.0f, 1.0f);             //右下(左面)  
  
            glTexCoord2f(1.0f, 1.0f);  
            glVertex3f(1.0f, 1.0f, -1.0f);              //右上(右面)  
            glTexCoord2f(0.0f, 1.0f);  
            glVertex3f(1.0f, 1.0f, 1.0f);               //左上(右面)  
            glTexCoord2f(0.0f, 0.0f);  
            glVertex3f(1.0f, -1.0f, 1.0f);              //左下(右面)  
            glTexCoord2f(1.0f, 0.0f);  
            glVertex3f(1.0f, -1.0f, -1.0f);             //右下(右面)  
        glEnd();  
    glEndList();                                        //第一个显示列表结束  
  
    m_Top = m_Box + 1;                                  //m_Box+1得到第二个显示列表的指针  
    glNewList(m_Top, GL_COMPILE);                       //开始创建第二个显示列表  
        glBegin(GL_QUADS);  
            glTexCoord2f(1.0f, 1.0f);  
            glVertex3f(1.0f, 1.0f, -1.0f);              //右上(顶面)  
            glTexCoord2f(0.0f, 1.0f);  
            glVertex3f(-1.0f, 1.0f, -1.0f);             //左上(顶面)  
            glTexCoord2f(0.0f, 0.0f);  
            glVertex3f(-1.0f, 1.0f, 1.0f);              //左下(顶面)  
            glTexCoord2f(1.0f, 0.0f);  
            glVertex3f(1.0f, 1.0f, 1.0f);               //右下(顶面)  
        glEnd();  
    glEndList();  
}  
