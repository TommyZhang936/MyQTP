#include "myglwidget24.h"
#include <QKeyEvent>
#include <QTimer>
#include <GL/glu.h>

MyGLWidget::MyGLWidget(QWidget *parent) :  
    QGLWidget(parent)
{  
    fullscreen = false;  
    setFixedSize(640, 480);                             //设置固定的窗口大小  
  
    m_Scroll = 0;  
    m_Maxtokens = 0;  
    m_FileName = "E:/QtP/myOpenGL/QtImage/Font1.tga";       //应根据实际存放图片的路径进行修改     

    QTimer *timer = new QTimer(this);                   //创建一个定时器
    //将定时器的计时信号与updateGL()绑定
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(10);                                   //以10ms为一个计时周期
}  
  
MyGLWidget::~MyGLWidget()  
{
    killFont();                                         //删除显示列表 
}  

void MyGLWidget::killFont()                             //删除显示列表  
{  
    glDeleteLists(m_Base, 256);                         //删除256个显示列表  
}  


void MyGLWidget::buildFont()                            //创建位图字体  
{  
    float cx, cy;                                       //储存字符的x、y坐标  
    m_Base = glGenLists(256);                           //创建256个显示列表  
    glBindTexture(GL_TEXTURE_2D, m_Texture);            //选择字符纹理  
  
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
            glTranslated(14, 0, 0);                     //绘制完一个字符，向右平移14个单位  
        glEndList();                                    //字符显示列表完成  
    }  
}  

void MyGLWidget::glPrint(GLuint x, GLuint y, int set, const char *fmt, ...)  
{  
    char text[1024];                                    //保存字符串  
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
  
    glScalef(1.0f, 2.0f, 1.0f);                         //如果是第一个字符集，放大字体  
  
    glCallLists(strlen(text), GL_BYTE, text);           //把字符串写到屏幕  
    glDisable(GL_TEXTURE_2D);                           //禁用纹理  
}  

//此处开始对OpenGL进行所以设置  
void MyGLWidget::initializeGL()                         
{  
    m_Texture = bindTexture(QPixmap(m_FileName));  
    glEnable(GL_TEXTURE_2D);                            //启用纹理映射  
    buildFont();                                        //创建字体  
  
    glClearColor(0.0, 0.0, 0.0, 0.0);                   //黑色背景  
    glShadeModel(GL_SMOOTH);                            //启用阴影平滑  
    glClearDepth(1.0);                                  //设置深度缓存  
    glEnable(GL_BLEND);                                 //启用融合  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //设置融合因子  
} 

//重置OpenGL窗口的大小  
void MyGLWidget::resizeGL(int w, int h)                 
{  
    m_Swidth = w;                                       //记录窗口大小用于计算剪切矩形的大小  
    m_Sheight = h;  
  
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
  
    glColor3f(1.0f, 0.5f, 0.5f);                        //设置为红色  
    glPrint(50, 16, 1, "Renderer");  
    glPrint(80, 48, 1, "Vendor");  
    glPrint(66, 80, 1, "Version");  
  
    glColor3f(1.0, 0.7f, 0.4f);                         //设置为橘黄色  
    glPrint(200, 16, 1, (char*)glGetString(GL_RENDERER));//显示OpenGL的实现组织  
    glPrint(200, 48, 1, (char*)glGetString(GL_VENDOR)); //显示销售商  
    glPrint(200, 80, 1, (char*)glGetString(GL_VERSION));//显示当前版本  
  
    glColor3f(0.5f, 0.5f, 1.0f);                        //设置为蓝色  
    glPrint(192, 432, 1, "NeHe Productions");           //在屏幕底部显示"NeHe Productions"字符串  
  
    glLoadIdentity();                                   //重置模型观察矩阵  
    glColor3f(1.0f, 1.0f, 1.0f);                        //设置为白色  
    //绘制边框  
    glBegin(GL_LINE_STRIP);  
        glVertex2d(639, 417);  
        glVertex2d(0, 417);  
        glVertex2d(0, 480);  
        glVertex2d(639, 480);  
        glVertex2d(639, 128);  
    glEnd();  
    glBegin(GL_LINE_STRIP);  
        glVertex2d(0, 128);  
        glVertex2d(639, 128);  
        glVertex2d(639, 1);  
        glVertex2d(0, 1);  
        glVertex2d(0, 417);  
    glEnd();  
  
    glScissor(1, int(0.135416f*m_Sheight), m_Swidth-2,  //设置剪裁区域  
              int(0.597916f*m_Sheight));  
    glEnable(GL_SCISSOR_TEST);                          //使用剪裁测试  
  
    //为保存OpenGL扩展名的字符串分配内存空间  
    char* text = (char*)malloc(strlen((char*)glGetString(GL_EXTENSIONS)) + 1);  
    strcpy(text, (char*)glGetString(GL_EXTENSIONS));    //返回OpenGL扩展名的字符串  
  
    char *token = strtok(text, " ");                    //按空格分割text字符串  
    int cnt = 0;                                        //记录字符串的个数  
    while (token != NULL)                               //如果token不为NULL  
    {  
        cnt++;                                          //增加计数器  
        if (cnt > m_Maxtokens)  
        {  
            m_Maxtokens = cnt;                          //记录最大的扩展名数量  
        }  
  
        glColor3f(0.5f, 1.0f, 0.5f);                    //设置颜色为绿色  
        glPrint(0, 96+(cnt*32)-m_Scroll, 0, "%i", cnt); //绘制第几个扩展名  
  
        glColor3f(1.0f, 1.0f, 0.5f);                    //设置颜色为黄色  
        glPrint(50, 96+(cnt*32)-m_Scroll, 0, token);    //输出第cnt个扩展名  
  
        token = strtok(NULL, " ");                      //获得下一个扩展名  
    }  
  
    glDisable(GL_SCISSOR_TEST);                         //禁用剪裁测试  
    free(text);                                         //释放分配的内存  
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
    case Qt::Key_Up:                                    //按下向上字幕向上滚动  
        if (m_Scroll > 0)  
        {  
            m_Scroll -= 8;  
        }  
        break;  
    case Qt::Key_Down:                                  //按下向下字幕向下滚动  
        if (m_Scroll < 32*(m_Maxtokens-9))  
        {  
            m_Scroll += 8;  
        }  
        break; 
    }  
}

GLuint MyGLWidget::loadTGATexture(QString filename)     //加载TGA文件并转为纹理  
{  
    GLubyte TGAheader[12] = {0, 0, 2, 0, 0, 0,          //无压缩的TGA文件头  
                             0, 0, 0, 0, 0, 0};  
    GLubyte TGAcompare[12];                             //保存读入的文件头信息  
    GLubyte header[6];                                  //保存最有用的图像信息(宽、高、位深)  
    GLuint bytesPerPixel;                               //记录每个颜色所占用的字节数  
    GLuint imageSize;                                   //记录文件大小  
    GLuint type = GL_RGBA;                              //设置默认的格式为GL_RGBA，即32位图像  
  
    struct TextureImage                                 //创建加载TGA图像文件结构体  
    {  
        GLubyte *imageData;                             //图像数据指针  
        GLuint bpp;                                     //每个数据所占的位数(必须为24或32)  
        GLuint width;                                   //图像宽度  
        GLuint height;                                  //图像高度  
        GLuint texID;                                   //纹理的ID值  
    } texture;  
  
    FILE *file = fopen(filename.toUtf8(), "rb");        //打开一个TGA文件  
    if (file == NULL                                                            //文件存在么？  
        || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) //是否包含12个字节的文件头？  
        || memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0                //是否是我们需要的格式？  
        || fread(header, 1, sizeof(header), file) != sizeof(header))            //如果是读取下面六个图像信息  
    {  
        if (file == NULL)  
        {  
            return 0;                                   //文件不存在则返回0  
        }  
        else  
        {  
            fclose(file);                               //否则关闭文件，返回0  
            return 0;  
        }  
    }  
  
    texture.width = header[1] * 256 + header[0];        //记录文件高度  
    texture.height = header[3] * 256 + header[2];       //记录文件宽度  
    if (texture.width <= 0                              //宽度是否小于0  
        || texture.height <= 0                          //高度是否小于0  
        || (header[4] != 24 && header[4] != 32))        //TGA文件是24位/32位？  
    {  
        fclose(file);                                   //出错则关闭文件，返回0  
        return 0;  
    }  
  
    texture.bpp = header[4];                            //记录文件的位深  
    bytesPerPixel = texture.bpp / 8;                    //记录每个像素所占的字节数  
    //计算TGA文件加载所需要的内存大小  
    imageSize = texture.width * texture.height * bytesPerPixel;  
  
    texture.imageData = (GLubyte *)malloc(imageSize);   //分配内存去保存TGA数据  
    if (texture.imageData == NULL                                               //是否成功分配内存？  
        || fread(texture.imageData, 1, imageSize, file) != imageSize)           //是否成功读入内存？  
    {  
        if (texture.imageData != NULL)                  //是否有数据被加载  
        {  
            free(texture.imageData);                    //如果是，则释放载入的数据  
        }  
        fclose(file);                                   //关闭文件，返回0  
        return 0;  
    }  
  
    for (int i=0; i<int(imageSize); i+=bytesPerPixel)   //循环所有的像素  
    {  
        GLuint temp = texture.imageData[i];             //交换R和B的值  
        texture.imageData[i] = texture.imageData[i + 2];  
        texture.imageData[i + 2] = temp;  
    }  
    fclose(file);                                       //关闭文件  
  
    glGenTextures(1, &texture.texID);                   //创建纹理空间，并记录其ID  
    glBindTexture(GL_TEXTURE_2D, texture.texID);        //绑定纹理  
    //设置过滤器为线性过滤  
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
    if (texture.bpp == 24)                              //是否为24位图像  
    {  
        type = GL_RGB;                                  //如果是，设置类型为GL_RGB  
    }  
    //在内存中创建一个纹理  
    glTexImage2D(GL_TEXTURE_2D, 0, type, texture.width, texture.height,  
                 0, type, GL_UNSIGNED_BYTE, texture.imageData);  
  
    return texture.texID;                               //返回纹理地址  
}
