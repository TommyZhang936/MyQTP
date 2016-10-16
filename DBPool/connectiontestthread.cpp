#include "ConnectionTestThread.h"
#include "DBConnectionPool.h"
#include <QDebug>
#include <QSqlQuery>
#include <QTime>

void ConnectionTestThread::run()
{
    // 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    qDebug() << "In thread run():" << db.connectionName();

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    //qsrand(NULL);
    
    QSqlQuery query(db);
    QString strSql = QString("SELECT * FROM \"data0060\" where rkey = %1").arg((int)qrand() % 6);
    query.exec(strSql);

    qDebug() << strSql;
    while (query.next())
    {
        qDebug() << "This is : " <<  query.value("rkey").toInt() << query.value("sales_order").toString();
    }

    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}
