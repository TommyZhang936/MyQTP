#include "mywidget.h"
#include <QDebug>

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

//    for(int i = 0; i < 10; i++)
//    {
//        newDB->addFieldsValueToTbl("FieldTest", "int1%d", i, "str3text%s", QString::number(i, 10), "|");
//    }

    if(newDB->moveFirst(0))
    {
        qDebug() << "Moved to first!";
        QString strOrders;
        int intRkey;
        QString strGuid;
        for(long i = 0; i < intRCount; i++)
        {
            newDB->getFieldsValueFromRec(0, "int1%d", &intRkey,"str3text%s",  &strOrders, "guid%s", &strGuid, "|");
            qDebug() << i << " = "  << intRkey << strOrders << strGuid;
            newDB->moveNext(0);
        }
    }
}

MyWidget::~MyWidget()
{
    
}
