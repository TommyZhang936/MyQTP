#include "myglwidget13.h"
#include <QKeyEvent>
#include <QTimer>
#include <QtMath>

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;  
    
    m_FontSize = -18;  
    m_Cnt1 = 0.0f;  
    m_Cnt2 = 0.0f;  
  
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
    
    glTranslatef(0.0f, 0.0f, -10.0f);                   //移入屏幕10.0单位  
    //根据字体位置设置颜色  
    glColor3f(1.0f*float(cos(m_Cnt1)), 1.0f*float(sin(m_Cnt2)), 1.0f-0.5f*float(cos(m_Cnt1+m_Cnt2)));  
    //设置光栅化位置，即字体位置  
    glRasterPos2f(-4.5f+0.5f*float(cos(m_Cnt1)), 1.92f*float(sin(m_Cnt2)));  
    //输出文字到屏幕上  
    //实测汉字无效
    glPrint("测试文字Active OpenGL Text With NeHe - %7.2f", m_Cnt1);  
    m_Cnt1 += 0.051f;                                   //增加两个计数器的值  
    m_Cnt2 += 0.005f; 
    
    //值得注意的是，深入屏幕并不能缩小字体，只会给字体变化移动范围
  
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
            m_FontSize -= 1;  
            if (m_FontSize < -75)  
            {  
                m_FontSize = -75;  
            }  
            buildFont();  
            break;  
        case Qt::Key_PageDown:                              //PageDown按下字体放大  
            m_FontSize += 1;  
            if (m_FontSize > -5)  
            {  
                m_FontSize = -5;  
            }  
            buildFont();  
            break;  

    }  
}
//创建位图字体  
void MyGLWidget::buildFont()                            
{  
    HFONT font;                                         //字体句柄  
    m_Base = glGenLists(96);                            //创建96个显示列表  
      
    font = CreateFont(m_FontSize,                       //字体高度  
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
                      LPCWSTR("微软雅黑"));          //字体名称(电脑中已装的)  
  
    wglUseFontBitmaps(m_HDC, 32, 96, m_Base);           //创建96个显示列表，绘制ASCII码为32-128的字符  
    SelectObject(m_HDC, font);                          //选择字体  
} 
//删除显示列表 
void MyGLWidget::killFont()                              
{  
    glDeleteLists(m_Base, 96);                          //删除96个显示列表  
}  
//自定义输出文字函数 
void MyGLWidget::glPrint(const char *fmt, ...)           
{  
    char text[256];                                     //保存字符串  
    va_list ap;                                         //指向一个变量列表的指针  
  
    if (fmt == NULL)                                    //如果无输入则返回  
    {  
        return;  
    }  
  
    va_start(ap, fmt);                                  //分析可变参数  
        vsprintf(text, fmt, ap);                        //把参数值写入字符串  
    va_end(ap);                                         //结束分析  
  
    glPushAttrib(GL_LIST_BIT);                          //把显示列表属性压入属性堆栈  
    glListBase(m_Base - 32);                            //设置显示列表的基础值  
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);  //调用显示列表绘制字符串  
    glPopAttrib();                                      //弹出属性堆栈  
}  
