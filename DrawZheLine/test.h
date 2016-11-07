#ifndef TEST_H  
#define TEST_H  
  
#include <QGraphicsView>
#include "ui_test.h"  
  
class testView : public QGraphicsView  
{  
    Q_OBJECT  
  
public:  
    testView(QWidget* parent = NULL);  
    ~testView();  
  
private slots:  
    void slot_update();  
};  
  
class testScene : public QGraphicsScene  
{  
    Q_OBJECT  
  
public:  
    testScene();  
    ~testScene();  
  
private slots:  
    void slot_update();  
  
protected:  
    virtual void drawBackground(QPainter *painter, const QRectF &rect);  
    virtual void drawForeground(QPainter *painter, const QRectF &rect);  
  
private:  
    QPainterPath mPath;  
};  
  
class test : public QWidget  
{  
    Q_OBJECT  
  
public:  
    test();  
    ~test();  
  
protected:  
    void paintEvent ( QPaintEvent * event );  
  
private:  
    Ui::test ui;  
};  
  
#endif // TEST_H  
