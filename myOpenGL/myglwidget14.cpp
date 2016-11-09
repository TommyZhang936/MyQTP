#include "myglwidget14.h"
#include <QKeyEvent>
#include <QTimer>
#include <QtMath>

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;  
    
    m_Deep = -10.0f;  
    m_Rot = 0.0f; 
  
    HWND hWND = (HWND)winId();                      //获取当前窗口句柄  
    m_HDC = GetDC(hWND);                                //通过窗口句柄获得HDC  

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
  
    glTranslatef(0.0f, 0.0f, m_Deep);                   //移入屏幕10.0单位  
    glRotatef(m_Rot, 1.0f, 0.0f, 0.0f);                 //绕x轴旋转  
    glRotatef(m_Rot*1.5f, 0.0f, 1.0f, 0.0f);            //绕y轴旋转  
    glRotatef(m_Rot*1.4f, 0.0f, 0.0f, 1.0f);            //绕z轴旋转  
    //根据字体位置设置颜色  
    glColor3f(1.0f*float(cos(m_Rot/20.0f)), 1.0f*float(sin(m_Rot/25.0f)),  
              1.0f-0.5f*float(cos(m_Rot/17.0f)));  
    //输出文字到屏幕上  
    glPrint("China - %3.2f", m_Rot/50.0f);  
    m_Rot += 0.5f;                                      //旋转变量增加  
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
        
    case Qt::Key_PageUp:                                //PageUp按下字体缩小  
            m_Deep -= 0.2f;  
            break;  
    case Qt::Key_PageDown:                              //PageDown按下字体放大  
            m_Deep += 0.2f;  
            break;

    }  
}
//创建位图字体  
void MyGLWidget::buildFont()                            
{  
    HFONT font;                                         //字体句柄  
    m_Base = glGenLists(256);                           //创建256个显示列表  
    font = CreateFont(-18,                              //字体高度  
                      0,                                //字体宽度  
                      0,                                //字体的旋转角度  
                      0,                                //字体底线的旋转角度  
                      FW_BOLD,                          //字体的重量  
                      FALSE,                            //是否斜体  
                      FALSE,                            //是否使用下划线  
                      FALSE,                            //是否使用删除线  
                      ANSI_CHARSET,                     //设置字符集  
                      OUT_TT_PRECIS,                    //输出精度  
                      CLIP_DEFAULT_PRECIS,              //剪裁精度  
                      ANTIALIASED_QUALITY,              //输出质量  
                      FF_DONTCARE | DEFAULT_PITCH,      //Family and Pitch的设置  
                      LPCWSTR("微软雅黑"));        //字体名称(电脑中已装的)  
  
    SelectObject(m_HDC, font);                          //选择字体  
  
    wglUseFontOutlines(m_HDC,                           //当前HDC  
                       0,                               //从ASCII码第一个字符开始  
                       255,                             //字符数  
                       m_Base,                          //第一个显示列表的名称  
                       0.0f,                            //字体光滑度，越小越光滑  
                       0.2f,                            //在z方向突出的距离(字体的厚度)  
                       WGL_FONT_POLYGONS,               //使用多边形来生成字符，每个顶点具有独立法线  
                       m_Gmf);                          //用于储存字形度量数据(高度，宽度等) 
} 
//删除显示列表 
void MyGLWidget::killFont()                              
{  
    glDeleteLists(m_Base, 256);                          //删除96个显示列表
}  
//自定义输出文字函数 
void MyGLWidget::glPrint(const char *fmt, ...)           
{  
    float length = 0;  
    char text[256];                                     //保存字符串  
    va_list ap;                                         //指向一个变量列表的指针  
  
    if (fmt == NULL)                                    //如果无输入则返回  
    {  
        return;  
    }  
  
    va_start(ap, fmt);                                  //分析可变参数  
        vsprintf(text, fmt, ap);                        //把参数值写入字符串  
    va_end(ap);                                         //结束分析  
  
    for (unsigned int i=0; i<strlen(text); i++)                  //计算整个字符串的长度  
    {  
        length += m_Gmf[(int)text[i]].gmfCellIncX;  
    }  
    glTranslatef(-length / 2, 0.0f, 0.0f);              //左移字符串一半的长度  
  
    glPushAttrib(GL_LIST_BIT);                          //把显示列表属性压入堆栈  
    glListBase(m_Base);                                 //设置显示列表的基础值  
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);  //调用显示列表绘制字符串  
    glPopAttrib();                                      //弹出属性堆栈 
}  
