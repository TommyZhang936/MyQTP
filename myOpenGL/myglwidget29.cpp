#include "myglwidget29.h"
#include <QKeyEvent>
#include <QTimer>
#include <QTextStream>
#include <QMessageBox>
#include <GL/glu.h>

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;

    m_xRot = 0.0f;  
    m_yRot = 0.0f;  
    m_zRot = 0.0f;  

    QTimer *timer = new QTimer(this);                   //创建一个定时器
    //将定时器的计时信号与updateGL()绑定
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(10);                                   //以10ms为一个计时周期
}  
  
MyGLWidget::~MyGLWidget()  
{
}  

P_TEXTURE_IMAGE MyGLWidget::allocateTextureBuffer(GLuint w, GLuint h, GLuint f)  
{  
    P_TEXTURE_IMAGE ti = NULL;  
    unsigned char *c = NULL;  
  
    ti = (P_TEXTURE_IMAGE)malloc(sizeof(Texture_Image));//分配图像结构体内存  
    if (ti != NULL)  
    {  
        ti->width = w;                                  //设置宽度  
        ti->height = h;                                 //设置高度  
        ti->format = f;                                 //设置格式(位深/8)  
  
        c = (unsigned char *)malloc(w * h *f);          //分配w*h*f字节来存放图像数据  
        if (c != NULL )  
        {  
            ti->data = c;  
        }  
        else  
        {  
            QMessageBox::warning(this, "内存不足", "分配图像内存错误", QMessageBox::Ok);  
            exit(1);  
        }  
    }  
    else  
    {  
        QMessageBox::warning(this, "内存不足", "分配图像结构体内存错误", QMessageBox::Ok);  
        exit(1);  
    }  
  
    return ti;                                          //返回图像结构体指针  
}  

void MyGLWidget::deallocateTexture(P_TEXTURE_IMAGE t)  
{  
    if (t != NULL)  
    {  
        if (t->data != NULL)  
        {  
            free(t->data);                              //释放存放图像数据的内存  
        }  
  
        free(t);                                        //释放图像结构体的内存  
    }  
}  

void MyGLWidget::loadRAWData(const char *filename, P_TEXTURE_IMAGE buffer)  
{  
    int stride = buffer->width * buffer->format;        //记录每一行的字节数  
    FILE *f = fopen(filename, "rb");                    //打开文件  
  
    if (f != NULL)                                      //如果文件存在  
    {  
        for (int i=buffer->height-1; i>=0; i--)         //循环所有的行，从最下面的行开始读入  
        {  
            unsigned char *p = buffer->data + (i*stride);  
            for (int j=0; j<buffer->width; j++)         //读取每一行的数据  
            {  
                for (int k=0; k<buffer->format-1; k++, p++)  
                {  
                    *p = fgetc(f);                      //读取一个字节  
                }  
                *p = 255;                               //把255储存在alpha通道中  
                p++;  
            }  
        }  
        fclose(f);                                      //关闭文件  
    }  
    else  
    {  
        QMessageBox::warning(this, "不能打开文件", "图像错误", QMessageBox::Ok);  
        exit(1);  
    }  
}

GLuint MyGLWidget::buildTexture(P_TEXTURE_IMAGE tex)  
{  
    GLuint texture;  
  
    glGenTextures(1, &texture);                         //创建纹理空间，并记录其地址  
    glBindTexture(GL_TEXTURE_2D, texture);              //绑定纹理  
    //设置过滤器为线性过滤  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
    //在内存中创建一个纹理  
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, tex->width, tex->height,  
                      GL_RGBA, GL_UNSIGNED_BYTE, tex->data);  
    return texture;                                     //返回纹理地址  
} 

/*blit函数将一个纹理贴到另一个纹理上 
 * src为源图像，dst为目标图像 
 * src_xstart，src_ystart为要复制的部分在源图像中的位置 
 * src_width，src_height为复制的部分的宽度和高度 
 * dst_xstart，dst_ystart为复制到目标图像时的起始位置 
 * blend设置是否启用混合，true为启用，false为不启用 
 * alpha设置源图像中在混合时所占的百分比 
 */  
void MyGLWidget::blit(P_TEXTURE_IMAGE src, P_TEXTURE_IMAGE dst, int src_xstart,  
                      int src_ystart, int src_width, int src_height,  
                      int dst_xstart, int dst_ystart, bool blend, int alpha)  
{  
    if (alpha > 255)                                    //保证alpha的值有效  
    {  
        alpha = 255;  
    }  
    if (alpha < 0)  
    {  
        alpha = 0;  
    }  
  
    //计算要复制的像素在源图像数据中的开始行  
    unsigned char *s = src->data + (src_ystart*src->width*src->format);  
    //计算要复制的像素在目标图像数据中的开始行  
    unsigned char *d = dst->data + (dst_ystart*dst->width*dst->format);  
  
    for (int i=0; i<src_height; i++)                    //循环每一行  
    {  
        s = s + (src_xstart*src->format);               //移动到这一行要复制像素的开始位置  
        d = d + (dst_xstart*dst->format);  
        for (int j=0; j<src_width; j++)                 //循环复制一行  
        {  
            for (int k=0; k<src->format; k++, d++, s++) //复制每一个字节  
            {  
                if (blend)                              //如果启用了混合  
                {  
                    //根据alpha值计算颜色值  
                    *d = ((*s * alpha) + (*d * (255-alpha))) >> 8;  
                }  
                else  
                {  
                    *d = *s;                            //否则直接复制  
                }  
            }  
        }  
        //移动到下一行  
        d = d + (dst->width - (src_width+dst_xstart))*dst->format;  
        s = s + (src->width - (src_width+src_xstart))*src->format;  
    }  
}

//此处开始对OpenGL进行所以设置  
void MyGLWidget::initializeGL()                         
{  
    t1 = allocateTextureBuffer(256, 256, 4);            //为t1分配内存  
    loadRAWData("E:/QtP/myOpenGL/QtImage/Monitor.raw", t1); //读入t1的数据  
    t2 = allocateTextureBuffer(256, 256, 4);            //为t2分配内存  
    loadRAWData("E:/QtP/myOpenGL/QtImage/GL.raw", t2);      //读入t2的数据  
    blit(t2, t1, 127, 127, 128, 128, 64, 64, true, 127);//调用blit函数实现图像的拼接  
    m_Texture = buildTexture(t1);                       //创建纹理  
    deallocateTexture(t1);                              //释放t1的内存  
    deallocateTexture(t2);                              //释放t2的内存  
    glEnable(GL_TEXTURE_2D);                            //启用纹理映射  
  
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
    glLoadIdentity();                                   //重置当前的模型观察矩阵
    
    glTranslatef(0.0f, 0.0f, -5.0f);                    //移入屏幕5.0单位  
    glRotatef(m_xRot, 1.0f, 0.0f, 0.0f);                //绕x轴旋转  
    glRotatef(m_yRot, 0.0f, 1.0f, 0.0f);                //绕y轴旋转  
    glRotatef(m_zRot, 0.0f, 0.0f, 1.0f);                //绕z轴旋转  
  
    glBindTexture(GL_TEXTURE_2D, m_Texture);            //选择纹理  
    glBegin(GL_QUADS);                                  //开始绘制立方体  
        glTexCoord2f(1.0f, 1.0f);  
        glVertex3f(1.0f, 1.0f, -1.0f);                  //右上(顶面)  
        glTexCoord2f(0.0f, 1.0f);  
        glVertex3f(-1.0f, 1.0f, -1.0f);                 //左上(顶面)  
        glTexCoord2f(0.0f, 0.0f);  
        glVertex3f(-1.0f, 1.0f, 1.0f);                  //左下(顶面)  
        glTexCoord2f(1.0f, 0.0f);  
        glVertex3f(1.0f, 1.0f, 1.0f);                   //右下(顶面)  
  
        glTexCoord2f(0.0f, 0.0f);  
        glVertex3f(1.0f, -1.0f, 1.0f);                  //右上(底面)  
        glTexCoord2f(1.0f, 0.0f);  
        glVertex3f(-1.0f, -1.0f, 1.0f);                 //左上(底面)  
        glTexCoord2f(1.0f, 1.0f);  
        glVertex3f(-1.0f, -1.0f, -1.0f);                //左下(底面)  
        glTexCoord2f(0.0f, 1.0f);  
        glVertex3f(1.0f, -1.0f, -1.0f);                 //右下(底面)  
  
        glTexCoord2f(1.0f, 1.0f);  
        glVertex3f(1.0f, 1.0f, 1.0f);                   //右上(前面)  
        glTexCoord2f(0.0f, 1.0f);  
        glVertex3f(-1.0f, 1.0f, 1.0f);                  //左上(前面)  
        glTexCoord2f(0.0f, 0.0f);  
        glVertex3f(-1.0f, -1.0f, 1.0f);                 //左下(前面)  
        glTexCoord2f(1.0f, 0.0f);  
        glVertex3f(1.0f, -1.0f, 1.0f);                  //右下(前面)  
  
        glTexCoord2f(0.0f, 0.0f);  
        glVertex3f(1.0f, -1.0f, -1.0f);                 //右上(后面)  
        glTexCoord2f(1.0f, 0.0f);  
        glVertex3f(-1.0f, -1.0f, -1.0f);                //左上(后面)  
        glTexCoord2f(1.0f, 1.0f);  
        glVertex3f(-1.0f, 1.0f, -1.0f);                 //左下(后面)  
        glTexCoord2f(0.0f, 1.0f);  
        glVertex3f(1.0f, 1.0f, -1.0f);                  //右下(后面)  
  
        glTexCoord2f(1.0f, 1.0f);  
        glVertex3f(-1.0f, 1.0f, 1.0f);                  //右上(左面)  
        glTexCoord2f(0.0f, 1.0f);  
        glVertex3f(-1.0f, 1.0f, -1.0f);                 //左上(左面)  
        glTexCoord2f(0.0f, 0.0f);  
        glVertex3f(-1.0f, -1.0f, -1.0f);                //左下(左面)  
        glTexCoord2f(1.0f, 0.0f);  
        glVertex3f(-1.0f, -1.0f, 1.0f);                 //右下(左面)  
  
        glTexCoord2f(1.0f, 1.0f);  
        glVertex3f(1.0f, 1.0f, -1.0f);                  //右上(右面)  
        glTexCoord2f(0.0f, 1.0f);  
        glVertex3f(1.0f, 1.0f, 1.0f);                   //左上(右面)  
        glTexCoord2f(0.0f, 0.0f);  
        glVertex3f(1.0f, -1.0f, 1.0f);                  //左下(右面)  
        glTexCoord2f(1.0f, 0.0f);  
        glVertex3f(1.0f, -1.0f, -1.0f);                 //右下(右面)  
    glEnd();                                            //立方体绘制结束  
  
    m_xRot += 0.6f;                                     //x轴旋转  
    m_yRot += 0.4f;                                     //y轴旋转  
    m_zRot += 0.8f;                                     //z轴旋转  
  
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
