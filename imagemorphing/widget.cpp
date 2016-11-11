#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //setWindowState(Qt::WindowMaximized);

    picFrom.load(":/image/from.png");
    picTo.load(":/image/to.png");
//    picFrom.load(":/image/cherry512.png");
//    picTo.load(":/image/pear512.png");
    picGoto = picFrom;    
    imageLenth = qMin(picFrom.width(), picTo.width());
    morphing = false;
    resize(300 + imageLenth,  600);    
    ui->pushButton->move(width() >> 1, 5);
    
    // 预读取图像数据
    readPic();

    timer = new QTimer(this);                   //创建一个定时器
    connect(timer, SIGNAL(timeout()), this, SLOT(imageMorph()));
    timer->start(imageLenth / 10);                                   //以10ms为一个计时周期
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int x0 = 5;
    int y0 = 55;
    
    QRect rectOut = QRect(x0 + 265, 54, imageLenth + 2, imageLenth + 2);
    QRect rectImage = QRect(x0 + 266, 55, imageLenth, imageLenth);
    QRect rect1 = QRect(x0, y0, 256, 256);
    QRect rect2 = QRect(x0, y0 + 256 + 5, 256, 256);
    painter.drawRect(rectOut);
    painter.drawImage(rect1, picFrom.scaled(256, 256, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    painter.drawImage(rectImage, picGoto);
    painter.drawImage(rect2, picTo.scaled(256, 256, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
 
}

void Widget::imageMorph()
{
    QTime time;
    time.start();
    int no;
    int r, g, b, a;
    for(int i = 0; i < imageLenth; i++)
    {
        for(int j = 0; j < imageLenth; j++)
        {
            no = i * imageLenth + j;
            //Qvector默认使用隐式共享，可以用setSharable改变其隐式共享。
            //使用non-const操作和函数将引起深拷贝。即是Copy后地址不一样，是两个不同的量
            //at()比operator[](),快，因为它不进行深拷贝   Qvector取值都会检查越界问题
            if(yzColors.at(no).eq)
            {
                r = (qAbs(yzColors.at(no).rf - yzColors.at(no).rt) > mStep) ? yzColors.at(no).rf + yzColors.at(no).r * mStep : yzColors.at(no).rt;                
                g = (qAbs(yzColors.at(no).gf - yzColors.at(no).gt) > mStep) ? yzColors.at(no).gf + yzColors.at(no).g * mStep : yzColors.at(no).gt;
                b = (qAbs(yzColors.at(no).bf - yzColors.at(no).bt) > mStep) ? yzColors.at(no).bf + yzColors.at(no).b * mStep : yzColors.at(no).bt;
                a = (qAbs(yzColors.at(no).af - yzColors.at(no).at) > mStep) ? yzColors.at(no).af + yzColors.at(no).a * mStep : yzColors.at(no).at;     
                QRgb pixGoto = qRgba(r, g, b, a);
                picGoto.setPixel(i, j, pixGoto);
            }
        }            
    }
    qDebug() <<  time.elapsed() <<" ms";
    update();
    mStep = (mStep + 1) % 256;
}

//按钮交换From和To图片
void Widget::on_pushButton_clicked()
{
    if(!morphing)
    {
        morphing = true;
        picGoto = picTo;
        picTo = picFrom;
        picFrom = picGoto; 
    }
    else
    {
        morphing = false;
        picGoto = picFrom;
        picFrom = picTo;
        picTo = picGoto;
    }
    readPic();
}

void Widget::readPic()
{
    QColor from, to;
    DOTCOLOR newColor;       
    yzColors.clear();
    for(int i = 0; i < imageLenth; i++)
    {
        for(int j = 0; j < imageLenth; j++)
        {
            from = picFrom.pixelColor(i, j);
            to = picTo.pixelColor(i, j);            
            if(from != to)
            {
                newColor.eq = true;
                newColor.rf = from.red();
                newColor.rt = to.red();
                newColor.r = (newColor.rf == newColor.rt) ? 0 : (newColor.rf > newColor.rt) ? -1 : 1 ;                
                newColor.gf = from.green();
                newColor.gt = to.green();
                newColor.g = (newColor.gf == newColor.gt) ? 0 : (newColor.gf > newColor.gt) ? -1 : 1 ;
                newColor.bf = from.blue();
                newColor.bt = to.blue();
                newColor.b = (newColor.bf == newColor.bt) ? 0 : (newColor.bf > newColor.bt) ? -1 : 1 ;
                newColor.af = from.alpha();
                newColor.at = to.alpha();
                newColor.a = (newColor.af == newColor.at) ? 0 : (newColor.af > newColor.at) ? -1 : 1 ;
            }
            else
            {
                newColor.eq = false;            
                //其余数据不关心也用不到，所以可以不用管
            }
            yzColors.push_back(newColor);
        }
    }

}
