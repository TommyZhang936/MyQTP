#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground); // ****这里很重要**** 
    
    m_nRotationAngle = 0;
    
    // 利用定时器，定时变换角度，进行旋转。
    QTimer *pTimer = new QTimer(this);
    pTimer->setInterval(50);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(updatePaint()));
    pTimer->start();
    
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 改变角度，进行旋转
void MainWindow::updatePaint()
{
    m_nRotationAngle++;
    if (m_nRotationAngle > 360)
        m_nRotationAngle = 0;
    update();
}

void MainWindow::paintEvent(QPaintEvent *)
{
/*
        QPainter painter(this);
        QPixmap pix;
        pix.load(":/images/sailing512_01.png");//将图片置于项目的根目录
        //图一
        painter.drawPixmap(0, 0, 55, 55, pix);//图片尺寸22乘以22，设置大小不一样的话，图片会自动伸缩
        painter.drawText(QPointF(0, 0), "00001");
        //图二
        painter.translate(100, 100); //将（100, 100）设为坐标原点
        painter.drawPixmap(0, 0, 55, 55, pix);//图片以（100, 100）为原点
        painter.drawText(QPointF(0, 0), "000002");
        //图三
        qreal width = pix.width(); //获得以前图片的宽
        qreal height = pix.height();//获得以前图片的高
        pix = pix.scaled(width*2, height*2, Qt::KeepAspectRatio);//将图片的宽和高都扩大两倍，并且在给定的矩形内保持宽高的比值不变
        painter.drawPixmap(20, 20,pix);//继续以前面设置好的(100, 100)为坐标原点
        painter.drawText(QPointF(0, 0), "000003");
        //图四
        painter.translate(100, 100); //再次设置新的(100,100)为坐标原点，相对于最开始的坐标原点QPointF(0, 0)来说，此处相当于（200, 200）
        painter.rotate(45); //顺时针旋转90度
        painter.drawPixmap(0, 0, 55, 55, pix);//注意此处的坐标要写成QPointF(0, 0)，即在新坐标原点放置旋转后的图片
        painter.drawText(QPointF(0, 0), "000004");
        //图五
        painter.shear(0.5, 0.1); //横向扭曲0.5，纵向扭曲0.1，值越大，扭曲程度越大
        painter.drawPixmap(-50, -50, 55, 55, pix);//前面设置的反转属性没有更改，实际是将整个坐标系进行了180度反转，因此，向右下方移动为负x负y
        painter.drawText(QPointF(0, 0), "0000005");
        //图六
        painter.shear(-0.5, -0.1); //若想后面的图形不再扭曲，要进行前面扭曲的相反设置
        painter.drawPixmap(-100, -80, 55, 55, pix);//前
        painter.drawText(QPointF(0, 0), "00000006");
*/
    
    QPainter painter(this);
    painter.save();
    //------------------------------------
    //QPainter::Antialiasing           告诉绘图引擎应该在可能的情况下进行边的反锯齿绘制
    //QPainter::TextAntialiasing       尽可能的情况下文字的反锯齿绘制
    //QPainter::SmoothPixmapTransform  使用平滑的pixmap变换算法(双线性插值算法),而不是近邻插值算法
    //------------------------------------
    //painter.setRenderHint(QPainter::Antialiasing, true);
    //我们通过这条语句，将Antialiasing属性（也就是反走样）设置为 true。经过这句设置，我们就打开了QPainter的反走样功能。
    //painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

    int radius = 150;
    int arcHeight = 30;

    // >> 1（右移1位）相当于width() / 2
    painter.translate(width() >> 1, height() >> 1);
    // 旋转
    painter.rotate(m_nRotationAngle);
    //painter.rotate(45);

    // 画图片    
    QPixmap pix;
    //pix.load(":/images/CuteBall-Favorites000.png");
    pix.load(":/images/internet.png");
    //pix.load(":/images/4.png");
    pix = pix.scaled(pix.width(), pix.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    int stx = (- pix.width()) >> 1;
    int sty = (- pix.height()) >> 1;
    QRect rect = QRect(stx, sty, pix.width(), pix.height());
    qDebug() << rect << width() << height();
    

    
    /**
     * 参数二：半径
     * 参数三：开始的角度
     * 参数四：指扫取的角度-顺时针（360度 / 8 = 45度）
     * 参数五：圆环的高度
     * 参数六：填充色
    **/
    gradientArc(&painter, radius, 0,  45, arcHeight, qRgb(200, 200, 0));
    gradientArc(&painter, radius, 45, 45, arcHeight, qRgb(200, 0, 200));
    gradientArc(&painter, radius, 90, 45, arcHeight, qRgb(0, 200, 200));
    gradientArc(&painter, radius, 135, 45, arcHeight, qRgb(200, 0, 0));
    gradientArc(&painter, radius, 225, 45, arcHeight, qRgb(0, 200, 0));
    gradientArc(&painter, radius, 180, 45, arcHeight, qRgb(0, 0, 200));
    gradientArc(&painter, radius, 270, 45, arcHeight, qRgb(0, 0, 0));
    gradientArc(&painter, radius, 315, 45, arcHeight, qRgb(150, 150, 150));
    
    //中心画图片
    painter.drawPixmap(rect, pix);

    //还原画坐标线
    painter.restore();
    painter.setPen(Qt::red);

    painter.drawLine(QPoint(width() >> 1, 0), QPoint(width() >> 1, height()));
    painter.drawLine(QPoint(0, height() >> 1), QPoint(width(), height() >> 1));

    int fontSize = 20;
    QFont font("Arial", fontSize, QFont::Bold, true);
    painter.setFont(font);
    painter.drawText(QRect(0, height() - (fontSize << 1), width(), fontSize << 1), Qt::AlignCenter, QStringLiteral("旋转角度：%1").arg(m_nRotationAngle));

}

void MainWindow::gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QRgb color)
{
    // 渐变色
    QRadialGradient gradient(0, 0, radius);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1.0, color);
    painter->setBrush(gradient);

    // << 1（左移1位）相当于radius*2 即：150*2=300
    //QRectF(-150, -150, 300, 300)
    QRectF rect(-radius, -radius, radius << 1, radius << 1);
    QPainterPath path;
    path.arcTo(rect, startAngle, angleLength);
    
//    // QRectF(-120, -120, 240, 240)
//    QPainterPath subPath;
//    subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));

//    // path为扇形 subPath为椭圆
//    path -= subPath;
    
//    QFont font;
//    font.setFamily("Microsoft YaHei");
//    font.setPointSize(14);
//    path.addText(path.pointAtPercent(0.5), font, QStringLiteral("一去丶二三里"));
    
    
    painter->setPen(Qt::NoPen);    
    painter->drawPath(path);
}
