#include "mywidget.h"
#include "ui_mywidget.h"
#include <QDebug>
#include <QTime>

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    newDB = new CDBProcess("SQLSERVER");
    QString dbSvrName = "127.0.0.1";
    QString dbName = "MyDB";
    QString dbUserName = "sa";
    QString dbUserPwd = "lxb2249984";
    newDB->openDB(dbSvrName, dbName, dbUserName, dbUserPwd); 
    
    mapSql.clear();
    //取总记录数
    QString sqlSelect0 = QString("SELECT COUNT(*) AS TOTALCOUNT FROM FieldTest");
    mapSql.insert(0, sqlSelect0);
    QString sqlSelect1 = QString("SELECT * FROM FieldTest");
    mapSql.insert(1, sqlSelect1);
    //newDB->openRecordsetBySql(mapSql.value(1), 1);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    
    intRCount = 0;
    currentRec = 1;
    QTime time0;

//    time0.start();
//    if(newDB->openRecordsetBySql(mapSql.value(1), 1))
//    {
//        intRCount = newDB->getRecordCount(1);
//        qDebug() << "Count : " << intRCount;
//    }
//    qreal timea0 = time0.elapsed();
//    qDebug()<< "0读取总记录数时间为：" << timea0 / 1000 << "s";
    //--New--
    time0.start();
    if(newDB->openRecordsetBySql(mapSql.value(0), 0))
    {
        newDB->moveFirst(0);
        newDB->getFieldsValueFromRec(0, "TOTALCOUNT%d", &intRCount, "|");
        qDebug() << "Count : " << intRCount;
    }
    qreal timea0 = time0.elapsed();
    qDebug()<< "New读取总记录数时间为：" << timea0 / 1000 << "s";
    
    int intRM = 22222222;
    QTime time1;
    time1.start();
    double dbdig1, dbdig2, dbdig3;
    for(int i = 0; i < intRM; i++)
    {
        dbdig1 = (double)(qrand() % 98) +  (double)(1) / (i % 100 +1);
        dbdig2 = sin(i) / 11;
        dbdig3 = cos(i) / 2222;
        bool insertOK = newDB->addFieldsValueToTbl("FieldTest",
                                   "int1%d",  qrand(), "int2big%d",  qrand(), "int3small%d",  qrand() % 10000, "int4tiny%d",  qrand() % 256,
                                   "str1vchar%s", char(50 + i) + QString::number(i, 10), "str2char%s", char(40 + i) + QString::number(i, 10), "str3text%s", char(60 + i) + QString::number(i, 10),
                                   "dig1num%f", dbdig1, "dig2dec%f", dbdig2, "dig3float%f", dbdig3,
                                   "|");
//        newDB->updateTblFieldsValue("FieldTest", QString("WHERE int4tiny = %1").arg(i),
//                                    "dig1num%f", (double)(i) / qrand(),
//                                    "|");
        if(!insertOK or (i % 10000 == 0))
            qDebug() << "double : " << i  << dbdig1 << dbdig2 << dbdig3;
    }
    qreal timea1 = time1.elapsed();
    qDebug()<< "插入" << intRM << "条记录运行总时间为：" << timea1 / 1000 << "s" << " ,一条记录平均时间为：" << timea1 / intRM << " ms";

//--下面是读取例子
//    QTime time2;
//    time2.start();
//    if(newDB->moveFirst(0))
//    {
//        qDebug() << "Moved to first!";
//        QString strOrders;
//        int intRkey;
//        double dig1num;
//        QString strGuid;
//        for(long i = 0; i < intRCount; i++)
//        {
//            newDB->getFieldsValueFromRec(0, "int1%d", &intRkey,"str3text%s",  &strOrders, "dig1num%f", &dig1num, "guid%s", &strGuid, "|");
//            qDebug() << i << " = "  << intRkey << strOrders << QString::number(dig1num, 'g', 8) << strGuid;
//            newDB->moveNext(0);
//        }
//    }
//    qreal timea2 = time2.elapsed();
//    qDebug()<< "读取" << intRCount << "条记录运行总时间为：" << timea2 / 1000 << "s" << " ,一条记录平均时间为：" << timea2 / intRCount << " ms"; 
    QTime time2;
    time2.start();
    if(newDB->openRecordsetBySql(mapSql.value(1), 1))
    {
        newDB->moveLast(1);
        showValue(1);
    }
    qreal timea2 = time1.elapsed();
    qDebug()<< "运行前时间为：" << timea2 / 1000 << "s" ;

}

MyWidget::~MyWidget()
{
    
}

void MyWidget::showValue(int idx)
{
    readOK = newDB->getFieldsValueFromRec(idx, "int1%d", &intRkey,"str3text%s",  &strOrders, "dig1num%f", &dig1num, "guid%s", &strGuid, "|");
    if(readOK)
    {
        qDebug() <<  " = "  << intRkey << strOrders << QString::number(dig1num, 'g', 8) << strGuid;
        ui->label_r1->setText(QString("%1").arg(intRkey));
        ui->label_r2->setText(strOrders);
        ui->label_r3->setText(QString("%1").arg(dig1num));
        ui->label_r4->setText(strGuid);

        ui->lineEdit_all->setText(QString("%1").arg(intRCount));
        ui->lineEdit_cu->setText(QString("%1").arg(currentRec));

    }
}

void MyWidget::on_pushButton_F_clicked()
{
    if(newDB->recBOF(1))
        return;
    
    newDB->moveFirst(1);
    currentRec = 1;
    showValue(1);
}

void MyWidget::on_pushButton_P_clicked()
{
    qDebug() << "Record : " << currentRec;
    if(newDB->recBOF(1))
        return;
    
    newDB->movePrevious(1);
    currentRec--;
    showValue(1);
}

void MyWidget::on_pushButton_N_clicked()
{
    qDebug() << "Record : " << currentRec;
    if(newDB->recEOF(1))
        return;
    
    newDB->moveNext(1);
    currentRec++;
    showValue(1);
    
}

void MyWidget::on_pushButton_L_clicked()
{
    if(newDB->recEOF(1))
        return;
    
    newDB->moveLast(1);
    currentRec = intRCount;
    showValue(1);
}

void MyWidget::on_lineEdit_Seek_textEdited(const QString &arg1)
{
    long intGoto = arg1.toInt();

    if(intGoto != currentRec)
    {
        newDB->moveTo(intGoto - 1, 1);
        currentRec = intGoto;
        showValue(1);
    }
}
