#include <QDebug>
#include <QTime>
#include <sys/time.h>
#include <windows.h>
#include <math.h>

void function();

int main(void)
{
    qDebug()<<"-------------------------------";
    //-1-方法1 利用QTime，其精度为ms级
    QTime time;
    time.start();
    function();
    qDebug() << time.elapsed() / 1000.0 <<"s";

    //-2-方法2 利用gettimeofday()，其精度为us级
    struct timeval tpstart, tpend;
    float timeuse;
    gettimeofday(&tpstart, NULL);
    function();
    gettimeofday(&tpend, NULL);
    timeuse=(1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec) / 1000000.0;
    qDebug() << timeuse << "s";

    //-3-方法3 利用clock()，其精度为ms级
    double time_Start = (double)clock();
    function();
    double time_End = (double)clock();
    qDebug() << (time_End - time_Start) / 1000.0 << "s";

    //-4-方法4 利用windows.h(VC)函数，提精度为us级
    LARGE_INTEGER litmp;
    LONGLONG Qpart1, Qpart2, Useingtime;
    double dfMinus, dfFreq, dfTime;

    //获得CPU计时器的时钟频率
    QueryPerformanceFrequency(&litmp);//取得高精度运行计数器的频率f,单位是每秒多少次（n/s），
    dfFreq = (double)litmp.QuadPart;

    QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
    Qpart1 = litmp.QuadPart; //开始计时

    function(); //待测试的计算函数等

    QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
    Qpart2 = litmp.QuadPart; //终止计时

    dfMinus = (double)(Qpart2 - Qpart1);//计算计数器值
    dfTime = dfMinus / dfFreq;//获得对应时间，单位为秒,可以乘1000000精确到微秒级（us）
    Useingtime = dfTime * 1000000;

    qDebug() << dfTime << "s" <<Useingtime << "us";

    return 0;
}

void function()
{
    unsigned int i, j;
    double y;

    for(i = 0; i < 1000; i++)
        for(j = 0; j < 1000; j++)
            y = sin((double)i);
    
    qDebug() << "y = " << y;
}
