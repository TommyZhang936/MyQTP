#include "widget.h"
#include <QTime>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    generateRandomNumber();
    generateUniqueRandomNumber();
    generateAscendRandomNumber();
}

Widget::~Widget()
{
    
}
//1.生成随机数
void Widget::generateRandomNumber()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=0; i<2; i++)
    {
        int test =qrand();
        qDebug()<<test;
    }
    qDebug() << "--1=================";
}
//2.生成不重复的随机数
void Widget::generateUniqueRandomNumber()
{
    int i,j;
    QList<int> numbersList;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(i=0;i<10;i++)
    {
        numbersList.append(qrand()%10);
        bool flag=true;
        while(flag)
        {
            for(j=0;j<i;j++)
            {
                if(numbersList[i]==numbersList[j])
                {
                    break;
                }
            }
            if(j<i)
            {
                numbersList[i]=rand()%10;
            }
            if(j==i)
            {
                flag=!flag;
            }
        }
    }
    for(i=0;i<10;i++)
    {
        qDebug()<<numbersList[i];
    }
    qDebug() << "--2=================";
}
//3.生成递增的随机数
void Widget::generateAscendRandomNumber()
{
    int i;
    QList<int> numbersList;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(i=0;i<10;i++)
    {
        numbersList.append(qrand()%10);
    }
    qSort(numbersList.begin(),numbersList.end());
    for(i=0;i<10;i++)
    {
        qDebug()<<numbersList[i];
    }
    qDebug() << "--3=================";
}
