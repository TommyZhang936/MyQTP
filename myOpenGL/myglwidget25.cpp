#include "myglwidget25.h"
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
    m_xSpeed = 0.0f;  
    m_ySpeed = 0.0f;  
    m_zSpeed = 0.0f;  
    m_xPos = 0.0f;  
    m_yPos = 0.0f;  
    m_zPos = -10.0f;  
  
    m_Key = 1;                                              //当前模型为球  
    m_Step = 0;  
    m_Steps = 200;  
    m_MorphOrNot = false;  
  
    loadObject("E:/QtP/myOpenGL/QtImage/Sphere.txt", &m_Morph1);//加载球模型  
    loadObject("E:/QtP/myOpenGL/QtImage/Torus.txt", &m_Morph2); //加载圆环模型  
    loadObject("E:/QtP/myOpenGL/QtImage/Tube.txt", &m_Morph3);  //加载立方体模型  
    m_Morph4.verts = 486;  
    for (int i=0; i<m_Morph4.verts; i++)                    //随机设置486个顶点在[-7,7]  
    {  
        VERTEX object;  
        object.x = ((float)(rand()%14000)/1000)-7;  
        object.y = ((float)(rand()%14000)/1000)-7;  
        object.z = ((float)(rand()%14000)/1000)-7;  
        m_Morph4.vPoints.push_back(object);  
    }  
  
    loadObject("E:/QtP/myOpenGL/QtImage/Sphere.txt", &m_Helper);//初始化中间模型为球  
    m_Src = m_Dest = &m_Morph1;                             //源模型和目标模型都设置为球  

    QTimer *timer = new QTimer(this);                   //创建一个定时器
    //将定时器的计时信号与updateGL()绑定
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(10);                                   //以10ms为一个计时周期
}  
  
MyGLWidget::~MyGLWidget()  
{
}  

void MyGLWidget::loadObject(QString filename, OBJECT *k)//从文件加载一个模型  
{  
    QFile file(filename);  
    file.open(QIODevice::ReadOnly | QIODevice::Text);   //将要读入数据的文本打开  
    QTextStream in(&file);  
    QString line = in.readLine();                       //读入第一行  
    sscanf(line.toUtf8(), "Vertices: %d\n", &k->verts); //获取物体顶点的个数  
  
    for (int i=0; i<k->verts; i++)                      //循环保存每个顶点的数据  
    {  
        do                                              //读入数据并保证数据有效  
        {  
            line = in.readLine();  
        }  
        while (line[0] == '/' || line == "");  
  
        VERTEX object;  
        QTextStream inLine(&line);  
        inLine >> object.x  
               >> object.y  
               >> object.z;  
        k->vPoints.push_back(object);  
    }  
  
    file.close();  
} 

//此处开始对OpenGL进行所以设置  
void MyGLWidget::initializeGL()                         
{  
    glClearColor(0.0, 0.0, 0.0, 0.0);                   //黑色背景  
    glShadeModel(GL_SMOOTH);                            //启用阴影平滑  
    glClearDepth(1.0);                                  //设置深度缓存  
    glEnable(GL_DEPTH_TEST);                            //启用深度测试  
    glDepthFunc(GL_LESS);                               //所作深度测试的类型  
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);  
} 

VERTEX MyGLWidget::calculate(int i)                     //计算第i个顶点变形过程每一步的位移  
{  
    VERTEX a;  
    a.x = (m_Src->vPoints[i].x - m_Dest->vPoints[i].x) / m_Steps;  
    a.y = (m_Src->vPoints[i].y - m_Dest->vPoints[i].y) / m_Steps;  
    a.z = (m_Src->vPoints[i].z - m_Dest->vPoints[i].z) / m_Steps;  
    return a;  
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
    glTranslatef(m_xPos, m_yPos, m_zPos);               //平移和旋转  
    glRotatef(m_xRot, 1.0f, 0.0f, 0.0f);  
    glRotatef(m_yRot, 0.0f, 1.0f, 0.0f);  
    glRotatef(m_zRot, 0.0f, 0.0f, 1.0f);  
  
    GLfloat tx, ty, tz;                                 //顶点坐标临时变量  
    VERTEX q;                                           //保存计算的临时顶点  
    glBegin(GL_POINTS);                                 //点绘制开始  
        for (int i=0; i<m_Morph1.verts; i++)            //循环绘制每一个顶点  
        {  
            if (m_MorphOrNot)  
            {  
                q = calculate(i);  
            }  
            else  
            {  
                q.x = 0.0f;  
                q.y = 0.0f;  
                q.z = 0.0f;  
            }  
  
            m_Helper.vPoints[i].x -= q.x;               //如果在变形过程，则计算中间模型  
            m_Helper.vPoints[i].y -= q.y;  
            m_Helper.vPoints[i].z -= q.z;  
            tx = m_Helper.vPoints[i].x;                 //保存计算结果到tx、ty、tz中  
            ty = m_Helper.vPoints[i].y;  
            tz = m_Helper.vPoints[i].z;  
  
            glColor3f(0.0f, 1.0f, 1.0f);                //设置颜色  
            glVertex3f(tx, ty, tz);                     //绘制顶点  
  
            glColor3f(0.0f, 0.5f, 1.0f);                //把颜色变蓝一些  
            tx -= 2*q.x;                                //如果在变形过程，则绘制2步后的顶点  
            ty -= 2*q.y;  
            tz -= 2*q.z;  
            glVertex3f(tx, ty, tz);  
  
            glColor3f(0.0f, 0.0f, 1.0f);                //把颜色变蓝一些  
            tx -= 2*q.x;                                //如果在变形过程，则绘制2步后的顶点  
            ty -= 2*q.y;  
            tz -= 2*q.z;  
            glVertex3f(tx, ty, tz);  
        }  
    glEnd();                                            //绘制结束  
  
    if (m_MorphOrNot && (m_Step <= m_Steps))  
    {  
        m_Step++;                                       //如果在变形过程则把当前变形步数增加  
    }  
    else  
    {  
        m_MorphOrNot = false;                           //当前变形步数大于总步数时，退出变形过程  
        m_Src = m_Dest;  
        m_Step = 0;  
    }  
  
    m_xRot += m_xSpeed;                                 //自动增加旋转角度  
    m_yRot += m_ySpeed;  
    m_zRot += m_zSpeed;  
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
    case Qt::Key_PageUp:                                //PageUp按下增加m_zSpeed  
        m_zSpeed += 0.1f;  
        break;  
    case Qt::Key_PageDown:                              //PageDown按下减少m_zSpeed  
        m_zSpeed -= 0.1f;  
        break;  
    case Qt::Key_Down:                                  //Down按下增加m_xSpeed  
        m_xSpeed += 0.1f;  
        break;  
    case Qt::Key_Up:                                    //Up按下减少m_xSpeed  
        m_xSpeed -= 0.1f;  
        break;  
    case Qt::Key_Right:                                 //Right按下增加m_ySpeed  
        m_ySpeed += 0.1f;  
        break;  
    case Qt::Key_Left:                                  //Left按下减少m_ySpeed  
        m_ySpeed -= 0.1f;  
        break;  
  
    case Qt::Key_Q:                                     //Q按下放大物体  
        m_zPos -= 0.1f;  
        break;  
    case Qt::Key_Z:                                     //Z按下缩小物体  
        m_zPos += 0.1f;  
        break;  
    case Qt::Key_W:                                     //W按下上移物体  
        m_yPos -= 0.1f;  
        break;  
    case Qt::Key_S:                                     //S按下下移物体  
        m_yPos += 0.1f;  
        break;  
    case Qt::Key_D:                                     //D按下右移物体  
        m_xPos -= 0.1f;  
        break;  
    case Qt::Key_A:                                     //A按下左移物体  
        m_xPos += 0.1f;  
        break;  
  
    case Qt::Key_1:                                     //1按下进入变形过程，变形到模型1  
        if ((m_Key != 1) && !m_MorphOrNot)  
        {  
            m_Key = 1;  
            m_MorphOrNot = true;  
            m_Dest = &m_Morph1;  
        }  
        break;  
    case Qt::Key_2:                                     //2按下进入变形过程，变形到模型2  
        if ((m_Key != 2) && !m_MorphOrNot)  
        {  
            m_Key = 2;  
            m_MorphOrNot = true;  
            m_Dest = &m_Morph2;  
        }  
        break;  
    case Qt::Key_3:                                     //3按下进入变形过程，变形到模型3  
        if ((m_Key != 3) && !m_MorphOrNot)  
        {  
            m_Key = 3;  
            m_MorphOrNot = true;  
            m_Dest = &m_Morph3;  
        }  
        break;  
    case Qt::Key_4:                                     //4按下进入变形过程，变形到模型4  
        if ((m_Key != 4) && !m_MorphOrNot)  
        {  
            m_Key = 4;  
            m_MorphOrNot = true;  
            m_Dest = &m_Morph4;  
        }  
        break;  
    }  
}
