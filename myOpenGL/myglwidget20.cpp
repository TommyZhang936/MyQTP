#include "myglwidget20.h"
#include <QKeyEvent>
#include <QTimer>
#include <GL/glu.h>

/*一点内容的补充：上面我们提到当调用glTexCoord选择纹理坐标时，如果大于1.0，OpenGL默认截取小数部分进行处理。其实这只是OpenGL默认的处理模式：GL_REPEAT。对于纹理坐标大于1.0，OpenGL有以下几种处理模式：  
GL_CLAMP - 截取
GL_REPEAT - 重复（OpenGL默认的模式）
GL_MIRRORED_REPEAT - 镜像重复
GL_CLAMP_TO_EDGE - 忽略边框截取
GL_CLAMP_TO_BORDER - 带边框的截取
我们可以利用glTexParameter函数来进行模式的转换，如：x方向的转换为glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP)，变换模式只需更改第三个参数。
而第二参数代表方向，GL_TEXTURE_WRAP_S代表x方向，GL_TEXTURE_WRAP_T代表y方向，GL_TEXTURE_WRAP_R代表z方向。
*/

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;

    m_Masking = true;  
    m_Scene = false;  
  
    m_FileName[0] = "E:/QTP/MyOpenGL/QtImage/Logo.bmp";     //纹理0  
    m_FileName[1] = "E:/QTP/MyOpenGL/QtImage/Mask1.bmp";    //掩模纹理1,作为掩模使用  
    m_FileName[2] = "E:/QTP/MyOpenGL/QtImage/Image1.bmp";   //纹理1  
    m_FileName[3] = "E:/QTP/MyOpenGL/QtImage/Mask2.bmp";    //掩模纹理2,作为掩模使用  
    m_FileName[4] = "E:/QTP/MyOpenGL/QtImage/Image2.bmp";   //纹理2  
  
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
    for (int i=0; i<5; i++)
    {  
        m_Texture[i] = bindTexture(QPixmap(m_FileName[i]));  
    }  
    glEnable(GL_TEXTURE_2D);                            //启用纹理映射  
  
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);               //黑色背景  
    glShadeModel(GL_SMOOTH);                            //启用阴影平滑  
    glClearDepth(1.0);                                  //设置深度缓存
} 

//重置OpenGL窗口的大小  
void MyGLWidget::resizeGL(int w, int h)                 
{  
    glViewport(0, 0, (GLint)w, (GLint)h);               //重置当前的视口  
    glMatrixMode(GL_PROJECTION);                        //选择投影矩阵  
    glLoadIdentity();                                   //重置投影矩阵  
    //设置视口的大小  
    gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0);  
    
    glMatrixMode(GL_MODELVIEW);                         //选择模型观察矩阵  
    glLoadIdentity();                                   //重置模型观察矩阵  
} 

//从这里开始进行所有的绘制  
void MyGLWidget::paintGL()                              
{  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除屏幕和深度缓存  
    glLoadIdentity();                                   //重置模型观察矩阵  
    glTranslatef(0.0f, 0.0f, -2.0f);                    //移入屏幕2.0单位  
  
    glBindTexture(GL_TEXTURE_2D, m_Texture[0]);         //选择Logo纹理  
    glBegin(GL_QUADS);                                  //绘制纹理四边形  
        glTexCoord2f(0.0f, -m_Rot+0.0f);  
        glVertex3f(-1.1f, -1.1f, 0.0f);  
        glTexCoord2f(3.0f, -m_Rot+0.0f);  
        glVertex3f(1.1f, -1.1f, 0.0f);  
        glTexCoord2f(3.0f, -m_Rot+3.0f);  
        glVertex3f(1.1f, 1.1f, 0.0f);  
        glTexCoord2f(0.0f, -m_Rot+3.0f);  
        glVertex3f(-1.1f, 1.1f, 0.0f);  
    glEnd();  
  
    glEnable(GL_BLEND);                                 //启用混合  
    glDisable(GL_DEPTH_TEST);                           //禁用深度测试  
  
    if (m_Masking)                                      //是否启用掩模  
    {  
        glBlendFunc(GL_DST_COLOR, GL_ZERO);             //使用黑白掩模  
    }  
  
    if (m_Scene)  
    {  
        glTranslatef(0.0f, 0.0f, -1.0f);                //移入屏幕1.0单位  
        glRotatef(m_Rot*360, 0.0f, 0.0f, 1.0f);         //绕z轴旋转  
  
        if (m_Masking)                                  //掩模是否打开  
        {  
            glBindTexture(GL_TEXTURE_2D, m_Texture[3]); //选择第二个掩模纹理  
            glBegin(GL_QUADS);                          //开始绘制四边形  
                glTexCoord2f(0.0f, 0.0f);  
                glVertex3f(-1.1f, -1.1f, 0.0f);  
                glTexCoord2f(1.0f, 0.0f);  
                glVertex3f(1.1f, -1.1f, 0.0f);  
                glTexCoord2f(1.0f, 1.0f);  
                glVertex3f(1.1f, 1.1f, 0.0f);  
                glTexCoord2f(0.0f, 1.0f);  
                glVertex3f(-1.1f, 1.1f, 0.0f);  
            glEnd();  
        }  
  
        glBlendFunc(GL_ONE, GL_ONE);                    //把纹理2复制到屏幕上  
        glBindTexture(GL_TEXTURE_2D, m_Texture[4]);     //选择第二个纹理  
        glBegin(GL_QUADS);                              //绘制四边形  
            glTexCoord2f(0.0f, 0.0f);  
            glVertex3f(-1.1f, -1.1f, 0.0f);  
            glTexCoord2f(1.0f, 0.0f);  
            glVertex3f(1.1f, -1.1f, 0.0f);  
            glTexCoord2f(1.0f, 1.0f);  
            glVertex3f(1.1f, 1.1f, 0.0f);  
            glTexCoord2f(0.0f, 1.0f);  
            glVertex3f(-1.1f, 1.1f, 0.0f);  
        glEnd();  
    }  
    else  
    {  
        if (m_Masking)                                  //掩模是否打开  
        {  
            glBindTexture(GL_TEXTURE_2D, m_Texture[1]); //选择第一个掩模纹理  
            glBegin(GL_QUADS);                          //绘制四边形  
                glTexCoord2f(m_Rot+0.0f, 0.0f);  
                glVertex3f(-1.1f, -1.1f, 0.0f);  
                glTexCoord2f(m_Rot+4.0f, 0.0f);  
                glVertex3f(1.1f, -1.1f, 0.0f);  
                glTexCoord2f(m_Rot+4.0f, 4.0f);  
                glVertex3f(1.1f, 1.1f, 0.0f);  
                glTexCoord2f(m_Rot+0.0f, 4.0f);  
                glVertex3f(-1.1f, 1.1f, 0.0f);  
            glEnd();  
        }  
  
        glBlendFunc(GL_ONE, GL_ONE);                    //把纹理1复制到屏幕  
        glBindTexture(GL_TEXTURE_2D, m_Texture[2]);     //选择第一个纹理  
        glBegin(GL_QUADS);                              //绘制四边形  
            glTexCoord2f(m_Rot+0.0f, 0.0f);  
            glVertex3f(-1.1f, -1.1f, 0.0f);  
            glTexCoord2f(m_Rot+4.0f, 0.0f);  
            glVertex3f(1.1f, -1.1f, 0.0f);  
            glTexCoord2f(m_Rot+4.0f, 4.0f);  
            glVertex3f(1.1f, 1.1f, 0.0f);  
            glTexCoord2f(m_Rot+0.0f, 4.0f);  
            glVertex3f(-1.1f, 1.1f, 0.0f);  
        glEnd();  
    }  
  
    glEnable(GL_DEPTH_TEST);                            //启用深度测试  
    glDisable(GL_BLEND);                                //禁用混合  
  
    m_Rot += 0.002f;                                    //增加调整纹理滚动旋转变量  
    if (m_Rot > 1.0f)  
    {  
        m_Rot -= 1.0f;  
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
    case Qt::Key_Space:                                 //空格为场景(图层)的切换键  
        m_Scene = !m_Scene;  
        break;  
    case Qt::Key_M:                                     //M为是否"掩膜"的切换键  
        m_Masking = !m_Masking;  
        break;  
    }  
}
