#ifndef MYGLWIDGET_H  
#define MYGLWIDGET_H  
  
#include <QWidget>  
#include <QGLWidget>

typedef struct Texture_Image                        //图像结构体  
{  
    int width;  
    int height;  
    int format;                                     //格式(图像每一像素内存)  
    unsigned char *data;                            //储存图像数据  
}* P_TEXTURE_IMAGE;  

class MyGLWidget : public QGLWidget
{  
    Q_OBJECT  
    
public:  
    explicit MyGLWidget(QWidget *parent = 0);  
    ~MyGLWidget();  
      
protected:  
    //对3个纯虚函数的重定义  
    void initializeGL();  
    void resizeGL(int w, int h);  
    void paintGL();  
      
    void keyPressEvent(QKeyEvent *event);           //处理键盘按下事件  
      
private:  
    //为图像结构体分配内存  
    P_TEXTURE_IMAGE allocateTextureBuffer(GLuint w, GLuint h, GLuint f);  
    void deallocateTexture(P_TEXTURE_IMAGE t);      //释放图像结构体内存  
    //读取图像结构体数据  
    void loadRAWData(const char *filename, P_TEXTURE_IMAGE buffer);  
    GLuint buildTexture(P_TEXTURE_IMAGE tex);       //建立纹理  
    //将一个纹理贴到另一个纹理上  
    void blit(P_TEXTURE_IMAGE src, P_TEXTURE_IMAGE dst, int src_xstart,  
              int src_ystart, int src_width, int src_height,  
              int dst_xstart, int dst_ystart, bool blend, int alpha);  
  
private:  
    bool fullscreen;                                //是否全屏显示  
    
    GLfloat m_xRot;                                 //绕x轴旋转的角度  
    GLfloat m_yRot;                                 //绕y轴旋转的角度  
    GLfloat m_zRot;                                 //绕z轴旋转的角度  
      
    P_TEXTURE_IMAGE t1, t2;                         //图像结构体指针 
    GLuint m_Texture;                               //储存一个纹理 
    
};  
  
#endif // MYGLWIDGET_H 
