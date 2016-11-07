#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
//    resize(600,500);    //���ڴ�С����Ϊ600*500

    p = t = 0;

    Point.setX(0);  /* ��ʼ����ʼ���������Ϊ0 */
    Point.setY(0);  /* ��ʼ����ʼ��ĺ�����Ϊ0 */

    path = new QPainterPath;
    timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));  //������ʱ�������źź���Ӧ�Ĳۺ���
    timer->start(500);
}

Dialog::~Dialog()
{
    delete ui;
}

/* ��ʱʱ�䵽 */
void Dialog::timerUpDate()
{
    t += 10;
    Point.setX(t);       /* ʱ��Ӷ��� */
    Point.setY(qrand() % 100);    /* ����������ֵ */
    path->lineTo(Point);    /* */

    if(t > width()) /* ��ʱ��ֵ T���ڴ��ڵĿ��ʱ���������ԭ��  */
        p -= 10;    /* ��������ԭ�� */

    update();
}

/* �����滭���� Ҳ���ǿ�����ʱ�� */
void Dialog::startPainting(void)
{
    timer->start(2000);
}

/* ֹͣ�滭���� Ҳ����ֹͣ��ʱ�� */
void Dialog::closePainting(void)
{
    timer->stop();
}

/* �ػ��¼����� */
void Dialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(QPen(Qt::red, 2)); //���û�����ɫ�ʹ�С

    painter.translate(p,0);     //��������ԭ��

    painter.drawPath(*path);    /* ����·�� */

}
