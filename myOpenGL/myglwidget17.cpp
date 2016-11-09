#include "myglwidget17.h"
#include <QKeyEvent>
#include <QTimer>

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;  
    
    m_Cnt1 = 0.0f;  
    m_Cnt2 = 0.0f;  
    m_FileName[0] = "E:/QTP/MyOpenGL/QtImage/Font.bmp";        //应根据实际存放图片的路径进行修改  
    m_FileName[1] = "E:/QTP/MyOpenGL/QtImage/Bumps.bmp"; 

    QTimer *timer = new QTimer(this);                   //创建一个定时器
    //将定时器的计时信号与updateGL()绑定
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(10);                                   //以10ms为一个计时周期
}  
  
MyGLWidget::~MyGLWidget()  
{
    killFont();                                         //删除显示列表 
}  

//此处开始对OpenGL进行所以设置  
void MyGLWidget::initializeGL()                         
{  
    m_Texture[0] = bindTexture(QPixmap(m_FileName[0])); //载入位图并转换成纹理  
    m_Texture[1] = bindTexture(QPixmap(m_FileName[1]));  
    glEnable(GL_TEXTURE_2D);                            //启用纹理映射    

    glClearColor(0.0, 0.0, 0.0, 0.0);                   //黑色背景  
    glShadeModel(GL_SMOOTH);                            //启用阴影平滑  
      
    glClearDepth(1.0);                                  //设置深度缓存  
    glEnable(GL_DEPTH_TEST);                            //启用深度测试
    glDepthFunc(GL_LEQUAL);                             //所作深度测试的类型
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正
 
    buildFont();                                        //创建字体
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
    
    glBindTexture(GL_TEXTURE_2D, m_Texture[1]);         //设置为图像纹理  
    glTranslatef(0.0f, 0.0f, -5.0f);                    //移入屏幕5.0单位  
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);                 //绕z轴旋转45度  
    glRotatef(m_Cnt1*30.0f, 1.0f, 1.0f, 0.0f);          //绕(1,1,0)轴旋转  
    glDisable(GL_BLEND);                                //关闭融合  
    glColor3f(1.0f, 1.0f, 1.0f);                        //设置颜色为白色  
    glBegin(GL_QUADS);                                  //绘制纹理四边形  
        glTexCoord2d(0.0f, 0.0f);  
        glVertex2f(-1.0f, 1.0f);  
        glTexCoord2d(1.0f, 0.0f);  
        glVertex2f(1.0f, 1.0f);  
        glTexCoord2d(1.0f, 1.0f);  
        glVertex2f(1.0f, -1.0f);  
        glTexCoord2d(0.0f, 1.0f);  
        glVertex2f(-1.0f, -1.0f);  
    glEnd();  
  
    glRotatef(90.0f, 1.0f, 1.0f, 0.0);                  //绕(1,1,0)轴旋转90度  
    glBegin(GL_QUADS);                                  //绘制第二个四边形，与第一个垂直  
        glTexCoord2d(0.0f, 0.0f);  
        glVertex2f(-1.0f, 1.0f);  
        glTexCoord2d(1.0f, 0.0f);  
        glVertex2f(1.0f, 1.0f);  
        glTexCoord2d(1.0f, 1.0f);  
        glVertex2f(1.0f, -1.0f);  
        glTexCoord2d(0.0f, 1.0f);  
        glVertex2f(-1.0f, -1.0f);  
    glEnd();  
  
    glEnable(GL_BLEND);                                 //启用混合  
    glLoadIdentity();                                   //重置视口  
    //根据字体位置设置颜色  
    glColor3f(1.0f*float(cos(m_Cnt1)), 1.0*float(sin(m_Cnt2)), 1.0f-0.5f*float(cos(m_Cnt1+m_Cnt2)));  
    glPrint(int((280+250*cos(m_Cnt1))), int(235+200*sin(m_Cnt2)), "NeHe", 0);  
    glColor3f(1.0*float(sin(m_Cnt2)), 1.0f-0.5f*float(cos(m_Cnt1+m_Cnt2)), 1.0f*float(cos(m_Cnt1)));  
    glPrint(int((280+230*cos(m_Cnt2))), int(235+200*sin(m_Cnt1)), "OpenGL", 1);  
    glColor3f(0.0f, 0.0f, 1.0f); 
    glPrint(int(240+200*cos((m_Cnt1+m_Cnt2)/5)), 2,  "Giuseppe D'Agata", 0);  
    glColor3f(1.0f, 1.0f, 1.0f);  
    glPrint(int(242+200*cos((m_Cnt1+m_Cnt2)/5)), 2,  "China", 0);  
  
    m_Cnt1 += 0.01f;                                   //增加两个计数器的值  
    m_Cnt2 += 0.0081f;  
  
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

void MyGLWidget::buildFont()                            //创建位图字体  
{  
    float cx, cy;                                       //储存字符的x、y坐标  
    m_Base = glGenLists(256);                           //创建256个显示列表  
    glBindTexture(GL_TEXTURE_2D, m_Texture[0]);         //选择字符纹理  
  
    for (int i=0; i<256; i++)                           //循环256个显示列表  
    {  
        cx = float(i%16)/16.0f;                         //当前字符的x坐标  
        cy = float(i/16)/16.0f;                         //当前字符的y坐标  
  
        glNewList(m_Base+i, GL_COMPILE);                //开始创建显示列表  
            glBegin(GL_QUADS);                          //使用四边形显示每一个字符  
                glTexCoord2f(cx, 1-cy-0.0625f);  
                glVertex2i(0, 0);  
                glTexCoord2f(cx+0.0625f, 1-cy-0.0625f);  
                glVertex2i(16, 0);  
                glTexCoord2f(cx+0.0625f, 1-cy);  
                glVertex2i(16, 16);  
                glTexCoord2f(cx, 1-cy);  
                glVertex2i(0, 16);  
            glEnd();                                    //四边形字符绘制完成  
            glTranslated(10, 0, 0);                     //绘制完一个字符，向右平移10个单位  
        glEndList();                                    //字符显示列表完成  
    }  
}  

void MyGLWidget::killFont()                             //删除显示列表  
{  
    glDeleteLists(m_Base, 256);                         //删除256个显示列表  
}  

void MyGLWidget::glPrint(GLuint x, GLuint y,            //输入字符串  
                         const char *string, int set)  
{  
    if (set > 1)                                        //如果字符集大于1  
    {  
        set = 1;                                        //设置其为1  
    }  
  
    glBindTexture(GL_TEXTURE_2D, m_Texture[0]);         //绑定为字体纹理  
    glDisable(GL_DEPTH_TEST);                           //禁止深度测试  
    glMatrixMode(GL_PROJECTION);                        //选择投影矩阵  
    glPushMatrix();                                     //保存当前的投影矩阵  
    glLoadIdentity();                                   //重置投影矩阵  
    glOrtho(0, 640, 0, 480, -1, 1);                     //设置正投影的可视区域  
    glMatrixMode(GL_MODELVIEW);                         //选择模型观察矩阵  
    glPushMatrix();                                     //保存当前的模型观察矩阵  
    glLoadIdentity();                                   //重置模型观察矩阵  
  
    glTranslated(x, y ,0);                              //把字符原点移动到(x,y)位置  
    glListBase(m_Base-32+(128*set));                    //选择字符集  
    glCallLists(strlen(string), GL_BYTE, string);       //把字符串写到屏幕  
    glMatrixMode(GL_PROJECTION);                         //选择投影矩阵  
    glPopMatrix();                                      //设置为保存的矩阵  
    glMatrixMode(GL_MODELVIEW);                        //选择模型观察矩阵  
    glPopMatrix();                                      //设置为保存  
    glEnable(GL_DEPTH_TEST);                            //启用深度测试  
}
