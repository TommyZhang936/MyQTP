#include "myglwidget16.h"
#include <QKeyEvent>
#include <QTimer>
#include <QTextStream>

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;

    m_FileName = "E:/QTP/MyOpenGL/QtImage/Crate.bmp";        //应根据实际存放图片的路径进行修改
    m_Light = false;  
    m_Fog = 0; 
      
    m_xRot = 0.0f;  
    m_yRot = 0.0f;  
    m_xSpeed = 0.0f;  
    m_ySpeed = 0.0f;  
    m_Deep = -5.0f;  
    
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
    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);               //设置背景的颜色为雾气的颜色  
    glShadeModel(GL_SMOOTH);                            //启用阴影平滑   
      
    glClearDepth(1.0);                                  //设置深度缓存  
    glEnable(GL_DEPTH_TEST);                            //启用深度测试
    glDepthFunc(GL_LEQUAL);                             //所作深度测试的类型
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正
    //设置光源
    GLfloat LightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};  //环境光参数  
    GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  //漫散光参数  
    GLfloat LightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f}; //光源位置  
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);     //设置环境光  
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);     //设置漫射光  
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   //设置光源位置  
    glEnable(GL_LIGHT1);                                //启动一号光源 
    
    //雾部分  
    GLfloat fogColor[] = {0.5f, 0.5f, 0.5f, 1.0f};      //雾的颜色  
    glFogi(GL_FOG_MODE, GL_EXP);                        //设置雾气的初始模式  
    glFogfv(GL_FOG_COLOR, fogColor);                    //设置雾的颜色  
    glFogf(GL_FOG_DENSITY, 0.35);                       //设置雾的密度  
    glHint(GL_FOG_HINT, GL_DONT_CARE);                  //设置系统如何计算雾气  
    glFogf(GL_FOG_START, 1.0f);                         //雾的开始位置  
    glFogf(GL_FOG_END, 5.0f);                           //雾的结束位置  
    glEnable(GL_FOG);                                   //启动雾效果 -- 此行必须
 
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
    
    glTranslatef(0.0f, 0.0f, m_Deep);                   //移入屏幕  
        glRotatef(m_xRot, 1.0f, 0.0f, 0.0f);                //绕x轴旋转  
        glRotatef(m_yRot, 0.0f, 1.0f, 0.0f);                //绕y轴旋转  
      
        glBindTexture(GL_TEXTURE_2D, m_Texture);            //选择纹理  
        glBegin(GL_QUADS);                                  //开始绘制立方体  
            glNormal3f(0.0f, 1.0f, 0.0f);  
            glTexCoord2f(1.0f, 1.0f);  
            glVertex3f(1.0f, 1.0f, -1.0f);                  //右上(顶面)  
            glTexCoord2f(0.0f, 1.0f);  
            glVertex3f(-1.0f, 1.0f, -1.0f);                 //左上(顶面)  
            glTexCoord2f(0.0f, 0.0f);  
            glVertex3f(-1.0f, 1.0f, 1.0f);                  //左下(顶面)  
            glTexCoord2f(1.0f, 0.0f);  
            glVertex3f(1.0f, 1.0f, 1.0f);                   //右下(顶面)  
      
            glNormal3f(0.0f, -1.0f, 0.0f);  
            glTexCoord2f(0.0f, 0.0f);  
            glVertex3f(1.0f, -1.0f, 1.0f);                  //右上(底面)  
            glTexCoord2f(1.0f, 0.0f);  
            glVertex3f(-1.0f, -1.0f, 1.0f);                 //左上(底面)  
            glTexCoord2f(1.0f, 1.0f);  
            glVertex3f(-1.0f, -1.0f, -1.0f);                //左下(底面)  
            glTexCoord2f(0.0f, 1.0f);  
            glVertex3f(1.0f, -1.0f, -1.0f);                 //右下(底面)  
      
            glNormal3f(0.0f, 0.0f, 1.0f);  
            glTexCoord2f(1.0f, 1.0f);  
            glVertex3f(1.0f, 1.0f, 1.0f);                   //右上(前面)  
            glTexCoord2f(0.0f, 1.0f);  
            glVertex3f(-1.0f, 1.0f, 1.0f);                  //左上(前面)  
            glTexCoord2f(0.0f, 0.0f);  
            glVertex3f(-1.0f, -1.0f, 1.0f);                 //左下(前面)  
            glTexCoord2f(1.0f, 0.0f);  
            glVertex3f(1.0f, -1.0f, 1.0f);                  //右下(前面)  
      
            glNormal3f(0.0f, 0.0f, -1.0f);  
            glTexCoord2f(0.0f, 0.0f);  
            glVertex3f(1.0f, -1.0f, -1.0f);                 //右上(后面)  
            glTexCoord2f(1.0f, 0.0f);  
            glVertex3f(-1.0f, -1.0f, -1.0f);                //左上(后面)  
            glTexCoord2f(1.0f, 1.0f);  
            glVertex3f(-1.0f, 1.0f, -1.0f);                 //左下(后面)  
            glTexCoord2f(0.0f, 1.0f);  
            glVertex3f(1.0f, 1.0f, -1.0f);                  //右下(后面)  
      
            glNormal3f(-1.0f, 0.0f, 0.0f);  
            glTexCoord2f(1.0f, 1.0f);  
            glVertex3f(-1.0f, 1.0f, 1.0f);                  //右上(左面)  
            glTexCoord2f(0.0f, 1.0f);  
            glVertex3f(-1.0f, 1.0f, -1.0f);                 //左上(左面)  
            glTexCoord2f(0.0f, 0.0f);  
            glVertex3f(-1.0f, -1.0f, -1.0f);                //左下(左面)  
            glTexCoord2f(1.0f, 0.0f);  
            glVertex3f(-1.0f, -1.0f, 1.0f);                 //右下(左面)  
      
            glNormal3f(1.0f, 0.0f, 0.0f);  
            glTexCoord2f(1.0f, 1.0f);  
            glVertex3f(1.0f, 1.0f, -1.0f);                  //右上(右面)  
            glTexCoord2f(0.0f, 1.0f);  
            glVertex3f(1.0f, 1.0f, 1.0f);                   //左上(右面)  
            glTexCoord2f(0.0f, 0.0f);  
            glVertex3f(1.0f, -1.0f, 1.0f);                  //左下(右面)  
            glTexCoord2f(1.0f, 0.0f);  
            glVertex3f(1.0f, -1.0f, -1.0f);                 //右下(右面)  
        glEnd();                                            //立方体绘制结束  
      
        m_xRot += m_xSpeed;                                 //x轴旋转  
        m_yRot += m_ySpeed;                                 //y轴旋转  
  
}  

//处理键盘事件
void MyGLWidget::keyPressEvent(QKeyEvent *event)  
{  
    static GLuint fogMode[] = {GL_EXP, GL_EXP2, GL_LINEAR};
    
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
    //记住，只对光源进行设置、定位、甚至启用，光源都不会工作，除非我们启用GL_LIGHTING。
    case Qt::Key_L:                                     //L为开启关闭光源的切换键  
            m_Light = !m_Light;  
            if (m_Light)  
            {  
                glEnable(GL_LIGHTING);                      //开启光源  
            }  
            else  
            {  
                glDisable(GL_LIGHTING);                     //关闭光源  
            }  
            break;  
    //G为雾模式的切换键  
    case Qt::Key_G:                                     
            m_Fog++;  
            if (m_Fog == 3)  
            {  
                m_Fog = 0;  
            }  
            glFogi(GL_FOG_MODE, fogMode[m_Fog]);  
            break;  
            
        case Qt::Key_PageUp:                                //PageUp按下使木箱移向屏幕内部  
            m_Deep -= 0.1f;  
            break;  
        case Qt::Key_PageDown:                              //PageDown按下使木箱移向观察者  
            m_Deep += 0.1f;  
            break;  
        case Qt::Key_Up:                                    //Up按下减少m_xSpeed  
            m_xSpeed -= 0.1f;  
            break;  
        case Qt::Key_Down:                                  //Down按下增加m_xSpeed  
            m_xSpeed += 0.1f;  
            break;  
        case Qt::Key_Right:                                 //Right按下减少m_ySpeed  
            m_ySpeed -= 0.1f;  
            break;  
        case Qt::Key_Left:                                  //Left按下增加m_ySpeed  
            m_ySpeed += 0.1f;  
            break;  
            
    }  
}
