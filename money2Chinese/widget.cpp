#include "widget.h"
#include "ui_widget.h"
#include <QPainter>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
   newMHZ1 = new MoneyToHZ;
   
   ui->textBrowser->setVisible(false);
   
   m_timer.start(100, this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        return;
    }
    
    QString str = newMHZ1->getHZ(arg1.toDouble());
//    QString str = newMHZ1->getHZ(arg1.trimmed());
    ui->textBrowser->setText(str);
    
    m_strText = str;

}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    // 计算文本绘制的起始坐标
    QFontMetrics metrics(font());
//    int x = (width() - metrics.width(m_strText)) / 2;
    int y = (height() + metrics.ascent() - metrics.descent()) / 2;
    int x = 10;
    
    QColor color;
    QPainter painter(this);
    for (int i = 0; i < m_strText.size(); ++i)
    {
        // 设置色调（H）、饱和度（S）、亮度（V）
        int nIndex = (m_nStep + i) % 16;
        color.setHsv((15 - nIndex) * 16, 255, 191);
        painter.setPen(color);

        // 单个字符绘制
        painter.drawText(x, y, QString(m_strText[i]));

        // 计算下一个字符的x坐标起始点
        x += metrics.width(m_strText[i]);
        x -= 10;
    }
}

void Widget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if (event->timerId() == m_timer.timerId())
    {
        ++m_nStep;
        update();
    }
    else
    {
        QWidget::timerEvent(event);
    }
}
