#include "chinesenumber.h"
#include <QDebug>

ChineseNumber::ChineseNumber(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);//无边框|置顶
    setAttribute(Qt::WA_TranslucentBackground, true);

    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignVCenter);  //垂直居中
    //设置字号
    int fSize = 88;
    QFont ft;
    ft.setFamily("仿宋");
    ft.setPointSize(fSize);
    m_label->setFont(ft);
    //setGeometry(QRect(0, 400, 1000, 100));
    setGeometry(QRect(this->width() / 2, this->height() / 2, this->width(),  (int)(1.5 * fSize + fSize / 3)));
    m_label->setFixedSize(1000, 150);
    //m_label->setMinimumHeight(this->height());    //修改窗口尺寸后立即更新label宽度

    //设置颜色
    QPalette pa;
    m_red = 0;
    m_green = 0;
    m_blue = 0;
    pa.setColor(QPalette::WindowText, QColor(m_red, m_green, m_blue));
    m_label->setPalette(pa);

    m_number = 0;
    cNumber<<"零"<<"壹"<<"贰"<<"叁"<<"肆"<<"伍"<<"陆"<<"柒"<<"捌"<<"玖";

    timerTicker = new QTimer(this);
    connect(timerTicker, &QTimer::timeout, this, &ChineseNumber::showHZ);
    timerTicker->start(100);
}

ChineseNumber::~ChineseNumber()
{
}

void ChineseNumber::showHZ()
{
    m_number++;
    QString strText = setHZ(m_number);
    m_label->setText(strText);
    //设置颜色
    m_rgbindex = (m_number / 256) % 6;
    m_red += m_plus[0][m_rgbindex];
    if(m_red > 255) m_red = 255;
    if(m_red < 0) m_red = 0;
    m_green += m_plus[1][m_rgbindex];
    if(m_green > 255) m_green = 255;
    if(m_green < 0) m_green = 0;
    m_blue += m_plus[2][m_rgbindex];
    if(m_blue > 255)  m_blue = 255;
    if(m_blue < 0) m_blue = 0;
    //qDebug() << m_number << " = " << m_rgbindex << m_red << m_green << m_blue;
    QPalette pa;
    pa.setColor(QPalette::WindowText, QColor(m_red, m_green, m_blue));
    m_label->setPalette(pa);
    //m_label->adjustSize();
}

QString ChineseNumber::setHZ(const int &number)
{
    if( number <= 0)
        return cNumber.at(0);

    QString strNumber;
    int i = number;
    while(i > 0)
    {
        strNumber.insert(0, cNumber.at(i % 10));
        i =(int) (i / 10);
    }
    return strNumber;
}

void ChineseNumber::setStart(const int &intValue)
{
    m_number = intValue;
}
