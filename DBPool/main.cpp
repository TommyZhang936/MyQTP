#include "ConnectionTestThread.h"
#include "DBConnectionPool.h"

#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QPushButton *button = new QPushButton("Access Database");
    button->setStyleSheet("QPushButton{border-width: 1px; "
                                              "font: 75 10pt 微软雅黑; "
                                              "border-style: solid; "
                                              "border-color:transparent; "
                                              "color: rgb(255, 255, 255); "
                                              "border-left-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.378109 rgba(255, 255, 255, 50), stop:1 rgba(0, 0, 0, 0)); "
                                              "border-right-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.378109 rgba(255, 255, 255, 50), stop:1 rgba(0, 0, 0, 0)); "
                                              "background-color: qlineargradient(spread:reflect, x1:0, y1:0.489, x2:0, y2:1, stop:0.228856 rgba(255, 255, 255, 15), stop:1 rgba(39, 134, 190, 0));} "
                          );
    button->show();
    
    QObject::connect(button, &QPushButton::clicked, []()
    {
        for (int i = 0; i < 10; ++i)
        {
            ConnectionTestThread *thread = new ConnectionTestThread();
            thread->start();
        }
    });

    int ret = a.exec();
    ConnectionPool::release();               // 程序结束时关闭连接，以免造成连接泄漏

    return ret;
}


/*

Case 1
点击按钮 Access Database，正常输出。
然后关闭数据库，点击按钮 Access Database，应该提示连不上数据库。
启动数据库，点击按钮 Access Database，正常输出。

Case 2
把线程数增加到 100 个，1000 个。
同时测试关闭和再次打开数据库。

Case 3
在线程的 run() 函数里随机等待一段时间，例如 0 到 100 毫秒。

数据库连接池基本已经完成，但是并不是很完善。
考虑一下如果我们设置最大连接数为 100，高峰期访问比较多，创建满了 100 个连接，但是当闲置下来后可能只需要 2 个连接，其余 98 个连接都不长时间不用，
但它们一直都和数据库保持着连接，这对资源（Socket 连接）是很大的浪费。
需要有这样的机制，当发现连接一段时间没有被使用后就把其关闭，并从 unusedConnectionNames 里删除。
还有例如连接被分配后没有释放回连接池，即一直在 usedConnectionNames 里面，即连接泄漏，超过一定时间后连接池应该主动把其回收。
怎么实现这些的功能，这里就不在一一说明，大家独自思考一下应该怎么实现这些功能。
*/
