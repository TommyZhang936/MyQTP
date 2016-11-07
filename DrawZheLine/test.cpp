#include "test.h"  
#include <QCompleter>  
#include <QDebug>  
#include <QTime>  
  
using   namespace   std;  
  
test::test()  
{  
    ui.setupUi(this);  
  
    // View && Scene  
    testView* tView = new testView(this);  
    ui.verticalLayout->addWidget(tView);  
  
    testScene* scene = new testScene();  
  
    connect(ui.btn_update, SIGNAL(clicked()), scene, SLOT(slot_update()));  
    connect(ui.btn_update, SIGNAL(clicked()), tView, SLOT(slot_update()));  
  
    tView->setScene(scene);  
}  
  
test::~test()  
{  
  
}  
  
void test::paintEvent( QPaintEvent * event )  
{  
    //qDebug()<<"paintEvenet()";  
  
    //QPainter painter(this);  
    //painter.drawPath(mPath);  
  
    QWidget::paintEvent(event);  
}  
  
testScene::testScene()  
{  
    setSceneRect(QRectF(0,0,2000,600));  
  
    qsrand(QTime::currentTime().msec());  
  
    mPath.moveTo(0,0);  
    for(int i=0; i<1500; i=i+20)  
    {  
        int x = i;  
        int y = qrand()%500;  
        mPath.lineTo(QPointF(x,y));  
    }  
}  
  
testScene::~testScene()  
{  
  
}  
  
void testScene::drawBackground( QPainter *painter, const QRectF &rect )  
{  
    qDebug()<<"drawBackground";  
  
    painter->save();  
  
    painter->setBrush(Qt::darkCyan);  
    painter->drawRect(rect);  
  
    painter->setPen(Qt::yellow);  
    const double w = sceneRect().width();  
    const double h = sceneRect().height();  
    for(int i=0; i<h; i+=100)  
    {  
        QLineF line(QPointF(0,i),QPointF(w,i));  
        painter->drawLine(line);  
    }  
  
    painter->restore();  
}  
  
void testScene::drawForeground( QPainter *painter, const QRectF &rect )  
{  
    qDebug()<<"drawForeground";  
  
    painter->save();  
  
    painter->setPen(QPen(Qt::red,5));  
    painter->drawPath(mPath);  
      
    painter->restore();  
}  
  
void testScene::slot_update()  
{  
    qsrand(QTime::currentTime().msec());  
  
    mPath = QPainterPath();  
    mPath.moveTo(0,0);  
    for(int i=0; i<1500; i=i+20)  
    {  
        int x = i;  
        int y = qrand()%500;  
        mPath.lineTo(QPointF(x,y));  
    }  
  
    double w = sceneRect().width();  
    double h = sceneRect().height();  
    double step = w/10;  
  
    for(int i=0; i<w; i+=step)  
    {  
        update (i,0,step,h);  
        //QTest::qSleep(1000);  
        qDebug()<<"scene update()"<<i;  
    }  
}  
  
void testView::slot_update()  
{  
    //double w = rect().width();  
    //double h = rect().height();  
    //double step = w/10;  
  
    //for(int i=0; i<w; i+=step)  
    //{  
    //  update (i,0,step,h);  
    //  //QTest::qSleep(1000);  
    //  qDebug()<<"view update()"<<i;  
    //}  
}  
  
testView::testView( QWidget* parent /*= NULL*/ )  
    : QGraphicsView(parent)  
{  
    centerOn(0,0);  
  
    setCacheMode(QGraphicsView::CacheBackground);  
    setRenderHint(QPainter::Antialiasing, true);  
    //setRenderHint(QPainter::TextAntialiasing, true);  
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);  
}  
  
testView::~testView()  
{  
  
}  
