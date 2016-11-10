#include "myglwidget28.h"
#include <QKeyEvent>
#include <QTimer>
#include <QTextStream>

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;  

    m_ShowCPoints = true;  
    m_Rot = 0.0f;  
    m_Divs = 7;  
    initBezier();  
    
    QTimer *timer = new QTimer(this);                   //创建一个定时器
    //将定时器的计时信号与updateGL()绑定
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(10);                                   //以10ms为一个计时周期
}  
  
MyGLWidget::~MyGLWidget()  
{
    glDeleteLists(m_Mybezier.dlBPatch, 1);              //删除显示列表  
}  

void MyGLWidget::initBezier()                           //初始化贝塞尔曲面  
{  
    //设置贝塞尔曲面的控制点  
    m_Mybezier.anchors[0][0] = POINT_3D(-0.75, -0.75, -0.50);  
    m_Mybezier.anchors[0][1] = POINT_3D(-0.25, -0.75,  0.00);  
    m_Mybezier.anchors[0][2] = POINT_3D( 0.25, -0.75,  0.00);  
    m_Mybezier.anchors[0][3] = POINT_3D( 0.75, -0.75, -0.50);  
    m_Mybezier.anchors[1][0] = POINT_3D(-0.75, -0.25, -0.75);  
    m_Mybezier.anchors[1][1] = POINT_3D(-0.25, -0.25,  0.50);  
    m_Mybezier.anchors[1][2] = POINT_3D( 0.25, -0.25,  0.50);  
    m_Mybezier.anchors[1][3] = POINT_3D( 0.75, -0.25, -0.75);  
    m_Mybezier.anchors[2][0] = POINT_3D(-0.75,  0.25,  0.00);  
    m_Mybezier.anchors[2][1] = POINT_3D(-0.25,  0.25, -0.50);  
    m_Mybezier.anchors[2][2] = POINT_3D( 0.25,  0.25, -0.50);  
    m_Mybezier.anchors[2][3] = POINT_3D( 0.75,  0.25,  0.00);  
    m_Mybezier.anchors[3][0] = POINT_3D(-0.75,  0.75, -0.50);  
    m_Mybezier.anchors[3][1] = POINT_3D(-0.25,  0.75, -1.00);  
    m_Mybezier.anchors[3][2] = POINT_3D( 0.25,  0.75, -1.00);  
    m_Mybezier.anchors[3][3] = POINT_3D( 0.75,  0.75, -0.50);  
  
    m_Mybezier.dlBPatch = 0;                            //默认的显示列表为0  
} 

POINT_3D MyGLWidget::bernstein(float u, POINT_3D *p)    //计算贝塞尔方程的值  
{  
    POINT_3D a = p[0] * pow(u, 3);  
    POINT_3D b = p[1] * (3*pow(u, 2)*(1-u));  
    POINT_3D c = p[2] * (3*u*pow(1-u, 2));  
    POINT_3D d = p[3] * pow(1-u, 3);  
  
    POINT_3D r = a + b + c + d;  
    return r;  
} 

GLuint MyGLWidget::genBezier()                          //生成贝塞尔曲面的显示列表  
{  
    GLuint drawlist = glGenLists(1);                    //分配1个显示列表的空间  
    POINT_3D temp[4];  
    //根据每一条曲线的细分数，分配相应的内存  
    POINT_3D *last = (POINT_3D*)malloc(sizeof(POINT_3D)*(m_Divs+1));  
  
    if (m_Mybezier.dlBPatch != 0)                       //如果显示列表存在，则删除  
    {  
        glDeleteLists(m_Mybezier.dlBPatch, 1);  
    }  
  
    temp[0] = m_Mybezier.anchors[0][3];                 //获得u方向的四个控制点  
    temp[1] = m_Mybezier.anchors[1][3];  
    temp[2] = m_Mybezier.anchors[2][3];  
    temp[3] = m_Mybezier.anchors[3][3];  
  
    for (int v=0; v<=m_Divs; v++)                       //根据细分数，创建各个分割点的参数  
    {  
        float py = ((float)v)/((float)m_Divs);  
        last[v] = bernstein(py, temp);                  //使用bernstein函数求得分割点坐标  
    }  
  
    glNewList(drawlist, GL_COMPILE);                    //绘制一个新的显示列表  
    glBindTexture(GL_TEXTURE_2D, m_Mybezier.texture);   //绑定纹理  
    for (int u=1; u<=m_Divs; u++)  
    {  
        float px = ((float)u)/((float)m_Divs);          //计算v方向上的细分点的参数  
        float pxold = ((float)u-1.0f)/((float)m_Divs);  //上一个v方向的细分点的参数  
  
        temp[0] = bernstein(px, m_Mybezier.anchors[0]); //计算每个细分点v方向上贝塞尔曲面的控制点  
        temp[1] = bernstein(px, m_Mybezier.anchors[1]);  
        temp[2] = bernstein(px, m_Mybezier.anchors[2]);  
        temp[3] = bernstein(px, m_Mybezier.anchors[3]);  
  
        glBegin(GL_TRIANGLE_STRIP);                     //开始绘制三角形带  
            for (int v=0; v<=m_Divs; v++)  
            {  
                float py = ((float)v)/((float)m_Divs);  //沿着u方向顺序绘制  
                glTexCoord2f(pxold, py);                //设置纹理坐标并绘制一个顶点  
                glVertex3d(last[v].x(), last[v].y(), last[v].z());  
  
                last[v] = bernstein(py, temp);          //计算下一个顶点  
                glTexCoord2f(px, py);                   //设置纹理坐标并绘制新的顶点  
                glVertex3d(last[v].x(), last[v].y(), last[v].z());  
            }  
        glEnd();                                        //结束三角形带的绘制  
    }  
    glEndList();                                        //显示列表绘制结束  
  
    free(last);                                         //释放分配的内存  
    return drawlist;                                    //返回创建的显示列表  
}  

//此处开始对OpenGL进行所以设置  
void MyGLWidget::initializeGL()                         
{  
    m_Mybezier.texture = bindTexture(QPixmap("E:/QtP/myOpenGL/QtImage/NeHe.bmp"));  
    glEnable(GL_TEXTURE_2D);                            //启用纹理映射  
    m_Mybezier.dlBPatch = genBezier();  
  
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);               //黑色背景  
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
    glLoadIdentity();                                   //重置模型观察矩阵  
    glTranslatef(0.0f, 0.2f, -3.0f);  
    glRotatef(-75.0f, 1.0f, 0.0f, 0.0f);  
    glRotatef(m_Rot, 0.0f, 0.0f, 1.0f);                 //绕z轴旋转  
  
    glCallList(m_Mybezier.dlBPatch);                    //调用显示列表，绘制贝塞尔曲面  
  
    if (m_ShowCPoints)                                  //是否绘制控制点  
    {  
        glDisable(GL_TEXTURE_2D);                       //禁用纹理贴图  
        glColor3f(1.0f, 0.0f, 0.0f);                    //设置颜色为红色  
        for (int i=0; i<4; i++)                         //绘制水平线  
        {  
            glBegin(GL_LINE_STRIP);  
                for (int j=0; j<4; j++)  
                {  
                    glVertex3d(m_Mybezier.anchors[i][j].x(),  
                               m_Mybezier.anchors[i][j].y(),  
                               m_Mybezier.anchors[i][j].z());  
                }  
            glEnd();  
        }  
        for (int i=0; i<4; i++)                         //绘制垂直线  
        {  
            glBegin(GL_LINE_STRIP);  
                for (int j=0; j<4; j++)  
                {  
                    glVertex3d(m_Mybezier.anchors[j][i].x(),  
                               m_Mybezier.anchors[j][i].y(),  
                               m_Mybezier.anchors[j][i].z());  
                }  
            glEnd();  
        }  
        glColor3f(1.0f, 1.0f, 1.0f);                    //恢复OpenGL属性  
        glEnable(GL_TEXTURE_2D);  
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
    case Qt::Key_Space:                                 //空格为是否显示控制点的切换键  
        m_ShowCPoints = !m_ShowCPoints;  
        break;  
    case Qt::Key_Left:                                  //Left按下向左旋转  
        m_Rot -= 1.0f;  
        break;  
    case Qt::Key_Right:                                 //Right按下向右旋转  
        m_Rot += 1.0f;  
        break;  
    case Qt::Key_Up:                                    //Up按下增加细分数  
        m_Divs++;  
        m_Mybezier.dlBPatch = genBezier();  
        break;  
    case Qt::Key_Down:                                  //Down按下减少细分数  
        if (m_Divs > 1)  
        {  
            m_Divs--;  
            m_Mybezier.dlBPatch = genBezier();  
        }  
        break; 
    }  
}
