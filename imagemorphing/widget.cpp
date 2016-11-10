#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QTimer>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //setWindowState(Qt::WindowMaximized);
    resize(800, 600);

    imageLenth = 256;
    morphing = false;

    picFrom.load(":/image/from.png");
    picTo.load(":/image/to.png");
    picGoto.load(":/image/to.png");
    for(int i = 0; i < imageLenth; i++)
    {
        for(int j = 0; j < imageLenth; j++)
        {
            rgbFrom[i][j] = picFrom.pixel(i, j);
            rgbTo[i][j] = picTo.pixel(i, j);
        }
    }

    timer = new QTimer(this);                   //创建一个定时器
    connect(timer, SIGNAL(timeout()), this, SLOT(imageMorph()));
    //timer->start(10);                                   //以10ms为一个计时周期
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int x0 = 100;
    int y0 = 50;
    QRect rectImage = QRect(x0, y0, imageLenth, imageLenth);
    QRect rectOut = QRect(x0 - 1, y0 - 1, imageLenth + 2, imageLenth + 2);
    QRect rect2 = QRect(x0 + imageLenth + 5, y0, imageLenth, imageLenth);
    painter.drawRect(rectOut);

    if(morphing)
    {
        //painter.drawImage(rectImage, picGoto);
        painter.drawImage(rect2, picGoto);
        qDebug() << "sx";
    }
    else
    {
        painter.drawImage(rectImage, picFrom);
    }
}

void Widget::imageMorph()
{
    int r, g ,b;
    for(int i = 0; i < imageLenth; i++)
    {
        int redFrom, greenFrom, blueFrom;
        int redTo, greenTo, blueTo;
        for(int j = 0; j < imageLenth; j++)
        {
            redFrom = qRed(rgbFrom[i][j]);
            redTo = qRed(rgbTo[i][j]);
            greenFrom = qGreen(rgbFrom[i][j]);
            greenTo = qGreen(rgbTo[i][j]);
            blueFrom = qBlue(rgbFrom[i][j]);
            blueTo = qBlue(rgbTo[i][j]);

            if(redFrom != redTo)
            {
                r = (redFrom > redTo) ? redFrom - 1 : redFrom + 1;
            }
            if(greenFrom != greenTo)
            {
                g = (greenFrom > greenTo) ? greenFrom - 1 : greenFrom  + 1;
            }
            if(blueFrom != blueTo)
            {
                b = (blueFrom > blueTo) ? blueFrom - 1 : blueFrom + 1;
            }
            //qDebug() << m << QString("%1 , %2 = ").arg(i).arg(j) << r << g << b;
            QRgb pixGoto = qRgb(r, g, b);
            picGoto.setPixel(i, j, pixGoto);
        }
    }
    qDebug() <<  " = "  << morphing << r << g << b;
    update();


    //morphing = false;
}

void Widget::on_pushButton_clicked()
{
    if(!morphing)
    {
        morphing = true;
        timer->start(10);                                   //以10ms为一个计时周期
    }
    else
    {
        morphing = false;
        timer->stop();
    }
}
