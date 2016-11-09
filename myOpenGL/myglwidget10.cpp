#include "myglwidget10.h"
#include <QKeyEvent>
#include <QTimer>
#include <QTextStream>

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;  

    m_FileName = "E:/QtP/myOpenGL/QtImage/Mud.bmp";        //应根据实际存放图片的路径进行修改
    m_WorldFile = "E:/QtP/myOpenGL/QtImage/World.txt";
    m_Sector.numtriangles = 0;

    QFile file(m_WorldFile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);   //将要读入数据的文本打开
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line[3];
        for (int i=0; i<3; i++)                         //循环读入3个点数据
        {
            do                                          //读入数据并保证数据有效
            {
                line[i] = in.readLine();
            }
            while (line[i][0] == '/' || line[i] == "");
        }
        m_Sector.numtriangles++;                        //每成功读入3个点构成一个三角形
        TRIANGLE tempTri;
        for (int i=0; i<3; i++)                         //将数据储存于一个三角形中
        {
            QTextStream inLine(&line[i]);
            inLine >> tempTri.vertexs[i].x
                    >> tempTri.vertexs[i].y
                    >> tempTri.vertexs[i].z
                    >> tempTri.vertexs[i].u
                    >> tempTri.vertexs[i].v;
        }
        m_Sector.vTriangle.push_back(tempTri);          //将三角形放入m_Sector中
    }
    file.close();
    
    m_xPos = 0.0f;
    m_zPos = 0.0f;
    m_yRot = 0.0f;
    m_LookUpDown = 0.0f;

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
 
    GLfloat x_m, y_m, z_m, u_m, v_m;                    //顶点的临时x、y、z、u、v值
        GLfloat xTrans = -m_xPos;                           //游戏者沿x轴平移时的大小
        GLfloat zTrans = -m_zPos;                           //游戏者沿z轴平移时的大小
        GLfloat yTrans = -0.25f;                            //游戏者沿y轴略作平移，使视角准确
        GLfloat sceneroty = 360.0f - m_yRot;                //游戏者的旋转

        glRotatef(m_LookUpDown, 1.0f, 0.0f, 0.0f);          //抬头低头的旋转
        glRotatef(sceneroty, 0.0f, 1.0f, 0.0f);             //根据游戏者正面所对方向所作的旋转
        glTranslatef(xTrans, yTrans, zTrans);               //以游戏者为中心平移场景

        glBindTexture(GL_TEXTURE_2D, m_Texture);            //绑定纹理
        for (int i=0; i<m_Sector.numtriangles; i++)         //遍历所有的三角形
        {
            glBegin(GL_TRIANGLES);                          //开始绘制三角形
                glNormal3f(0.0f, 0.0f, 1.0f);               //指向前面的法线
                x_m = m_Sector.vTriangle[i].vertexs[0].x;
                y_m = m_Sector.vTriangle[i].vertexs[0].y;
                z_m = m_Sector.vTriangle[i].vertexs[0].z;
                u_m = m_Sector.vTriangle[i].vertexs[0].u;
                v_m = m_Sector.vTriangle[i].vertexs[0].v;
                glTexCoord2f(u_m, v_m);
                glVertex3f(x_m, y_m, z_m);

                x_m = m_Sector.vTriangle[i].vertexs[1].x;
                y_m = m_Sector.vTriangle[i].vertexs[1].y;
                z_m = m_Sector.vTriangle[i].vertexs[1].z;
                u_m = m_Sector.vTriangle[i].vertexs[1].u;
                v_m = m_Sector.vTriangle[i].vertexs[1].v;
                glTexCoord2f(u_m, v_m);
                glVertex3f(x_m, y_m, z_m);

                x_m = m_Sector.vTriangle[i].vertexs[2].x;
                y_m = m_Sector.vTriangle[i].vertexs[2].y;
                z_m = m_Sector.vTriangle[i].vertexs[2].z;
                u_m = m_Sector.vTriangle[i].vertexs[2].u;
                v_m = m_Sector.vTriangle[i].vertexs[2].v;
                glTexCoord2f(u_m, v_m);
                glVertex3f(x_m, y_m, z_m);
            glEnd();                                        //三角形绘制结束
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

    case Qt::Key_PageUp:                                //按下PageUp视角向上转
            m_LookUpDown -= 1.0f;
            if (m_LookUpDown < -90.0f)
            {
                m_LookUpDown = -90.0f;
            }
            break;
        case Qt::Key_PageDown:                              //按下PageDown视角向下转
            m_LookUpDown += 1.0f;
            if (m_LookUpDown > 90.0f)
            {
                m_LookUpDown = 90.0f;
            }
            break;
        case Qt::Key_Right:                                 //Right按下向左旋转场景
            m_yRot -= 1.0f;
            break;
        case Qt::Key_Left:                                  //Left按下向右旋转场景
            m_yRot += 1.0f;
            break;
        case Qt::Key_Up:                                    //Up按下向前移动
            //向前移动分到x、z上的分量
            m_xPos -= (float)sin(m_yRot * m_PIOVER180) * 0.05f;
            m_zPos -= (float)cos(m_yRot * m_PIOVER180) * 0.05f;
            break;
        case Qt::Key_Down:                                  //Down按下向后移动
            //向后移动分到x、z上的分量
            m_xPos += (float)sin(m_yRot * m_PIOVER180) * 0.05f;
            m_zPos += (float)cos(m_yRot * m_PIOVER180) * 0.05f;
            break;

    }  
}
