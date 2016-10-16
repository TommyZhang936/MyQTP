#include "DBConnectionPool.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

QMutex ConnectionPool::mutex;
QWaitCondition ConnectionPool::waitConnection;
ConnectionPool* ConnectionPool::instance = NULL;

ConnectionPool::ConnectionPool()
{
    // 创建数据库连接的这些信息在实际开发的时都需要通过读取配置文件得到，这里为了演示方便所以写死在了代码里。
    hostName     = "localhost";
    databaseName = "MyDB";
    username     = "postgres";
    password     = "lxb2249984";
    databaseType = "QPSQL";
    testOnBorrow = true;
    testOnBorrowSql = "SELECT 1";

    maxWaitTime  = 1000;
    waitInterval = 200;
    maxConnectionCount  = 5;
    qDebug() << hostName;
}

ConnectionPool::~ConnectionPool()
{
    // 销毁连接池的时候删除所有的连接
    foreach(QString connectionName, usedConnectionNames)
    {
        QSqlDatabase::removeDatabase(connectionName);
    }

    foreach(QString connectionName, unusedConnectionNames)
    {
        QSqlDatabase::removeDatabase(connectionName);
    }
}

ConnectionPool& ConnectionPool::getInstance()
{
    if (NULL == instance)
    {
        QMutexLocker locker(&mutex);

        if (NULL == instance)
        {
            instance = new ConnectionPool();
        }
    }
    return *instance;
}

void ConnectionPool::release()
{
    QMutexLocker locker(&mutex);
    delete instance;
    instance = NULL;
}

/*
openConnection() 函数相对比较复杂，也是 ConnectionPool 的核心

如果没有可复用连接 pool.unusedConnectionNames.size() == 0 且已经创建的连接数达到最大，则等待，等待期间有连接被释放回连接池就复用这个连接，如果超时都没有可用连接，则返回一个无效的连接 QSqlDatabase()。
如果没有可复用连接，但是已经创建的连接数没有达到最大，那么就创建一个新的连接，并把这个连接的名字添加到 usedConnectionNames。
如果有可复用的连接，则复用它，把它的名字从 unusedConnectionNames 里删除并且添加到 usedConnectionNames。
*/
QSqlDatabase ConnectionPool::openConnection()
{
    ConnectionPool& pool = ConnectionPool::getInstance();
    QString connectionName;

    QMutexLocker locker(&mutex);

    // 已创建连接数
    int connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();

    // 如果连接已经用完，等待 waitInterval 毫秒看看是否有可用连接，最长等待 maxWaitTime 毫秒
    for (int i = 0;
         i < pool.maxWaitTime && pool.unusedConnectionNames.size() == 0 && connectionCount == pool.maxConnectionCount;
         i += pool.waitInterval)
    {
        waitConnection.wait(&mutex, pool.waitInterval);

        // 重新计算已创建连接数
        connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();
    }

    if (pool.unusedConnectionNames.size() > 0)
    {
        // 有已经回收的连接，复用它们
        connectionName = pool.unusedConnectionNames.dequeue();
    }
    else if (connectionCount < pool.maxConnectionCount)
    {
        // 没有已经回收的连接，但是没有达到最大连接数，则创建新的连接
        connectionName = QString("Connection-%1").arg(connectionCount + 1);
    }
    else
    {
        // 已经达到最大连接数
        qDebug() << "Cannot create more connections.";
        return QSqlDatabase();
    }

    // 创建连接
    QSqlDatabase db = pool.createConnection(connectionName);

    // 有效的连接才放入 usedConnectionNames
    if (db.isOpen())
    {
        pool.usedConnectionNames.enqueue(connectionName);
    }

    return db;
}

/*
closeConnection() 并不是真的断开连接

需要判断连接是否我们创建的，如果不是就不处理。
把连接的名字从 usedConnectionNames 里删除并放到 unusedConnectionNames 里，表示这个连接已经被回收，可以被复用了。
唤醒一个等待的线程，告诉它有一个连接可用了。
*/
void ConnectionPool::closeConnection(QSqlDatabase connection)
{
    ConnectionPool& pool = ConnectionPool::getInstance();
    QString connectionName = connection.connectionName();

    // 如果是我们创建的连接，从 used 里删除，放入 unused 里
    if (pool.usedConnectionNames.contains(connectionName))
    {
        QMutexLocker locker(&mutex);
        pool.usedConnectionNames.removeOne(connectionName);
        pool.unusedConnectionNames.enqueue(connectionName);
        waitConnection.wakeOne();
    }
}

/*
createConnection() 是真正创建连接的函数

如果连接已经被创建，不需要重新创建，而是复用它。
testOnBorrow 为 true 的话，返回这个连接前会先用 SQL 语句 testOnBorrowSql 访问一下数据库，没问题就返回这个连接，如果出错则说明连接已经断开了，需要重新和数据库建立连接。
如果连接没有被创建过，才会真的建立一个新的连接。
*/
QSqlDatabase ConnectionPool::createConnection(const QString &connectionName)
{
    // 连接已经创建过了，复用它，而不是重新创建
    if (QSqlDatabase::contains(connectionName))
    {
        QSqlDatabase db1 = QSqlDatabase::database(connectionName);

        if (testOnBorrow)
        {
            // 返回连接前访问数据库，如果连接断开，重新建立连接
            qDebug() << "Test connection on borrow, execute:" << testOnBorrowSql << ", for" << connectionName;
            QSqlQuery query(testOnBorrowSql, db1);

            if (query.lastError().type() != QSqlError::NoError && !db1.open()) {
                qDebug() << "Open datatabase error:" << db1.lastError().text();
                return QSqlDatabase();
            }
        }

        return db1;
    }

    // 创建一个新的连接
    QSqlDatabase db = QSqlDatabase::addDatabase(databaseType, connectionName);
    db.setHostName(hostName);
    db.setDatabaseName(databaseName);
    db.setUserName(username);
    db.setPassword(password);
    
//    db.setDatabaseName(QString("DRIVER={SQL SERVER};"
//                               "SERVER=%1;"
//                               "DATABASE=%2;"
//                               "UID=%3;"
//                               "PWD=%4;")
//                        .arg(hostName)
//                       .arg(databaseName)
//                       .arg(username)
//                       .arg(password));

    if (!db.open())
    {
        qDebug() << "Open datatabase error:" << db.lastError().text();
        return QSqlDatabase();
    }

    return db;
}
