#include "widget.h"  

const GLfloat PI = 3.1415926536f;  
  
GLWidget::GLWidget(QWidget * parent, const QOpenGLWidget * shareWidget, Qt::WindowFlags f)  
{  
    setMinimumSize(320,240);  
    resize(640,480);  
    setWindowTitle(tr("第一个OpenGL程序"));  
    short angle = 18;  
    for(short i=0; i<5; i++) {  
        Point[i][0] = cos(angle * PI/180);  
        Point[i][1] = sin(angle * PI/180);  
        Point[i][2] = 0.0;  
        angle += 72;  
    }  
}  

GLWidget::~GLWidget()  
{  
} 

void GLWidget::initializeGL()  
{  
    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    //f->glShadeModel(GL_SMOOTH);  
    f->glClearColor(0.5f, 0.5f, 0.5f, 0.0f);  
    //f->glClearDepth(1.0);  
    f->glEnable(GL_DEPTH_TEST);  
    f->glDepthFunc(GL_LEQUAL);  
    f->glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  
    //f->glEnableClientState(GL_VERTEX_ARRAY);  
    //f->glVertexPointer(3, GL_FLOAT, 0, Point);  
}  

void GLWidget::paintGL()  
{  
    qDebug()<<"----------paintGL---------";  
    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    f->glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  
    //f->glLoadIdentity();  
    //f->glTranslatef( -1.5,  0.0, -6.0 );  
//    f->glBegin( GL_TRIANGLES );  
//        f->glVertex3f(  0.0,  1.0,  0.0 );  
//        f->glVertex3f( -1.0, -1.0,  0.0 );  
//        f->glVertex3f(  1.0, -1.0,  0.0 );  
//    f->glEnd();  
//    f->glTranslatef(  3.0,  0.0,  0.0 );  
  
    //glDrawArrays(GL_LINE_LOOP, 0, 5);  
  
//    f->glBegin(GL_LINE_LOOP);  
//        f->glArrayElement(1);  
//        f->glArrayElement(4);  
//        f->glArrayElement(2);  
//        f->glArrayElement(0);  
//        f->glArrayElement(3);  
//    f->glEnd();  
}  

void GLWidget::resizeGL(int width, int height)  
{  
    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    //f->glViewport( 0, 0, (GLint)width, (GLint)height );  
    //f->glMatrixMode( GL_PROJECTION );  
    //f->glLoadIdentity();  
    //gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );  
    //f->glMatrixMode( GL_MODELVIEW );  
    //f->glLoadIdentity();  
}  

void GLWidget::mouseDoubleClickEvent( QMouseEvent *event )  
{  
    if(windowState() &  Qt::WindowFullScreen)  
        showNormal();  
    else  
        showFullScreen();  
}  

void GLWidget::keyPressEvent(QKeyEvent *e)  
{  
    if (e->key() == Qt::Key_Escape)  
        close();  
} 
