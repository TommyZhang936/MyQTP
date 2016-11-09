#ifndef MYGLWIDGET_H  
#define MYGLWIDGET_H  
  
#include <QWidget>  
#include <QGLWidget>

typedef struct tagVERTEX                            //创建Vertex顶点结构体
{
    float x, y, z;                                  //3D坐标
    float u, v;                                     //纹理坐标
} VERTEX;

typedef struct tagTRIANGLE                          //创建Triangle三角形结构体
{
    VERTEX vertexs[3];                              //3个顶点构成一个Triangle
} TRIANGLE;

typedef struct tagSECTOR                            //创建Sector区段结构体
{
    int numtriangles;                               //Sector中的三角形个数
    QVector<TRIANGLE> vTriangle;                            //储存三角形的向量
} SECTOR;
  
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
    bool fullscreen;                                //是否全屏显示  

    QString m_FileName;                             //图片的路径及文件名
    GLuint m_Texture;                               //储存一个纹理
    QString m_WorldFile;                            //存放世界的路径及文本名
    SECTOR m_Sector;                                //储存一个区段的数据

    const float m_PIOVER180 = 0.0174532925f;     //实现度和弧度直接的折算
    GLfloat m_xPos;                                 //储存当前位置
    GLfloat m_zPos;
    GLfloat m_yRot;                                 //视角的旋转
    GLfloat m_LookUpDown;                           //记录抬头和低头

};  
  
#endif // MYGLWIDGET_H 
