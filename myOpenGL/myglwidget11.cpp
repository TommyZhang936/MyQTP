#include "myglwidget11.h"
#include <QKeyEvent>
#include <QTimer>
#include <QTextStream>

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;  
    
    m_xRot = 0.0f;  
    m_yRot = 0.0f;  
    m_zRot = 0.0f;  
    m_FileName = "E:/QTP/MyOpenGL/QtImage/Tim.bmp";         //应根据实际存放图片的路径进行修改  
    
    for (int x=0; x<45; x++)                            //初始化数组产生波浪效果(静止)  
    {  
        for (int y=0; y<45; y++)  
        {  
            m_Points[x][y][0] = float((x / 5.0f) - 4.5f);  
            m_Points[x][y][1] = float((y / 5.0f) - 4.5f);  
            m_Points[x][y][2] = float(sin((((x/5.0f)*40.0f)/360.0f)*3.141592654*2.0f));  
        }  
    }  
    m_WiggleCount = 0;  

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
  
    glClearColor(0.0, 0.0, 0.0, 0.0);                   //黑色背景  
    glShadeModel(GL_SMOOTH);                            //启用阴影平滑  
    glClearDepth(1.0);                                  //设置深度缓存  
    glEnable(GL_DEPTH_TEST);                            //启用深度测试  
    glDepthFunc(GL_LEQUAL);                             //所作深度测试的类型  
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正  
  
    glPolygonMode(GL_BACK, GL_FILL);                    //后表面完全填充  
    glPolygonMode(GL_FRONT, GL_LINE);                   //前表面使用线条绘制  
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
  
    glTranslatef(0.0f, 0.0f, -15.0f);                   //移入屏幕15.0单位  
    glRotatef(m_xRot, 1.0f, 0.0f, 0.0f);                //绕x旋转  
    glRotatef(m_yRot, 0.0f, 1.0f, 0.0f);                //绕y旋转  
    glRotatef(m_zRot, 0.0f, 0.0f, 1.0f);                //绕z旋转  
  
    glBindTexture(GL_TEXTURE_2D, m_Texture);            //旋转纹理  
    float flag_x1, flag_y1, flag_x2, flag_y2;           //用来将纹理分割成小的四边形方便纹理映射  
    glBegin(GL_QUADS);  
    for (int x=0; x<44; x++)  
    {  
        for (int y=0; y<44; y++)  
        {  
            //分割纹理  
            flag_x1 = float(x) / 44.0f;  
            flag_y1 = float(y) / 44.0f;  
            flag_x2 = float(x+1) / 44.0f;  
            flag_y2 = float(y+1) / 44.0f;  
  
            //绘制一个小的四边形  
            glTexCoord2f(flag_x1, flag_y1);  
            glVertex3f(m_Points[x][y][0], m_Points[x][y][1], m_Points[x][y][2]);  
            glTexCoord2f(flag_x1, flag_y2);  
            glVertex3f(m_Points[x][y+1][0], m_Points[x][y+1][1], m_Points[x][y+1][2]);  
            glTexCoord2f(flag_x2, flag_y2);  
            glVertex3f(m_Points[x+1][y+1][0], m_Points[x+1][y+1][1], m_Points[x+1][y+1][2]);  
            glTexCoord2f(flag_x2, flag_y1);  
            glVertex3f(m_Points[x+1][y][0], m_Points[x+1][y][1], m_Points[x+1][y][2]);  
        }  
    }  
    glEnd();  
  
    if (m_WiggleCount == 3)                             //用来变换波浪形状(每2帧一次)产生波浪动画  
    {  
        //利用循环使波浪值集体左移，最左侧波浪值到了最右侧  
        for (int y=0; y<45; y++)  
        {  
            float temp = m_Points[0][y][2];  
            for (int x=0; x<44; x++)  
            {  
                m_Points[x][y][2] = m_Points[x+1][y][2];  
            }  
            m_Points[44][y][2] = temp;  
        }  
        m_WiggleCount = 0;                              //计数器清零  
    }  
    m_WiggleCount++;                                    //计数器加一  
  
    m_xRot += 0.3f;  
    m_yRot += 0.2f;  
    m_zRot += 0.4f;  

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
