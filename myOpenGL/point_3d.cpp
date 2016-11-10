#include "point_3d.h"  
  
POINT_3D::POINT_3D()  
{  
    m_x = 0.0;  
    m_y = 0.0;  
    m_z = 0.0;  
}  
  
POINT_3D::POINT_3D(double x, double y, double z)  
{  
    m_x = x;  
    m_y = y;  
    m_z = z;  
}  
  
double POINT_3D::x() const  
{  
    return m_x;  
}  
  
double POINT_3D::y() const  
{  
    return m_y;  
}  
  
double POINT_3D::z() const  
{  
    return m_z;  
}  
  
POINT_3D POINT_3D::operator +(const POINT_3D &p)        //向量加法  
{  
    return POINT_3D(m_x + p.m_x, m_y + p.m_y, m_z + p.m_z);  
}  
  
POINT_3D POINT_3D::operator *(double c)                 //向量数乘  
{  
    return POINT_3D(m_x * c, m_y * c, m_z * c);  
}  
