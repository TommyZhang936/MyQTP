#include "mainwindow.h"
#include "math.h"

#include <QDebug>

#define PI 3.1415926

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setMinimumSize(400,400);
    this->resize(400,400);

    this->setFocusPolicy(Qt::StrongFocus);

    m_machine = new ParticleMachine;
    m_machine->setRoomRect(this->rect());

    m_mousePos = QPoint(this->width() / 2.0,this->height() / 2.0);
    m_updateSpeed = 10;

    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeOut()));
    m_emiter = new QTimer(this);
    connect(m_emiter,SIGNAL(timeout()),this,SLOT(emitSome()));

    m_timer->setInterval(21 - m_updateSpeed);
    m_timer->start();

    debug_msg("Welcome to the Particle System Demo!");
    debug_msg("Code by Unixzii(unixzii@gmail.com)");
    debug_msg("Click your mouse to add and press F1 for more,enjoy it!");
}

MainWindow::~MainWindow()
{

}

void MainWindow::timeOut()
{
    m_machine->step();
    update();
}

void MainWindow::emitSome()
{
    Particle p;
    int randSeed = qrand() % 100; //随即发射角度,360度都顾及到了
    int life = 10;
    qreal vx = cos((randSeed * PI) / 10.0) * 20;
    qreal vy = sin((randSeed * PI) / 10.0) * 30;

    debug_msg(QString("%0: New particle(Vx: %1 Vy: %2)").arg(QTime::currentTime().toString("HH:mm:ss")).arg(vx).arg(vy));

    /*第二个向量为速度,第三个向量为加速度
     *几种效果方案:
     *1.速度vx,vy 加速度vy,vx  很好地爆炸效果
     *2.速度vx,vy 加速度vx,vy  另一种爆炸效果
     *3.速度vx,vy 加速度0,0  又一种爆炸效果,但不如上面两种激烈
     *4.速度vx,vy 加速度0,9.8  小球弹射加自由落体运动(建议将ParticleMachine中的checkBound启用)
     *5.速度vx,vy 加速度0,-5  小球上浮
     *6.速度0,0 加速度0,0  静止 ⊙﹏⊙
     */
    p.setProperties(QVector2D(m_mousePos),QVector2D(vx,vy),QVector2D(vy, vx), life);

    m_machine->emitParticle(p);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    //绘制函数,没什么好说的
    QPainter p(this);
    p.fillRect(this->rect(),Qt::white);

    p.setRenderHint(QPainter::Antialiasing);
    m_machine->render(p);

    p.save();
    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(QColor(0,0,0,180)));
    QRect textRect(0,0,this->width(),40);
    p.drawRect(textRect);
    QTextOption textOpt;
    textOpt.setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    p.setPen(Qt::white);
    p.setFont(QFont("Courier",8));
    p.drawText(textRect,m_messages.join("\n"),textOpt);
    p.restore();
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    //将房间矩形设置与窗体一致
    m_machine->setRoomRect(this->rect());
}

void MainWindow::debug_msg(QString msg)
{
    if(m_messages.count() >= 3)
        m_messages.takeFirst();

    m_messages << msg;
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    m_mousePos = e->pos();
    debug_msg(QString("Mouse position changed: (%0,%1)").arg(e->x()).arg(e->y()));
    e->accept();
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    m_mousePos = e->pos();
    m_emiter->start(10);
    debug_msg("Start emiting...");
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    m_emiter->stop();
    debug_msg("Stop emiting!");
}

void MainWindow::wheelEvent(QWheelEvent *e)
{
    if(m_updateSpeed >= 20 && e->delta() > 0)
    {
        e->ignore();
        return;
    }else if(m_updateSpeed <= 0 && e->delta() < 0)
    {
        e->ignore();
        return;
    }

    m_updateSpeed += e->delta() / 8 / 15;
    debug_msg(QString("Updating Speed changed: %0").arg(m_updateSpeed));
    m_timer->setInterval(21 - m_updateSpeed);
    e->accept();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_F1:
        debug_msg("Instruction:");
        debug_msg("Mouse Wheel: change updating speed;");
        debug_msg("C: clear screen;  P: pause.");
        break;
    case Qt::Key_C:
        m_machine->clear();
        update();
        debug_msg("Screen cleaned.");
        break;
    case Qt::Key_P:
        m_timer->stop();
        debug_msg("Paused.");
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *)
{
    if(!m_timer->isActive())
    {
        m_timer->start();
        debug_msg("Continue...");
    }
}
