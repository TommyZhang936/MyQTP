#ifndef POINT_3D_H
#define POINT_3D_H
  
class POINT_3D  
{  
public:  
    POINT_3D();  
    POINT_3D(double x, double y, double z);  
  
    double x()const;                                    //x、y、z的access函数  
    double y()const;  
    double z()const;  
  
    POINT_3D operator +(const POINT_3D &p);             //向量加法  
    POINT_3D operator *(double c);                      //向量数乘  
  
private:  
    double m_x, m_y ,m_z;                               //3D坐标  
}; 

#endif // POINT_3D_H
