#include "widget.h"
#include "pyslidebutton.h"
#include "sliderbar.h"
#include <QVBoxLayout>

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    PYSlideButton *w = new PYSlideButton;

    w->addItem("Monday");
    w->addItem("Tuesday");
    w->addItem("Wednesday");
    w->addItem("Thursday");
    w->addItem("Friday");
    w->addItem("Saturday");
    w->addItem("Sunday");
    w->show();
    
    SliderBar *s = new SliderBar;
    s->addItem(QStringLiteral("第一项"));
    s->addItem(QStringLiteral("第2项"));
    s->addItem(QStringLiteral("第三项"));
    s->addItem(QStringLiteral("第四项"));

    
    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(w);
    vl->addWidget(s);
    
    setLayout(vl);
}
