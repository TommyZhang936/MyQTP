#include "myglwidget27.h"
#include "globject.h"
#include <QKeyEvent>
#include <QTimer>
#include <QTextStream>
#include <GL/glu.h>

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;  
    
    m_xRot = 0.0f;  
    m_yRot = 0.0f;  
    m_xSpeed = 0.0f;  
    m_ySpeed = 0.0f;  
    obj = new glObject("E:/QtP/myOpenGL/QtImage/Object2.txt");  

    QTimer *timer = new QTimer(this);                   //创建一个定时器
    //将定时器的计时信号与updateGL()绑定
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(10);                                   //以10ms为一个计时周期
}  
  
MyGLWidget::~MyGLWidget()  
{
    delete obj;  
    gluDeleteQuadric(m_Quadratic); 
}  

void MyGLWidget::VMatMult(GLmatrix16f M, GLvector4f v)  //完成矩阵乘法v=M*v  
{  
    GLfloat res[4];  
    res[0] = M[0]*v[0] + M[4]*v[1] + M[8]*v[2] + M[12]*v[3];  
    res[1] = M[1]*v[0] + M[5]*v[1] + M[9]*v[2] + M[13]*v[3];  
    res[2] = M[2]*v[0] + M[6]*v[1] + M[10]*v[2] + M[14]*v[3];  
    res[3] = M[3]*v[0] + M[7]*v[1] + M[11]*v[2] + M[15]*v[3];  
    v[0] = res[0];  
    v[1] = res[1];  
    v[2] = res[2];  
    v[3] = res[3];  
}  

void MyGLWidget::drawRoom()                             //绘制房间  
{  
    glBegin(GL_QUADS);  
        //地面  
        glNormal3f(0.0f, 1.0f, 0.0f);  
        glVertex3f(-10.0f, -10.0f, -20.0f);  
        glVertex3f(-10.0f, -10.0f, 20.0f);  
        glVertex3f(10.0f, -10.0f, 20.0f);  
        glVertex3f(10.0f, -10.0f, -20.0f);  
        //天花板  
        glNormal3f(0.0f, -1.0f, 0.0f);  
        glVertex3f(-10.0f, 10.0f, 20.0f);  
        glVertex3f(-10.0f, 10.0f, -20.0f);  
        glVertex3f(10.0f, 10.0f, -20.0f);  
        glVertex3f(10.0f, 10.0f, 20.0f);  
        //前面  
        glNormal3f(0.0f, 0.0f, 1.0f);  
        glVertex3f(-10.0f, 10.0f, -20.0f);  
        glVertex3f(-10.0f, -10.0f, -20.0f);  
        glVertex3f(10.0f, -10.0f, -20.0f);  
        glVertex3f(10.0f, 10.0f, -20.0f);  
        //后面  
        glNormal3f(0.0f, 0.0f, -1.0f);  
        glVertex3f(10.0f, 10.0f, 20.0f);  
        glVertex3f(10.0f, -10.0f, 20.0f);  
        glVertex3f(-10.0f, -10.0f, 20.0f);  
        glVertex3f(-10.0f, 10.0f, 20.0f);  
        //左面  
        glNormal3f(1.0f, 0.0f, 0.0f);  
        glVertex3f(-10.0f, 10.0f, 20.0f);  
        glVertex3f(-10.0f, -10.0f, 20.0f);  
        glVertex3f(-10.0f, -10.0f, -20.0f);  
        glVertex3f(-10.0f, 10.0f, -20.0f);  
        //右面  
        glNormal3f(-1.0f, 0.0f, 0.0f);  
        glVertex3f(10.0f, 10.0f, -20.0f);  
        glVertex3f(10.0f, -10.0f, -20.0f);  
        glVertex3f(10.0f, -10.0f, 20.0f);  
        glVertex3f(10.0f, 10.0f, 20.0f);  
    glEnd();  
}  

//此处开始对OpenGL进行所以设置  
void MyGLWidget::initializeGL()                         
{  
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);               //黑色背景  
    glShadeModel(GL_SMOOTH);                            //启用阴影平滑  
    glClearDepth(1.0);                                  //设置深度缓存  
    glClearStencil(0);  
    glEnable(GL_DEPTH_TEST);                            //启用深度测试  
    glDepthFunc(GL_LEQUAL);                             //所作深度测试的类型  
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正  
  
    //光源部分  
    GLfloat LightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};  //环境光参数  
    GLfloat LightDiffuse[] = {0.6f, 0.6f, 0.6f, 1.0f};  //漫散光参数  
    GLfloat LightPosition[] = {0.0f, 5.0f, -4.0f, 1.0f};//光源位置  
    GLfloat LightSpc[] = {-0.2f, -0.2f, 0.2f, 1.0f};    //反射光参数  
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);     //设置环境光  
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);     //设置漫射光  
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   //设置光源位置  
    glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpc);        //设置反射光  
    glEnable(GL_LIGHT1);                                //启动一号光源  
    glEnable(GL_LIGHTING);                              //打开光源  
  
    //设置受光照的物体材质颜色  
    GLfloat MatAmb[] = {0.4f, 0.4f, 0.4f, 1.0f};        //材质的环境颜色  
    GLfloat MatDif[] = {0.2f, 0.6f, 0.9f, 1.0f};        //材质的散射颜色  
    GLfloat MatSpc[] = {0.0f, 0.0f, 0.0f, 1.0f};        //材质的镜面反射颜色  
    GLfloat MatShn[] = {0.0f};                          //镜面反射指数  
    glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmb);         //设置环境颜色  
    glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDif);         //设置散射颜色  
    glMaterialfv(GL_FRONT, GL_SPECULAR, MatSpc);        //设置反射颜色  
    glMaterialfv(GL_FRONT, GL_SHININESS, MatShn);       //设置反射指数  
  
    glCullFace(GL_BACK);                                //设置剔除面为背面  
    glEnable(GL_CULL_FACE);                             //启用剔除  
  
    m_Quadratic = gluNewQuadric();                      //二次几何体的初始化  
    gluQuadricNormals(m_Quadratic, GL_SMOOTH);  
    gluQuadricTexture(m_Quadratic, GL_FALSE);  
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
    GLfloat LightPos[] = {0.0f, 5.0f, -4.0f, 1.0f};     //光源位置  
    GLfloat SpherePos[] = {-4.0f, -5.0f, -6.0f};        //球体位置  
    GLfloat ObjPos[] = {-2.0f, -2.0f, -5.0f};           //模型位置  
  
    GLmatrix16f Minv;  
    GLvector4f lp;  
  
    //清空缓存  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  
    glLoadIdentity();                                   //重置模型观察矩阵  
    glTranslatef(0.0f, 0.0f, -20.0f);                   //移入屏幕20.0单位  
    glLightfv(GL_LIGHT1, GL_POSITION, LightPos);        //设置光源位置  
    //平移并绘制球体  
    glTranslatef(SpherePos[0], SpherePos[1], SpherePos[2]);  
    gluSphere(m_Quadratic, 1.5f, 64, 64);  
  
    //下面计算光源在模型坐标系中的位置  
    glLoadIdentity();  
    glRotatef(-m_yRot, 0.0f, 1.0f, 0.0f);               //以相反顺序相反方向旋转和平移  
    glRotatef(-m_xRot, 1.0f, 0.0f, 0.0f);  
    glTranslatef(-ObjPos[0], -ObjPos[1], -ObjPos[2]);  
    glGetFloatv(GL_MODELVIEW_MATRIX, Minv);             //获得从世界坐标系变化到模型坐标系的变换矩阵  
    lp[0] = LightPos[0];                                //保存光源位置  
    lp[1] = LightPos[1];  
    lp[2] = LightPos[2];  
    lp[3] = LightPos[3];  
    VMatMult(Minv, lp);                                 //矩阵乘法计算模型坐标系中的光源位置  
  
    glLoadIdentity();  
    glTranslatef(0.0f, 0.0f, -20.0f);  
    drawRoom();                                         //绘制房间  
    glTranslatef(ObjPos[0], ObjPos[1], ObjPos[2]);      //平移旋转顺序与上面相反  
    glRotatef(m_xRot, 1.0f, 0.0f, 0.0f);  
    glRotatef(m_yRot, 0.0f, 1.0f, 0.0f);  
    obj->draw();                                        //绘制模型  
    obj->castShadow(lp);                                //根据光源位置绘制阴影  
  
    glColor4f(0.7f, 0.4f, 0.0f, 1.0f);                  //设置为橘黄色  
    glDisable(GL_LIGHTING);                             //关闭光源(否则其他颜色绘制不出来)  
    glDepthMask(GL_FALSE);                              //禁用深度缓存的写入  
    glTranslatef(lp[0], lp[1], lp[2]);                  //平移并绘制光源  
    gluSphere(m_Quadratic, 0.2f, 64, 64);  
    glEnable(GL_LIGHTING);                              //打开光源  
    glDepthMask(GL_TRUE);                               //启用深度缓存的写入  
  
    m_xRot += m_xSpeed;                                 //模型绕x轴旋转  
    m_yRot += m_ySpeed;         
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
