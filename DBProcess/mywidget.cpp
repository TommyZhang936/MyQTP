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
    QString sqlSelect1 = QString("SELECT * FROM FieldTest");
    mapSql.insert(0, sqlSelect1);
    
    intRCount = 0;
    currentRec = 1;
    if(newDB->openRecordsetBySql(mapSql.value(0), 0))
    {
        intRCount = newDB->getRecordCount(0);
        qDebug() << "Count : " << intRCount;
    }

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  
    
    int intRM = 11111;
    QTime time1;
    time1.start();
    double dbdig1, dbdig2, dbdig3;
    for(int i = 0; i < intRM; i++)
    {
        dbdig1 = (double)(qrand() % 10) / (i +1);
        dbdig2 = (double)(i) / qrand();
        dbdig3 = (double)(i)/ qrand();
        newDB->addFieldsValueToTbl("FieldTest", 
                                   "int1%d",  qrand(), "int2big%d",  qrand(), "int3small%d",  qrand() % 10000, "int4tiny%d",  qrand() % 256, 
                                   "str1vchar%s", char(50 + i) + QString::number(i, 10), "str2char%s", char(40 + i) + QString::number(i, 10), "str3text%s", char(60 + i) + QString::number(i, 10), 
                                   "dig1num%f", dbdig1, "dig2dec%f", dbdig2, "dig3float%f", dbdig3,
                                   "|");
//        newDB->updateTblFieldsValue("FieldTest", QString("WHERE int4tiny = %1").arg(i),
//                                    "dig1num%f", (double)(i) / qrand(),                                    
//                                    "|");
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
    
    newDB->moveFirst(0);
    showValue();

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
    if(newDB->recBOF(0))
        return;
    
    newDB->moveFirst(0);
    currentRec = 1;
    showValue();
}

void MyWidget::on_pushButton_P_clicked()
{
    qDebug() << "Record : " << currentRec;
    if(newDB->recBOF(0))
        return;
    
    newDB->movePrevious(0);
    currentRec--;
    showValue();
}

void MyWidget::on_pushButton_N_clicked()
{
    qDebug() << "Record : " << currentRec;
    if(newDB->recEOF(0))
        return;
    
    newDB->moveNext(0);
    currentRec++;
    showValue();
    
}

void MyWidget::on_pushButton_L_clicked()
{
    if(newDB->recEOF(0))
        return;
    
    newDB->moveLast(0);
    currentRec = intRCount;
    showValue();
}

void MyWidget::on_lineEdit_Seek_textEdited(const QString &arg1)
{
    long intGoto = arg1.toInt();

    if(intGoto != currentRec)
    {
        newDB->moveTo(intGoto - 1, 0);
        currentRec = intGoto;
        showValue();
    }
}
