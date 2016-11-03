#include "shapedwidget.h"
#include <QtGui>
#include <QAction>
#include <assert.h>

/** public: */
ShapedWidget::ShapedWidget(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint) //无边框窗体
{
    initData(); //初始化数据，这个demo里面就两个，具体下面有解释
    initVisualComponents();//初始化私有可视部件，这个demo里面没有私有的其它部件，所以只做了窗口的一些属性设置
    setupContextMenu();//设置右键菜单，分配各种QAction内存空间以及连接SIGNALS和SLOTS
    setAttribute(Qt::WA_DeleteOnClose);//关闭事件发生后就主动从内存中销毁窗口
    setAttribute(Qt::WA_TranslucentBackground);//透明背景支持
}

ShapedWidget::~ShapedWidget()
{
    delete dragPos_;
    delete backgroundPixmap_;
}

/** private slots: */
void ShapedWidget::changeSkin()
{
    QAction* source = qobject_cast<QAction*>(sender());//获取发送该信号的信号源，并转换对象
    backgroundPixmap_->load(source->data().toString());//因为给每个 QAction 赋了图片的地址的字符串
                                                                                                      //所以可以通过 data() 来获取该值，并更新背景图片的内容
    resize(backgroundPixmap_->width(), backgroundPixmap_->height());//窗口大小尽可能小，尽管在 mask 外的窗体不会响应 mouse event
    clearMask();//清除原来的 mask
    setMask(backgroundPixmap_->mask()); //根据新的背景图片来重新设置 mask
    update();//更新窗口
}

/** private: */
void ShapedWidget::initData()
{
    dragPos_ = new QPoint;
    backgroundPixmap_ = new QPixmap(":/images/yoda.png");
}

void ShapedWidget::initVisualComponents()
{
    this->setObjectName(tr("Shaped Widget"));
    this->setWindowTitle(tr("Shaped widget using setMask() function"));
    this->resize(backgroundPixmap_->width(), backgroundPixmap_->height());
    this->setMask(backgroundPixmap_->mask());
}

void ShapedWidget::setupContextMenu()
{
    setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction *act_yoda = new QAction(QIcon(":/images/yoda.png"), tr("Yoda"), this);
    act_yoda->setData(tr(":/images/yoda.png"));
    QAction *act_rabbit = new QAction(QIcon(":/images/rabbit.png"), tr("Rabbit"), this);
    act_rabbit->setData(tr(":/images/rabbit.png"));
    QAction *act_cyclops = new QAction(QIcon(":/images/cyclops.png"), tr("Cyclops"), this);
    act_cyclops->setData(tr(":/images/cyclops.png"));
    QAction *act_star_beast = new QAction(QIcon(":/images/star_beast.png"), tr("Star Beast"), this);
    act_star_beast->setData(tr(":/images/star_beast.png"));
    QAction *act_island = new QAction(QIcon(":/images/island.png"), tr("Island"), this);
    act_island->setData(tr(":/images/island.png"));
    QAction *act_agnes = new QAction(QIcon(":/images/agnes.png"), tr("Agnes"), this);
    act_agnes->setData(tr(":/images/agnes.png"));
    QAction *act_redstar = new QAction(QIcon(":/images/redstar.png"), tr("RedStar"), this);
    act_redstar->setData(tr(":/images/redstar.png"));
    QAction *act_p8 = new QAction(QIcon(":/images/p8.png"), tr("P8"), this);
    act_p8->setData(tr(":/images/p8.png"));

    QAction *act_quit = new QAction(tr("&Quit"), this);

    addAction(act_yoda);
    addAction(act_rabbit);
    addAction(act_cyclops);
    addAction(act_star_beast);
    addAction(act_island);
    addAction(act_agnes);
    addAction(act_redstar);
    addAction(act_p8);

    addAction(act_quit);

    connect(act_yoda, SIGNAL(triggered()), this, SLOT(changeSkin()));
    connect(act_rabbit, SIGNAL(triggered()), this, SLOT(changeSkin()));
    connect(act_cyclops, SIGNAL(triggered()), this, SLOT(changeSkin()));
    connect(act_star_beast, SIGNAL(triggered()), this, SLOT(changeSkin()));
    connect(act_island, SIGNAL(triggered()), this, SLOT(changeSkin()));
    connect(act_agnes, SIGNAL(triggered()), this, SLOT(changeSkin()));
    connect(act_redstar, SIGNAL(triggered()), this, SLOT(changeSkin()));
    connect(act_p8, SIGNAL(triggered()), this, SLOT(changeSkin()));

    connect(act_quit, SIGNAL(triggered()), this, SLOT(close()));
}


/** protected: */
void ShapedWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        *dragPos_ = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void ShapedWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        move(event->globalPos() - *dragPos_);
        event->accept();
    }
}

void ShapedWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(0, 0, backgroundPixmap_->width(), backgroundPixmap_->height(), *backgroundPixmap_);
}
