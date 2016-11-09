#include "myglwidget21.h"
#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include <GL/glu.h>
#include <QSound>
#include <QCoreApplication>

/*
一点内容的补充：我们介绍一下反走样，在光栅图形显示器上绘制非水平且非垂直的直线或多边形边界时，或多或少会呈现锯齿状或台阶状外观。
这是因为直线、多边形、色彩边界等是连续的，而光栅则是由离散的点组成，在光栅显示设备上表现直线、多边形等，必须在离散位置采样。
由于采样不充分重建后造成的信息失真，就叫走样(aliasing)。而用于减少或消除这种效果的技术，就称为反走样(antialiasing)。
 
而我们的程序在正投影后，绘制图像就是在光栅图形显示器上绘制了；正常关闭反走样时，是看得到锯齿状线段的，但是由于我们游戏窗口设置为640×480，各元素看起来都比较小，我们肉眼很难捕捉到开启关闭反走样之间绘图的差异。
*/

const int MyGLWidget::s_Steps[] = {1, 2, 4, 5, 10, 20}; 

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;  

    setFixedSize(640, 480);                             //设置固定的窗口大小  
  
    for (int i=0; i<11; i++)                            //初始化每个线段都没被走过  
    {  
        for (int j=0; j<11; j++)  
        {  
            if (i < 10)  
            {  
                m_Hline[i][j] = false;  
            }  
            if (j < 10)  
            {  
                m_Vline[i][j] = false;  
            }  
        }  
    }  
    m_Filled = false;  
    m_Gameover = false;  
    m_Anti = true;  
  
    m_Delay = 0;  
    m_Adjust = 3;  
    m_Lives = 5;  
    m_Level = 1;  
    m_Level2 = m_Level;  
    m_Stage = 1;  
  
    resetObjects();                                     //初始化玩家和敌人信息  
    m_Hourglass.fx = 0;                                 //初始化宝物沙漏信息  
    m_Hourglass.fy = 0;  
    m_FileName[0] = "E:/QTP/MyOpenGL/QtImage/Font.bmp";     //应根据实际存放图片的路径进行修改  
    m_FileName[1] = "E:/QTP/MyOpenGL/QtImage/Image.bmp";  
    m_Sound = new QSound("E:/QTP/MyOpenGL/QtImage/Freeze.wav");  

    
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
    buildFont();                                        //创建字体  
  
    glClearColor(0.0, 0.0, 0.0, 0.0);                   //黑色背景  
    glShadeModel(GL_SMOOTH);                            //启用阴影平滑  
    glClearDepth(1.0);                                  //设置深度缓存  
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正  
    glEnable(GL_BLEND);                                 //启用融合  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //设置融合因子 
} 

//重置OpenGL窗口的大小  
void MyGLWidget::resizeGL(int w, int h)                 
{  
    glViewport(0, 0, (GLint)w, (GLint)h);               //重置当前的视口  
    glMatrixMode(GL_PROJECTION);                        //选择投影矩阵  
    glLoadIdentity();                                   //重置投影矩阵  
    glOrtho(0.0f, 640, 480, 0.0f, -1.0f, 1.0f);         //设置正投影  
    glMatrixMode(GL_MODELVIEW);                         //选择模型观察矩阵  
    glLoadIdentity();                                   //重置模型观察矩阵  
} 

//从这里开始进行所有的绘制  
void MyGLWidget::paintGL()                              
{  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除屏幕和深度缓存  
    glBindTexture(GL_TEXTURE_2D, m_Texture[0]);         //旋转字符纹理  
    glColor3f(1.0f, 0.5f, 1.0f);  
    glPrint(207, 24, 0, "GRID CRAZY");                  //绘制游戏名称"GRID CRAZY"  
    glColor3f(1.0f, 1.0f, 0.0f);  
    glPrint(20, 20, 1, "Level:%2i", m_Level2);          //绘制当前的级别  
    glPrint(20, 40, 1, "Stage:%2i", m_Stage);           //绘制当前级别的关卡  
  
    if (m_Gameover)                                     //游戏是否结束  
    {  
        glColor3ub(rand()%255, rand()%255, rand()%255); //随机选择一种颜色  
        glPrint(472, 20, 1, "GAME OVER");               //绘制"GAME OVER"  
        glPrint(456, 40, 1, "PRESS SPACE");             //提示玩家按空格重新开始  
    }  
  
    for (int i=0; i<m_Lives-1; i++)                     //循环绘制玩家的剩余生命  
    {  
        glLoadIdentity();                               //重置当前的模型观察矩阵  
        glTranslatef(490+(i*40.0f), 40.0f, 0.0f);       //移动到屏幕右上角  
        //绘制剩余的生命图标  
        glRotatef(-m_Player.spin, 0.0f, 0.0f, 1.0f);    //旋转动画  
        glColor3f(0.0f, 1.0f, 0.0f);  
        glBegin(GL_LINES);  
            glVertex2d(-5, -5);  
            glVertex2d(5, 5);  
            glVertex2d(5, -5);  
            glVertex2d(-5, 5);  
        glEnd();  
        glRotatef(-m_Player.spin*0.5f, 0.0f, 0.0f, 1.0f);  
        glColor3f(0.0f, 0.75f, 0.0f);  
        glBegin(GL_LINES);  
            glVertex2d(-7, 0);  
            glVertex2d(7, 0);  
            glVertex2d(0, -7);  
            glVertex2d(0, 7);  
        glEnd();  
    }  
  
    m_Filled = true;                                    //在测试前，假定填充变量m_Filled为true  
    glLineWidth(2.0f);                                  //设置线宽为2.0  
    glDisable(GL_LINE_SMOOTH);                          //禁用反走样  
    glLoadIdentity();                                   //重置当前的模型观察矩阵  
    for (int i=0; i<11; i++)                            //循环11根线  
    {  
        for (int j=0; j<11; j++)                        //循环每段线段  
        {  
            glColor3f(0.0f, 0.5f, 1.0f);                //设置线为蓝色  
            if (m_Hline[i][j])                          //是否走过  
            {  
                glColor3f(1.0f, 1.0f, 1.0f);            //是，设置线为白色  
            }  
            if (i < 10)                                 //绘制水平线  
            {  
                if (!m_Hline[i][j])                     //如果当前线段没有走过，设置m_Filled为false  
                {  
                    m_Filled = false;  
                }  
  
                glBegin(GL_LINES);                      //绘制当前的线段  
                    glVertex2d(20+(i*60), 70+(j*40));  
                    glVertex2d(80+(i*60), 70+(j*40));  
                glEnd();  
            }  
  
            glColor3f(0.0f, 0.5f, 1.0f);                //设置线为蓝色  
            if (m_Vline[i][j])                          //是否走过  
            {  
                glColor3f(1.0f, 1.0f, 1.0f);            //是，设置线为白色  
            }  
            if (j < 10)                                 //绘制垂直线  
            {  
                if (!m_Vline[i][j])                     //如果当前线段没有走过，设置m_Filled为false  
                {  
                    m_Filled = false;  
                }  
  
                glBegin(GL_LINES);                      //绘制当前的线段  
                    glVertex2d(20+(i*60), 70+(j*40));  
                    glVertex2d(20+(i*60), 110+(j*40));  
                glEnd();  
            }  
  
            glEnable(GL_TEXTURE_2D);                    //使用纹理映射  
            glColor3f(1.0f, 1.0f, 1.0f);                //设置为白色  
            glBindTexture(GL_TEXTURE_2D, m_Texture[1]); //绑定纹理  
            if ((i < 10) && (j < 10))                   //绘制走过的四边形  
            {  
                if (m_Hline[i][j] && m_Hline[i][j+1]    //是否走过  
                        && m_Vline[i][j] && m_Vline[i+1][j])  
                {  
                    glBegin(GL_QUADS);                  //是，绘制它  
                        glTexCoord2f(float(i/10.0f)+0.1f, 1.0f-(float(j/10.0f)));  
                        glVertex2d(20+(i*60)+59, 70+(j*40)+1);  
                        glTexCoord2f(float(i/10.0f), 1.0f-(float(j/10.0f)));  
                        glVertex2d(20+(i*60)+1, 70+(j*40)+1);  
                        glTexCoord2f(float(i/10.0f), 1.0f-(float(j/10.0f)+0.1f));  
                        glVertex2d(20+(i*60)+1, 70+(j*40)+39);  
                        glTexCoord2f(float(i/10.0f)+0.1f, 1.0f-(float(j/10.0f)+0.1f));  
                        glVertex2d(20+(i*60)+59, 70+(j*40)+39);  
                    glEnd();  
                }  
            }  
            glDisable(GL_TEXTURE_2D);  
        }  
    }  
    glLineWidth(1.0f);  
  
    if (m_Anti)                                         //是否启用反走样  
    {  
        glEnable(GL_LINE_SMOOTH);  
    }  
  
    if (m_Hourglass.fx == 1)                            //宝物沙漏是否存在  
    {  
        //是，把宝物沙漏绘制出来  
        glLoadIdentity();  
        glTranslatef(20.0f+(m_Hourglass.x*60), 70.0f+(m_Hourglass.y*40), 0.0f);  
        glRotatef(m_Hourglass.spin, 0.0f, 0.0f, 1.0f);  
        glColor3ub(rand()%255, rand()%255, rand()%255);  
  
        glBegin(GL_LINES);  
            glVertex2d(-5, -5);  
            glVertex2d(5, 5);  
            glVertex2d(5, -5);  
            glVertex2d(-5, 5);  
            glVertex2d(-5, 5);  
            glVertex2d(5, 5);  
            glVertex2d(-5, -5);  
            glVertex2d(5, -5);  
        glEnd();  
    }  
  
    //绘制玩家  
    glLoadIdentity();  
    glTranslatef(m_Player.fx+20.0f,                     //设置玩家的位置  
                 m_Player.fy+70.0f, 0.0f);  
    glRotatef(m_Player.spin, 0.0f, 0.0f, 1.0f);         //旋转动画  
    glColor3f(0.0f, 1.0f, 0.0f);  
    glBegin(GL_LINES);  
        glVertex2d(-5, -5);  
        glVertex2d(5, 5);  
        glVertex2d(5, -5);  
        glVertex2d(-5, 5);  
    glEnd();  
    glRotatef(m_Player.spin*0.5f, 0.0f, 0.0f, 1.0f);  
    glColor3f(0.0f, 0.75f, 0.0f);  
    glBegin(GL_LINES);  
        glVertex2d(-7, 0);  
        glVertex2d(7, 0);  
        glVertex2d(0, -7);  
        glVertex2d(0, 7);  
    glEnd();  
  
    //循环绘制所有敌人  
    for (int i=0; i<(m_Stage*m_Level); i++)  
    {  
        glLoadIdentity();  
        glTranslatef(m_Enemy[i].fx+20.0f,               //设置敌人的位置  
                     m_Enemy[i].fy+70.0f, 0.0f);  
        glColor3f(1.0f, 0.5f, 0.5f);  
        glBegin(GL_LINES);  
            glVertex2d(0, -7);  
            glVertex2d(-7, 0);  
            glVertex2d(-7, 0);  
            glVertex2d(0, 7);  
            glVertex2d(0, 7);  
            glVertex2d(7, 0);  
            glVertex2d(7, 0);  
            glVertex2d(0, -7);  
        glEnd();  
        glRotatef(m_Enemy[i].spin, 0.0f, 0.0f, 1.0f);   //旋转动画  
        glColor3f(1.0f, 0.0f, 0.0f);  
        glBegin(GL_LINES);  
            glVertex2d(-7, -7);  
            glVertex2d(7, 7);  
            glVertex2d(-7, 7);  
            glVertex2d(7, -7);  
        glEnd();  
    }  
  
    updateData();                                       //更新下一帧的绘图数据  
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
    case Qt::Key_A:                                     //A为开启禁用反走样的切换键  
        m_Anti = !m_Anti;  
        break;  
    case Qt::Key_Space:                                 //空格为游戏结束时重置键  
        if (m_Gameover)                                 //游戏结束则重置变量及数据  
        {  
            m_Gameover = false;  
            m_Filled = true;  
            m_Level = 1;  
            m_Level2 = m_Level;  
            m_Stage = 0;  
            m_Lives = 5;  
            updateData();  
        }  
        break;  
    case Qt::Key_Right:                                 //按下向右右行一格  
        if ((m_Player.x < 10) && (m_Player.fx == m_Player.x*60)  
                && (m_Player.fy == m_Player.y*40) && (!m_Filled))  
        {  
            m_Hline[m_Player.x][m_Player.y] = true;  
            m_Player.x++;  
        }  
        break;  
    case Qt::Key_Left:                                  //按下向左左行一格  
        if ((m_Player.x > 0) && (m_Player.fx == m_Player.x*60)  
                && (m_Player.fy == m_Player.y*40) && (!m_Filled))  
        {  
            m_Player.x--;  
            m_Hline[m_Player.x][m_Player.y] = true;  
        }  
        break;  
    case Qt::Key_Down:                                  //按下向下下行一格  
        if ((m_Player.y < 10) && (m_Player.fx == m_Player.x*60)  
                && (m_Player.fy == m_Player.y*40) && (!m_Filled))  
        {  
            m_Vline[m_Player.x][m_Player.y] = true;  
            m_Player.y++;  
        }  
        break;  
    case Qt::Key_Up:                                    //按下向上上行一格  
        if ((m_Player.y > 0) && (m_Player.fx == m_Player.x*60)  
                && (m_Player.fy == m_Player.y*40) && (!m_Filled))  
        {  
            m_Player.y--;  
            m_Vline[m_Player.x][m_Player.y] = true;  
        }  
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
                glTexCoord2f(cx, 1.0f-cy-0.0625f);  
                glVertex2i(0, 16);  
                glTexCoord2f(cx+0.0625f, 1.0f-cy-0.0625f);  
                glVertex2i(16, 16);  
                glTexCoord2f(cx+0.0625f, 1.0f-cy);  
                glVertex2i(16, 0);  
                glTexCoord2f(cx, 1.0f-cy);  
                glVertex2i(0, 0);  
            glEnd();                                    //四边形字符绘制完成  
            glTranslated(15, 0, 0);                     //绘制完一个字符，向右平移10个单位  
        glEndList();                                    //字符显示列表完成  
    }  
}  

void MyGLWidget::killFont()                             //删除显示列表  
{  
    glDeleteLists(m_Base, 256);                         //删除256个显示列表  
} 

void MyGLWidget::glPrint(GLuint x, GLuint y, int set, const char *fmt, ...)  
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
  
    if (set > 1)                                        //如果字符集大于1  
    {  
        set = 1;                                        //设置其为1  
    }  
    glEnable(GL_TEXTURE_2D);                            //启用纹理  
    glLoadIdentity();                                   //重置模型观察矩阵  
    glTranslated(x, y ,0);                              //把字符原点移动到(x,y)位置  
    glListBase(m_Base-32+(128*set));                    //选择字符集  
  
    if (set == 0)  
    {  
        glScalef(1.5f, 2.0f, 1.0f);                     //如果是第一个字符集，放大字体  
    }  
  
    glCallLists(strlen(text), GL_BYTE, text);           //把字符串写到屏幕  
    glDisable(GL_TEXTURE_2D);                           //禁用纹理  
}  

void MyGLWidget::resetObjects()                         //重置玩家和敌人信息  
{  
    m_Player.x = 0;                                     //把玩家重置在屏幕的左上角  
    m_Player.y = 0;  
    m_Player.fx = 0;  
    m_Player.fy = 0;  
  
    for (int i=0; i<(m_Stage*m_Level); i++)             //循环随机放置所有的敌人  
    {  
        m_Enemy[i].x = 5 + rand()%6;  
        m_Enemy[i].y = rand()%11;  
        m_Enemy[i].fx = m_Enemy[i].x * 60;  
        m_Enemy[i].fy = m_Enemy[i].y * 40;  
    }  
}

void MyGLWidget::updateData()  
{  
    if (!m_Gameover)                                    //如果游戏没有结束，则进行数据更新  
    {  
        for (int i=0; i<(m_Stage*m_Level); i++)         //循环所有的敌人，敌人数由m_Stage×m_Level求得  
        {  
            //根据玩家的位置，让敌人追击玩家  
            if ((m_Enemy[i].x < m_Player.x) && (m_Enemy[i].fy == m_Enemy[i].y*40))  
            {  
                m_Enemy[i].x++;  
            }  
            if ((m_Enemy[i].x > m_Player.x) && (m_Enemy[i].fy == m_Enemy[i].y*40))  
            {  
                m_Enemy[i].x--;  
            }  
            if ((m_Enemy[i].y < m_Player.y) && (m_Enemy[i].fx == m_Enemy[i].x*60))  
            {  
                m_Enemy[i].y++;  
            }  
            if ((m_Enemy[i].y > m_Player.y) && (m_Enemy[i].fx == m_Enemy[i].x*60))  
            {  
                m_Enemy[i].y--;  
            }  
  
            //当前时间不为吃到宝物沙漏后的敌人静止时间，也不为敌人延迟时间(影响敌人的速度)  
            if ((m_Delay > (3-m_Level)) && (m_Hourglass.fx != 2))  
            {  
                m_Delay = 0;                            //重置宝物沙漏计时  
                for (int j=0; j<(m_Stage*m_Level); j++) //循环设置每个敌人的位置  
                {  
                    //每个敌人调整位置，并调整旋转变量实现动画  
                    if (m_Enemy[j].fx < m_Enemy[j].x*60)  
                    {  
                        m_Enemy[j].fx += s_Steps[m_Adjust];  
                        m_Enemy[j].spin += s_Steps[m_Adjust];  
                    }  
                    if (m_Enemy[j].fx > m_Enemy[j].x*60)  
                    {  
                        m_Enemy[j].fx -= s_Steps[m_Adjust];  
                        m_Enemy[j].spin -= s_Steps[m_Adjust];  
                    }  
                    if (m_Enemy[j].fy < m_Enemy[j].y*40)  
                    {  
                        m_Enemy[j].fy += s_Steps[m_Adjust];  
                        m_Enemy[j].spin += s_Steps[m_Adjust];  
                    }  
                    if (m_Enemy[j].fy > m_Enemy[j].y*40)  
                    {  
                        m_Enemy[j].fy -= s_Steps[m_Adjust];  
                        m_Enemy[j].spin -= s_Steps[m_Adjust];  
                    }  
                }  
            }  
  
            //敌人的位置和玩家的位置相遇  
            if ((m_Enemy[i].fx == m_Player.fx) && (m_Enemy[i].fy == m_Player.fy))  
            {  
                m_Lives--;                              //如果是，生命值减1  
  
                if (m_Lives == 0)                       //如果生命值为0，则游戏结束  
                {  
                    m_Gameover = true;  
                }  
  
                resetObjects();                         //重置玩家和敌人信息  
  
                //播放死亡音乐并延迟2秒  
                QSound::play("D:/QtOpenGL/QtImage/Die.wav");  
                QTime time;  
                time.start();  
                while (time.elapsed() < 2000)  
                {  
                    QCoreApplication::processEvents();  
                }  
            }  
        }  
  
        //调整玩家位置，使移动自然  
        if (m_Player.fx < m_Player.x*60)  
        {  
            m_Player.fx += s_Steps[m_Adjust];  
            m_Filled = false;                           //需要调整说明当前线段未走完，还不算网格填满  
        }  
        if (m_Player.fx > m_Player.x*60)  
        {  
            m_Player.fx -= s_Steps[m_Adjust];  
            m_Filled = false;  
        }  
        if (m_Player.fy < m_Player.y*40)  
        {  
            m_Player.fy += s_Steps[m_Adjust];  
            m_Filled = false;  
        }  
        if (m_Player.fy > m_Player.y*40)  
        {  
            m_Player.fy -= s_Steps[m_Adjust];  
            m_Filled = false;  
        }  
    }  
  
    if (m_Filled)                                       //所有网格是否填满  
    {  
        //播放过关音乐并延迟4秒  
        QSound::play("D:/QtOpenGL/QtImage/Complete.wav");  
        QTime time;  
        time.start();  
        while (time.elapsed() < 4000)  
        {  
            QCoreApplication::processEvents();  
        }  
  
        m_Stage++;                                      //增加游戏难度  
        if (m_Stage > 3)                                //如果当前的关卡大于3，则进入到下一难度等级  
        {  
            m_Stage = 1;                                //重置当前的关卡  
            m_Level++;                                  //增加当前的难度等级  
            m_Level2++;  
            if (m_Level > 3)  
            {  
                m_Level = 3;                            //如果难度等级大于3，则不再增加  
                m_Lives++;                              //完成一局给玩家奖励一条生命  
                if (m_Lives > 5)                        //如果玩家有5条生命，则不再增加  
                {  
                    m_Lives = 5;  
                }  
            }  
        }  
  
        resetObjects();                                 //重置玩家和敌人信息  
        for (int i=0; i<11; i++)                        //初始化每个线段都没被走过  
        {  
            for (int j=0; j<11; j++)  
            {  
                if (i < 10)  
                {  
                    m_Hline[i][j] = false;  
                }  
                if (j < 10)  
                {  
                    m_Vline[i][j] = false;  
                }  
            }  
        }  
    }  
  
    if ((m_Player.fx == m_Hourglass.x*60)               //玩家吃到宝物沙漏  
            && (m_Player.fy == m_Hourglass.y*40) && (m_Hourglass.fx == 1))  
    {  
        //循环播放一段计时音乐  
        m_Sound->setLoops(5);  
        m_Sound->play();  
        m_Hourglass.fx = 2;                             //设置fx为2，表示吃到宝物沙漏  
        m_Hourglass.fy = 0;                             //设置fy为0  
    }  
  
    m_Player.spin += 0.5f*s_Steps[m_Adjust];            //玩家旋转动画  
    if (m_Player.spin > 360.0f)  
    {  
        m_Player.spin -= 360.0f;  
    }  
  
    m_Hourglass.spin -= 0.25f*s_Steps[m_Adjust];        //宝物旋转动画  
    if (m_Hourglass.spin < 0.0f)  
    {  
        m_Hourglass.spin += 360.0f;  
    }  
  
    m_Hourglass.fy += s_Steps[m_Adjust];                //增加fy的值，当大于一定值时，产生宝物沙漏  
    if ((m_Hourglass.fx == 0) && (m_Hourglass.fy > 6000/m_Level))  
    {  
        //播放提示宝物沙漏产生的音乐  
        QSound::play("D:/QtOpenGL/QtImage/Hourglass.wav");  
        m_Hourglass.x = rand()%10 + 1;  
        m_Hourglass.y = rand()%11;  
        m_Hourglass.fx = 1;                             //fx=1表示宝物沙漏出现  
        m_Hourglass.fy = 0;  
    }  
  
    //玩家没有吃掉宝物沙漏，则过一段时间后会消失  
    if ((m_Hourglass.fx == 1) && (m_Hourglass.fy > 6000/m_Level))  
    {  
        m_Hourglass.fx = 0;                             //消失后重置宝物沙漏  
        m_Hourglass.fy = 0;  
    }  
  
    if ((m_Hourglass.fx == 2) && (m_Hourglass.fy > 500+(500*m_Level)))  
    {  
        m_Sound->stop();                                //停止播放计时音乐  
        m_Hourglass.fx = 0;                             //重置宝物沙漏  
        m_Hourglass.fy = 0;  
    }  
  
    m_Delay++;                                          //增加敌人的延迟计数器的值  
} 
