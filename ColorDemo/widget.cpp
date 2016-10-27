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

    ReadColorTXT();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmapTwo(32, 16);

    QString string;
    int countColor = listColor.length() / 2;
    for (int i = 0; i < countColor; i++)
    {
        string = "#" + listColor[i * 2 + 1];
        pixmapTwo.fill(QColor(string));
        //painter.setPen(QPen(listColor[i * 2 + 1]));
        painter.drawPixmap( i * 40, 20, 32, 16, pixmapTwo);
    }
}

void Widget::ReadColorTXT()
{
    QFile file("D:\\MyQTP\\颜色相关资料\\中国传统色彩大全.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QTextStream stream(&file);
    QString line_in;
    listColor.clear();
    stream.seek(0);//将当前读取文件指针移动到文件开始
    while( !stream.atEnd())
    {
        line_in = stream.readLine();
        listColor.append(line_in.split("#"));
    }
    qDebug() << listColor;
}
