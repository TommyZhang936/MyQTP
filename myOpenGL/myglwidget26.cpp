#include "myglwidget26.h"
#include <QKeyEvent>
#include <QTimer>
#include <GL/glu.h>

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;
    m_Light = false;

    m_FileName[0] = "E:/QTP/MyOpenGL/QtImage/Envwall.bmp";  //应根据实际存放图片的路径进行修改  
    m_FileName[1] = "E:/QTP/MyOpenGL/QtImage/Ball.bmp";  
    m_FileName[2] = "E:/QTP/MyOpenGL/QtImage/Envroll.bmp";  
  
    m_xRot = 0.0f;  
    m_yRot = 0.0f;  
    m_xSpeed = 0.0f;  
    m_ySpeed = 0.0f;  
    m_Deep = -6.0f;  
    m_Height = 2.0f;   
    
    QTimer *timer = new QTimer(this);                   //创建一个定时器
    //将定时器的计时信号与updateGL()绑定
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(10);                                   //以10ms为一个计时周期
}  
  
MyGLWidget::~MyGLWidget()  
{
    gluDeleteQuadric(m_Quadratic);  
}  

void MyGLWidget::drawObject()                           //绘制球体  
{  
    glColor3f(1.0f, 1.0f, 1.0f);                        //设置为白色  
    glBindTexture(GL_TEXTURE_2D, m_Texture[1]);         //设置为球的纹理  
    gluSphere(m_Quadratic, 0.35f, 64, 64);              //绘制球  
  
    glBindTexture(GL_TEXTURE_2D, m_Texture[2]);         //设置为环境纹理  
    glColor4f(1.0f, 1.0f, 1.0f, 0.4f);                  //使用alpha为40%的白色  
    glEnable(GL_BLEND);                                 //启用混合  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);                  //设置混合因子  
    glEnable(GL_TEXTURE_GEN_S);                         //启用自动生成纹理坐标  
    glEnable(GL_TEXTURE_GEN_T);  
    gluSphere(m_Quadratic, 0.35f, 64, 64);              //绘制球体，并混合  
  
    glDisable(GL_TEXTURE_GEN_S);                        //让OpenGL恢复为默认的属性  
    glDisable(GL_TEXTURE_GEN_T);  
    glDisable(GL_BLEND);  
}  

void MyGLWidget::drawFloor()                            //绘制地面  
{  
    glBindTexture(GL_TEXTURE_2D, m_Texture[0]);         //选择地面纹理，地面由一个四边形组成  
    glBegin(GL_QUADS);  
        glNormal3f(0.0f, 1.0f, 0.0f);  
        glTexCoord2f(0.0f, 1.0f);                       //左下  
        glVertex3f(-2.0f, 0.0f, 2.0f);  
        glTexCoord2f(0.0f, 0.0f);                       //左上  
        glVertex3f(-2.0f, 0.0f, -2.0f);  
        glTexCoord2f(1.0f, 0.0f);                       //右上  
        glVertex3f(2.0f, 0.0f, -2.0f);  
        glTexCoord2f(1.0f, 1.0f);                       //右下  
        glVertex3f(2.0f, 0.0f, 2.0f);  
    glEnd();  
}  

//此处开始对OpenGL进行所以设置  
void MyGLWidget::initializeGL()                         
{  
    m_Texture[0] = bindTexture(QPixmap(m_FileName[0])); //载入位图并转换成纹理  
    m_Texture[1] = bindTexture(QPixmap(m_FileName[1]));  
    m_Texture[2] = bindTexture(QPixmap(m_FileName[2]));  
    glEnable(GL_TEXTURE_2D);                            //启用纹理映射  
  
    glClearColor(0.2f, 0.5f, 1.0f, 1.0f);               //浅蓝色背景  
    glShadeModel(GL_SMOOTH);                            //启用阴影平滑  
    glClearDepth(1.0);                                  //设置深度缓存  
    glEnable(GL_DEPTH_TEST);                            //启用深度测试  
    glDepthFunc(GL_LEQUAL);                             //所作深度测试的类型  
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正  
  
    glClearStencil(0);                                  //设置蒙板值  
  
    m_Quadratic = gluNewQuadric();                      //创建二次几何体  
    gluQuadricNormals(m_Quadratic, GLU_SMOOTH);         //使用平滑法线  
    gluQuadricTexture(m_Quadratic, GL_TRUE);            //使用纹理  
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);//设置球纹理映射，自动生成纹理坐标  
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);  
  
    //光源部分  
    GLfloat LightAmbient[] = {0.7f, 0.7f, 0.7f, 1.0f};  //环境光参数  
    GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  //漫散光参数  
    GLfloat LightPosition[] = {4.0f, 4.0f, 6.0f, 1.0f}; //光源位置  
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);     //设置环境光  
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);     //设置漫射光  
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   //设置光源位置  
    glEnable(GL_LIGHT1);                                //启动一号光源  
    //glEnable(GL_LIGHTING);                              //开启光源  
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT   //清除缓存  
            | GL_STENCIL_BUFFER_BIT);  
    double eqr[] = {0.0f, -1.0f, 0.0f, 0.0f};           //设置剪切平面  
    GLfloat LightPosition[] = {4.0f, 4.0f, 6.0f, 1.0f}; //光源位置  
  
    glLoadIdentity();                                   //重置模型观察矩阵  
    glTranslatef(0.0f, -0.6f, m_Deep);                  //平移和缩放地面  
  
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);//设置颜色掩码，不能绘制任何颜色  
    glEnable(GL_STENCIL_TEST);                          //启用蒙板缓存  
    glStencilFunc(GL_ALWAYS, 1, 1);                     //设置蒙板测试总是通过，参考值和掩码值均设为1  
    //设置当蒙板测试不通过时，保留蒙板中的值不变。如果通过则使用参考值代替蒙板值  
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
    glDisable(GL_DEPTH_TEST);                           //禁用深度测试  
    drawFloor();                                        //绘制地面  
  
    glEnable(GL_DEPTH_TEST);                            //启用深度测试  
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);    //设置颜色掩码，可以绘制任何颜色  
    //下面的设置指定当我们绘制时，不改变蒙板缓存区的值  
    glStencilFunc(GL_LEQUAL, 1, 1);  
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);  
  
    glEnable(GL_CLIP_PLANE0);                           //使用剪切平面  
    glClipPlane(GL_CLIP_PLANE0, eqr);                   //设置剪切平面为地面，并设置它的法线向下  
    glPushMatrix();                                     //保存当前的矩阵  
        glScalef(1.0f, -1.0f, 1.0f);                    //沿y轴反转  
        glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);  
        glTranslatef(0.0f, m_Height, 0.0f);  
        glRotatef(m_xRot, 1.0f, 0.0f, 0.0f);  
        glRotatef(m_yRot, 0.0f, 1.0f, 0.0f);  
        drawObject();                                   //绘制反射的球  
    glPopMatrix();                                      //弹出保存的矩阵  
    glDisable(GL_CLIP_PLANE0);                          //禁用剪切平面  
    glDisable(GL_STENCIL_TEST);                         //禁用蒙板测试  
  
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);  
    glEnable(GL_BLEND);                                 //启用混合  
    glDisable(GL_LIGHTING);                             //关闭光源  
    glColor4f(1.0f, 1.0f, 1.0f, 0.8f);                  //设置颜色为白色  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //设置混合因子  
    drawFloor();                                        //绘制地面  
  
    glEnable(GL_LIGHTING);                              //打开光源  
    glDisable(GL_BLEND);                                //禁用混合  
    glTranslatef(0.0f, m_Height, 0.0f);                 //移动和旋转  
    glRotatef(m_xRot, 1.0f, 0.0f, 0.0f);  
    glRotatef(m_yRot, 0.0f, 1.0f, 0.0f);  
    drawObject();                                       //绘制真正的球  
  
    m_xRot += m_xSpeed;                                 //球绕x轴旋转  
    m_yRot += m_ySpeed;                                 //球绕y轴旋转  
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
    case Qt::Key_PageUp:                                //PageUp按下视图移入屏幕  
        m_Deep -= 0.1f;  
        break;  
    case Qt::Key_PageDown:                              //PageDown按下视图移向观察者  
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
    case Qt::Key_Q:                                     //Q按下使球体上移  
        m_Height += 0.1f;  
        break;  
    case Qt::Key_Z:                                     //Z按下使球体下移  
        m_Height -= 0.1f;  
        break;  
            
    }  
}
