#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QPainter>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    
    // 设置文字大小
    QFont newFont = font();
    newFont.setBold(true);
    newFont.setPointSize(newFont.pointSize());
    setFont(newFont);
    
    numWidth = 10;                 //每行显示色块数
    numHeight = numColor / numWidth + 1; 
    
    pxWidth = 100;                       //色块显示大小
    pxHeight = 40;
    sppxWidth = 40;                     //色块间距
    sppxHeight = 20;    
    startWidth = 10;                     //色块显示起始位置
    startHeight = 10;    

    ReadColorTXT();
    
    int pbx = startWidth + numWidth * (pxWidth + sppxWidth);
    int pby = startHeight;
    ui->pushButton_R->move(pbx, pby);
    ui->pushButton_G->move(pbx, pby + 50);
    ui->pushButton_B->move(pbx, pby + 100);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    QPixmap pixmapTwo(pxWidth, pxHeight);
    QColor color;
    QColor colorPen;

    QString strColor;                  //色块颜色 
    QString strText;                    //色块颜色值 #FFFFFF
    
    int x;
    int y = startHeight;
    int counter = 0;
    for ( int i = 0; i < numHeight; i ++)
    {
        x = startWidth;
        for(int j = 0; j < numWidth; j ++)
        {          
            if(counter >= (numColor * 2))
            {
                break;
            }
            painter.save();
            
            strColor = hashRGB.value(listColor[counter]);
            strText = listColor[counter] + " #" + listColor[counter + 1];
            color = QColor(strColor);
            pixmapTwo.fill(color);

            painter.setBrush(color);
            painter.drawPixmap(x,  y, pxWidth, pxHeight, pixmapTwo);
            //colorPen.setRgb(color.red() ^ 0xff, color.green() ^ 0xff, color.blue() ^ 0xff);
            colorPen = ( color.red() < 128) ? Qt::white : Qt::black;
            painter.setPen(colorPen);
            painter.drawText(x, y + 10, strText);
            
            x +=  (pxWidth + sppxWidth);           //下一列 增加 色块宽 + 列空白
            counter += 2;
            
            painter.restore();    
        }
        y += (pxHeight + sppxHeight);              //下一行 增加 色块高 + 行空白
    }
    
}

void Widget::ReadColorTXT()
{
    QFile file("E:\\MyQTP\\颜色相关资料\\中国传统色彩大全.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QTextStream stream(&file);
    QString line_in;
    listColor.clear();
    stream.seek(0);//将当前读取文件指针移动到文件开始
    int i = 0;
    while( !stream.atEnd())
    {
        line_in = stream.readLine();
        listColor.append(line_in.split("#"));
        hashRGB.insert(listColor[i], "#" + listColor[i+1]);
        i += 2;
    }
    
    numColor = listColor.length() / 2;
    //qDebug() << listColor << numColor;
    //qDebug() << hashRGB;
}
