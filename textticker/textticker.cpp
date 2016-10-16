#include "textticker.h"

TextTicker::TextTicker(QWidget *parent)
	: QWidget(parent)
{
    m_curIndex = 0;
	m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignVCenter);  //垂直居中
	
    setWindowFlags(Qt::FramelessWindowHint);//无边框
    
    timerTicker = new QTimer(this);
	connect(timerTicker, &QTimer::timeout, this, &TextTicker::showTicker);
    timerTicker->start();
}

TextTicker::~TextTicker()
{
}
//移动显示信息
void TextTicker::showTicker()
{
	int iSize = m_showText.size();

	if (m_curIndex >= iSize)
		m_curIndex = 0;
	m_label->setText(m_showText.mid(m_curIndex++));
	//m_label->setText(m_showText.right(iSize - m_curIndex++));
}
//设置显示文本
void TextTicker::setTickerText(const QString & strTicker)
{
    m_showText = strTicker;
}
//调整小时时间间隔
void TextTicker::startTicker(const int &timeIn)
{
	if(timeIn > 0)
    {
        timerTicker->start(timeIn);
    }
}
//设置显示窗体尺寸
void TextTicker::setRect(const QRect &trect)
{
    setGeometry(trect);
    m_label->setMinimumWidth(this->width());    //修改窗口尺寸后立即更新label宽度
}
//设置窗体背景色
void TextTicker::setBGColor(const QColor& color)
{
    QPalette palette;
    palette.setColor(QPalette::Background, color);
    //palette.setBrush(QPalette::Background, QBrush(QPixmap(":/background.png")));
    setPalette(palette);
}
//设置文本大小、颜色和字体
void TextTicker::setTickerFont(const int &fsize, const QColor &tcolor, const QString &ffont)
{
    //设置字号    
    QFont ft;
    ft.setFamily(ffont);
    ft.setPointSize(fsize);
    m_label->setFont(ft);
    
    //设置颜色
    QPalette pa;
    pa.setColor(QPalette::WindowText, tcolor);
    m_label->setPalette(pa);
}
