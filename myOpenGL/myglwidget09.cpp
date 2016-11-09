#include "myglwidget09.h"
#include <QKeyEvent>
#include <QTimer>
#include <QTextStream>

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;  
    
    m_FileName = "E:/QTP/MyOpenGL/QtImage/Star.bmp";        //应根据实际存放图片的路径进行修改  
    m_Twinkle = false;                                  //默认初始状态为不闪烁  
    
    for (int i=0; i<num; i++)                           //循环初始化所有的星星  
    {  
        //随机获得星星颜色  
        m_stars[i].r = rand() % 256;  
        m_stars[i].g = rand() % 256;  
        m_stars[i].b = rand() % 256;  
        
        m_stars[i].dist = ((float)i / num) * 5.0f;      //计算星星离中心的距离，最大半径为5.0  
        m_stars[i].angle = 0.0f;                        //所以星星都从0度开始旋转  
    }  
    
    m_Deep = -15.0f;                                    //深入屏幕15.0单位  
    m_Tilt = 90.0f;                                     //初始倾角为90度(面对观察者)  
    m_Spin = 0.0f;                                      //从0度开始自转  

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
    m_Texture = bindTexture(QPixmap(m_FileName));  
    glEnable(GL_TEXTURE_2D);  
    
    glClearColor(0.0, 0.0, 0.0, 0.0);                   //黑色背景  
    glShadeModel(GL_SMOOTH);                            //启用阴影平滑  
      
    glClearDepth(1.0);                                  //设置深度缓存  
    //不打算使用深度测试，说是效果会不好，不知为何？
    //glEnable(GL_DEPTH_TEST);                            //启用深度测试
    //glDepthFunc(GL_LEQUAL);                             //所作深度测试的类型
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);                  //设置混色函数取得半透明效果  
    glEnable(GL_BLEND);                                 //开启混合(混色)
 
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
    
    for (int i=0; i<num; i++)  
        {  
            glLoadIdentity();                               //绘制每颗星星之前，重置模型观察矩阵  
            glTranslatef(0.0f, 0.0f, m_Deep);               //深入屏幕里面  
            glRotatef(m_Tilt, 1.0f, 0.0f, 0.0f);            //倾斜视角  
            glRotatef(m_stars[i].angle, 0.0f, 1.0f, 0.0f);  //旋转至当前所画星星的角度  
            glTranslatef(m_stars[i].dist, 0.0f, 0.0f);      //沿x轴正向移动  
            glRotatef(-m_stars[i].angle, 0.0f, 1.0f, 0.0f); //取消当前星星的角度  
            glRotatef(-m_Tilt, 1.0f, 0.0f, 0.0f);           //取消视角倾斜  
      
            if (m_Twinkle)                                  //启动闪烁效果  
            {  
                //使用byte型数据值指定一个颜色  
                glColor4ub(m_stars[num-i-1].r, m_stars[num-i-1].g, m_stars[num-i-1].b, 255);  
                glBegin(GL_QUADS);                          //开始绘制纹理映射过的四边形  
                    glTexCoord2f(0.0f, 0.0f);  
                    glVertex3f(-1.0f, -1.0f, 0.0f);  
                    glTexCoord2f(1.0f, 0.0f);  
                    glVertex3f(1.0f, -1.0f, 0.0f);  
                    glTexCoord2f(1.0f, 1.0f);  
                    glVertex3f(1.0f, 1.0f, 0.0f);  
                    glTexCoord2f(0.0f, 1.0f);  
                    glVertex3f(-1.0f, 1.0f, 0.0f);  
                glEnd();                                    //四边形绘制结束  
            }  
      
            glRotatef(m_Spin, 0.0f, 0.0f, 1.0f);            //绕z轴旋转星星  
            //使用byte型数据值指定一个颜色  
            glColor4ub(m_stars[i].r, m_stars[i].g, m_stars[i].b, 255);  
            glBegin(GL_QUADS);                          //开始绘制纹理映射过的四边形  
                glTexCoord2f(0.0f, 0.0f);  
                glVertex3f(-1.0f, -1.0f, 0.0f);  
                glTexCoord2f(1.0f, 0.0f);  
                glVertex3f(1.0f, -1.0f, 0.0f);  
                glTexCoord2f(1.0f, 1.0f);  
                glVertex3f(1.0f, 1.0f, 0.0f);  
                glTexCoord2f(0.0f, 1.0f);  
                glVertex3f(-1.0f, 1.0f, 0.0f);  
            glEnd();                                    //四边形绘制结束  
      
            m_Spin += 0.01f;                            //星星的自转  
            m_stars[i].angle += (float)i / num;         //改变星星的公转角度  
            m_stars[i].dist -= 0.01f;                   //改变星星离中心的距离  
            if (m_stars[i].dist < 0.0f)                 //星星到达中心了么  
            {  
                m_stars[i].dist += 5.0f;                //往外移5.0单位  
                m_stars[i].r = rand() % 256;  
                m_stars[i].g = rand() % 256;  
                m_stars[i].b = rand() % 256;  
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
    case Qt::Key_T:                                 //T为星星开启关闭闪烁的切换键  
            m_Twinkle = !m_Twinkle;  
            break;  
        case Qt::Key_Up:                                //Up按下屏幕向上倾斜  
            m_Tilt -= 0.5f;  
            break;  
        case Qt::Key_Down:                              //Down按下屏幕向下倾斜  
            m_Tilt += 0.5f;  
            break;  
        case Qt::Key_PageUp:                            //PageUp按下缩小  
            m_Deep -= 0.1f;  
            break;  
        case Qt::Key_PageDown:                          //PageDown按下放大  
            m_Deep += 0.1f;  
            break;  
    
    }  
}
