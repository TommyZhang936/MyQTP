#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QWidget>  
#include <QGLWidget>  
  
struct sPoint                                       //3D顶点结构体  
{  
    GLfloat x, y, z;  
};  
  
struct sPlaneEq                                     //平面结构体(平面方程为ax+by+cz+d=0)  
{  
    GLfloat a, b, c, d;  
};  
  
struct sPlane                                       //三角形面结构体  
{  
    unsigned int p[3];                              //三角形面的三个顶点的编号  
    sPoint normals[3];                              //三角形面的法线  
    int neigh[3];                                   //与三角形三条边相邻的面的编号  
    sPlaneEq planeEq;                               //三角形所在平面的平面方程  
    bool visible;                                   //指明这个三角形是否面向光源  
};  
  
class glObject                                      //产生阴影的模型  
{  
  
public:  
    glObject(QString filename);  
  
    void draw();                                    //绘制模型  
    void castShadow(GLfloat *lightPos);             //绘制阴影  
  
private:  
    void readData(QString filename);                //读取模型数据  
    void calPlane(sPlane &plane);                   //计算平面方程的参数  
    void setConnectivity();                         //设置相邻平面信息  
    void doShadowPass(GLfloat *lightPos);           //绘制阴影边界的投影  
  
private:  
    int nPoints;                                    //模型的顶点数  
    QVector<sPoint> vPoints;                        //储存顶点的向量  
    int nPlanes;                                    //模型的三角形面数  
    QVector<sPlane> vPlanes;                        //储存三角形面的向量  
};  

#endif // GLOBJECT_H
