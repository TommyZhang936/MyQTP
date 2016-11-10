#include "globject.h"
#include <GL/glu.h>
#include <QFile>
#include <QTextStream>

glObject::glObject(QString filename)  
{  
    readData(filename);  
    setConnectivity();  
    for (int i=0; i<nPlanes; i++)  
    {  
        calPlane(vPlanes[i]);  
    }  
} 

void glObject::readData(QString filename)           //读取模型数据  
{  
    QFile file(filename);  
    file.open(QIODevice::ReadOnly | QIODevice::Text);//将要读入数据的文本打开  
    QTextStream in(&file);                          //创建文本流  
  
    in >> nPoints;                                  //读取模型的顶点数  
    vPoints.push_back(sPoint());  
    for (int i=0; i<nPoints; i++)                   //循环读取每个顶点数据  
    {  
        sPoint tPoint;  
        in >> tPoint.x >> tPoint.y >> tPoint.z;  
        vPoints.push_back(tPoint);  
    }  
  
    in >> nPlanes;                                  //读取模型的三角形面数  
    for (int i=0; i<nPlanes; i++)                   //循环读取每个三角形面数据  
    {  
        sPlane tPlane;  
        in >> tPlane.p[0] >> tPlane.p[1] >> tPlane.p[2]  
           >> tPlane.normals[0].x >> tPlane.normals[0].y >> tPlane.normals[0].z  
           >> tPlane.normals[1].x >> tPlane.normals[1].y >> tPlane.normals[1].z  
           >> tPlane.normals[2].x >> tPlane.normals[2].y >> tPlane.normals[2].z;  
        //初始化三角形面的邻面编号为-1(表示未设置或没有邻面)  
        tPlane.neigh[0] = tPlane.neigh[1] = tPlane.neigh[2] = -1;  
        vPlanes.push_back(tPlane);  
    }  
  
    file.close();  
} 

void glObject::setConnectivity()                    //设置相邻平面信息  
{  
    for (int i=0; i<nPlanes-1; i++)                 //对于模型中的每一个面A  
    {  
        for (int j=i+1; j<nPlanes; j++)             //对于除了此面的其它面B  
        {  
            for (int ki=0; ki<3; ki++)              //对于A中的每一条边(当前顶点与下一顶点组成一条边)  
            {  
                if (vPlanes[i].neigh[ki] == -1)     //如果这条边的邻面没有被设置  
                {  
                    for (int kj=0; kj<3; kj++)      //对于B中的每一条边  
                    {  
                        int p1i = ki;  
                        int p1j = kj;  
                        int p2i = (ki+1)%3;  
                        int p2j = (kj+1)%3;  
  
                        p1i = vPlanes[i].p[p1i];    //A当前顶点编号  
                        p1j = vPlanes[j].p[p1j];    //B当前顶点编号  
                        p2i = vPlanes[i].p[p2i];    //A下一顶点编号  
                        p2j = vPlanes[j].p[p2j];    //B下一顶点编号  
  
                        int P1i = ((p1i+p2i) - abs(p1i-p2i)) / 2;   //A两个顶点(编号)较小者  
                        int P1j = ((p1j+p2j) - abs(p1j-p2j)) / 2;   //B两个顶点较小者  
                        int P2i = ((p1i+p2i) + abs(p1i-p2i)) / 2;   //A两个顶点较大者  
                        int P2j = ((p1j+p2j) + abs(p1j-p2j)) / 2;   //B两个顶点较大者  
  
                        if ((P1i == P1j) && (P2i == P2j))   //如果两顶点编号相同，说明相邻  
                        {  
                            vPlanes[i].neigh[ki] = j;       //相互设置为邻面  
                            vPlanes[j].neigh[kj] = i;  
                        }  
                    }  
                }  
            }  
        }  
    }  
}  

void glObject::calPlane(sPlane &plane)              //计算平面方程的参数  
{  
    //获得三角形面的三个顶点  
    const sPoint &v1 = vPoints[plane.p[0]];  
    const sPoint &v2 = vPoints[plane.p[1]];  
    const sPoint &v3 = vPoints[plane.p[2]];  
  
    //由高数知识可求得公式  
    plane.planeEq.a = v1.y*(v2.z-v3.z) + v2.y*(v3.z-v1.z) + v3.y*(v1.z-v2.z);  
    plane.planeEq.b = v1.z*(v2.x-v3.x) + v2.z*(v3.x-v1.x) + v3.z*(v1.x-v2.x);  
    plane.planeEq.c = v1.x*(v2.y-v3.y) + v2.x*(v3.y-v1.y) + v3.x*(v1.y-v2.y);  
    plane.planeEq.d = -(v1.x*(v2.y*v3.z-v3.y*v2.z) + v2.x*(v3.y*v1.z-v1.y*v3.z)  
                        + v3.x*(v1.y*v2.z-v2.y*v1.z));  
}  

void glObject::draw()                               //绘制模型  
{  
    glBegin(GL_TRIANGLES);  
    for (int i=0; i<nPlanes; i++)                   //循环绘制每一个三角形面  
    {  
        for (int j=0; j<3; j++)                     //循环绘制每一个顶点  
        {  
            glNormal3f(vPlanes[i].normals[j].x,  
                       vPlanes[i].normals[j].y,  
                       vPlanes[i].normals[j].z);  
            glVertex3f(vPoints[vPlanes[i].p[j]].x,  
                       vPoints[vPlanes[i].p[j]].y,  
                       vPoints[vPlanes[i].p[j]].z);  
        }  
    }  
    glEnd();  
} 

void glObject::doShadowPass(GLfloat *lightPos)      //绘制阴影边界的投影  
{  
    sPoint v1, v2;  
    for (int i=0; i<nPlanes; i++)                   //循环每一个三角形面  
    {  
        if (vPlanes[i].visible)                     //如果面对灯光(背对灯光不会产生阴影)  
        {  
            for (int j=0; j<3; j++)                 //对于被灯光照射的面的每一个邻面  
            {  
                int k = vPlanes[i].neigh[j];  
                //如果邻面不存在或不被灯光照射，那么这条边是阴影边界  
                if ((k == -1) || (!vPlanes[k].visible))  
                {  
                    //获得这条边的两个顶点  
                    int p1 = vPlanes[i].p[j];  
                    int jj = (j+1)%3;  
                    int p2 = vPlanes[i].p[jj];  
  
                    //计算边的顶点到灯光的方向，并放大100倍  
                    v1.x = (vPoints[p1].x - lightPos[0])*100;  
                    v1.y = (vPoints[p1].y - lightPos[1])*100;  
                    v1.z = (vPoints[p1].z - lightPos[2])*100;  
                    v2.x = (vPoints[p2].x - lightPos[0])*100;  
                    v2.y = (vPoints[p2].y - lightPos[1])*100;  
                    v2.z = (vPoints[p2].z - lightPos[2])*100;  
  
                    //绘制构成阴影体边界的面  
                    glBegin(GL_TRIANGLE_STRIP);  
                        glVertex3f(vPoints[p1].x, vPoints[p1].y,  
                                   vPoints[p1].z);  
                        glVertex3f(vPoints[p1].x + v1.x, vPoints[p1].y + v1.y,  
                                   vPoints[p1].z + v1.z);  
                        glVertex3f(vPoints[p2].x, vPoints[p2].y,  
                                   vPoints[p2].z);  
                        glVertex3f(vPoints[p2].x + v2.x, vPoints[p2].y + v2.y,  
                                   vPoints[p2].z + v2.z);  
                    glEnd();  
                }  
            }  
        }  
    }  
}  

void glObject::castShadow(GLfloat *lightPos)        //绘制阴影  
{  
    for (int i=0; i<nPlanes; i++)                   //设置哪些面是面向灯光(通过向量点乘与0比较判断)  
    {  
        const sPlaneEq &planeEq = vPlanes[i].planeEq;  
        GLfloat side = planeEq.a*lightPos[0] + planeEq.b*lightPos[1]  
                     + planeEq.c*lightPos[2] + planeEq.d*lightPos[3];  
        if (side > 0)  
        {  
            vPlanes[i].visible = true;  
        }  
        else  
        {  
            vPlanes[i].visible = false;  
        }  
    }  
  
    glDisable(GL_LIGHTING);                         //关闭灯光  
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);    //关闭颜色缓存的写入  
    glDepthFunc(GL_LEQUAL);                         //设置深度比较函数  
    glDepthMask(GL_FALSE);                          //关闭深度缓存的写入  
    glEnable(GL_STENCIL_TEST);                      //使用蒙板缓存  
    glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);        //设置蒙板函数  
  
    //如果是逆时针(即面向视点)的多边形，通过了蒙板和深度测试，则把蒙板的值增加1  
    glFrontFace(GL_CCW);                            //设置逆时针绘制为正面  
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);  
    doShadowPass(lightPos);  
  
    //如果是顺时针(即背向视点)的多边形，通过了蒙板和深度测试，则把蒙板的值减少1  
    glFrontFace(GL_CW);                             //设置顺时针绘制为背面  
    glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);  
    doShadowPass(lightPos);  
  
    glFrontFace(GL_CCW);  
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);//恢复颜色缓存的写入  
  
    glColor4f(0.0f, 0.0f, 0.0f, 0.4f);              //阴影的颜色  
    glEnable(GL_BLEND);                             //启用混合  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //设置混合因子  
    glStencilFunc(GL_NOTEQUAL, 0, 0xFFFFFFFF);  
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);  
    glPushMatrix();                                 //保存模型观察矩阵  
    glLoadIdentity();  
    glBegin(GL_TRIANGLE_STRIP);  
        glVertex3f(-0.1f, 0.1f, -0.1f);  
        glVertex3f(-0.1f, -0.1f, -0.1f);  
        glVertex3f(0.1f, 0.1f, -0.1f);  
        glVertex3f(0.1f, -0.1f, -0.1f);  
    glEnd();  
    glPopMatrix();                                  //恢复模型观察矩阵  
  
    glDisable(GL_BLEND);                            //恢复OpenGL其他设置  
    glDepthFunc(GL_LEQUAL);  
    glDepthMask(GL_TRUE);  
    glEnable(GL_LIGHTING);  
    glDisable(GL_STENCIL_TEST);  
    glShadeModel(GL_SMOOTH);  
}

