#include "mywidget.h"
#include <QDebug>
#include <QTime>

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
{
    newDB = new CDBProcess("SQLSERVER");
    QString dbSvrName = "127.0.0.1";
    QString dbName = "MyDB";
    QString dbUserName = "sa";
    QString dbUserPwd = "lxb2249984";
    newDB->openDB(dbSvrName, dbName, dbUserName, dbUserPwd); 
    
    mapSql.clear();
    QString sqlSelect1 = QString("SELECT * FROM FieldTest");
    mapSql.insert(0, sqlSelect1);
    
    long intRCount = 0;
    if(newDB->openRecordsetBySql(mapSql.value(0), 0))
    {
        intRCount = newDB->getRecordCount(0);
        qDebug() << "Count : " << intRCount;
    }

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  
    
//    int intRM = 1;
//    QTime time1;
//    time1.start();
//    for(int i = 0; i < intRM; i++)
//    {
//        newDB->addFieldsValueToTbl("FieldTest", 
//                                   "int1%d",  qrand(), "int2big%d",  qrand(), "int3small%d",  qrand() % 10000, "int4tiny%d",  qrand() % 256, 
//                                   "str1vchar%s", char(50 + i) + QString::number(i, 10), "str2char%s", char(40 + i) + QString::number(i, 10), "str3text%s", char(60 + i) + QString::number(i, 10), 
//                                   "dig1num%f", (double)(i) / qrand(), "dig2dec%f", (double)(i) / qrand(), "dig3float%f", (double)(i)/ qrand(),
//                                   "|");
//        newDB->updateTblFieldsValue("FieldTest", QString("WHERE int4tiny = %1").arg(i),
//                                    "dig1num%f", (double)(i) / qrand(),                                    
//                                    "|");
//        qDebug() << "double : " << (double)(i) / qrand(); 
//    }
//    qreal timea1 = time1.elapsed();
//    qDebug()<< "插入" << intRM << "条记录运行总时间为：" << timea1 / 1000 << "s" << " ,一条记录平均时间为：" << timea1 / intRM << " ms";  

//--下面是读取例子
    QTime time2;
    time2.start();
    if(newDB->moveFirst(0))
    {
        qDebug() << "Moved to first!";
        QString strOrders;
        int intRkey;
        double dig1num;
        QString strGuid;
        for(long i = 0; i < intRCount; i++)
        {
            newDB->getFieldsValueFromRec(0, "int1%d", &intRkey,"str3text%s",  &strOrders, "dig1num%f", &dig1num, "guid%s", &strGuid, "|");
            qDebug() << i << " = "  << intRkey << strOrders << QString::number(dig1num, 'g', 8) << strGuid;
            newDB->moveNext(0);
        }
    }
    qreal timea2 = time2.elapsed();
    qDebug()<< "读取" << intRCount << "条记录运行总时间为：" << timea2 / 1000 << "s" << " ,一条记录平均时间为：" << timea2 / intRCount << " ms"; 
}

MyWidget::~MyWidget()
{
    
}
