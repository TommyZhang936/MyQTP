#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QMouseEvent>
#include <QGraphicsBlurEffect>
#include <QCursor>

#include <QDebug>

const float Widget::COLORS[][3] =                 //彩虹的颜色  
{  
    {1.0f, 0.5f, 0.5f},   {1.0f, 0.75f, 0.5f}, {1.0f, 1.0f, 0.5f},  
    {0.75f, 1.0f, 0.5f}, {0.5f, 1.0f, 0.5f},   {0.5f, 1.0f, 0.75f},  
    {0.5f, 1.0f, 1.0f},   {0.5f, 0.75f, 1.0f}, {0.5f, 0.5f, 1.0f},  
    {0.75f, 0.5f, 1.0f}, {1.0f, 0.5f, 1.0f},   {1.0f, 0.5f, 0.75f}  
}; 

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    fullscreen = false;
    resize(800, 600);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    
    m_FileName = "E:/QtP/MyParticle/image/star32.png";        //应根据实际存放图片的路径进行修改
    m_Img.load(m_FileName);    
    
    m_Color = 0;  
    m_Slowdown = 2.0f;       //数值越大减速得越慢 
    m_xSpeed = 0.0f;  
    m_ySpeed = 0.0f;  
  
    for (int i=0; i<MAX_PARTICLES; i++)                 //循环初始化所以粒子  
    {  
        m_Particles[i].active = true;                   //使所有粒子为激活状态  
        m_Particles[i].life = 1.0f;                     //所有粒子生命值为最大  
        //随机生成衰减速率  
        m_Particles[i].fade = float(rand()%100)/1000.0f+0.001;  
  
        //粒子的颜色  
        m_Particles[i].r = COLORS[int(i*(12.0f/MAX_PARTICLES))][0];  
        m_Particles[i].g = COLORS[int(i*(12.0f/MAX_PARTICLES))][1];  
        m_Particles[i].b = COLORS[int(i*(12.0f/MAX_PARTICLES))][2];  
  
        //粒子的初始位置  
        m_Particles[i].x = 0.0f;  
        m_Particles[i].y = 0.0f;  
  
        //随机生成x、y、z轴方向速度  
        m_Particles[i].xi = float((rand()%50)-26.0f)*10.0f;  
        m_Particles[i].yi = float((rand()%50)-25.0f)*10.0f;  
  
        m_Particles[i].xg = 0.2f;                       //设置x方向加速度为0  
        m_Particles[i].yg = -0.8f;                      //设置y方向加速度为-0.8  
    }  
    
    debug_msg("粒子系统Demo1~");
    debug_msg("粒子系统Demo2~");
    debug_msg("粒子系统Demo3~");
    
    QTimer *timer = new QTimer(this);                   //创建一个定时器
    //将定时器的计时信号与changeParticle()绑定
    connect(timer, SIGNAL(timeout()), this, SLOT(changeParticle()));
    timer->start(10);                                   //以10ms为一个计时周期
    
}

Widget::~Widget()
{
    delete ui;
}
//提示信息，显示三行
void Widget::debug_msg(const QString &msg)
{
    if(m_messages.count() >= 3)
        m_messages.takeFirst();

    m_messages << msg;
    update();
}

void Widget::writeDebugMsg(QPainter &p)
{
    p.save();
    p.setPen(Qt::white);
    p.setFont(QFont("微软雅黑", 10));
    QRect iRect(0, 0, this->width(), 20);
    QString iText = QString("xs:%1 ys:%2 xg:%3 yg:%4 x:%5 y:%6")
            .arg(m_xSpeed)
            .arg(m_ySpeed)
            .arg(m_Particles[0].xg)
            .arg(m_Particles[0].yg)
            .arg(m_Particles[0].x)
            .arg(m_Particles[0].y);
    p.drawText(iRect, iText);
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
    p.setRenderHint(QPainter::Antialiasing);
    p.save();
    p.translate(width() >> 1, height() >> 1);
    drawParticle(p);
    p.restore();
    //写提示信息
    writeDebugMsg(p);
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    m_mousePos =e->pos();
    
    debug_msg(QString("鼠标位移到: (%0, %1)").arg(m_mousePos.x()).arg(m_mousePos.y()));
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    m_mousePos = e->pos();
    
    for (int i=0; i<MAX_PARTICLES; i++)  
    {  
        int wid2 = width() >> 1;
        int het2 = height() >> 1;
        m_Particles[i].x = m_mousePos.x() - wid2;  
        m_Particles[i].y = m_mousePos.y() - het2;          
        //m_Particles[i].x = rand() % width() - wid2 ;
        //m_Particles[i].y = rand() % height() - het2;
        //随机生成速度  
        m_Particles[i].xi = float((rand() % 50) - 26.0f) * 10.0f;  
        m_Particles[i].yi = float((rand() % 50) - 25.0f) * 10.0f;   
        //设置加速度
        m_Particles[i].xg = (rand() % 10) / 10.0f;                        
        m_Particles[i].yg = (rand() % 10) / 10.0f;                       
    }
    
    debug_msg("点击鼠标~");
}

void Widget::changeParticle()
{
    for (int i=0; i<MAX_PARTICLES; i++)                 //循环所有的粒子  
    {  
        if (m_Particles[i].active)                      //如果粒子为激活的  
        {  
            //float x = m_Particles[i].x;                 //x轴位置  
            //float y = m_Particles[i].y;                 //y轴位置  

            //更新各方向坐标及速度  
            m_Particles[i].x += m_Particles[i].xi / (m_Slowdown * 100);  
            m_Particles[i].y += m_Particles[i].yi / (m_Slowdown * 100);  

            m_Particles[i].xi += m_Particles[i].xg;  
            m_Particles[i].yi += m_Particles[i].yg;  
            
            m_Particles[i].life -= m_Particles[i].fade; //减少粒子的生命值  
            if (m_Particles[i].life < 0.0f)             //如果粒子生命值小于0  
            {  
                m_Particles[i].life = 1.0f;             //产生一个新粒子  
                m_Particles[i].fade = float(rand()%100)/1000.0f+0.003f;  
                
                m_Particles[i].r = COLORS[m_Color][0];  //设置颜色  
                m_Particles[i].g = COLORS[m_Color][1];  
                m_Particles[i].b = COLORS[m_Color][2];  
                
                m_Particles[i].x = 0.0f;                //粒子出现在屏幕中央  
                m_Particles[i].y = 0.0f;  
                
                //随机生成粒子速度  
                m_Particles[i].xi = m_xSpeed + float((rand()%60)-32.0f);  
                m_Particles[i].yi = m_ySpeed + float((rand()%60)-30.0f);  
            }  
        }  
    }  
    m_Color++;                                      //进行颜色的变换  
    if (m_Color > 11)  
    {  
        m_Color = 0;  
    }  
    update();
}

void Widget::drawParticle(QPainter &p)
{
    p.save();
    for (int i=0; i<MAX_PARTICLES; i++)                 //循环所有的粒子  
    {
        //设置粒子颜色  
        float x = m_Particles[i].x;                 //x轴位置  
        float y = m_Particles[i].y;                 //y轴位置  
        
        QColor color = QColor(m_Particles[i].r * 255, m_Particles[i].g * 255, m_Particles[i].b * 255, m_Particles[i].life * 255);   
        p.setPen(Qt::NoPen);
        p.setBrush(QBrush(color));
        //qDebug() << x << y << color.red();
        //p.setPen(QColor(Qt::red));
        //p.setBrush(QBrush(QColor(255,0,0,180)));
        p.drawEllipse(x, y, 10, 10);
        //QImage aImg =  m_Img.scaled(m_Img.width() * (0.9f - 0.05f * i) , m_Img.height() * (0.9f - 0.05f * i), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        //            QGraphicsColorizeEffect *e1 = new QGraphicsColorizeEffect(this);
        //            e1->setColor(color);            
        //            this->setGraphicsEffect(e1);
        //p.drawImage(x, y, m_Img);
    }
    p.restore();
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
        debug_msg("键 F1 切换全屏显示~");
        break;  
    //ESC为退出键  
    case Qt::Key_Escape:  
        close();  
        break;
    case Qt::Key_Tab:                                   //Tab按下使粒子回到原点，产生爆炸  
        for (int i=0; i<MAX_PARTICLES; i++)  
        {  
            m_Particles[i].x = 0.0f;  
            m_Particles[i].y = 0.0f;  

            //随机生成速度  
            m_Particles[i].xi = float((rand()%50)-26.0f)*10.0f;  
            m_Particles[i].yi = float((rand()%50)-25.0f)*10.0f;  

        }  
        debug_msg("键 Tab 复活所有粒子~");
        break;  
    case Qt::Key_8:                                     //按下8增加y方向加速度  
        for (int i=0; i<MAX_PARTICLES; i++)  
        {  
            if (m_Particles[i].yg < 3.0f)  
            {  
                m_Particles[i].yg += 0.05f;  
            }  
        }  
        debug_msg("键 8 增加 y 方向加速度 0.05 ~");
        break;  
    case Qt::Key_2:                                     //按下2减少y方向加速度  
        for (int i=0; i<MAX_PARTICLES; i++)  
        {  
            if (m_Particles[i].yg > -3.0f)  
            {  
                m_Particles[i].yg -= 0.05f;  
            }  
        }  
        debug_msg("键 2 降低 y 方向加速度 0.05 ~");
        break;  
    case Qt::Key_6:                                     //按下6增加x方向加速度  
        for (int i=0; i<MAX_PARTICLES; i++)  
        {  
            if (m_Particles[i].xg < 3.0f)  
            {  
                m_Particles[i].xg += 0.05f;  
            }  
        }  
        debug_msg("键 6 增加 x 方向加速度 0.05 ~");
        break;  
    case Qt::Key_4:                                     //按下4减少x方向加速度  
        for (int i=0; i<MAX_PARTICLES; i++)  
        {  
            if (m_Particles[i].xg > -3.0f)  
            {  
                m_Particles[i].xg -= 0.05f;  
            }  
        }  
        debug_msg("键 4 增加 x 方向加速度 0.05 ~");
        break;  
    case Qt::Key_Plus:                                  //+ 号按下加速粒子  
        if (m_Slowdown > 1.0f)  
        {  
            m_Slowdown -= 0.05f;  
        }  
        debug_msg("键 + 粒子加速 0.05 ~");
        break;  
    case Qt::Key_Minus:                                 //- 号按下减速粒子  
        if (m_Slowdown < 3.0f)  
        {  
            m_Slowdown += 0.05f;  
        }  
        debug_msg("键 - 粒子减速 0.05 ~");
        break;  

    case Qt::Key_Up:                                    //Up按下增加粒子y轴正方向的速度  
        if (m_ySpeed < 400.0f)  
        {  
            m_ySpeed += 5.0f;  
        }  
        debug_msg("键 Up 粒子 y 正方向 加速 5 ~");
        break;  
    case Qt::Key_Down:                                  //Down按下减少粒子y轴正方向的速度  
        if (m_ySpeed > -400.0f)  
        {  
            m_ySpeed -= 5.0f;  
        }  
        debug_msg("键 Down 粒子 y 正方向 减速 5 ~");
        break;  
    case Qt::Key_Right:                                 //Right按下增加粒子x轴正方向的速度  
        if (m_xSpeed < 400.0f)  
        {  
            m_xSpeed += 5.0f;  
        }  
        debug_msg("键 Right 粒子 x 正方向 加速 5 ~");
        break;  
    case Qt::Key_Left:                                  //Left按下减少粒子x轴正方向的速度  
        if (m_xSpeed > -400.0f)  
        {  
            m_xSpeed -= 5.0f;  
        }  
        debug_msg("键 Left 粒子 x 正方向 减速 5 ~");
        break;  
    }  
}
