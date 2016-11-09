#include "myglwidget23.h"
#include <QKeyEvent>
#include <QTimer>
#include <GL/glu.h>

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;

    m_FileName[0] = "E:/QTP/MyOpenGL/QtImage/BG.bmp";       //应根据实际存放图片的路径进行修改
    m_FileName[1] = "E:/QTP/MyOpenGL/QtImage/Reflect.bmp";
    m_Light = false;

    m_xRot = 0.0f;
    m_yRot = 0.0f;
    m_xSpeed = 0.0f;
    m_ySpeed = 0.0f;
    m_Deep = -10.0f;
    m_Object = 0;
    
    QTimer *timer = new QTimer(this);                   //创建一个定时器
    //将定时器的计时信号与updateGL()绑定
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(10);                                   //以10ms为一个计时周期
}  
  
MyGLWidget::~MyGLWidget()  
{
    gluDeleteQuadric(m_Quadratic);  
}  

//此处开始对OpenGL进行所以设置  
void MyGLWidget::initializeGL()                         
{  
    m_Texture[0] = bindTexture(QPixmap(m_FileName[0])); //载入位图并转换成纹理
    m_Texture[1] = bindTexture(QPixmap(m_FileName[1]));
    glEnable(GL_TEXTURE_2D);                            //启用纹理映射
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);//设置s方向的纹理坐标自动生成
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);//设置t方向的纹理坐标自动生成
    
    glClearColor(0.0, 0.0, 0.0, 0.0);                   //黑色背景  
    glShadeModel(GL_SMOOTH);                            //启用阴影平滑  
      
    glClearDepth(1.0);                                  //设置深度缓存  
    glEnable(GL_DEPTH_TEST);                            //启用深度测试
    glDepthFunc(GL_LEQUAL);                             //所作深度测试的类型
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正
    
    m_Quadratic = gluNewQuadric();                        //创建二次几何体  
    gluQuadricNormals(m_Quadratic, GLU_SMOOTH);           //使用平滑法线  
    gluQuadricTexture(m_Quadratic, GL_TRUE);              //使用纹理 
    
    //设置光源
    GLfloat LightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};  //环境光参数  
    GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  //漫散光参数  
    GLfloat LightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f}; //光源位置  
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);     //设置环境光  
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);     //设置漫射光  
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   //设置光源位置  
    glEnable(GL_LIGHT1);                                //启动一号光源 
 
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
    glTranslatef(0.0f, 0.0f, m_Deep);                   //移入屏幕

    glEnable(GL_TEXTURE_GEN_S);                         //启用自动生成s方向纹理坐标
    glEnable(GL_TEXTURE_GEN_T);                         //启用自动生成t方向纹理坐标

    glBindTexture(GL_TEXTURE_2D, m_Texture[1]);         //选择球体映射纹理
    glPushMatrix();                                     //保存模型观察矩阵
    glRotatef(m_xRot, 1.0f, 0.0f, 0.0f);                //绕x轴旋转
    glRotatef(m_yRot, 0.0f, 1.0f, 0.0f);                //绕y轴旋转
    switch(m_Object)
    {
    case 0:                                             //绘制立方体
        glDrawCube();
        break;
    case 1:                                             //绘制圆柱体
        glTranslatef(0.0f, 0.0f, -1.5f);
        gluCylinder(m_Quadratic, 1.0f, 1.0f, 3.0f, 64, 64);
        break;
    case 2:                                             //绘制球
        glTranslatef(0.0f, 0.0f, -1.5f);
        gluSphere(m_Quadratic, 1.3f, 64, 64);
        break;
    case 3:                                             //绘制圆锥
        glTranslatef(0.0f, 0.0f, -1.5f);
        gluCylinder(m_Quadratic, 1.0f, 0.0f, 3.0f, 64, 64);
        break;
    }

    glPopMatrix();                                      //恢复模型观察矩阵
    glDisable(GL_TEXTURE_GEN_S);                        //禁用自动生成纹理坐标
    glDisable(GL_TEXTURE_GEN_T);

    glBindTexture(GL_TEXTURE_2D, m_Texture[0]);         //选择平面纹理
    glPushMatrix();                                     //保存模型观察矩阵
    glTranslatef(0.0f, 0.0f, -24.0);                    //移入屏幕24.0单位
    glBegin(GL_QUADS);                                  //绘制背景四边形
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-13.3f, -10.0f, 10.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(13.3f, -10.0f, 10.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(13.3f, 10.0f, 10.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-13.3f, 10.0f, 10.0f);
    glEnd();
    glPopMatrix();                                      //恢复模型观察矩阵

    m_xRot += m_xSpeed;                                 //x轴旋转
    m_yRot += m_ySpeed;                                 //y轴旋转
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
    case Qt::Key_Space:                                 //空格为物体的切换键  
            m_Object++;  
            if (m_Object == 4)
            {  
                m_Object = 0;  
            }  
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

void MyGLWidget::glDrawCube()  
{  
    glBegin(GL_QUADS);                                  //开始绘制立方体
        glNormal3f(0.0f, 0.5f, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);                  //右上(顶面)
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);                 //左上(顶面)
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);                  //左下(顶面)
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);                   //右下(顶面)

        glNormal3f(0.0f, -0.5f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);                  //右上(底面)
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);                 //左上(底面)
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);                //左下(底面)
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);                 //右下(底面)

        glNormal3f(0.0f, 0.0f, 0.5f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);                   //右上(前面)
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);                  //左上(前面)
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);                 //左下(前面)
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);                  //右下(前面)

        glNormal3f(0.0f, 0.0f, -0.5f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);                 //右上(后面)
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);                //左上(后面)
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);                 //左下(后面)
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);                  //右下(后面)

        glNormal3f(-0.5f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);                  //右上(左面)
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);                 //左上(左面)
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);                //左下(左面)
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);                 //右下(左面)

        glNormal3f(0.5f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);                  //右上(右面)
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);                   //左上(右面)
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);                  //左下(右面)
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);                 //右下(右面)
    glEnd();
}  
