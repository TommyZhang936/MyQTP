#include "widget.h"
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QMouseEvent>
#include <QGraphicsBlurEffect>
#include <QCursor>

#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    fullscreen = false;
    resize(800, 600);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
       
    makeDebugMsg("粒子系统Demo1~");
    makeDebugMsg("粒子系统Demo2~");
    makeDebugMsg("粒子系统Demo3~");

    //mOBJECT.verts = 0;
    //mOBJECT.vDOTs.clear();

    QTimer *timer = new QTimer(this);                   //创建一个定时器
    //将定时器的计时信号与addDot()绑定
    connect(timer, SIGNAL(timeout()), this, SLOT(addDot()));
    timer->start(100);                                   //以10ms为一个计时周期
    
}

Widget::~Widget()
{
}
//提示信息，显示三行
void Widget::makeDebugMsg(const QString &msg)
{
    if(m_messages.count() >= 3)
        m_messages.takeFirst();

    m_messages << msg;
    update();
}

void Widget::writeDebugMsg(QPainter &p)
{
    p.save();
    //无边框（颜色）矩形-背景
    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(QColor(200, 20, 20, 180)));
    QRect textRect(0, height() - 60, this->width(), 60);
    p.drawRect(textRect);
    //文字摆放等属性
    QTextOption textOpt;
    textOpt.setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    //框内写文字，行尾回车换行
    p.setPen(Qt::white);
    p.setFont(QFont("微软雅黑", 10));
    p.drawText(textRect, m_messages.join("\n"), textOpt);
    p.restore();
}

void Widget::paintEvent(QPaintEvent *)
{
    //绘制函数,没什么好说的
    QPainter p(this);
    p.fillRect(this->rect(), Qt::black);
    //QPainter::RenderHint的可取如表6-1中的值
    //------------------------------------
    //QPainter::Antialiasing           告诉绘图引擎应该在可能的情况下进行边的反锯齿绘制
    //QPainter::TextAntialiasing       尽可能的情况下文字的反锯齿绘制
    //QPainter::SmoothPixmapTransform  使用平滑的pixmap变换算法(双线性插值算法),而不是近邻插值算法
    p.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing, true);
    p.save();
    //画坐标轴
    p.setPen(Qt::red);
    p.drawLine(QPoint(width() >> 1, 0), QPoint(width() >> 1, height()));
    p.drawLine(QPoint(0, height() >> 1), QPoint(width(), height() >> 1));
    //以屏幕中心为原点画随机点
    p.translate(width() >> 1, height() >> 1);
    drawDot(p);
    p.restore();
    //-----------------
    p.setPen(Qt::green);
    wLED = 15.0f;
    hLED = wLED * 4;
    whLED = hLED;
    QPointF pointO = QPointF(wLED +5, wLED +5);
    //左边竖行
    QPointF point0 = pointO + QPointF(wLED, wLED);
    calcHPoints(point0);
    p.drawPolygon(mPoints);
    point0 = pointO + QPointF(wLED, wLED * 4 + wLED + hLED);
    calcHPoints(point0);
    p.drawPolygon(mPoints);
    //右边竖行
    point0 = pointO + QPointF(wLED * 5 + whLED, wLED);
    calcHPoints(point0);
    p.drawPolygon(mPoints);
    point0 = pointO + QPointF(wLED * 5 + whLED, wLED * 5 + hLED);
    calcHPoints(point0);
    p.drawPolygon(mPoints);
    //横画
    point0 = pointO + QPointF(wLED + wLED, 0);
    calcWPoints(point0);
    p.drawPolygon(mPoints);
    point0 = pointO + QPointF(wLED + wLED, wLED * 4 + hLED);
    calcWPoints(point0);
    p.drawPolygon(mPoints);
    point0 = pointO + QPointF(wLED + wLED, wLED * 8 + hLED * 2);
    calcWPoints(point0);
    p.drawPolygon(mPoints);


    //写提示信息
    writeDebugMsg(p);
}

void Widget::calcHPoints(QPointF sp)
{
    mPoints.clear();
    mPoints << sp
            << QPointF(sp + QPointF(-wLED, wLED))
            << QPointF(sp + QPointF(-wLED, wLED + hLED))
            << QPointF(sp + QPointF(0, wLED + wLED + hLED))
            << QPointF(sp + QPointF(wLED, wLED + hLED))
            << QPointF(sp + QPointF(wLED, wLED));
}

void Widget::calcWPoints(QPointF sp)
{
    mPoints.clear();
    mPoints << sp
            << QPointF(sp + QPointF(wLED, wLED))
            << QPointF(sp + QPointF(wLED + whLED, wLED))
            << QPointF(sp + QPointF(wLED + whLED + wLED, 0))
            << QPointF(sp + QPointF(wLED + whLED, -wLED))
            << QPointF(sp + QPointF(wLED, -wLED));
}

void Widget::addDot()
{
    DOT dot;
    dot.c = QColor(qrand() % 256, qrand() % 256, qrand() % 256);
    dot.x = qrand() % width() - (width() >> 1);
    dot.y = qrand() % height() - (height() >> 1);
    mOBJECT.verts += 1;
    mOBJECT.vDOTs.push_back(dot);
    makeDebugMsg(QString("粒子数：%1").arg(mOBJECT.verts));
    qDebug() << mOBJECT.verts;
    update();
}

void Widget::drawDot(QPainter &p)
{
    if(mOBJECT.verts == 0)
        return;

    for(int i = 0; i < mOBJECT.verts; ++i)
    {
        p.setPen(mOBJECT.vDOTs[i].c);
        p.drawPoint(mOBJECT.vDOTs[i].x, mOBJECT.vDOTs[i].y);
    }
}

//处理键盘事件
void Widget::keyPressEvent(QKeyEvent *event)  
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
        update();
        makeDebugMsg("键 F1 切换全屏显示~");
        break;  
    //ESC为退出键  
    case Qt::Key_Escape:  
        close();  
        break;
    case Qt::Key_Tab:                                   //Tab按下使粒子回到原点，产生爆炸  
        mOBJECT.verts = 0;
        mOBJECT.vDOTs.clear();
        makeDebugMsg("键 Tab 复活所有粒子~");
        break;  
    }
}
