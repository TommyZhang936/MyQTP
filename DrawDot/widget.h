#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

struct DOT                                       //顶点结构体
{
    int x, y;
    QColor c;
};

struct OBJECT                                       //物体结构体
{
    int verts;                                      //物体中顶点的个数
    QVector<DOT> vDOTs;                        //包含顶点数据的向量
    OBJECT()
    {
        verts = 0;
        vDOTs.clear();
    }
};

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
protected:
    void paintEvent(QPaintEvent *);

    void keyPressEvent(QKeyEvent *event);           //处理键盘按下事件  
      
private slots:
    void addDot();
    
private:
    void writeDebugMsg(QPainter &p);
    void drawDot(QPainter &p);
    void makeDebugMsg(const QString &msg);
    void calcHPoints(QPointF sp);
    void calcWPoints(QPointF sp);
    
private:
    bool fullscreen;                                //是否全屏显示  
    
    QStringList m_messages;
    
    OBJECT mOBJECT;

    float wLED;
    float hLED;
    float whLED;
    QPolygonF mPoints;

};

#endif // WIDGET_H
